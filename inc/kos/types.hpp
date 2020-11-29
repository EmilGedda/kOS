#pragma once

#define __ALIAS(NAME, SIZE, SIZESUFFIX) using NAME##SIZE##SIZESUFFIX

#define __CSTDINT(PREFIX, SIZE, TYPE)          \
  __ALIAS(PREFIX##int, SIZE, _t)       = TYPE; \
  __ALIAS(PREFIX##int_fast, SIZE, _t)  = TYPE; \
  __ALIAS(PREFIX##int_least, SIZE, _t) = TYPE

#define _ALIAS(TYPE, SIZE)                                                                                            \
  static_assert(sizeof(TYPE) * 8 == SIZE, "Specified size for type alias does not match the size of the given type"); \
  __ALIAS(u, SIZE, ) = unsigned TYPE;                                                                                 \
  __ALIAS(s, SIZE, ) = TYPE;                                                                                 \
  __CSTDINT(u, SIZE, unsigned TYPE);                                                                                  \
  __CSTDINT(, SIZE, TYPE)

namespace kos {
  inline namespace types {

    using u0 = void;
    using u1 = bool;

    _ALIAS(char, 8);
    _ALIAS(short, 16);
    _ALIAS(int, 32);
    _ALIAS(long long, 64);

    using size_t  = u64;
    using ssize_t = s64;

  } // namespace types
} // namespace kos

#undef _ALIAS
#undef __CSTDINT
#undef __ALIAS
