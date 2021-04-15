#include "state/dstate.h"

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <string_view>
#include <vector>

namespace regex
{
    dstate::dstate( state::context ctx ) : state( ctx ), transitions_( )
    {
    }

    dstate::dstate( const dstate &src ) : state( src.get_type( ) ), transitions_( )
    {
        for( const auto &transition : src.transitions_ )
        {
            transitions_.insert( { transition.first, std::make_shared<dstate>( *transition.second ) } );
        }
    }

    void dstate::connect( std::shared_ptr<dstate> src, std::shared_ptr<dstate> dest, regex::language::character_type symbol )
    {
        src->transitions_[symbol] = dest;
    }

    void dstate::merge( std::shared_ptr<dstate> source, std::shared_ptr<dstate> target, std::set<std::shared_ptr<dstate>> visited )
    {
        target->set( source->get_type( ) == state::context::accepting || target->get_type( ) == state::context::accepting ? state::context::accepting : state::context::rejecting );

        for( const auto &source_transition : source->transitions_ )
        {
            const auto &result = target->transitions_.insert( source_transition );
            const auto &existed = !result.second;
            const auto &position = result.first;

            if( existed && ( visited.find( target ) == std::cend( visited ) ) )
            {
                visited.insert( target );
                merge( source_transition.second, position->second, visited );
            }
        }

        if( source != target )
        {
            source->transitions_.clear( );   
        }
    }

    void dstate::shallow_copy( std::shared_ptr<const dstate> source, std::shared_ptr<dstate> target, std::set<std::shared_ptr<dstate>> visited )
    {
        for( const auto &source_transition : source->transitions_ )
        {
            const auto &result = target->transitions_.insert( source_transition );
            const auto &existed = !result.second;
            const auto &position = result.first;

            if( existed && ( visited.find( target ) == std::cend( visited ) ) )
            {
                visited.insert( target );
                shallow_copy( source_transition.second, position->second, visited );
            }
        }
    }

    std::pair<std::shared_ptr<dstate>, std::set<std::shared_ptr<dstate>>> dstate::duplicate( std::shared_ptr<const dstate> src )
    {
        auto dup = std::make_shared<dstate>( src->get_type( ) );

        std::set<std::shared_ptr<dstate>> accepting_states;

        for( const auto &transition : src->transitions_ )
        {
            auto next = duplicate( transition.second );

            accepting_states.merge( next.second );

            dup->transitions_.insert( { transition.first, next.first } );
        }

        if( src->get_type( ) == state::context::accepting )
        {
            accepting_states.insert( dup );
        }

        return { dup, accepting_states };
    }

    match dstate::execute( std::basic_string_view<language::character_type> str )
    {
        if( str.empty( ) )
        {
            return get_type( ) == state::context::accepting ? match::accepted : match::rejected;
        }

        if( transitions_.find( str[0] ) != std::cend( transitions_ ) )
        {
            return transitions_.find( str[0] )->second->execute( str.substr( 1 ) );
        }
        else
        {
            return match::rejected;
        }
    }

    void dstate::walk( std::function<void( std::shared_ptr<dstate> )> callback, std::set<std::shared_ptr<dstate>> visited )
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

            for( auto &transitionForCharacter : transitions_ )
            {
                transitionForCharacter.second->walk( callback, visited );
            }
        }
    }
}  // namespace fa