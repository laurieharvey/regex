#pragma once

#include <sstream>
#include <memory>

#include "automata/fa.h"
#include "automata/nfa.h"
#include "automata/dfa.h"
#include "ast.h"

enum class compile_flag
{
    nfa,
    dfa,
};
/*
 * Compile the regular expression to its finite automaton
 */
std::shared_ptr<regex::fa> compile(std::basic_stringstream<regex::character_type> pattern, compile_flag flag);
/*
 * Compile the regular expression to its non-deterministic finite automaton
 */
std::shared_ptr<regex::nfa> compile_nfa(std::basic_stringstream<regex::character_type> pattern);
/*
 * Compile the regular expression to its deterministic finite automaton
 */
std::shared_ptr<regex::dfa> compile_dfa(std::basic_stringstream<regex::character_type> pattern);