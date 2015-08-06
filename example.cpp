#include <iostream>
#include <string>

#include "assert_reached.h"

ASSERT_REACHED_BEGIN(fileGuard)

namespace Foo {

ASSERT_REACHED_BEGIN(fooGuard)
void foo(bool runBranch) {
    fooGuard magic("a");

    if (runBranch) {
        ASSERT_REACHED("a");
    } else {
        ASSERT_REACHED("b");
    }
}
ASSERT_REACHED_END

void multi1(bool flag) {
    if (flag) ASSERT_REACHED("c");
}
void multi2(bool flag) {
    if (!flag) ASSERT_REACHED("d");
}

}

int main(int argc, char **argv) {
    using namespace Foo;
    bool flag = (argc == 2 && std::string(argv[1]) == "true");

    fileGuard x("a");

    {
        foo(flag);
        multi1(flag);

        fileGuard y("b");
        multi2(flag);
    }
}

ASSERT_REACHED_END
