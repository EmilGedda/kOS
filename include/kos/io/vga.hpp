#pragma once

#include <stddef.h>

namespace kos::io {

enum color : unsigned char {
  black,
  blue,
  grueen,
  cyan,
  red,
  magenta,
  brown,
  light_gray,
};

struct vga_color {
  color c;
  bool bright;

  constexpr operator unsigned short() {
    return bright << 3 | c;
  }

};

template<size_t ptr = 0xB8000>
class vga_buffer {
  public:
    volatile short * const address = reinterpret_cast<volatile short * const>(ptr);
    vga_color foreground = {light_gray, false};
    vga_color background = {black, false};

    constexpr volatile short& operator[](size_t idx) {
      return address[idx];
    }

    void write(short c) {
      if ((c & 0xFF) == '\n')
        next_row();
      else
        this->operator[](count++) = c;
    }

    void next_row() {
      count = (count / 80) * 80 + 80;
    }
  private:
    int count = 0;
};


template<size_t ptr>
constexpr vga_buffer<ptr>& operator<<(vga_buffer<ptr>& vga, const char * str) {
  unsigned short color = vga.background << 4 | vga.foreground;
  while(*str != '\0')
    vga.write(color << 8  | *str++);
  return vga;
}

//template<size_t ptr>
//constexpr vga_buffer<ptr>& operator<<(vga_buffer<ptr>& vga, float f) {
//  return vga;
//}

} // namespace kos::io

