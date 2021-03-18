#include <sstream>

#include "compile.h"
#include "parser.h"
#include "automata/fa.h"
#include "automata/nfa.h"
#include "automata/dfa.h"
#include "ast.h"

std::shared_ptr<regex::fa> compile(std::basic_stringstream<regex::character_type> pattern, compile_flags flags)
{
  return compile_nfa(std::move(pattern));
}

std::shared_ptr<regex::nfa> compile_nfa(std::basic_stringstream<regex::character_type> pattern)
{
  std::basic_stringstream<regex::character_type> explicit_pattern = regex::make_explicit(pattern);
  std::unique_ptr<regex::token> ast = regex::parse(explicit_pattern);

  regex::nfa_generator g;

  ast->walk(std::bind(&regex::nfa_generator::callback, &g, std::placeholders::_1));

  return g.result();
}

std::shared_ptr<regex::dfa> compile_dfa(std::basic_stringstream<regex::character_type> pattern)
{
  return std::shared_ptr<regex::dfa>();
}