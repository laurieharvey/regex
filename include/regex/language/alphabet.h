#pragma once

#include <array>
#include <limits>

namespace regex::language
{
    using character_type = char;
    using operator_array_type = std::array<character_type, 7>;
    using alphabet_array_type = std::array<character_type, std::numeric_limits<character_type>::max() -
                                                               std::numeric_limits<character_type>::min() + 1>;

    extern const operator_array_type operators;

    extern const alphabet_array_type alphabet;
} // namespace regex::language