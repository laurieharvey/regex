#include "state/nstate.h"

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <string_view>
#include <vector>

#include "state/state.h"

namespace regex
{
    nstate::nstate( state::context ctx ) : state( ctx ), strong_transitions_( ), weak_transitions_( )
    {
    }

    void nstate::connect( regex::character_type symbol, std::shared_ptr<nstate> st )
    {
        strong_transitions_[symbol].insert( st );
    }

    void nstate::connect( regex::character_type symbol, std::weak_ptr<nstate> st )
    {
        weak_transitions_[symbol].insert( st );
    }

    group nstate::get_transitions( regex::character_type symbol )
    {
        group result;

        typename std::map<regex::character_type, std::set<std::shared_ptr<nstate>>>::const_iterator strong_transitions = strong_transitions_.find( symbol );

        if( strong_transitions != std::cend( strong_transitions_ ) )
        {
            std::copy( strong_transitions->second.cbegin( ), strong_transitions->second.cend( ), std::inserter( result, result.begin( ) ) );
        }

        typename std::map<regex::character_type, std::set<std::weak_ptr<nstate>, std::owner_less<std::weak_ptr<nstate>>>>::const_iterator weak_transitions = weak_transitions_.find( symbol );

        if( weak_transitions != std::cend( weak_transitions_ ) )
        {
            for( auto &weak_transition : weak_transitions->second )
            {
                result.insert( weak_transition.lock( ) );
            }
        }

        return result;
    }

    group nstate::get_epsilon_closure( )
    {
        group result;

        result.insert( std::enable_shared_from_this<nstate>::shared_from_this( ) );

        for( const auto &next : get_transitions( 0x01 ) )
        {
            result.merge( next->get_epsilon_closure( ) );
        }

        return result;
    }

    std::map<character_type, group> nstate::get_transitions( )
    {
        std::map<character_type, group> result;

        for( const auto &weak_transition : weak_transitions_ )
        {
            result.insert( { weak_transition.first, get_transitions( weak_transition.first ) } );
        }

        for( const auto &strong_transition : strong_transitions_ )
        {
            if( result.find( strong_transition.first ) == std::cend( result ) )
            {
                result.insert( { strong_transition.first, get_transitions( strong_transition.first ) } );
            }
        }

        result[0x01] = get_epsilon_closure( );

        return result;
    }

    void nstate::walk( std::function<void( std::shared_ptr<state> )> callback, std::set<std::shared_ptr<state>> visited )
    {
        if( visited.find( shared_from_this( ) ) != std::end( visited ) )
        {
            return;
        }
        else
        {
            visited.insert( shared_from_this( ) );

            auto temp = shared_from_this( );

            callback( temp );

            for( auto &transitionsForCharacter : strong_transitions_ )
            {
                for( auto &transition : transitionsForCharacter.second )
                {
                    transition->walk( callback, visited );
                }
            }

            for( auto &transitionsForCharacter : weak_transitions_ )
            {
                for( auto &transition : transitionsForCharacter.second )
                {
                    transition.lock( )->walk( callback, visited );
                }
            }
        }
    }

    match nstate::run( std::basic_string_view<regex::character_type> str, std::set<std::shared_ptr<nstate>> visited )
    {
        if( visited.find( shared_from_this( ) ) != std::end( visited ) )
        {
            return match::rejected;
        }

        visited.insert( std::enable_shared_from_this<nstate>::shared_from_this( ) );

        if( str.empty( ) )
        {
            if( get_type( ) == context::accepting )
            {
                return match::accepted;
            }

            for( const std::shared_ptr<nstate> &next : strong_transitions_[0x01] )
            {
                if( next->run( str, visited ) == match::accepted )
                {
                    return match::accepted;
                }
            }

            for( const std::weak_ptr<nstate> &next : weak_transitions_[0x01] )
            {
                if( next.lock( )->run( str, visited ) == match::accepted )
                {
                    return match::accepted;
                }
            }

            return match::rejected;
        }

        for( const std::shared_ptr<nstate> &next : strong_transitions_[str[0]] )
        {
            if( next->run( str.substr( 1 ) ) == match::accepted )
            {
                return match::accepted;
            }
        }

        for( const std::weak_ptr<nstate> &next : weak_transitions_[str[0]] )
        {
            if( next.lock( )->run( str.substr( 1 ) ) == match::accepted )
            {
                return match::accepted;
            }
        }

        for( const std::shared_ptr<nstate> &next : strong_transitions_[0x02] )
        {
            if( next->run( str.substr( 1 ) ) == match::accepted )
            {
                return match::accepted;
            }
        }

        for( const std::weak_ptr<nstate> &next : weak_transitions_[0x02] )
        {
            if( next.lock( )->run( str.substr( 1 ) ) == match::accepted )
            {
                return match::accepted;
            }
        }

        for( const std::shared_ptr<nstate> &next : strong_transitions_[0x01] )
        {
            if( next->run( str, visited ) == match::accepted )
            {
                return match::accepted;
            }
        }

        for( const std::weak_ptr<nstate> &next : weak_transitions_[0x01] )
        {
            if( next.lock( )->run( str, visited ) == match::accepted )
            {
                return match::accepted;
            }
        }

        return match::rejected;
    }
}  // namespace fa