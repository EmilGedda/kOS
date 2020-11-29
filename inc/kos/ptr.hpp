#pragma once

#include <type_safe/strong_typedef.hpp>
#include <type_traits>
#include <utility>

namespace kos {
  inline namespace types {

    namespace ts = type_safe;

    template <typename Tag, typename T>
    struct pointer_base
        : ts::strong_typedef<pointer_base<Tag, T>, u64>
        , ts::strong_typedef_op::equality_comparison<pointer_base<Tag, T>>
        , ts::strong_typedef_op::mixed_equality_comparison<pointer_base<Tag, T>, u64>
        , ts::strong_typedef_op::bitmask<pointer_base<Tag, T>>
        , ts::strong_typedef_op::bitshift<pointer_base<Tag, T>, u64>
        , ts::strong_typedef_op::addition<pointer_base<Tag, T>>
        , ts::strong_typedef_op::mixed_addition<pointer_base<Tag, T>, u64>
        , ts::strong_typedef_op::subtraction<pointer_base<Tag, T>>
        , ts::strong_typedef_op::mixed_subtraction<pointer_base<Tag, T>, u64>
        , ts::strong_typedef_op::increment<pointer_base<Tag, T>>
        , ts::strong_typedef_op::decrement<pointer_base<Tag, T>> {
      using ts::strong_typedef<pointer_base<Tag, T>, u64>::strong_typedef;
      using type = T;

      template<typename U>
      pointer_base(pointer_base<Tag, U> address) noexcept {
        *this = pointer_base<Tag, T>{static_cast<u64>(address)};
      }

      explicit operator bool() const noexcept {
        return static_cast<u64>(*this) != 0;
      }

      // aligns upwards, for n = 2
      // 0 -> 0
      // 1 -> 2
      // 2 -> 2
      // 3 -> 4
      // ...
      void align(u64 n) const noexcept {
        *this += n - 1;
        *this &= ~(n - 1);
      }
    };

    template <typename Tag, typename T>
    struct pointer : pointer_base<Tag, T> {
      using pointer_base<Tag, T>::pointer_base;

      using reference = std::add_lvalue_reference_t<T>;

      auto operator*() noexcept -> reference {
        return *reinterpret_cast<T*>(static_cast<u64>(*this));
      }

      auto operator->() noexcept -> pointer<Tag, T> {
        return *this;
      }
    };

    template <typename Tag>
    struct pointer<Tag, void> : pointer_base<Tag, void> {
      using pointer_base<Tag, void>::pointer_base;
      // dereferencing a void pointer is a compile error
    };

    struct physical_address_tag;
    struct virtual_address_tag;

    template<typename T>
    using physical_pointer = pointer<physical_address_tag, T>;

    template<typename T>
    using virtual_pointer = pointer<virtual_address_tag, T>;

    using physical_address = physical_pointer<void>;
    using virtual_address  = virtual_pointer<void>;

    template <typename T, typename... Ts>
    inline constexpr bool is_any_of = (std::is_same_v<T, Ts> || ...);

    template <typename T>
    inline constexpr bool io_port_type_v = is_any_of<T, u8, u16, u32>;


    template<typename StartIter, typename EndIter = StartIter>
      struct iterator_pair {

        StartIter start_iterator;
        EndIter end_iterator;

        iterator_pair(StartIter start_iterator, EndIter end_iterator)
          : start_iterator(std::move(start_iterator))
          , end_iterator(std::move(end_iterator)) { };

        auto begin() const noexcept -> StartIter {
          return start_iterator;
        }

        auto end() const noexcept -> EndIter {
          return end_iterator;
        }
      };


  } // namespace types
} // namespace kos
