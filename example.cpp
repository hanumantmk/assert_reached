#include <iostream>
#include <string>

#include "assert_reached.h"

ASSERT_REACHED_BEGIN(foo)
void foo(bool runBranch) {
    AR magic{};

    if (runBranch) {
        ASSERT_REACHED("a");
    } else {
        ASSERT_REACHED("b");
    }
}
ASSERT_REACHED_END

ASSERT_REACHED_BEGIN(bar)
void foo(bool runBranch) {
    AR magic{};

    if (! runBranch) {
        ASSERT_REACHED("c");
    } else {
        ASSERT_REACHED("d");
    }
}
ASSERT_REACHED_END

ASSERT_REACHED_BEGIN(baz)
void foo(bool runBranch) {
    AR magic{};
}
ASSERT_REACHED_END

int main(int argc, char **argv) {
    if (argc == 2 && std::string(argv[1]) == "true") {
        foo::foo(true);
        bar::foo(true);
        baz::foo(true);
    } else {
        foo::foo(false);
        bar::foo(false);
        baz::foo(false);
    }
}
