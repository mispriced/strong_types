#pragma once
#include <type_traits>

namespace strong_type {
struct noop {};
template <typename T>
concept has_operator = !std::is_same<T, noop>::value;

template <typename T>
concept ScalarType = std::is_scalar<T>::value;

template <typename T, typename Tag> struct type {
public:
  using value_type = T;
  using tag_type = Tag;

  explicit type(const T &v) : value(v) {}

  constexpr const T &get() const { return value; }
  constexpr auto operator<=>(const type &rhs) const = default;

  T value;
};

template <class In1, class In2 = void> struct operators {
#define X(OP, NAME) using NAME = noop;
#include "binary_operators.hpp"
#include "unary_operators.hpp"
  BINARY_OPERATORS
  UNARY_OPERATORS
#undef X
};

#define DEFINE_UNARY_OPERATOR(OP, OP_NAME)                                     \
  template <class In1>                                                         \
  requires has_operator<typename operators<In1>::OP_NAME>                      \
  constexpr auto operator OP(const In1 &rhs)                                   \
      ->typename operators<In1>::OP_NAME {                                     \
    return typename operators<In1>::OP_NAME(OP rhs.get());                     \
  }

#define X(OP, OP_NAME) DEFINE_UNARY_OPERATOR(OP, OP_NAME)
#include "unary_operators.hpp"
UNARY_OPERATORS
#undef X

#define DEFINE_BINARY_OPERATORS(OP, OP_NAME)                                   \
  /* Regular binary operator */                                                \
  template <class In1, class In2>                                              \
  requires(!std::is_scalar<In1>::value && !std::is_scalar<In2>::value) &&      \
      has_operator<typename operators<In1, In2>::OP_NAME> constexpr auto       \
      operator OP(const In1 &lhs, const In2 &rhs)                              \
          ->typename operators<In1, In2>::OP_NAME {                            \
    return typename operators<In1, In2>::OP_NAME(lhs.get() OP rhs.get());      \
  }                                                                            \
  /* Scalar-left binary operator */                                            \
  template <class In1, class Scalar>                                           \
  requires std::is_scalar<Scalar>::value &&                                    \
      has_operator<typename operators<In1, Scalar>::OP_NAME>                   \
  constexpr auto operator OP(const In1 &lhs, Scalar rhs)                       \
      ->typename operators<In1, Scalar>::OP_NAME {                             \
    return typename operators<In1, Scalar>::OP_NAME(lhs.get() OP rhs);         \
  }                                                                            \
  /* Scalar-right binary operator */                                           \
  template <class Scalar, class In1>                                           \
  requires std::is_scalar<Scalar>::value &&                                    \
      has_operator<typename operators<Scalar, In1>::OP_NAME>                   \
  constexpr auto operator OP(Scalar lhs, const In1 &rhs)                       \
      ->typename operators<Scalar, In1>::OP_NAME {                             \
    return typename operators<Scalar, In1>::OP_NAME(lhs OP rhs.get());         \
  }

#define X(OP, OP_NAME) DEFINE_BINARY_OPERATORS(OP, OP_NAME)
#include "binary_operators.hpp"
BINARY_OPERATORS
#undef X

} // namespace strong_type