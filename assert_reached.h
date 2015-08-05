#pragma once

#include "meta_list.hpp"

#define ASSERT_REACHED(str) \
    do { \
        struct ARLine {  \
            static void check() { \
                if (AR::reached[LX::value<>::size()]) { \
                    std::cerr << "Reached: " << str << " - " << __FILE__ << ":" << __LINE__ << "\n"; \
                } else { \
                    std::cerr << "Failed to reach: " << str << " - " << __FILE__ << ":" << __LINE__ << "\n"; \
                } \
            } \
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
using LX = atch::meta_list<AR>;

#define ASSERT_REACHED_END \
bool AR::reached[LX::value<>::size()]; \
template <size_t N> \
void checkValues() { \
    LX::value<>::at<N>::result::check(); \
    checkValues<N+1>(); \
} \
template <> \
void checkValues<LX::value<>::size()>() { \
} \
AR::~AR() { \
    checkValues<0>(); \
} \
} \
