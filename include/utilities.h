#include <string>
#include <map>
#include <vector>

#include "state.h"
#include "nfa.h"

// {
//   '1': { 'ε*': [ 1, 2, 5 ] },
//   '2': { a: [ 3 ], 'ε*': [ 2 ] },
//   '3': { 'ε*': [ 3, 4 ] },
//   '4': { 'ε*': [ 4 ] },
//   '5': { b: [ 6 ], 'ε*': [ 5 ] },
//   '6': { 'ε*': [ 6, 4 ] },
// }
template<typename CharT>
std::map<fa::state<CharT>*, std::map<CharT, std::set<std::weak_ptr<fa::state<CharT>>, std::owner_less<std::weak_ptr<fa::state<CharT>>>>>> generate_nfa_table(std::shared_ptr<fa::nfa<CharT>> nfa)
{
    std::map<fa::state<CharT>*, std::map<CharT, std::set<std::weak_ptr<fa::state<CharT>>>>> table;

    nfa->walk([table&](const state<CharT> & state){

        if( table.find( &state ) != std::end( table ) )
        {
            return;
        }
        else
        {
            table[&state] = state->get_transitions();
        }        

    })
}
