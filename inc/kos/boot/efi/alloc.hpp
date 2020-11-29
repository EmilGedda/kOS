#pragma once
#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include <kos/view.hpp>
#include <kos/array.hpp>

namespace kos::boot::efi {

  using u64 = unsigned long long;

  enum memory_type {
    loader_data = EfiLoaderData,
  };

  enum alloc_type {
    any_address = AllocateAnyPages,
    max_address = AllocateMaxAddress
  };

  template<typename T, alloc_type type = alloc_type::any_address, auto Max = ~0ULL, memory_type region = memory_type::loader_data>
  struct page_allocator {
    inline constexpr static auto page_size = 4096;

    static void* allocate_pages(int n) {
      u64 buf;
      auto status = gBS->AllocatePages(static_cast<EFI_ALLOCATE_TYPE>(type)
                                      , static_cast<EFI_MEMORY_TYPE>(region), n, &buf);
      if (EFI_ERROR(status)) {
        return 0;
      }

      return reinterpret_cast<void*>(buf);
    }

    static void* allocate_page() {
      return allocate_pages(1);
    }

    [[nodiscard]] static constexpr T* allocate(size_t n) {
      auto num_pages = 1 + n / page_size;
      return static_cast<T*>(allocate_pages(num_pages));
    }

    static void deallocate(T* p, size_t n) {
      auto num_pages = 1 + n / page_size;
      free_pages(static_cast<void*>(p), num_pages);
    }

    static void free_pages(void* p, size_t n) {
      gBS->FreePages(reinterpret_cast<u64>(p), n);
    }

    static void free_page(void* p) {
      free_pages(p, 1);
    }
  };

  template<typename T, memory_type region = memory_type::loader_data>
  struct pool_allocator {
    [[nodiscard]] static T* allocate(size_t n) {
      void* buf;
      auto status = gBS->AllocatePool(static_cast<EFI_MEMORY_TYPE>(region), n * sizeof(T), &buf);
      if (EFI_ERROR(status)) {
        return 0;
      }
      return static_cast<T*>(buf);
    }

    static void deallocate(T* p, size_t n) {
      gBS->FreePool(p);
    }
  };

}
