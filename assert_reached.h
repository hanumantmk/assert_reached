#pragma once

#include "meta_list.hpp"
#include <cstring>
#include <memory>
#include <vector>
#include <map>
#include <iostream>

struct Root {
    struct TypeStackMeta {};
    struct LinesMeta {};
    using TypeStack = atch::meta_list<TypeStackMeta>;
    using Lines = atch::meta_list<LinesMeta>;

    struct Guard {
        Guard(const char* x) :
            str(x)
        {
            id = Root::counter++;
            Root::table[id] = this;
        }

        Guard(const Guard& guard) = delete;
        Guard(Guard&& guard) = delete;

        Guard& operator=(const Guard& guard) = delete;
        Guard& operator=(Guard&& guard) = delete;

        virtual ~Guard() {
            Root::table.erase(id);
        }

        virtual size_t begin() = 0;
        virtual size_t end() = 0;
        virtual const char* name() = 0;

        template <typename Line>
        void invoke() {
            size_t n = Line::n();
            if (n < begin() || n >= end()) return;
            if (lines[n]) {
                std::cerr << name() << "(" << str << ") Reached: " << Line::str() << " - " << Line::file() << ":" << Line::line() << "\n";
            } else {
                std::cerr << name() << "(" << str << ") Failed to reach: " << Line::str() << " - " << Line::file() << ":" << Line::line() << "\n";
            }
        }

        void flag(size_t n) {
            if (n >= begin() && n < end()) {
                lines[n] = true;
            }
        }

        std::map<size_t, bool> lines;
        const char* str;
        size_t id;
    };

    struct details {
        template <typename T, typename List, size_t N, size_t M>
        static typename std::enable_if<M <= N>::type
        apply(T* t) {
        }

        template <typename T, typename List, size_t N, size_t M>
        static typename std::enable_if<N < M>::type
        apply(T* t) {
            t->template invoke<typename List::template value<>::template at<N>::result>();
            apply<T, List, N+1, M>(t);
        }

        template <typename T, typename List>
        static void applyToList(T* t) {
            apply<T, List, 0, List::template value<>::size()>(t);
        }
    };

    static void flag(size_t n) {
        for (auto&& it : table) {
            it.second->flag(n);
        }
    }

    static size_t counter;
    static std::map<size_t, Root::Guard*> table;
};

size_t Root::counter;
std::map<size_t, Root::Guard*> Root::table;

#define ASSERT_REACHED(msg) \
    do { \
        struct ARLine {  \
            static const char* str() { return msg; } \
            static const char* file() { return __FILE__; } \
            static size_t line() { return __LINE__; } \
            static size_t n() { return Root::Lines::value<>::size(); } \
        }; \
        static_assert(Root::TypeStack::value<>::size(), "ASSERT_REACHED not within ASSERT_REACHED_BEGIN/END"); \
        Root::flag(Root::Lines::value<>::size()); \
        Root::Lines::push<ARLine>(); \
    } while(0)

#define ASSERT_REACHED_BEGIN(type) \
namespace { \
struct type : public Root::Guard { \
    type(const char* x) : Root::Guard(x) {}\
    ~type() override { cleanup(); } \
    size_t begin() override { return Root::Lines::value<>::size(); }; \
    size_t end() override; \
    const char* name() override { return #type; } \
    void cleanup(); \
    void extra() { \
        Root::TypeStack::push<type>(); \
    } \
}; \
}

#define ARG Root::TypeStack::value<>::at<Root::TypeStack::value<>::size() - 1>::result

#define ASSERT_REACHED_END \
static_assert(Root::TypeStack::value<>::size(), "ASSERT_REACHED_END not matched with ASSERT_REACHED_BEGIN"); \
size_t ARG::end() { return Root::Lines::value<>::size(); } \
void ARG::cleanup() { \
    Root::details::applyToList<ARG, Root::Lines>(this); \
    Root::TypeStack::pop(); \
}
