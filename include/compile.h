#include <sstream>
#include <memory>

#include "nfa.h"
#include "ast.h"

/*
 * Compile the regular expression to its finite automaton
 */
std::shared_ptr<fa::nfa> compile(std::basic_stringstream<ast::character_type>&& pattern);