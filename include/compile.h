#include <sstream>
#include <memory>

#include "nfa.h"
#include "ast.h"

/*
 * Compile the regular expression to its finite automaton
 */
std::shared_ptr<regex::nfa> compile(std::basic_stringstream<regex::character_type>&& pattern);