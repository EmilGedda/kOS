#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>

auto msg = L"Running kOS UEFI bootloader...\r\n";

extern "C" /* [[gnu::ms_abi]] */ auto efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE *system_table) -> EFI_STATUS {

  auto stdout = system_table->ConOut;

  stdout->ClearScreen(stdout);
  stdout->SetCursorPosition(stdout, 0, 0);
  stdout->OutputString(stdout, (CHAR16*)msg);

  while(1);

  return EFI_SUCCESS;
}

