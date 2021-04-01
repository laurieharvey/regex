#include <memory>
#include <string_view>

#include "state/state.h"
#include "stack.h"
#include "language/ast.h"
#include "automata/nfa.h"

namespace regex
{
    nfa::nfa( std::shared_ptr<nstate> input, std::shared_ptr<nstate> output ) : input_( input ), output_( output )
    {
    }

    std::shared_ptr<nfa> nfa::from_character( character_type c )
    {
        auto input = std::make_shared<nstate>( state::context::rejecting );
        auto output = std::make_shared<nstate>( state::context::accepting );

        input->connect( c, output );

        return std::make_shared<nfa>( input, output );
    }

    std::shared_ptr<nfa> nfa::from_epsilon( )
    {
        return from_character( epsilon );
    }

    std::shared_ptr<nfa> nfa::from_any( )
    {
        auto input = std::make_shared<nstate>( state::context::rejecting );
        auto output = std::make_shared<nstate>( state::context::accepting );

        for( const auto letter : get_alphabet( ) )
        {
            input->connect( letter, output );
        }

        return std::make_shared<nfa>( input, output );
    }

    std::shared_ptr<nfa> nfa::from_concatenation( std::shared_ptr<nfa> lhs, std::shared_ptr<nfa> rhs )
    {
        lhs->output_->connect( epsilon, rhs->input_ );

        lhs->output_->set( state::context::rejecting );
        rhs->output_->set( state::context::accepting );

        return std::make_shared<nfa>( lhs->input_, rhs->output_ );
    }

    std::shared_ptr<nfa> nfa::from_alternation( std::shared_ptr<nfa> lhs, std::shared_ptr<nfa> rhs )
    {
        std::shared_ptr<nstate> input = std::make_shared<nstate>( state::context::rejecting );
        std::shared_ptr<nstate> output = std::make_shared<nstate>( state::context::accepting );

        input->connect( epsilon, lhs->input_ );
        input->connect( epsilon, rhs->input_ );

        lhs->output_->connect( epsilon, output );
        lhs->output_->set( state::context::rejecting );
        rhs->output_->connect( epsilon, output );
        rhs->output_->set( state::context::rejecting );

        return std::make_shared<nfa>( input, output );
    }

    std::shared_ptr<nfa> nfa::from_kleene( std::shared_ptr<nfa> expression )
    {
        std::shared_ptr<nstate> input = std::make_shared<nstate>( state::context::rejecting );
        std::shared_ptr<nstate> output = std::make_shared<nstate>( state::context::accepting );

        expression->output_->set( state::context::rejecting );

        input->connect( epsilon, expression->input_ );
        input->connect( epsilon, output );
        expression->output_->connect( epsilon, output );
        output->connect( epsilon, std::weak_ptr<nstate>( expression->input_ ) );

        return std::make_shared<nfa>( input, output );
    }

    void nfa::walk( std::function<void( std::shared_ptr<state> )> callback )
    {
        input_->walk( callback );
    }

    match nfa::run( std::basic_string_view<character_type> str )
    {
        return input_->run( str );
    }

    void nfa_generator::callback( const regex::token &token )
    {
        std::shared_ptr<regex::nfa> lhs;
        std::shared_ptr<regex::nfa> rhs;
        std::vector<regex::character_type> alphabet = regex::get_alphabet( );

        switch( token.get_type( ) )
        {
            case regex::type::character:
                s_.push( nfa::from_character( token.get_token( ) ) );
                break;
            case regex::type::any:
                s_.push( nfa::from_any( ) );
                break;
            case regex::type::alternation:
                rhs = s_.pop( );
                lhs = s_.pop( );
                s_.push( nfa::from_alternation( lhs, rhs ) );
                break;
            case regex::type::concatenation:
                rhs = s_.pop( );
                lhs = s_.pop( );
                s_.push( nfa::from_concatenation( lhs, rhs ) );
                break;
            case regex::type::kleene:
                s_.push( nfa::from_kleene( s_.pop( ) ) );
                break;
            case regex::type::zero_or_one:
                s_.push( nfa::from_alternation( nfa::from_epsilon( ), s_.pop( ) ) );
                break;
            case regex::type::one_or_more:
                lhs = s_.pop( );
                rhs = lhs;
                s_.push( nfa::from_concatenation( lhs, nfa::from_kleene( rhs ) ) );
                break;
            case regex::type::parenthesis:
                break;
        }
    }

    std::shared_ptr<nfa> nfa_generator::result( )
    {
        return s_.pop( );
    }
}  // namespace fa
