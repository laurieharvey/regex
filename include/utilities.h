#pragma once

#include <string>
#include <map>
#include <tuple>
#include <vector>
#include <utility>
#include <queue>

#include "state/state.h"
#include "automata/nfa.h"
#include "table.h"

// std::pair<regex::table, regex::group> generate_nfa_table(std::shared_ptr<regex::fa> nfa);

// std::pair<regex::table, regex::group> generate_dfa_table(std::shared_ptr<regex::fa> nfa);

std::pair<regex::table, regex::group> generate_table(std::shared_ptr<regex::fa> fa);