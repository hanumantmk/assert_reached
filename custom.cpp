#include <iostream>
#include <string>
#include <stdexcept>

#include "assert_reached.h"

template <typename T>
class MyGuard : public T {
public:
    MyGuard(const char* str) : T(str) {}

    ~MyGuard() noexcept(false) override {
        this->cleanup();
    }

    void callback(const assert_reached::Line& line, bool hit) override {
        std::cerr << this->name() << "(" << this->str() << ") "
                  << (hit ? "Reached" : "Failed to reach") << ": " << line.str() << " - "
                  << line.file() << ":" << line.line() << "\n";
    }
};

ASSERT_REACHED_BEGIN(fileGuard)

namespace Foo {

ASSERT_REACHED_BEGIN(fooGuard)
void foo(bool runBranch) {
    MyGuard<fooGuard> magic("a");

    if (runBranch) {
        ASSERT_REACHED("a");
    } else {
        ASSERT_REACHED("b");
    }
}
ASSERT_REACHED_END

void multi1(bool flag) {
    if (flag)
        ASSERT_REACHED("c");
}
void multi2(bool flag) {
    if (!flag)
        ASSERT_REACHED("d");
}
}

int main(int argc, char** argv) {
    using namespace Foo;
    bool flag = (argc == 2 && std::string(argv[1]) == "true");

    MyGuard<fileGuard> x("a");

    {
        foo(flag);
        multi1(flag);

        MyGuard<fileGuard> y("b");
        multi2(flag);
    }

    try {
        fileGuard m("throwing");
    } catch (std::runtime_error& error) {
        std::cerr << error.what() << "\n";
    }

    try {
        fileGuard m("ignore");
        throw std::runtime_error("concurrent errors don't crash");
    } catch (std::runtime_error& error) {
        std::cerr << error.what() << "\n";
    }

    return 0;
}

ASSERT_REACHED_END
