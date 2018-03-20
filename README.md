# Regex literals

[**Flags**](#flags)<br>
[**Saving string in match results**](#saving-string-in-match-results)<br>
[**Examples**](#examples)<br>

regex_literal is a templated user defined literals wrapper for [std::regex](http://en.cppreference.com/w/cpp/regex/basic_regex). "[regex_search](http://en.cppreference.com/w/cpp/regex/regex_match)" and "[regex_match](http://en.cppreference.com/w/cpp/regex/regex_search)" functions are packed into a new structure 'struct_rm' to be able to write regex request in only one line.

Both char and wchar_t are compatible. It can be extended using struct_rm template if needed.

To use regex_literals, include the header, type `using namespace regex_literals;` and use `_rg` after a string literal to transform it to a struct_rm.

## Flags

struct_rm supports regex flags as `"/regex/flags"_rg` which follows [std::regex_constants::syntax_option_type](http://en.cppreference.com/w/cpp/regex/syntax_option_type) flags. Here is available flags:

* `i` : icase
* `b` : nosubs
* `o` : optimize
* `c` : collate
* `E` : ECMAScript
* `B` : basic
* `X` : extended
* `A` : awk
* `G` : grep
* `P` : egrep

C++17 flags compatibility: 

* `m` : multiline

Flags can be disabled using `#define REGEX_LITERALS_FLAGS 0` before including regex_literals.

## Saving string in match results

Another feature is the capability to save string into match_results with `match_results_ws` so a regex request doesn't need to get a string reference for match results (well... it still needs string reference but in this case, it's stored in match_results).

## Examples

```cpp
#include <iostream>
#include "regex_literals.h"
int main()
{
    using namespace regex_literals;
    /// simple match
    if ("abc"_rg.match("abc"))
        std::cout << "abc=abc: ok" << std::endl;
    /// simple case sensitive match  
    if ("abc"_rg.match("ABC"))
        std::cout << "abc=ABC: ok"<< std::endl;
    /// simple match with case insensitive flag
    if ("/abc/i"_rg.match("ABC"))
        std::cout << "abc=ABC (insensitive case): ok"<< std::endl;
    /// search 
    if ("/abc/i"_rg.search("xyzABC123"))
        std::cout << "xyzABC123 contains abc (insensitive case): ok"<< std::endl;

    /// get results
    std::smatch sm;
    std::string mystr="     xyzABC123  ";
    if (R"(/\s*(\w+)\s*/i)"_rg.search(mystr, sm))
        std::cout << "trim word: ok: '" << sm[1] << "'" << std::endl;
    
    /// save string in match results and get results
    match_results_ws<char> sm_ws;
    if (R"(\s*(\w+)\s*)"_rg.search("    mystr       ", sm_ws))
        std::cout << "trim word: ok: '" << sm_ws[1] << "'" << std::endl;
    if (R"(\s*(\w+)\s*)"_rg.search("    new_str       ", sm_ws))
        std::cout << "trim word: ok: '" << sm_ws[1] << "'" << std::endl;
    return 0;
}
```