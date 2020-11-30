#include <sstream>

#include "parser.h"
#include "nfa.h"
#include "ast.h"

template<typename CharT>
std::shared_ptr<fa::nfa<CharT>> compile(std::basic_stringstream<CharT> pattern)
{
  std::string input_line;

  std::basic_stringstream<CharT> explicit_pattern = ast::make_explicit(pattern);
  std::unique_ptr<ast::token<CharT>> ast = ast::parse(explicit_pattern);

  fa::generator<CharT> g;

  ast->walk(std::bind(&fa::generator<CharT>::callback, &g, std::placeholders::_1));
  
  return g.result();
}