#pragma once

#include <kos/types.hpp>
#include <optional>

namespace kos::multiboot {

#pragma pack(push, 1)
  struct memory_area {
    virtual_address<> base_address;
    u64 length;
    u32 type;
    u32 reserved;
  };

  template<bool Const = false>
    struct memory_map_iterator_impl {
      using value_type      = std::conditional_t<Const, const memory_area, memory_area>;
      using difference_type = std::ptrdiff_t;
      using pointer         = std::conditional_t<Const, const virtual_address<memory_area>, virtual_address<memory_area>>;
      using reference       = memory_area&;

      bool operator!=(const memory_map_iterator_impl<Const>& rhs) const noexcept;
      bool operator==(const memory_map_iterator_impl<Const>& rhs) const noexcept;
      auto operator*()  -> reference;
      auto operator->() -> pointer;

      memory_map_iterator_impl<Const>& operator=(memory_map_iterator_impl<Const>);

      reference operator++()    noexcept;
      reference operator++(int) noexcept;
      reference operator--()    noexcept;
      reference operator--(int) noexcept;

      private:
      pointer current_area;
      pointer last_area;
      u32 entry_size;
    };

  using memory_map_iterator = memory_map_iterator_impl<false>;
  using const_memory_map_iterator = memory_map_iterator_impl<true>;

  struct memory_map_tag {
    using iterator = memory_map_iterator;
    using const_iterator = const_memory_map_iterator;

    auto begin()              noexcept -> memory_map_iterator;
    auto end()                noexcept -> memory_map_iterator; 
    auto rbegin()             noexcept -> memory_map_iterator; 
    auto rend()               noexcept -> memory_map_iterator; 
    auto cbegin()       const noexcept -> const_memory_map_iterator;
    auto cend()         const noexcept -> const_memory_map_iterator;

    private:
    u32 type;
    u32 total_size;
    u32 entry_size;
    u32 entry_version;
    memory_area first_area;
  };

  struct boot_information_inner {
    u32 total_size;
    u32 reserved;

    bool has_valid_end_tag() const noexcept;
  };

  struct boot_information {
    virtual_address<boot_information_inner> multiboot;
    u64 offset;

    boot_information(virtual_address<boot_information_inner>, u64 offset = 0);
    auto start_address()  const noexcept -> u64;
    auto end_address()    const noexcept -> u64;
    auto total_size()     const noexcept -> u64;
    auto memory_map_tag() const noexcept -> std::optional<memory_map_tag>;
  };
#pragma pack(pop)
} // namespace kos::multiboot
