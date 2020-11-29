#pragma once
namespace kos {
  template<typename T>
  struct view {
    const T* start = 0;
    const T* finish = 0;

    view(const T* start, const T* finish) : start(start), finish(finish) {

    }

    template<size_t N>
    constexpr view(const T (&str)[N]) : start(str), finish(str + N) {
    }

    template<size_t N>
    bool starts_with(const T (&other)[N]) const {
      return starts_with(view{other}.slice(0, 1));
    }

    template<typename U>
    bool eq(const view<U>& other) const {
      // kassert(size() >= other.size());
      auto a = begin();
      auto b = other.begin();

      if (a == b)
        return true;

      while(b != other.end())
        if(*a++ != *b++)
          return false;

      return true;
    }

    template<typename U>
    bool starts_with(const view<U>& other) const {
      return other.size() > size() ? false : eq(other);
    }

    template<typename U>
    bool operator==(const view<U>& other) const {
      return other.size() != size() ? false : eq(other);
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

    template<typename U>
    bool operator!=(const U& other) const {
      return !(*this == other);
    }

    auto size() const -> size_t {
      return end() - begin();
    }

    constexpr auto begin() const -> const T* {
      return start;
    }

    constexpr auto end() const -> const T* {
      return finish;
    }

    constexpr auto operator[](size_t n) const -> const T& {
      return begin()[n];
    }

    constexpr auto slice(size_t prefix = 0, size_t suffix = 0) const -> view<T> {
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

}
