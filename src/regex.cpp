#include <iostream>
#include <functional>
#include <sstream>

#include "parser.h"
#include "nfa.h"

int main(int argc, char **argv)
{
  if (argc != 2)
    return 0;

  std::istringstream pattern(argv[1]);
  std::string input_line;

  std::stringstream regex = ast::make_explicit(pattern);
  std::unique_ptr<ast::token<std::stringstream::char_type>> ast = ast::parse(regex);

  fa::generator<std::stringstream::char_type> g;

  ast->walk(std::bind(&fa::generator<char>::callback, &g, std::placeholders::_1));
  std::shared_ptr<fa::nfa<std::stringstream::char_type>> n = g.result();

  while (std::getline(std::cin, input_line))
  {
    fa::match result = n->run(input_line);

    if (result == fa::match::accepted)
    {
      std::cout << input_line << std::endl;
    }
  }

  return 0;
}