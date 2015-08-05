#include <iostream>
#include <string>

#include "assert_reached.h"

ASSERT_REACHED_BEGIN(foo)
void foo(bool runBranch) {
    if (runBranch) {
        ASSERT_REACHED;
    } else {
        ASSERT_REACHED;
    }
}
ASSERT_REACHED_END

ASSERT_REACHED_BEGIN(bar)
void foo(bool runBranch) {
    if (! runBranch) {
        ASSERT_REACHED;
    } else {
        ASSERT_REACHED;
    }
}
ASSERT_REACHED_END

int main(int argc, char **argv) {
    if (argc == 2 && std::string(argv[1]) == "true") {
        foo::foo(true);
        bar::foo(true);
    } else {
        foo::foo(false);
        bar::foo(false);
    }
}
