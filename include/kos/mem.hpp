#pragma once

#include <kos/types.hpp>
#include <type_traits>

namespace kos::mem {

  using namespace kos;

  //  extern "C" void* memcpy(void* __restrict src, const void* __restrict dst, size_t n);
  //  extern "C" void* memset(void* src, int c, size_t n) _THROW;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wuninitialized"
  template <typename T, typename = std::enable_if_t<io_port_type_v<T>>>
  T in(void* port) {
    T payload;
    __asm__ volatile("in %w[port], %[data]" : : [data] "a"(payload), [port] "Nd"(port));
    return payload;
  }
#pragma clang diagnostic pop

  template <typename T, typename = std::enable_if_t<io_port_type_v<T>>>
  void out(void* port, T data) {
    __asm__ volatile("out %[data], %w[port]" : : [data] "a"(data), [port] "Nd"(port));
  }

} // namespace kos::mem
