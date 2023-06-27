#include <iostream>
#include <memory>
#include <queue>

#include "regex/automata/dfa.h"
#include "regex/state/dstate.h"

namespace regex
{

    dfa::dfa( state::dstate *input, const state::dstate::group_type &outputs,
              std::set<std::unique_ptr<state::dstate>> states )
        : states_( std::move( states ) ), input_( input ), outputs_( outputs )
    {
    }

    bool dfa::execute( std::basic_string_view<language::character_type> target )
    {
        return regex::state::execute( input_, outputs_, target );
    }
} // namespace regex