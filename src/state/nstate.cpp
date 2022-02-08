#include "state/nstate.h"

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <string_view>
#include <vector>

#include "state/state.h"

namespace regex::state
{

    const nstate::transition_label_type nstate::epsilon = 0x01;

    void nstate::connect( nstate *target, transition_label_type transition_label )
    {
        transitions_[transition_label].insert( target );
    }

    const nstate::transitions_type &nstate::transitions() const
    {
        return transitions_;
    }

    bool execute_internal(
        const nstate *state, const nstate *finish, std::basic_string_view<nstate::transition_label_type> target,
        std::vector<std::pair<const nstate *, std::basic_string_view<nstate::transition_label_type>>> &visited )
    {
        if ( std::find_if( std::cbegin( visited ), std::cend( visited ), [state, target]( const auto &visit ) {
                 return ( visit.first == state && visit.second == target );
             } ) != std::cend( visited ) )
        {
            return false;
        }

        visited.push_back( { state, target } );

        if ( target.empty() )
        {
            if ( state == finish )
            {
                return true;
            }
        }

        const auto next_etransitions = state->transitions().find( nstate::epsilon );

        if ( next_etransitions != std::cend( state->transitions() ) )
        {
            for ( const auto next : next_etransitions->second )
            {
                if ( execute_internal( next, finish, target, visited ) )
                {
                    return true;
                }
            }
        }

        if ( !target.empty() )
        {
            const auto next_transitions = state->transitions().find( target[0] );

            if ( next_transitions != std::cend( state->transitions() ) )
            {
                for ( const auto next : next_transitions->second )
                {
                    if ( execute_internal( next, finish, target.substr( 1 ), visited ) )
                    {
                        return true;
                    }
                }
            }
        }

        visited.pop_back();

        return false;
    };

    bool execute( const nstate *start, const nstate *finish,
                  std::basic_string_view<nstate::transition_label_type> target )
    {
        std::vector<std::pair<const nstate *, std::basic_string_view<nstate::transition_label_type>>> visited;

        return execute_internal( start, finish, target, visited );
    }
} // namespace regex::state