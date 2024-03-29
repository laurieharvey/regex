# Regular Expression 

## Building and installing
```bash
$ git clone https://github.com/shug4070/regex.git
$ cd regex
$ cmake -S . -B build
$ cmake --build build --target install -- -j16
```
## Using the library

```cpp
#include <memory>
#include <sstream>
#include <regex/utilities/compile.h>

int main()
{
    std::unique_ptr<regex::fa> automata = regex::compile( std::stringstream( "a?.*(c*|d+)b*e" ), 
                                                          regex::compile_flag::dfa );

    automata->execute( "adbbe" ); // true
    automata->execute( "adbba" ); // false

    return 0;
}
```

## Using the binary

```cpp
$ regex -t dfa  '.*include.*' README.md
#include <memory>
#include <sstream>
#include <regex/utilities/compile.h>                                                       
```
