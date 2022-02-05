#pragma once

#include <memory>
#include <sstream>

#include "automata/dfa.h"
#include "automata/nfa.h"
#include "language/alphabet.h"
#include "language/ast.h"

namespace regex {
enum class compile_flag {
  nfa,
  dfa,
};
/*
 * Compile the regular expression to its finite automaton
 */
// std::unique_ptr<fa> compile(std::unique_ptr<language::token> expression, compile_flag flag);
/*
 * Compile the regular expression to its finite automaton
 */
// std::unique_ptr<fa> compile(std::basic_stringstream<language::character_type> expression,
//                             compile_flag flag);
/*
 * Compile the regular expression to its non-deterministic finite automaton
 */
std::unique_ptr<nfa> compile_nfa(std::unique_ptr<language::token> expression);
/*
 * Compile the regular expression to its non-deterministic finite automaton
 */
std::unique_ptr<nfa> compile_nfa(std::basic_stringstream<language::character_type> expression);
/*
 * Compile the regular expression to its deterministic finite automaton
 */
std::unique_ptr<dfa> compile_dfa(std::unique_ptr<language::token> expression);
/*
 * Compile the regular expression to its deterministic finite automaton
 */
std::unique_ptr<dfa> compile_dfa(std::basic_stringstream<language::character_type> expression);
}  // namespace regex