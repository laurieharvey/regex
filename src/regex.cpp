#include <iostream>
#include <functional>
#include <sstream>

#include "parser.h"
#include "nfa.h"
#include "state.h"
#include "compile.h"

int main(int argc, char **argv)
{
  if (argc != 2)
    return 0;

  std::stringstream pattern(argv[1]);
  std::string input_line;

  std::shared_ptr<regex::nfa> nfa = compile(std::move(pattern));

  while (std::getline(std::cin, input_line))
  {
    regex::match result = nfa->run(input_line);

    if (result == regex::match::accepted)
    {
      std::cout << input_line << std::endl;
    }
  }

  return 0;
}