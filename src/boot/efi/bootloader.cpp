#include <kos/boot/efi/alloc.hpp>
#include <kos/boot/efi/cmdline.hpp>
#include <kos/boot/efi/console.hpp>
#include <kos/boot/efi/filesystem.hpp>

#include <kos/array.hpp>
#include <kos/types.hpp>
#include <kos/view.hpp>

#include <Protocol/LoadedImage.h>
#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>

static_assert(sizeof(wchar_t) == sizeof(CHAR16));

namespace kos::boot::efi {

extern "C" auto efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE *system_table) -> EFI_STATUS {

  using pool = pool_allocator<int>;
  using page = page_allocator<int>;

  efi::init(handle, system_table);
  println("Running kOS UEFI bootloader...");

  EFI_GUID image_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
  EFI_LOADED_IMAGE_PROTOCOL* loaded_image;

  auto status = gBS->OpenProtocol(gImageHandle, &image_guid, reinterpret_cast<void**>(&loaded_image), 0, gImageHandle, EFI_OPEN_PROTOCOL_GET_PROTOCOL);
  if (status != EFI_SUCCESS) {
    println("error in LoadedImage protocol: ", int(status));
    return -1;
  }

  auto ptr = L"";
  if(loaded_image->LoadOptions && loaded_image->LoadOptionsSize) {
    ptr = static_cast<wchar_t*>(loaded_image->LoadOptions);
    println("cmdline: ", view{ptr, ptr + loaded_image->LoadOptionsSize});
  } else {
    println("No cmdline given to bootloader");
  }

  auto boot_params = efi::parse_cmdline(ptr);
  auto arr  = array{1, 2, 3, 4, 5};
  auto heap = page::allocate(1);
  auto obj  = pool::allocate(1);
  auto esp  = filesystem::from_handle(loaded_image->DeviceHandle);
  auto conf = esp.open(L"EFI\\KOS\\config");
  auto data = reinterpret_cast<char*>(conf.data);

  println("boot params = ", boot_params
          , nl, "Array: ", arr
          , nl, "View: ", arr.slice(1, 1)
          , nl, "Stack: ", &boot_params
          , nl, "efi_main: ", efi_main
          , nl, "Heap: ", heap
          , nl, "Object: ", obj
          , nl, "Handle: ", handle);

  println("============FILE===========", nl, view{data, data + conf.size}, "============FILE===========");

  page::deallocate(heap, 1);
  pool::deallocate(obj, 1);

  while(1);

  __builtin_unreachable();

  // setup_randomness();
  // setup_clock();

  // auto memory_map = load_memory_map();
  // auto kernel = load_file(esp, boot_params.kernel);
  // auto ramfs = load_file(esp, boot_params.ramfs);
  // auto elf_binary = elf::open(kernel.address, kernel.size);

  // exit_boot_services();
  // start_kernel(elf_binary, boot_params);

  return EFI_SUCCESS;
}

}
