#pragma once

#include "meta_list.hpp"

#define ASSERT_REACHED(str) \
    do { \
        struct ARLine {  \
            static void check() { \
                if (AssertReachedGuard::reached[AssertReachedGuard::LX::value<>::size()]) { \
                    std::cerr << "Reached: " << str << " - " << __FILE__ << ":" << __LINE__ << "\n"; \
                } else { \
                    std::cerr << "Failed to reach: " << str << " - " << __FILE__ << ":" << __LINE__ << "\n"; \
                } \
            } \
        }; \
        AssertReachedGuard::reached[AssertReachedGuard::LX::value<>::size()] = true; \
        AssertReachedGuard::LX::push<ARLine>(); \
    } while(0)

#define ASSERT_REACHED_BEGIN(name) \
namespace name { \
struct AssertReachedGuard { \
    using LX = atch::meta_list<AssertReachedGuard>; \
    ~AssertReachedGuard(); \
    static bool reached[]; \
};

#define ASSERT_REACHED_END \
bool AssertReachedGuard::reached[AssertReachedGuard::LX::value<>::size()]; \
template <size_t N> \
void checkValues() { \
    AssertReachedGuard::LX::value<>::at<N>::result::check(); \
    checkValues<N+1>(); \
} \
template <> \
void checkValues<AssertReachedGuard::LX::value<>::size()>() { \
} \
AssertReachedGuard::~AssertReachedGuard() { \
    checkValues<0>(); \
} \
}
