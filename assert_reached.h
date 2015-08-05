#pragma once

#include "meta_list.hpp"
#include "meta_counter.hpp"

template <size_t>
struct AssertReachedGuard {
};

using Counter = atch::meta_counter<AssertReachedGuard<0>>;

#define ASSERT_REACHED(str) \
    do { \
        struct ARLine {  \
            static void check() { \
                if (AssertReachedGuard<Counter::value()>::reached[AssertReachedGuard<Counter::value()>::LX::value<>::size()]) { \
                    std::cerr << "Reached: " << str << " - " << __FILE__ << ":" << __LINE__ << "\n"; \
                } else { \
                    std::cerr << "Failed to reach: " << str << " - " << __FILE__ << ":" << __LINE__ << "\n"; \
                } \
            } \
        }; \
        AssertReachedGuard<Counter::value()>::reached[AssertReachedGuard<Counter::value()>::LX::value<>::size()] = true; \
        AssertReachedGuard<Counter::value()>::LX::push<ARLine>(); \
    } while(0)

#define ASSERT_REACHED_BEGIN \
template <> \
struct AssertReachedGuard<Counter::value()> { \
    using LX = atch::meta_list<AssertReachedGuard>; \
    ~AssertReachedGuard(); \
    static bool reached[]; \
    struct checkValues; \
};

#define ASSERT_REACHED_END \
bool AssertReachedGuard<Counter::value()>::reached[AssertReachedGuard<Counter::value()>::LX::value<>::size()]; \
struct AssertReachedGuard<Counter::value()>::checkValues { \
template <size_t N> \
static typename std::enable_if<AssertReachedGuard<Counter::value()>::LX::value<>::size() <= N>::type \
check() {} \
template <size_t N> \
static typename std::enable_if<N < AssertReachedGuard<Counter::value()>::LX::value<>::size()>::type \
check() { \
    AssertReachedGuard<Counter::value()>::LX::value<>::at<N>::result::check(); \
    check<N+1>(); \
} \
}; \
AssertReachedGuard<Counter::value()>::~AssertReachedGuard<Counter::value()>() { \
    checkValues::check<0>(); \
    Counter::next(); \
} \

#define ASSERT_REACHED_GUARD AssertReachedGuard<Counter::value()>
