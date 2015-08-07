#pragma once

#include "meta_list.hpp"

#include <map>
#include <mutex>

namespace assert_reached {

class Line;
class Guard;

struct details {
    struct TypeStackMeta {};
    struct LinesMeta {};
    using TypeStack = atch::meta_list<TypeStackMeta>;
    using Lines = atch::meta_list<LinesMeta>;

    struct Instance {
        std::mutex mutex;
        size_t counter;
        std::map<size_t, Guard*> table;
    };

    template <typename T, typename List, size_t N, size_t M>
    static typename std::enable_if<M <= N>::type apply(T* t) {}

    template <typename T, typename List, size_t N, size_t M>
        static typename std::enable_if < N<M>::type apply(T* t) {
        t->template invoke<typename List::template value<>::template at<N>::result>();
        apply<T, List, N + 1, M>(t);
    }

    template <typename T, typename List>
    static void applyToList(T* t) {
        apply<T, List, 0, List::template value<>::size()>(t);
    }

    static void flag(size_t n);

    static Instance instance;
};

class Line {
public:
    virtual const char* str() const = 0;
    virtual const char* file() const = 0;
    virtual size_t line() const = 0;
};

class Guard {
public:
    Guard(const char* x);

    Guard(const Guard& guard) = delete;
    Guard(Guard&& guard) = delete;

    Guard& operator=(const Guard& guard) = delete;
    Guard& operator=(Guard&& guard) = delete;

    virtual ~Guard() noexcept(false);

    virtual const char* name() const = 0;
    virtual void cleanup() = 0;

    virtual void callback(const Line& line, bool hit);

    const char* str() const {
        return str_.c_str();
    }

    template <typename L>
    void invoke() {
        L line;
        size_t n = L::n();

        if (n < begin() || n >= end())
            return;

        this->callback(line, lines_[n]);
    }

    void flag(size_t n);

protected:
    virtual size_t begin() const = 0;
    virtual size_t end() const = 0;

private:
    std::map<size_t, bool> lines_;
    std::string str_;
    size_t id_;
};
}

#define ASSERT_REACHED(msg)                                                             \
    do {                                                                                \
        class ARLine : public assert_reached::Line {                                    \
        public:                                                                         \
            const char* str() const override {                                          \
                return msg;                                                             \
            }                                                                           \
            const char* file() const override {                                         \
                return __FILE__;                                                        \
            }                                                                           \
            size_t line() const override {                                              \
                return __LINE__;                                                        \
            }                                                                           \
            static size_t n() {                                                         \
                return assert_reached::details::Lines::value<>::size();                 \
            }                                                                           \
        };                                                                              \
        static_assert(assert_reached::details::TypeStack::value<>::size(),              \
                      "ASSERT_REACHED not within ASSERT_REACHED_BEGIN/END");            \
        assert_reached::details::flag(assert_reached::details::Lines::value<>::size()); \
        assert_reached::details::Lines::push<ARLine>();                                 \
    } while (0)

#define ASSERT_REACHED_BEGIN(type)                                           \
    namespace {                                                              \
    class type : public assert_reached::Guard {                              \
    public:                                                                  \
        type(const char* x) : assert_reached::Guard(x), cleaned_up(false) {} \
        ~type() noexcept(false) override {                                   \
            if (!cleaned_up)                                                 \
                cleanup();                                                   \
        }                                                                    \
        size_t begin() const override {                                      \
            return assert_reached::details::Lines::value<>::size();          \
        };                                                                   \
        size_t end() const override;                                         \
        const char* name() const override {                                  \
            return #type;                                                    \
        }                                                                    \
        void cleanup() final;                                                \
                                                                             \
    private:                                                                 \
        void extra() {                                                       \
            assert_reached::details::TypeStack::push<type>();                \
        }                                                                    \
        bool cleaned_up;                                                     \
    };                                                                       \
    }

#define ASSERT_REACHED_END                                                                 \
    static_assert(assert_reached::details::TypeStack::value<>::size(),                     \
                  "ASSERT_REACHED_END not matched with ASSERT_REACHED_BEGIN");             \
    size_t assert_reached::details::TypeStack::value<>::at<                                \
        assert_reached::details::TypeStack::value<>::size() - 1>::result::end() const {    \
        return assert_reached::details::Lines::value<>::size();                            \
    }                                                                                      \
    void assert_reached::details::TypeStack::value<>::at<                                  \
        assert_reached::details::TypeStack::value<>::size() - 1>::result::cleanup() {      \
        cleaned_up = true;                                                                 \
        assert_reached::details::applyToList<std::remove_reference<decltype(*this)>::type, \
                                             assert_reached::details::Lines>(this);        \
        assert_reached::details::TypeStack::pop();                                         \
    }
