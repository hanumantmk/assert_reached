Terrible abuse of C++14 to enable an ASSERT_REACHED macro.  Specifically, we're plugging into stateful constexpr meta containers to gather type information on lines we don't actually execute.

Provides access to ASSERT_REACHED and ASSERT_REACHED_BEGIN/END, which can be used as follows:

```c++
#include <iostream>

#include "assert_reached.h"

ASSERT_REACHED_BEGIN(fooGuard)
void foo(bool a, bool b) { 
    if (a) { 
        ASSERT_REACHED("a");
    } 
    if (b) { 
        ASSERT_REACHED("b");
    } 
}
ASSERT_REACHED_END

int main() { 
    { 
        fooGuard guard("success");
        foo(true, true);
    } 

    try {  
        fooGuard guard("b failure");
        foo(true, false);
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
    } 

    try {  
        fooGuard guard("a failure");
        foo(false, true);
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
    } 
}
```

Which will produce results like:

```
fooGuard(b failure) Failed to reach : b - example.cpp:11
fooGuard(a failure) Failed to reach : a - example.cpp:8
```

See custom.cpp for an example with custom error reporting and more elaborate use cases

...

Implemented on top of: http://b.atch.se/posts/constexpr-meta-container/
