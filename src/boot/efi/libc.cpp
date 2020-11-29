
namespace {
  using size_t = unsigned long long;
}

extern "C" void* memcpy(void *dest, const void *src, size_t len) {
  auto *to = static_cast<unsigned char*>(dest);
  auto *from = static_cast<const unsigned char*>(src);
  while (len-- > 0)
    *to++ = *from++;
  return to;
}

extern "C" void* memset(void *s, int c, size_t len) {
  auto* dst = static_cast<unsigned char*>(s);
  while (len-- > 0)
    *dst++ = static_cast<unsigned char>(c);
  return s;
}

