#pragma once

#include <sstream>
#include <memory>

#include "automata/fa.h"
#include "automata/nfa.h"
#include "automata/dfa.h"
#include "ast.h"

enum compile_flag
{
    COMPILE_NFA = 0x01,
    COMPILE_DFA = 0x02,
};
using compile_flags = std::underlying_type<compile_flag>::type;
/*
 * Compile the regular expression to its finite automaton
 */
std::shared_ptr<regex::fa> compile(std::basic_stringstream<regex::character_type> pattern, compile_flags flags = COMPILE_NFA);
/*
 * Compile the regular expression to its non-deterministic finite automaton
 */
std::shared_ptr<regex::nfa> compile_nfa(std::basic_stringstream<regex::character_type> pattern);
/*
 * Compile the regular expression to its deterministic finite automaton
 */
std::shared_ptr<regex::dfa> compile_dfa(std::basic_stringstream<regex::character_type> pattern);