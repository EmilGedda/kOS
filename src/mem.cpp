#include <kos/mem.hpp>
#include <kos/types.hpp>

namespace kos::mem {

  using namespace kos::types;

  // forward only memcpy
  void* memcpy(void* dst, const void* src, size_t n) {

    auto* _src = reinterpret_cast<const char*>(src);
    char* _dst = reinterpret_cast<char*>(dst);

    while (n-- > 0)
      *_dst++ = *_src++;

    return dst;
  }

  void* memset(void* dst, int c, size_t n) {

    char* _dst = reinterpret_cast<char*>(dst);
    char value = c; 

    while(n-- > 0)
      *_dst++ = value;

    return dst;
  }

}
