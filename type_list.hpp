// author: Filip Roséen <filip.roseen@gmail.com>
// source: http://b.atch.se/posts/constexpr-meta-container

#ifndef ATCH_TYPE_LIST_HPP
#define ATCH_TYPE_LIST_HPP

#include <cstddef>
#include <utility>

namespace atch {
  template<std::size_t N>
  using ic = std::integral_constant<std::size_t, N>;

  template<std::size_t... Ns>
  using iseq = std::index_sequence<Ns...>;

  template<std::size_t N>
  using make_iseq = std::make_index_sequence<N>;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  template<class... Ts>
  struct type_list {
    using size_type = std::size_t;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

    static constexpr size_type size () { return sizeof... (Ts); }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

    template<size_type Idx>
    struct at {
      template<size_type N, class U, class... Us>
      struct access_helper
        : access_helper<N+1, Us...>
      { };

      template<class U, class... Us>
      struct access_helper<Idx, U, Us...> {
        using result = U;
      };

      using result = typename access_helper<0, Ts...>::result;
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

    template<class... Us>
    struct push {
      using result = type_list<Ts..., Us...>;
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

    template<size_type Idx, class U>
    struct set {

      template<size_type N>
      static auto at_helper (ic<N>)   -> typename at<N>::result;
      static auto at_helper (ic<Idx>) -> U;

      template<size_type... Ns>
      static auto set_helper (iseq<Ns...>)
        -> type_list<decltype (at_helper (ic<Ns> {}))...>
      ;

      using result = decltype (set_helper (make_iseq<size()> {}));
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

    struct init {
      template<size_type... Ns>
      static auto helper (iseq<Ns...>)
        -> type_list<typename at<Ns>::result...>
      ;

      using result = decltype (helper (make_iseq<size ()-1> {}));
    };
  };

} /* namespace atch */

#endif /* include guard */
