#pragma once

#include <kos/types.hpp>

namespace kos {

  template <class T, T v>
  struct integral_constant {
    static constexpr T value = v;
    using value_type         = T;
    using type               = integral_constant;
    constexpr operator value_type() const noexcept {
      return value;
    }
    constexpr value_type operator()() const noexcept {
      return value;
    }
  };

  template <bool B>
  using bool_constant = integral_constant<bool, B>;

  using true_type  = bool_constant<true>;
  using false_type = bool_constant<false>;

  template <bool B, class T = void>
  struct enable_if {};

  template <class T>
  struct enable_if<true, T> {
    typedef T type;
  };

  template <bool B, class T = void>
  using enable_if_t = typename enable_if<B, T>::type;

  template <typename T>
  struct io_port_type : false_type {};

  template <>
  struct io_port_type<u8> : true_type {};

  template <>
  struct io_port_type<u16> : true_type {};

  template <>
  struct io_port_type<u32> : true_type {};

  template <typename T>
  static constexpr bool io_port_type_v = io_port_type<T>::value;
} // namespace kos
