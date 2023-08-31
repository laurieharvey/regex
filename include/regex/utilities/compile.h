#pragma once

#include <memory>
#include <sstream>

#include "regex/automata/dfa.h"
#include "regex/automata/nfa.h"
#include "regex/language/alphabet.h"
#include "regex/language/ast.h"

namespace regex
{
    enum class compile_flag
    {
        nfa,
        dfa
    };
    /*
     * Compile the regular expression to its finite automaton
     */
    std::unique_ptr<fa> compile( std::unique_ptr<language::token> expression, compile_flag flag );
    /*
     * Compile the regular expression to its finite automaton
     */
    std::unique_ptr<regex::fa> compile( std::basic_string_view<language::character_type> expression, compile_flag flag );
} // namespace regex
