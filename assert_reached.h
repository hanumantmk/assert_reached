#pragma once

#include "meta_list.hpp"

#define ASSERT_REACHED(str) \
    do { \
        struct ARLine {  \
            static size_t line() { return __LINE__; } \
            static const char* msg() { return str; } \
        }; \
        AR::reached[LX::value<>::size()] = true; \
        LX::push<ARLine>(); \
    } while(0)

#define ASSERT_REACHED_BEGIN(name) \
namespace name { \
struct AR { \
    static bool reached[]; \
    ~AR(); \
}; \
using LX = atch::meta_list<class Example>;

#define ASSERT_REACHED_END \
bool AR::reached[LX::value<>::size()]; \
template <size_t N> \
typename std::enable_if<LX::value<>::size() <= N>::type \
checkValues() { \
} \
template <size_t N> \
typename std::enable_if<N < LX::value<>::size()>::type \
checkValues() { \
    size_t line = LX::value<>::at<N>::result::line(); \
    const char* msg = LX::value<>::at<N>::result::msg(); \
    if (AR::reached[N]) { \
        std::cerr << "Reached: " << msg << " - " << __FILE__ << ":" << line << "\n"; \
    } else { \
        std::cerr << "Failed to reach: " << msg << " - " << __FILE__ << ":" << line << "\n"; \
    } \
    checkValues<N+1>(); \
} \
AR::~AR() { \
    checkValues<0>(); \
} \
AR magic{}; \
} \
