#pragma once
#include "view.hpp"

namespace kos {
  template<typename T, size_t N>
  struct array {
    T t[N];


    auto size() const -> int {
      return N;
    }

    auto begin() -> T* {
      return t;
    }

    auto end() -> T* {
      return t + N;
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

    auto slice(int offset = 0, int cutoff = 0) -> view<T> {
      return { t + offset, t + N - cutoff };
    }

    operator view<T>&() {
      return this->slice();
    }

    auto operator=(const view<T> str) -> array<T, N>& {
      auto count = str.size();
      for(int i = 0; i < N && i < count; i++)  {
        t[i] = str[i];
      }
      return *this;
    }

    template<size_t S>
    bool operator==(const array<T, S>& rhs) {
      for(auto i = 0; i < (N < S ? N : S); i++)
        if(t[i] != rhs[i])
          return false;
      return true;
    }

    template<size_t S>
    bool operator!=(const array<T, S>& rhs) {
      return !(*this == rhs);
    }

  };

  template <class T, class... U>
  array(T, U...) -> array<T, 1 + sizeof...(U)>;
}
