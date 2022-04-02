# Regular Expression 

## Using the library

```cpp
#include <sstream>
#include <memory>
#include "regex/utilities/compile.h"

std::unique_ptr<fa> automata = regex::compile( std::stringstream( "a?.*(c*|d+)b*e" ), 
                                               regex::compile_flag::dfa );

automata->execute( "adbbe" ); // true
automata->execute( "adbba" ); // false
```

## Using the binary

