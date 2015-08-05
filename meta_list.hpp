// author: Filip Roséen <filip.roseen@gmail.com>
// source: http://b.atch.se/posts/constexpr-meta-container

#ifndef ATCH_META_LIST_HPP
#define ATCH_META_LIST_HPP

#include <cstddef>

#include "type_list.hpp"
#include "meta_counter.hpp"

namespace atch { namespace {

  template<class Tag>
  struct meta_list {
    using   counter = atch::meta_counter<meta_list<Tag>>;
    using size_type = typename counter::size_type;

    template<size_type N, class = void>
    struct ident {
      friend auto adl_lookup (ident<N>);

      static constexpr size_type value = N;
    };

    template<class Dummy>
    struct ident<0, Dummy> {
      friend auto adl_lookup (ident<0>) {
        return atch::type_list<> {};
      }
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    template<class Ident, class Value>
    struct writer {
      friend auto adl_lookup (ident<Ident::value>) {
        return Value {};
      }

      static constexpr size_type value = Ident::value;
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    template<
      class State,
      class     H = meta_list,
      class Ident = typename H::template ident<H::counter::next ()>
    >
    static constexpr size_type push_state (size_type R = writer<Ident, State>::value) {
      return R;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    template<
      class     H = meta_list,
      size_type N = H::counter::value ()>
    using value_ident = typename H::template ident<N>;

    template<
      class     H = meta_list,
      class Ident = typename H::template value_ident<>
    >
    using value = decltype (adl_lookup (Ident {}));

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    template<class... Ts, class H = meta_list>
    static constexpr void push (
      size_type = push_state<
        typename H::template value<>::template push<Ts...>::result
      > ()
    ) {} 

    template<class H = meta_list>
    static constexpr void pop (
      size_type = push_state<
        typename H::template value<>::init::result
      > ()
    ) {}

    template<size_type Idx, class T, class H = meta_list>
    static constexpr void set (
      size_type = push_state<
        typename H::template value<>::template set<Idx, T>::result
      > ()
    ) {}
  };
}}

#endif /* include guard */
