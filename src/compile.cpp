#include <sstream>

#include "parser.h"
#include "nfa.h"
#include "ast.h"

std::shared_ptr<fa::nfa> compile(std::basic_stringstream<ast::character_type>&& pattern)
{
  std::basic_stringstream<ast::character_type> explicit_pattern = ast::make_explicit(pattern);
  std::unique_ptr<ast::token> ast = ast::parse(explicit_pattern);

  fa::generator g;

  ast->walk(std::bind(&fa::generator::callback, &g, std::placeholders::_1));
  
  return g.result();
}