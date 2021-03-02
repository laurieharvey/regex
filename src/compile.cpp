#include <sstream>

#include "parser.h"
#include "fa.h"
#include "nfa.h"
#include "ast.h"

std::shared_ptr<regex::fa> compile(std::basic_stringstream<regex::character_type>&& pattern)
{
  std::basic_stringstream<regex::character_type> explicit_pattern = regex::make_explicit(pattern);
  std::unique_ptr<regex::token> ast = regex::parse(explicit_pattern);

  regex::generator g;

  ast->walk(std::bind(&regex::generator::callback, &g, std::placeholders::_1));
  
  return g.result();
}