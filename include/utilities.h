#pragma once

#include <map>
#include <queue>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "automata/nfa.h"
#include "state/state.h"
#include "table.h"

// std::pair<regex::table, regex::group> generate_nfa_table(std::shared_ptr<regex::fa> nfa);

// std::pair<regex::table, regex::group> generate_dfa_table(std::shared_ptr<regex::fa> nfa);

std::pair<regex::table, regex::group> generate_table( std::shared_ptr<regex::fa> fa );