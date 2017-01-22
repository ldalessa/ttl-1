// -*- C++ -*-
#ifndef TTL_EXPRESSIONS_TRAITS_H
#define TTL_EXPRESSIONS_TRAITS_H

#include <ttl/Tensor.h>
#include <tuple>
#include <type_traits>

namespace ttl {
namespace expressions {

/// Type traits for the Expression curiously-recurring-template-pattern (CRTP).
///
/// We use an expression tree framework based on the standard CRTP mechanism,
/// where we define the Expression "superclass" parametrically based on the
/// "subclass" type. This allows us to forward to the subclass's behavior
/// without any semantic overhead, and thus allows the compiler to see the "real
/// types" in the expression.
///
/// Inside the Expression template we need to know things about the subclass
/// type, but at the time of the declaration the subclass type is
/// incomplete, so we can't say things like `Subclass::Scalar`. The standard
/// solution to this is a traits class (i.e., a level of indirection).
///
/// Each expression type must also provide an associated traits specialization
/// to provide at least the required functionality. This is class concept-based
/// design... the Expression system will work for any class that defines the
/// right concepts.
///
/// The default traits class tries to generate useful errors for classes that
/// haven't defined traits.
template <class E>
struct traits;

/// This specialization is used to try and print a hopefully useful error when
/// Tensors are used without indices.
template <int R, int D, class S>
struct traits<Tensor<R, D, S>>
{
  using outer_type = std::tuple<>;
  using inner_type = std::tuple<>;
  using scalar_type = std::remove_pointer_t<S>;
  using dimension = std::integral_constant<int, D>;
  using rank = std::integral_constant<int, R>;
};

template <class E>
using rinse = std::remove_cv_t<std::remove_reference_t<E>>;

/// The following traits are required for all expression types.
template <class E>
using outer_type = typename traits<rinse<E>>::outer_type;

template <class E>
using inner_type = typename traits<rinse<E>>::inner_type;

template <class E>
using scalar_type = typename traits<rinse<E>>::scalar_type;

template <class E>
using dimension = typename traits<rinse<E>>::dimension;

template <class E>
using rank = typename traits<rinse<E>>::rank;

template <class E>
using tensor_type = Tensor<rank<E>::value, dimension<E>::value, rinse<scalar_type<E>>>;

} // namespace expressions
} // namespace traits

#endif // #ifndef TTL_EXPRESSIONS_TRAITS_H
