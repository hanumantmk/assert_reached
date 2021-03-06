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
