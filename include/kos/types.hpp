#pragma once

#include <type_safe/strong_typedef.hpp>
#include <type_traits>

#define __ALIAS(NAME, SIZE, SIZESUFFIX) using NAME##SIZE##SIZESUFFIX

#define __CSTDINT(PREFIX, SIZE, TYPE)          \
  __ALIAS(PREFIX##int, SIZE, _t)       = TYPE; \
  __ALIAS(PREFIX##int_fast, SIZE, _t)  = TYPE; \
  __ALIAS(PREFIX##int_least, SIZE, _t) = TYPE

#define _ALIAS(TYPE, SIZE)                                                                                            \
  static_assert(sizeof(TYPE) * 8 == SIZE, "Specified size for type alias does not match the size of the given type"); \
  __ALIAS(u, SIZE, ) = unsigned TYPE;                                                                                 \
  __ALIAS(s, SIZE, ) = signed   TYPE;                                                                                 \
  __CSTDINT(u, SIZE, unsigned TYPE);                                                                                  \
  __CSTDINT(, SIZE, signed TYPE)

namespace kos {
  inline namespace types {

    using u1 = bool;

    _ALIAS(char, 8);
    _ALIAS(short, 16);
    _ALIAS(int, 32);
    _ALIAS(long, 64);

    using size_t  = unsigned long;
    using ssize_t = signed long;

    namespace ts = type_safe;

    template <typename Tag, typename T>
    struct pointer
        : ts::strong_typedef<pointer<Tag, T>, u64>
        , ts::strong_typedef_op::equality_comparison<pointer<Tag, T>>
        , ts::strong_typedef_op::mixed_equality_comparison<pointer<Tag, T>, u64>
        , ts::strong_typedef_op::bitmask<pointer<Tag, T>>
        , ts::strong_typedef_op::bitshift<pointer<Tag, T>, u64>
        , ts::strong_typedef_op::addition<pointer<Tag, T>>
        , ts::strong_typedef_op::mixed_addition<pointer<Tag, T>, u64>
        , ts::strong_typedef_op::subtraction<pointer<Tag, T>>
        , ts::strong_typedef_op::mixed_subtraction<pointer<Tag, T>, u64>
        , ts::strong_typedef_op::increment<pointer<Tag, T>>
        , ts::strong_typedef_op::decrement<pointer<Tag, T>> {
      using ts::strong_typedef<pointer<Tag, T>, u64>::strong_typedef;

      explicit operator bool() const noexcept {
        return static_cast<u64>(*this) != 0;
      }

      T& operator* () {
        return *static_cast<std::remove_cvref_t<T>*>(*this);
      }

      T* operator-> () {
        return static_cast<T*>(*this);
      }
    };

    template <typename Tag>
    struct pointer<Tag, void>
        : ts::strong_typedef<pointer<Tag, void>, u64>
        , ts::strong_typedef_op::equality_comparison<pointer<Tag, void>>
        , ts::strong_typedef_op::mixed_equality_comparison<pointer<Tag, void>, u64>
        , ts::strong_typedef_op::bitmask<pointer<Tag, void>>
        , ts::strong_typedef_op::bitshift<pointer<Tag, void>, u64>
        , ts::strong_typedef_op::addition<pointer<Tag, void>>
        , ts::strong_typedef_op::mixed_addition<pointer<Tag, void>, u64>
        , ts::strong_typedef_op::subtraction<pointer<Tag, void>>
        , ts::strong_typedef_op::mixed_subtraction<pointer<Tag, void>, u64>
        , ts::strong_typedef_op::increment<pointer<Tag, void>>
        , ts::strong_typedef_op::decrement<pointer<Tag, void>> {
      using ts::strong_typedef<pointer<Tag, void>, u64>::strong_typedef;

      explicit operator bool() const noexcept {
        return static_cast<u64>(*this) != 0;
      }

    };

    struct physical_address_tag;
    struct virtual_address_tag;

    template<typename T = void>
    using physical_address = pointer<physical_address_tag, T>;
    template<typename T = void>
    using virtual_address  = pointer<virtual_address_tag, T>;

    template <typename T, typename... Ts>
    inline constexpr bool is_any_of = (std::is_same_v<T, Ts> || ...);

    template <typename T>
    inline constexpr bool io_port_type_v = is_any_of<T, u8, u16, u32>;

  } // namespace types
} // namespace kos

#undef _ALIAS
#undef __CSTDINT
#undef __ALIAS
