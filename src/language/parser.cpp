#include "language/parser.h"

#include <sstream>
#include <stack>

namespace regex
{
    namespace language
    {
        static const std::array<short, 128> precedence
        {

            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 2, 2, 0, 1, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        };

        constexpr static bool is_operator( language::character_type token )
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

        constexpr static bool is_unary_operator( language::character_type token )
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

        constexpr static bool is_character( language::character_type token )
        {
            return !is_operator( token ) && token != 0;
        }

        static std::basic_stringstream<language::character_type> make_explicit( std::basic_istream<language::character_type, std::char_traits<language::character_type>> &&input )
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

        std::unique_ptr<language::token> parse( std::basic_istream<language::character_type, std::char_traits<language::character_type>> &&expression )
        {
            auto explicit_pattern = make_explicit( std::move( expression ) );

            language::character_type token;
            std::stack<std::unique_ptr<language::token>> output;
            std::stack<language::character_type> ops;

            auto check_args = [ &output ]( short args_needed ){

                if( output.size( ) < args_needed )
                {
                    throw exception();
                }

            };

            std::unique_ptr<language::token> lhs, rhs;

            while( explicit_pattern.get( token ) )
            {
                if( token == '*' || token == '?' || token == '|' || token == '-' || token == '+' )
                {
                    while( ops.size( ) && ops.top( ) != '(' && precedence[token] < precedence[ops.top( )] )
                    {
                        switch( ops.top( ) )
                        {
                            case '*':
                                check_args( 1 );
                                lhs = std::move( output.top( ) );
                                output.pop( );
                                output.push( std::make_unique<language::kleene>( std::move( lhs ) ) );
                                break;
                            case '?':
                                check_args( 1 );
                                lhs = std::move( output.top( ) );
                                output.pop( );
                                output.push( std::make_unique<language::zero_or_one>( std::move( lhs ) ) );
                                break;
                            case '+':
                                check_args( 1 );                            
                                lhs = std::move( output.top( ) );
                                output.pop( );
                                output.push( std::make_unique<language::one_or_more>( std::move( lhs ) ) );
                                break;
                            case '-':
                                check_args( 2 );                            
                                rhs = std::move( output.top( ) );
                                output.pop( );
                                lhs = std::move( output.top( ) );
                                output.pop( );
                                output.push( std::make_unique<language::concatenation>( std::move( lhs ), std::move( rhs ) ) );
                                break;
                            case '|':
                                check_args( 2 );                            
                                rhs = std::move( output.top( ) );
                                output.pop( );
                                lhs = std::move( output.top( ) );
                                output.pop( );
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
                                check_args( 1 );                            
                                lhs = std::move( output.top( ) );
                                output.pop( );
                                output.push( std::make_unique<language::kleene>( std::move( lhs ) ) );
                                break;
                            case '?':
                                check_args( 1 );                            
                                lhs = std::move( output.top( ) );
                                output.pop( );
                                output.push( std::make_unique<language::zero_or_one>( std::move( lhs ) ) );
                                break;
                            case '+':
                                check_args( 1 );                            
                                lhs = std::move( output.top( ) );
                                output.pop( );
                                output.push( std::make_unique<language::one_or_more>( std::move( lhs ) ) );
                                break;
                            case '-':
                                check_args( 2 );                            
                                rhs = std::move( output.top( ) );
                                output.pop( );
                                lhs = std::move( output.top( ) );
                                output.pop( );
                                output.push( std::make_unique<language::concatenation>( std::move( lhs ), std::move( rhs ) ) );
                                break;
                            case '|':
                                check_args( 2 );
                                rhs = std::move( output.top( ) );
                                output.pop( );
                                lhs = std::move( output.top( ) );
                                output.pop( );
                                output.push( std::make_unique<language::alternation>( std::move( lhs ), std::move( rhs ) ) );
                                break;
                        }
                        ops.pop( );
                    }
                    lhs = std::move( output.top( ) );
                    output.pop( );
                    output.push( std::make_unique<language::parenthesis>( std::move( lhs ) ) );
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
                        check_args( 1 );                    
                        lhs = std::move( output.top( ) );
                        output.pop( );
                        output.push( std::make_unique<language::kleene>( std::move( lhs ) ) );
                        break;
                    case '?':
                        check_args( 1 );                    
                        lhs = std::move( output.top( ) );
                        output.pop( );
                        output.push( std::make_unique<language::zero_or_one>( std::move( lhs ) ) );
                        break;
                    case '+':
                        check_args( 1 );                    
                        lhs = std::move( output.top( ) );
                        output.pop( );
                        output.push( std::make_unique<language::one_or_more>( std::move( lhs ) ) );
                        break;
                    case '-':
                        check_args( 2 );                    
                        rhs = std::move( output.top( ) );
                        output.pop( );
                        lhs = std::move( output.top( ) );
                        output.pop( );
                        output.push( std::make_unique<language::concatenation>( std::move( lhs ), std::move( rhs ) ) );
                        break;
                    case '|':
                        check_args( 2 );                    
                        rhs = std::move( output.top( ) );
                        output.pop( );
                        lhs = std::move( output.top( ) );
                        output.pop( );
                        output.push( std::make_unique<language::alternation>( std::move( lhs ), std::move( rhs ) ) );
                        break;
                    case ')':
                        check_args( 1 );                    
                        lhs = std::move( output.top( ) );
                        output.pop( );
                        output.push( std::make_unique<language::parenthesis>( std::move( lhs ) ) );
                        break;
                    case '(':
                        check_args( 1 );                    
                        lhs = std::move( output.top( ) );
                        output.pop( );
                        output.push( std::make_unique<language::parenthesis>( std::move( lhs ) ) );
                        break;
                }
                ops.pop( );
            }

            if( output.size() == 0 )
            {
                throw exception();
            }

            return std::move( output.top( ) );
        }
    }
}