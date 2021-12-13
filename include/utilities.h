#pragma once

#include <map>
#include <queue>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "automata/dfa.h"
#include "automata/nfa.h"
#include "state/state.h"
#include "table.h"

regex::table<regex::group> generate_nfa_table(std::shared_ptr<regex::nfa> fa);

regex::table<std::shared_ptr<regex::dstate>> generate_dfa_table(std::shared_ptr<regex::dfa> fa);