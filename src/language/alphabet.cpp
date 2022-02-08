#include "language/alphabet.h"

#include <algorithm>

namespace regex
{
    namespace language
    {
        extern const operator_array_type operators = { '(', ')', '*', '?', '+', '.', '|' };

        static constexpr const alphabet_array_type generate_alphabet()
        {
            alphabet_array_type alphabet;
            alphabet_array_type::size_type index( 0 );

            for ( auto i = std::numeric_limits<character_type>::min(); i < std::numeric_limits<character_type>::max();
                  ++i )
            {
                if ( std::find( std::cbegin( operators ), std::cend( operators ), i ) == std::cend( operators ) )
                {
                    alphabet[index++] = i;
                }
            }

            if ( std::find( std::cbegin( operators ), std::cend( operators ),
                            std::numeric_limits<character_type>::max() ) == std::cend( operators ) )
            {
                alphabet[index] = std::numeric_limits<character_type>::max();
                ++index;
            }

            return alphabet;
        }

        extern const alphabet_array_type alphabet = generate_alphabet();
    } // namespace language
} // namespace regex