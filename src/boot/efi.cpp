#include <kos/boot/efi.hpp>

#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>

namespace kos::boot::efi {

  void init_screen(console* con) {
    stdout = con;
    clear_screen();
  }

  void clear_screen() {
    stdout->ClearScreen(stdout);
    move_cursor(0, 0);
  }

  void move_cursor(int x, int y) {
    stdout->SetCursorPosition(stdout, x, y);
  }


  void print(const wchar_t* msg) {
    stdout->OutputString(stdout, (CHAR16*)msg);
  }

  void print(const char* str) {
    for(wchar_t c = *str; c; c = *++str) {
      print(c);
    }
  }

  void println(const char* msg) {
    print(msg);
    print(L"\r\n");
  }

  void print(wchar_t c) {
    const wchar_t str[2] = { c, 0 };
    print(str);
  }

  void print(char c) {
    print(static_cast<wchar_t>(c));
  }

  void print(bool b) {
    print(b ? "true" : "false");
  }

  void print(int n) {
    if(n < 0) {
      print("-");
      n = -n;
    }


    array<char, 12> digits;
    int length = 0;

    for(; n && length < 12; length++) {
      digits[length] = (n % 10) + '0';
      n /= 10;
    }

    if(length == 0)
      print("0");
    else
      for(int i = length - 1; i >= 0; i--)
        print(digits[i]);
  }

  template<typename T>
  void print(view<T> str) {
    for(const auto& c: str)
      print(c);
  }

  constexpr auto strlen(const char* str) -> size_t {
    size_t len = 0;
    while(*str++)
      len++;
    return len;
  }

  template<typename T, typename U, typename S, size_t N>
  auto parse_param(view<T>& argument, U& param, const S (&name)[N]) {
    parse_param(argument, param, view<S>{name});
  }

  template<typename T>
  auto parse_param(view<T> argument, int& param, view<T> name) {
    if(!argument.starts_with(name)) return;
    if(argument[name.size() - 1] != L'=') return;
    param = argument.slice(name.size()).to_int();
  }

  template<typename T>
  auto parse_param(view<T> argument, bool& param, view<T> name) {
    if(argument == name) {
      param = true;
    }

    if(argument.starts_with("no") && argument.slice(2) == name) {
      param = false;
    }
  }

  template<typename T, size_t N>
  auto parse_param(view<T> argument, array<T, N>& param, view<T> name) {
    if(!argument.starts_with(name)) return;
    if(argument[name.size() - 1] != L'=') return;
    param = argument.slice(name.size());
  }

  template<typename T>
  static auto parse_word(const T* str) -> view<T> {
      while(*str == L' ')
        str++;
      auto* word_start = str;
      while(*str && *str != L' ')
        str++;
      auto* word_end = str;
      return { word_start, word_end };
  }

  auto parse_cmdline(const wchar_t* cmd) -> cmdline {

    cmdline boot_args;
    while(*cmd) {
      auto param = parse_word(cmd);
      cmd = param.end();

      parse_param(param, boot_args.initrd,    L"initramfs");
      parse_param(param, boot_args.debug,     L"debug");
      parse_param(param, boot_args.verbosity, L"verbosity");
    }

    return boot_args;
  }

  void print(const cmdline& boot_args) {
    println("{");
    print("  verbosity:\t");
    println(boot_args.verbosity);
    print("  debug:\t");
    println(boot_args.debug);
    print("  initramfs:\t");
    println(boot_args.initrd);
    println("}");
  }
}
