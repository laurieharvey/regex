#include <sstream>

#include "compile.h"
#include "parser.h"
#include "fa.h"
#include "nfa.h"
#include "dfa.h"
#include "ast.h"

std::shared_ptr<regex::fa> compile(std::basic_stringstream<regex::character_type>&& pattern, compile_flags flags)
{
  std::basic_stringstream<regex::character_type> explicit_pattern = regex::make_explicit(pattern);
  std::unique_ptr<regex::token> ast = regex::parse(explicit_pattern);

  regex::generator g;

  ast->walk(std::bind(&regex::generator::callback, &g, std::placeholders::_1));
  
  std::shared_ptr<regex::nfa> non_deterministic = g.result();
  
  return non_deterministic;
}