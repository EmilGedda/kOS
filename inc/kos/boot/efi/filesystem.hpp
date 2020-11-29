#pragma once
#include <kos/view.hpp>
#include <kos/types.hpp>

#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>
#include <Guid/FileInfo.h>
#include <Protocol/SimpleFileSystem.h>

namespace kos::boot::efi {

  struct file_metadata {
    u64 size = 0;
  };

  struct file {
    u8* data = 0;
    u64 size = 0;
    file_metadata info{};

    ~file();
  };

  struct filesystem {
    inline static EFI_GUID guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

    EFI_FILE_PROTOCOL *root = 0;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *protocol = 0;
    static auto from_handle(EFI_HANDLE device_handle) -> filesystem;

    auto stat(EFI_FILE_PROTOCOL* file) -> file_metadata;
    auto open(view<wchar_t> path) -> file;
  };
} // namespace kos::boot::efi
