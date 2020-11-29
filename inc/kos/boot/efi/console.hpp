#pragma once
#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>

#include <kos/view.hpp>
#include <kos/array.hpp>
#include <kos/types.hpp>

namespace kos::boot::efi {

  extern EFI_HANDLE           gImageHandle;
  extern EFI_SYSTEM_TABLE     *gST;
  extern EFI_BOOT_SERVICES    *gBS;
  extern EFI_RUNTIME_SERVICES *gRT;

  struct endl{};

  inline constexpr endl nl;

  using console = EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

  inline console* stdout;

  void init(EFI_HANDLE, EFI_SYSTEM_TABLE*);
  void clear_screen();
  void set_color(int fg, int bg);
  void move_cursor(int, int);

namespace impl {

  void print(view<wchar_t>);
  void print(view<char>);
  void print(const char*);
  void print(const wchar_t*);
  void print(wchar_t);
  void print(char);
  void print(int);
  void print(s64);
  void print(u64);
  void print(bool);
  void print(endl);
  void println();

  template<typename T>
    struct list : T {
      list(const T& arr) : T(arr) { }
    };

  template<typename T>
  void print(const list<T>& l, view<char> delim = ", ") {
    auto end = l.size() - 1;

    for(auto i = 0; i < end; i++)
      print(l[i], delim);

    if(end)
      print(l[end]);
  }

  template<size_t N>
  void print(const array<wchar_t, N>& str) {
    print(str.begin());
  }

  template<size_t N>
  void print(const array<char, N>& str) {
    print(str.begin());
  }

  template<typename T, size_t N>
  void print(const array<T, N>& str) {
    print('{', list{str}, '}');
  }


  template<typename T>
  void print(const view<T>& str) {
    print('[', list{str}, ']');
  }

  template<typename T, int width = 2>
  void print(const T* ptr) {
    constexpr view alphabet = "0123456789ABCDEF";
    constexpr view padding  = "0000000000000000";
    auto address = reinterpret_cast<size_t>(ptr);
    auto nibbles = array<char, 16>{};
    auto length  = 0;

    do nibbles[15 - length++] = alphabet[address & 15];
    while (address >>= 4);

    print("0x", padding.slice(16 - length % -width), nibbles.slice(16 - length));
  }

  template<typename Ret, typename... Params>
  void print(Ret (*func)(Params...)) {
    print(reinterpret_cast<void*>(func));
  }

  template<typename T, typename U, typename... Args>
  void print(const T& arg1, const U& arg2, const Args&... args) {
    print(arg1);
    print(arg2);
    (print(args), ...);
  }
} // namespace impl

  template<typename T, typename... Args>
  void print(const T& arg1, const Args&... args) {
    impl::print(arg1);
    (print(args), ...);
  }

  template<typename... Args>
  void println(const Args&... args) {
    print(args..., nl);
  }

}
