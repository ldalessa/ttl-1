#pragma once

#include <limits>
#include <tuple>
#include <type_traits>

namespace ttl {
template <class T>
struct is_scalar {
  enum : bool { value = std::is_arithmetic<T>::value };
};

template <class T>
struct is_integer {
  enum : bool { value = std::numeric_limits<T>::is_integer };
};

template <class T>
struct is_valid {
  enum : bool { value = is_index<T>::value or is_integer<T>::value };
};

template <template <class> class, class...>
struct all {
  enum : bool { value = true };
};

template <template <class> class P, class T, class... U>
struct all<P, T, U...> {
  enum : bool { value = P<T>::value and all<P, U...>::value };
};

template <class... U>
using all_scalar = all<is_scalar, U...>;

template <class... U>
using all_integer = all<is_integer, U...>;

template <class... U>
using all_valid = all<is_valid, U...>;

template <class To, class... U>
struct all_convertible {
  enum : bool { value = true };
};

template <class To, class From, class... U>
struct all_convertible<To, From, U...> {
  enum : bool { value = std::is_convertible<From, To>::value and
                        all_convertible<To, U...>::value };
};

template <class... U>
struct has_integer {
  enum : bool { value = false };
};

template <class T, class... U>
struct has_integer<T, U...> {
  enum : bool { value = is_integer<T>::value or has_integer<U...>::value };
};

template <class... T>
struct has_integer<std::tuple<T...>> {
  enum : bool { value = has_integer<T...>::value };
};

template <class Lhs, class Rhs>
struct dimension {
  static constexpr int L = Lhs::D;
  static constexpr int R = Rhs::D;
  static_assert(L == R or ((L == -1) xor (R == -1)));
  static constexpr int value = (L < R) ? R : L;
};

template <bool B, class T, class>
struct iif {
  using type = T;
};

template <class T, class F>
struct iif<false, T, F> {
  using type = F;
};

template <bool B, class T, class F>
using iif_t = typename iif<B, T, F>::type;
}