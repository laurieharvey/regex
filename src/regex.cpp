#include <iostream>
#include <functional>
#include <sstream>

#include "parser.h"
#include "nfa.h"

int main()
{
  std::stringstream regex = std::stringstream( "ab*|c+" );
  regex = ast::make_explicit( regex );

  auto ast = ast::parse(regex);

  fa::generator<char> g;

  ast->walk(std::bind(&fa::generator<char>::callback, &g, std::placeholders::_1));
  auto n = g.result();

  auto result = n->run("cc");

  if( result == fa::match::accepted )
    std::cout << "Accepted" << std::endl;
  else
  {
    std::cout << "Rejected" << std::endl;
  }
  

  return 0;
}