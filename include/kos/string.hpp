#pragma once

namespace kos::string {
  template<size_t SIZE>
  void reverse(char(&)[SIZE]);
  void reverse(char*);
  void reverse(char*, size_t);

  int itoa(int, unsigned char*, int, int base = 10);
}
