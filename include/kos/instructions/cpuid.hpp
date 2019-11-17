#pragma once
#include <array>
#include <kos/types.hpp>

namespace kos::cpuid {
  namespace {

    enum cpuid_requests {
      vendor_string,
      features,
      tlb,
      serial,

      highest_extended_implemented=0x80000000,
      extended_features,
      brand_string_start,
      brand_string_more,
      brand_string_end,
    };

    struct cpuid_results {
      u32 eax;
      u32 ebx;
      u32 ecx;
      u32 edx;
    };

    inline static auto call(u32 leaf) -> cpuid_results {
      cpuid_results r;
      asm volatile("cpuid":"=a"(r.eax),"=b"(r.ebx),"=c"(r.ecx),"=d"(r.edx):"a"(leaf));
      return r;
    }

    // compiles down to mov eax, edi; bswap eax; ret
    inline static auto bswap(u32 num) -> u32 {
      return (num >> 24 & 0xff)
        | (num >>  8 & 0xff00)
        | (num <<  8 & 0xff0000)
        | (num << 24 & 0xff000000);
    }
  }

  auto vendor_string() -> std::array<u8, 12> {
    auto r = call(cpuid_requests::vendor_string);
    auto ebx = bswap(r.ebx);
    auto ecx = bswap(r.ecx);
    auto edx = bswap(r.edx);
    return { 
      static_cast<u8>(ebx >> 24), static_cast<u8>(ebx >> 16), static_cast<u8>(ebx >> 8), static_cast<u8>(ebx),
      static_cast<u8>(edx >> 24), static_cast<u8>(edx >> 16), static_cast<u8>(edx >> 8), static_cast<u8>(edx),
      static_cast<u8>(ecx >> 24), static_cast<u8>(ecx >> 16), static_cast<u8>(ecx >> 8), static_cast<u8>(ecx)
    };
  }

  auto has_feature(u32) -> bool {
    return true;
  }
} // namespace kos::cpuid
