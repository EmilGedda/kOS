#include <kos/boot/efi/console.hpp>
#include <kos/types.hpp>

#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>

namespace kos::boot::efi {

  EFI_HANDLE           gImageHandle = 0;
  EFI_SYSTEM_TABLE     *gST         = 0;
  EFI_BOOT_SERVICES    *gBS         = 0;
  EFI_RUNTIME_SERVICES *gRT         = 0;

  void init(EFI_HANDLE handle, EFI_SYSTEM_TABLE* table) {
    gImageHandle = handle;
    gST = table;
    gBS = table->BootServices;
    gRT = table->RuntimeServices;

    stdout = table->ConOut;
    set_color(EFI_WHITE, EFI_BACKGROUND_BLACK);
    stdout->Reset(stdout, false);
  }

  void clear_screen() {
    stdout->ClearScreen(stdout);
    move_cursor(0, 0);
  }

  void move_cursor(int x, int y) {
    stdout->SetCursorPosition(stdout, x, y);
  }

  void set_color(int fg, int bg) {
    stdout->SetAttribute(stdout, EFI_TEXT_ATTR(fg,bg));
  }

namespace impl {

  void print(const wchar_t* msg) {
    stdout->OutputString(stdout, (CHAR16*)msg);
  }

  void print(const char* str) {
    while(*str)
      print(*str++);
  }

  void println() {
    print('\n');
  }

  void print(endl) {
    println();
  }

  void print(wchar_t c) {
    wchar_t str[] = { c, 0 };
    print(str);
  }

  void print(char c) {
    if(c == '\n')
      print(L'\r');
    print(static_cast<wchar_t>(c));
  }

  void print(bool b) {
    print(b ? "true" : "false");
  }

  void print(view<wchar_t> str) {
    for(auto i = 0; i < str.size() && str[i]; i++)
      print(str[i]);
  }

  void print(view<char> str) {
    for(auto i = 0; i < str.size() && str[i]; i++)
      print(str[i]);
  }

  void print(u64 n) {
    if (n == 0)
      print('0');

    array<char, 20> digits;
    int length = 0;

    for(; n && length < digits.size(); length++) {
      digits[length] = (n % 10) + '0';
      n /= 10;
    }

    for(int i = length - 1; i >= 0; i--)
      print(digits[i]);
  }

  void print(s64 n) {
    if(n < 0) {
      print("-");
      n = -n;
    }
    print(static_cast<u64>(n));
  }

  void print(int n) {
    print(static_cast<s64>(n));
  }

} // namespace impl

}
