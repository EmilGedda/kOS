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

    operator unsigned short() {
      return bright << 3 | c;
    }

  };

  template<size_t ptr = 0xB8000>
  class vga_buffer {
    public:
      volatile short * const address = reinterpret_cast<volatile short * const>(ptr);
      vga_color foreground = {light_gray, true};
      vga_color background = {blue, false};


      volatile short& operator[](size_t idx) {
        return address[idx];
      }
  };


  template<size_t ptr>
  vga_buffer<ptr>& operator<<(vga_buffer<ptr>& vga, const char * str) {
    unsigned short color = vga.background << 4 | vga.foreground;
    for(size_t i = 0; str[i] != '\0'; i++)
      vga[i] = color << 8  | str[i];
    return vga;
  }

} // namespace kos::io
