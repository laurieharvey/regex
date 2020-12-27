#include <iostream>
#include <functional>
#include <sstream>

#include "parser.h"
#include "nfa.h"
#include "state.h"
#include "compile.h"
#include "utilities.h"

int main(int argc, char **argv)
{
  if (argc != 2)
    return 0;

  std::stringstream pattern(argv[1]);
  std::string input_line;

  std::shared_ptr<fa::nfa<std::stringstream::char_type>> nfa = compile(std::move(pattern));

  auto table = generate_nfa_table(nfa);
  print_nfa_table( table.first );

  while (std::getline(std::cin, input_line))
  {
    fa::match result = nfa->run(input_line);

    if (result == fa::match::accepted)
    {
      std::cout << input_line << std::endl;
    }
  }

  return 0;
}