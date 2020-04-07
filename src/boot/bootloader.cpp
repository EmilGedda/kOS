#include <kos/boot/efi.hpp>

#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>

namespace kos::boot {
extern "C" auto efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE *system_table) -> EFI_STATUS {

  efi::init_screen(system_table->ConOut);

  efi::println("Running kOS UEFI bootloader...");

  auto boot_args = efi::parse_cmdline(L"initramfs=/EFI/kOS.ramfs.gz verbosity=5 debug");
  efi::print("boot args ");
  efi::println(boot_args);

  while(1);

  return EFI_SUCCESS;
}

}
