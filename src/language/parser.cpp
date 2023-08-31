#include <sstream>
#include <stack>

#include "regex/language/parser.h"

namespace regex::language
{
    const std::array<short, 128> precedence{

        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 2, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    };

    constexpr bool is_operator( language::character_type token )
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

    constexpr bool is_unary_operator( language::character_type token )
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

    std::basic_string<character_type> make_explicit( std::basic_string_view<character_type> expression )
    {
        character_type pcharacter = 0;
        std::basic_string<character_type> output;

        for( char character : expression )
        {
            if( ( is_character( character ) || character == '(' ) &&
                ( is_unary_operator( pcharacter ) || is_character( pcharacter ) ) )
            {
                output.push_back( '-' );
            }

            output.push_back( character );
            pcharacter = character;
        }

        return output;
    }
} // namespace regex::language