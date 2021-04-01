#include "parser.h"

#include <sstream>

namespace regex
{
    bool is_operator( language::character_type token )
    {
        switch( token )
        {
            case '*':
            case '?':
            case '+':
            case '-':
            case '|':
            case '(':
            case ')':
                return true;
            default:
                return false;
        }
    }

    bool is_unary_operator( language::character_type token )
    {
        switch( token )
        {
            case '*':
            case '?':
            case '+':
            case '-':
            case ')':
                return true;
            default:
                return false;
        }
    }

    bool is_character( language::character_type token )
    {
        return !is_operator( token ) && token != 0;
    }

    std::basic_stringstream<language::character_type> make_explicit( std::basic_istream<language::character_type> &input )
    {
        language::character_type current_token, previous_token = 0;

        std::basic_stringstream<language::character_type> output;

        while( input.get( current_token ) )
        {
            if( ( is_character( current_token ) || current_token == '(' ) && ( is_unary_operator( previous_token ) || is_character( previous_token ) ) ) output.put( '-' );
            output.put( current_token );
            previous_token = current_token;
        }

        return output;
    }

    std::unique_ptr<language::token> parse( std::basic_istream<language::character_type, std::char_traits<language::character_type>> &expression )
    {
        language::character_type token;
        stack<std::unique_ptr<language::token>> output;
        stack<language::character_type> ops;

        std::unique_ptr<language::token> lhs;
        std::unique_ptr<language::token> rhs;

        while( expression.get( token ) )
        {
            if( token == '*' || token == '?' || token == '|' || token == '-' || token == '+' )
            {
                while( ops.size( ) && ops.top( ) != '(' && precedence[token] < precedence[ops.top( )] )
                {
                    switch( ops.top( ) )
                    {
                        case '*':
                            output.push( std::make_unique<language::kleene>( output.pop( ) ) );
                            break;
                        case '?':
                            output.push( std::make_unique<language::zero_or_one>( output.pop( ) ) );
                            break;
                        case '+':
                            output.push( std::make_unique<language::one_or_more>( output.pop( ) ) );
                            break;
                        case '-':
                            rhs = output.pop( );
                            lhs = output.pop( );
                            output.push( std::make_unique<language::concatenation>( std::move( lhs ), std::move( rhs ) ) );
                            break;
                        case '|':
                            rhs = output.pop( );
                            lhs = output.pop( );
                            output.push( std::make_unique<language::alternation>( std::move( lhs ), std::move( rhs ) ) );
                            break;
                    }
                    ops.pop( );
                }
                ops.push( token );
            }
            else if( token == '(' )
            {
                ops.push( token );
            }
            else if( token == ')' )
            {
                while( ops.top( ) != '(' )
                {
                    switch( ops.top( ) )
                    {
                        case '*':
                            output.push( std::make_unique<language::kleene>( output.pop( ) ) );
                            break;
                        case '?':
                            output.push( std::make_unique<language::zero_or_one>( output.pop( ) ) );
                            break;
                        case '+':
                            output.push( std::make_unique<language::one_or_more>( output.pop( ) ) );
                            break;
                        case '-':
                            rhs = output.pop( );
                            lhs = output.pop( );
                            output.push( std::make_unique<language::concatenation>( std::move( lhs ), std::move( rhs ) ) );
                            break;
                        case '|':
                            rhs = output.pop( );
                            lhs = output.pop( );
                            output.push( std::make_unique<language::alternation>( std::move( lhs ), std::move( rhs ) ) );
                            break;
                    }
                    ops.pop( );
                }
                output.push( std::make_unique<language::parenthesis>( output.pop( ) ) );
                ops.pop( );
            }
            else
            {
                if( token == '.' )
                {
                    output.push( std::make_unique<language::any>( ) );
                }
                else
                {
                    output.push( std::make_unique<language::character>( token ) );
                }
            }
        }
        while( ops.size( ) )
        {
            switch( ops.top( ) )
            {
                case '*':
                    output.push( std::make_unique<language::kleene>( output.pop( ) ) );
                    break;
                case '?':
                    output.push( std::make_unique<language::zero_or_one>( output.pop( ) ) );
                    break;
                case '+':
                    output.push( std::make_unique<language::one_or_more>( output.pop( ) ) );
                    break;
                case '-':
                    rhs = output.pop( );
                    lhs = output.pop( );
                    output.push( std::make_unique<language::concatenation>( std::move( lhs ), std::move( rhs ) ) );
                    break;
                case '|':
                    rhs = output.pop( );
                    lhs = output.pop( );
                    output.push( std::make_unique<language::alternation>( std::move( lhs ), std::move( rhs ) ) );
                    break;
                case ')':
                    output.push( std::make_unique<language::parenthesis>( output.pop( ) ) );
                    break;
                case '(':
                    output.push( std::make_unique<language::parenthesis>( output.pop( ) ) );
                    break;
            }
            ops.pop( );
        }

        return std::move( output.top( ) );
    }
}