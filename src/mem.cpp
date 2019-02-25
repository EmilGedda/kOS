#include <kos/mem.hpp>
#include <kos/types.hpp>

extern "C" {

namespace kos::mem {

  void* malloc(size_t size) {
    return 0;
  }

  void free(void* ptr) {
  }

  void* calloc(size_t nmemb, size_t size) {
    return 0;
  }

  size_t strlen(const char *str) {
    size_t len = 0;
    while(*str++ != '\0')
      len++;
    return len;
  }
  int posix_memalign(void** memptr, size_t alignment, size_t size) {
    return 0;
  }

  void abort() {
    while (true) continue;
  }

  int strcmp(const char* s1, const char* s2) {
    while (*s1 && *s1 != *s2) {
      s1++;
      s2++;
    }
    return *(const unsigned char*) s1 - *(const unsigned char*) s2;
  }

  using namespace kos::types;

  // forward only memcpy
  void* memcpy(void* dst, const void* src, size_t n) {

    auto* _src = reinterpret_cast<const char*>(src);
    char* _dst = reinterpret_cast<char*>(dst);

    while (n-- > 0) *_dst++ = *_src++;

    return dst;
  }

  void* memset(void* dst, int c, size_t n) {
    char* _dst  = reinterpret_cast<char*>(dst);
    char  value = c;

    while (n-- > 0) *_dst++ = value;

    return dst;
  }

  void* memmove(void* dest, const void* src, size_t count) {
    auto d = static_cast<char*>(dest);
    auto s = static_cast<char const*>(src);

    // If s and d are in distinct objects, the comparison is
    // unspecified behaviour, but either branch will work.
    if (s < d) {
      s += count;
      d += count;
      while (count--) *--d = *--s;
    } else {
      while (count--) *d++ = *s++;
    }

    return dest;
  }

} // namespace kos::mem
}
