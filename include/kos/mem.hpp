#pragma once

#include <kos/types.hpp>

namespace kos::mem {

  using namespace kos;

  extern "C" void* memcpy(void* src, const void* dst, size_t n);
  extern "C" void* memset(void* src, int c, size_t n);

} // namespace kos::mem

