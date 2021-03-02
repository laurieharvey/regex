#include <sstream>
#include <memory>

#include "fa.h"
#include "ast.h"

/*
 * Compile the regular expression to its finite automaton
 */
std::shared_ptr<regex::fa> compile(std::basic_stringstream<regex::character_type>&& pattern);