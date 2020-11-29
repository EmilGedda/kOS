#pragma once
#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include <kos/view.hpp>
#include <kos/array.hpp>
#include <kos/types.hpp>

namespace kos::boot::efi {

  struct cmdline {
    array<wchar_t, 255> kernel{};
    array<wchar_t, 255> initramfs{};
    array<wchar_t, 255> root{};
    bool debug = false;
    int verbosity = 5;
  };

  void print(const cmdline&);

  auto parse_cmdline(const wchar_t*) -> cmdline;
}
