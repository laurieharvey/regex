#include "parser.h"

#include <sstream>

namespace regex
{
    bool is_operator( character_type token )
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

    bool is_unary_operator( character_type token )
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

    bool is_character( character_type token )
    {
        return !is_operator( token ) && token != 0;
    }

    std::basic_stringstream<character_type> make_explicit( std::basic_istream<character_type> &input )
    {
        character_type current_token, previous_token = 0;

        std::basic_stringstream<character_type> output;

        while( input.get( current_token ) )
        {
            if( ( is_character( current_token ) || current_token == '(' ) && ( is_unary_operator( previous_token ) || is_character( previous_token ) ) ) output.put( '-' );
            output.put( current_token );
            previous_token = current_token;
        }

        return output;
    }

    std::unique_ptr<regex::token> parse( std::basic_istream<character_type, std::char_traits<character_type>> &expression )
    {
        character_type token;
        stack<std::unique_ptr<regex::token>> output;
        stack<character_type> ops;

        std::unique_ptr<regex::token> lhs;
        std::unique_ptr<regex::token> rhs;

        while( expression.get( token ) )
        {
            if( token == '*' || token == '?' || token == '|' || token == '-' || token == '+' )
            {
                while( ops.size( ) && ops.top( ) != '(' && precedence[token] < precedence[ops.top( )] )
                {
                    switch( ops.top( ) )
                    {
                        case '*':
                            output.push( std::make_unique<kleene>( output.pop( ) ) );
                            break;
                        case '?':
                            output.push( std::make_unique<zero_or_one>( output.pop( ) ) );
                            break;
                        case '+':
                            output.push( std::make_unique<one_or_more>( output.pop( ) ) );
                            break;
                        case '-':
                            rhs = output.pop( );
                            lhs = output.pop( );
                            output.push( std::make_unique<concatenation>( std::move( lhs ), std::move( rhs ) ) );
                            break;
                        case '|':
                            rhs = output.pop( );
                            lhs = output.pop( );
                            output.push( std::make_unique<alternation>( std::move( lhs ), std::move( rhs ) ) );
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
                            output.push( std::make_unique<kleene>( output.pop( ) ) );
                            break;
                        case '?':
                            output.push( std::make_unique<zero_or_one>( output.pop( ) ) );
                            break;
                        case '+':
                            output.push( std::make_unique<one_or_more>( output.pop( ) ) );
                            break;
                        case '-':
                            rhs = output.pop( );
                            lhs = output.pop( );
                            output.push( std::make_unique<concatenation>( std::move( lhs ), std::move( rhs ) ) );
                            break;
                        case '|':
                            rhs = output.pop( );
                            lhs = output.pop( );
                            output.push( std::make_unique<alternation>( std::move( lhs ), std::move( rhs ) ) );
                            break;
                    }
                    ops.pop( );
                }
                output.push( std::make_unique<parenthesis>( output.pop( ) ) );
                ops.pop( );
            }
            else
            {
                if( token == '.' )
                {
                    output.push( std::make_unique<any>( ) );
                }
                else
                {
                    output.push( std::make_unique<character>( token ) );
                }
            }
        }
        while( ops.size( ) )
        {
            switch( ops.top( ) )
            {
                case '*':
                    output.push( std::make_unique<kleene>( output.pop( ) ) );
                    break;
                case '?':
                    output.push( std::make_unique<zero_or_one>( output.pop( ) ) );
                    break;
                case '+':
                    output.push( std::make_unique<one_or_more>( output.pop( ) ) );
                    break;
                case '-':
                    rhs = output.pop( );
                    lhs = output.pop( );
                    output.push( std::make_unique<concatenation>( std::move( lhs ), std::move( rhs ) ) );
                    break;
                case '|':
                    rhs = output.pop( );
                    lhs = output.pop( );
                    output.push( std::make_unique<alternation>( std::move( lhs ), std::move( rhs ) ) );
                    break;
                case ')':
                    output.push( std::make_unique<parenthesis>( output.pop( ) ) );
                    break;
                case '(':
                    output.push( std::make_unique<parenthesis>( output.pop( ) ) );
                    break;
            }
            ops.pop( );
        }

        return std::move( output.top( ) );
    }
}  // namespace regex