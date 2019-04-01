#pragma once

#include <kos/types.hpp>
#include <algorithm>
#include <optional>
#include <string_view>

namespace kos::multiboot {

#pragma pack(push, 1)
  struct memory_area {
    virtual_address base_address;
    u64 length;
    u32 type;
    u32 reserved; // vendor reserverd
  };

  template<typename T>
    struct tag_type;
  //todo: alignment and padding
  struct tag {
    u32 type;
    u32 size;

    template<typename T, typename = std::enable_if_t<std::is_base_of_v<tag, T>>>
    static bool valid(const T& t) {
      return std::is_same_v<std::remove_cvref_t<T>, tag> || t.type == tag_type<T>::value;
    }
  };

  // inheriting from tag means it is of variable length and should be pinned in memory.
  struct pinned_tag : tag {
    pinned_tag(const pinned_tag&) = delete;
    pinned_tag(const pinned_tag&&) = delete;
    void operator=(const pinned_tag&) = delete;
    void operator=(const pinned_tag&&) = delete;
  };

  template<typename T>
    struct memory_map_iterator {
      using iterator_category = std::forward_iterator_tag;
      using value_type        = T;
      using difference_type   = std::ptrdiff_t;
      using pointer           = virtual_pointer<T>;
      using reference         = std::add_lvalue_reference_t<T>;

      auto operator*()  const noexcept -> reference { return *current_area; }
      auto operator->() const noexcept -> pointer   { return  current_area; }

      bool operator==(const memory_map_iterator<T>& rhs) const noexcept {
        return this->current_area == rhs.current_area;
      }

      bool operator!=(const memory_map_iterator<T>& rhs) const noexcept {
        return !(*this == rhs);
      }

      memory_map_iterator<T>& operator++() noexcept {

      }

      memory_map_iterator<T>& operator++(int) noexcept {
        auto tmp = *this;
        ++*this;
        return tmp;
      }

      private:
        pointer current_area;
        pointer last_area;
        u32 entry_size;
    };

  struct memory_map_tag : tag {
    using iterator       = memory_map_iterator<memory_area>;
    using const_iterator = memory_map_iterator<const memory_area>;

    auto begin() const noexcept -> const_iterator;
    auto begin()       noexcept -> iterator;
    auto end()   const noexcept -> const_iterator;
    auto end()         noexcept -> iterator;

    private:
      u32 entry_size;
      u32 entry_version;
      memory_area first_area;
  };

  struct module_tag : pinned_tag {
    u32 module_start;
    u32 module_end;

    // this is the first byte in the variable length name of the module
    u8 name_byte;
  };

  struct boot_command_line_tag : pinned_tag {
    // this is the first byte in a UTF-8 zero terminated string
    const char name_byte;
    auto to_string_view() const noexcept -> std::string_view;
    auto to_c_str()       const noexcept -> const char *;
  };

  struct boot_information_header {
    u32 total_size;
    u32 reserved;

    bool has_valid_end_tag() const noexcept;
  };

  namespace {
    struct dummy {};
  };

  template<typename T>
    struct tag_type {
      static_assert(std::is_same_v<T, dummy>, // delay assertion until explicit instantiation
          "Specified type is not a valid multiboot2 tag type");
    };

  template<> struct tag_type<tag>                   : std::integral_constant<int, 0> {};
  template<> struct tag_type<boot_command_line_tag> : std::integral_constant<int, 1> {};
  template<> struct tag_type<module_tag>            : std::integral_constant<int, 3> {};
  template<> struct tag_type<memory_map_tag>        : std::integral_constant<int, 6> {};

  struct tag_end_iterator {};

  // by default, enda_tag is the type of the tag that marks the end of the tag list
  using end_tag = tag;

  template<typename T>
    struct tag_iterator {
      using iterator_category = std::forward_iterator_tag;
      using difference_type   = std::ptrdiff_t;
      using value_type = T;
      using reference = std::add_lvalue_reference_t<T>;
      using pointer = virtual_pointer<T>;

      pointer tag_ptr;

      auto operator*() const noexcept -> reference {
        return *tag_ptr;
      }

      auto operator->() const noexcept -> pointer {
        return tag_ptr;
      }

      auto operator++() noexcept -> tag_iterator<T>& {
        do {
          tag_ptr += tag_ptr->size;
          tag_ptr.align(8);
        } while(!tag::valid(*tag_ptr));
        return *this;
      }

      auto operator++(int) noexcept -> tag_iterator<T> {
        auto tmp = *this;
        ++*this;
        return tmp;
      }

      template<typename U, typename = std::enable_if_t<std::is_base_of_v<U, T> || std::is_base_of_v<T, U>>>
        bool operator==(tag_iterator<U> rhs) const noexcept {
          return static_cast<u64>(this->tag_ptr) == static_cast<u64>(rhs.tag_ptr);
        }

      bool operator==(tag_end_iterator) const noexcept {
        return this->type == 0 && this->size == 8;
      }

      template<typename U>
        bool operator!=(U rhs) const noexcept {
          return !(*this == rhs);
        }
    };

  struct boot_information {
    template<typename T>
      using iterator       = tag_iterator<T>;
    template<typename T>
      using const_iterator = tag_iterator<const T>;

    boot_information(virtual_pointer<boot_information_header>, u64 offset = 0);

    auto start_address() const noexcept -> u64;
    auto end_address()   const noexcept -> u64;
    auto total_size()    const noexcept -> u64;

    auto begin() const noexcept -> const_iterator<tag>;
    auto begin()       noexcept ->       iterator<tag>;
    auto end()   const noexcept -> tag_end_iterator;

    template<typename Tag>
      auto get_tag() const noexcept -> const_iterator<Tag> {
        return { virtual_pointer<Tag>{multiboot + sizeof(boot_information_header)} };
      }

    template<typename Tag>
      auto get_tag() noexcept -> iterator<Tag> {
        return { virtual_pointer<Tag>{multiboot + sizeof(boot_information_header)} };
      }

    auto memory_map_tags()       noexcept -> iterator_pair<iterator<memory_map_tag>>;
    auto memory_map_tags() const noexcept -> iterator_pair<const_iterator<memory_map_tag>>;

    private:
      virtual_pointer<boot_information_header> multiboot;
      u64 offset;

  };
#pragma pack(pop)
} // namespace kos::multiboot
