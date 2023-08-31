#pragma once

#include <array>
#include <exception>
#include <istream>
#include <memory>
#include <ostream>
#include <string>

#include "regex/language/alphabet.h"

namespace regex::language
{
    extern const std::array<short, 128> precedence;

    constexpr bool is_operator( language::character_type token );

    constexpr bool is_unary_operator( language::character_type token );

    using istream = std::basic_istream<language::character_type, std::char_traits<language::character_type>>;

    std::basic_string<character_type> make_explicit( std::basic_string_view<character_type> expression );

} // namespace regex::language