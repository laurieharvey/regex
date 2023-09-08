#include <sstream>

#include "regex/automata/dfa.h"
#include "regex/automata/nfa.h"
#include "regex/language/ast.h"
#include "regex/language/parser.h"
#include "regex/utilities/compile.h"

namespace regex
{
    std::unique_ptr<regex::nfa> compile_nfa( std::basic_string_view<language::character_type> expression )
    {
        return compile_nfa( language::parse<pool_allocator<language::token>>( expression ) );
    }

    std::unique_ptr<regex::dfa> compile_dfa( std::basic_string_view<language::character_type> expression )
    {
        return compile_dfa( language::parse<pool_allocator<language::token>>( expression ) );
    }

    std::unique_ptr<regex::fa> compile( std::basic_string_view<language::character_type> expression, compile_flag flag )
    {
        if( flag == compile_flag::nfa )
        {
            return compile_nfa( expression );
        }
        else
        {
            return compile_dfa( expression );
        }
    }
} // namespace regex
