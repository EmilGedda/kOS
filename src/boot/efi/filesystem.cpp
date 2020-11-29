#include "kos/boot/efi/alloc.hpp"
#include <kos/boot/efi/filesystem.hpp>
#include <kos/boot/efi/console.hpp>
#include <kos/view.hpp>
#include <kos/types.hpp>

#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>
#include <Guid/FileInfo.h>
#include <Protocol/SimpleFileSystem.h>


namespace kos::boot::efi {

using pool = pool_allocator<u8>;


file::~file() {
}

auto filesystem::from_handle(EFI_HANDLE device_handle) -> filesystem {
  filesystem fs;
  auto status = gBS->OpenProtocol(device_handle, &filesystem::guid, reinterpret_cast<void**>(&fs.protocol), 0, gImageHandle, EFI_OPEN_PROTOCOL_GET_PROTOCOL);

  if (status == EFI_UNSUPPORTED) {
    println("EfiSimpleFileSystem is not supported!");
    return {};
  }

  if (status != EFI_SUCCESS) {
    println("Missing EFI_OPEN_PROTOCOL_GET_PROTOCOL on handle: ", device_handle);
    return {};
  }

  status = fs.protocol->OpenVolume(fs.protocol, &fs.root);

  if (status != EFI_SUCCESS) {
    println("error during OpenVolume for print_config");
    return {};
  }

  return fs;
}

auto filesystem::stat(EFI_FILE_PROTOCOL* file) -> file_metadata {
  file_metadata info;
  EFI_GUID guid = EFI_FILE_INFO_ID;
  size_t size = 0;
  auto status = file->GetInfo(file, &guid, &size, 0);

  if (status != EFI_BUFFER_TOO_SMALL) {
    println("error during GetInfo for print_config: ", status);
    return {};
  }

  auto efi_info = reinterpret_cast<EFI_FILE_INFO*>(pool::allocate(size));
  status = file->GetInfo(file, &guid, &size, efi_info);

  if (status != EFI_SUCCESS) {
    println("error during GetInfo for print_config: ", status);
    return {};
  }

  info.size = efi_info->FileSize;
  pool::deallocate(reinterpret_cast<u8*>(efi_info), size);
  return info;
}

auto filesystem::open(view<wchar_t> path) -> file {
  EFI_FILE_PROTOCOL *proto;
  auto ptr = const_cast<wchar_t*>(path.begin());
  auto status = root->Open(root, &proto, reinterpret_cast<CHAR16*>(ptr), EFI_FILE_MODE_READ, 0);

  if (status != EFI_SUCCESS) {
    println("error during Open for print_config: ", status);
    return {};
  }

  auto info = stat(proto);
  auto size = info.size;

  file f{pool::allocate(size), size, info};
  status = proto->Read(proto, &size, f.data);

  if (size != info.size) {
    println("did not read the correct number of bytes: ", size, " != ", info.size, '!');
    return {};
  }

  if (status != EFI_SUCCESS) {
    println("error during Read for filesystem::open: ", status);
    return {};
  }

  proto->Close(proto);
  return f;
}

} // namespace kos::boot::efi
