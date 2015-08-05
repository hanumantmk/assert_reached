#include <iostream>
#include <string>

#include "assert_reached.h"

namespace Foo {

ASSERT_REACHED_BEGIN
void foo(bool runBranch) {
    ASSERT_REACHED_GUARD magic{};

    if (runBranch) {
        ASSERT_REACHED("a");
    } else {
        ASSERT_REACHED("b");
    }
}
ASSERT_REACHED_END

ASSERT_REACHED_BEGIN
void bar(bool runBranch) {
    ASSERT_REACHED_GUARD magic{};

    if (! runBranch) {
        ASSERT_REACHED("c");
    } else {
        ASSERT_REACHED("d");
    }
}
ASSERT_REACHED_END

ASSERT_REACHED_BEGIN
void baz(bool runBranch) {
    ASSERT_REACHED_GUARD magic{};
}
ASSERT_REACHED_END

ASSERT_REACHED_BEGIN
ASSERT_REACHED_GUARD multi_magic{};
void multi1() {
    ASSERT_REACHED("e");
}
void multi2() {
    ASSERT_REACHED("f");
}
ASSERT_REACHED_END

}

int main(int argc, char **argv) {
    using namespace Foo;
    bool flag = (argc == 2 && std::string(argv[1]) == "true");

    foo(flag);
    bar(flag);
    baz(flag);

    if (flag) {
        multi1();
    } else {
        multi2();
    }
}
