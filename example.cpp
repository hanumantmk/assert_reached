#include <iostream>
#include <string>

#include "assert_reached.h"

ASSERT_REACHED_BEGIN
void foo(bool runBranch) {
    if (runBranch) {
        ASSERT_REACHED;
    } else {
        ASSERT_REACHED;
    }
}
ASSERT_REACHED_END

int main(int argc, char **argv) {
    if (argc == 2 && std::string(argv[1]) == "true") {
        foo(true);
    } else {
        foo(false);
    }
}
