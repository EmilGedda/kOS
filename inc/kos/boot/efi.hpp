#pragma once
#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>


namespace kos::boot::efi {

  template<typename T, typename U>
  auto min(T a, U b) {
    return a < b ? a : b;
  }

  template<typename T>
  struct view {
    const T* start = 0;
    const T* finish = 0;

    view(const T* start, const T* finish) : start(start), finish(finish) {

    }

    template<size_t N>
    view(const T (&str)[N]) : start(str), finish(str + N) {
    }

    template<typename U>
    bool operator==(const U* other) const {
      auto it = begin();
      while(it != end() && *other) {
        if(*it++ != *other++)
          return false;
      }
      return true;
    }

    template<typename T, size_t N>
    bool starts_with(const T (&other)[N]) const {
      return starts_with(view<T>{other});
    }

    template<typename U>
    bool starts_with(const view<U>& other) const {
      auto a = begin();
      auto b = other.begin();
      while(a != end() && b != other.end() && *a && *b) {
        if(*a++ != *b++)
          return false;
      }
      return true;
    }

    template<typename U>
    bool operator==(const view<U>& other) const {
      auto a = begin();
      auto b = other.begin();
      if (a == b && end() == other.end())
        return true;
      while(a != end()) {
        if(*a++ != *b++)
          return false;
      }
      return true;
    }

    template<typename U>
    bool operator!=(const U& other) const {
      return !(*this == other);
    }

    auto size() const -> size_t {
      return end() - begin();
    }

    auto begin() const -> const T* {
      return start;
    }

    auto end() const -> const T* {
      return finish;
    }

    auto operator[](size_t n) const -> const T& {
      return begin()[n];
    }

    auto slice(size_t prefix = 0, size_t suffix = 0) const -> view<T> {
      return { begin() + prefix, end() - suffix };
    }

    auto to_int() const -> int {
      int value = 0;
      int negate = 1;
      auto str = begin();

      if(*str == '-') {
        negate = -1;
        str++;
      }

      while(*str >= L'0' && *str <= L'9') {
        value *= 10;
        value += *str++ - L'0';
      }

      return negate * value;
    }
  };

  template<typename T, size_t N>
  struct array {
    T t[N] = { 0 };

    auto size() const -> size_t {
      return N;
    }

    auto begin() const -> const T* {
      return t;
    }

    auto end() const -> const T* {
      return t + N;
    }

    auto operator[](size_t n) -> T& {
      return t[n];
    }

    auto operator[](size_t n) const -> const T& {
      return t[n];
    }

    auto operator=(const view<T> str) -> array<T, N>& {
      auto count = str.size();
      for(int i = 0; i < N && i < count; i++)  {
        t[i] = str[i];
      }
      return *this;
    }
  };

  template<auto Max = ~0ULL>
  struct allocator {
  };

  struct cmdline {
    array<wchar_t, 255> initrd{};
    bool debug = false;
    int verbosity = 5;
  };

  using console = EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

  inline console* stdout;

  void init_screen(console*);
  void clear_screen();
  void move_cursor(int, int);
  void print(const char*);
  void print(const wchar_t*);
  void print(const cmdline&);
  void print(wchar_t);
  void print(int);
  void print(bool);

  template<size_t N>
  void print(const array<wchar_t, N>& str) {
    for(auto it = str.begin(); *it != '0' && it != str.end(); it++)
      print(*it);
  }

  template<size_t N>
  void print(const array<char, N>& str) {
    for(char c: str)
      print(str);
  }

  template<typename T, size_t N>
  void print(const array<T, N>& str) {
    auto end = str.end() - 1;
    for(auto it = str.begin(); *it && it != end; it++) {
      print(*it);
      print(L", ");
    }
    print(*end);
    print(L']');
    print(str);
  }

  template<typename T>
  void println(const T& str) {
    print(str);
    print("\r\n");
  }

  auto parse_cmdline(const wchar_t*) -> cmdline;
}
