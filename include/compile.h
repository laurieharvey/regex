#include <sstream>

#include "parser.h"
#include "nfa.h"
#include "ast.h"

std::shared_ptr<fa::nfa> compile(std::basic_stringstream<ast::character_type>&& pattern);