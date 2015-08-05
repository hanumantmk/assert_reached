#include <iostream>
#include <string>

#include "assert_reached.h"

ASSERT_REACHED_BEGIN

namespace Foo {

ASSERT_REACHED_BEGIN
void foo(bool runBranch) {
    ASSERT_REACHED_GUARD(magic);

    if (runBranch) {
        ASSERT_REACHED("a");
    } else {
        ASSERT_REACHED("b");
    }
}
ASSERT_REACHED_END

ASSERT_REACHED_GUARD(multi_magic);
void multi1(bool flag) {
    if (flag) ASSERT_REACHED("c");
}
void multi2(bool flag) {
    if (!flag) ASSERT_REACHED("d");
}

}

ASSERT_REACHED_END

int main(int argc, char **argv) {
    using namespace Foo;
    bool flag = (argc == 2 && std::string(argv[1]) == "true");

    foo(flag);
    multi1(flag);
    multi2(flag);
}
