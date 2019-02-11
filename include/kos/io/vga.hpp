#pragma once

#include <stddef.h>
#include <algorithm>
#include <kos/mem.hpp>

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
    short * const address = reinterpret_cast<short * const>(ptr);
    vga_color foreground = {light_gray, false};
    vga_color background = {black, false};

    constexpr short& operator[](size_t idx) {
      return address[idx];
    }

    void write(short c) {
      switch(c & 0xFF) {
        case '\n':
          next_row();
          break;
        case '\r':
          next_row();
          offset -= 80;
          break;
        case '\t':
          for(auto i = 0; i < 4; i++)
            this->operator[](offset++) = (c & ~0xFF) | ' ';
          break;
        default:
          if(offset == 80 * 25) {
            std::copy(begin() + 80, end(), begin());
            std::fill(end() - 80, end(), (c & ~0xFF) | ' '); // clear the new line
            offset -= 80;
          }
          this->operator[](offset++) = c;
          break;

      }}   

    void next_row() {
      offset = (offset / 80) * 80 + 80;
    }

    auto begin() {
      return address;
    }

    auto current() {
      return begin() + offset;
    }
    
    auto end() {
      return address + 80 * 25;
    }

    void move_cursor(int pos) {
      auto upper = reinterpret_cast<void*>(0x3D4);
      auto lower = reinterpret_cast<void*>(0x3D5);
      mem::out<u8>(upper, 0x0F);
      mem::out<u8>(lower, (pos & 0xFF));
      mem::out<u8>(upper, 0x0E);
      mem::out<u8>(lower, ((pos >> 8) & 0xFF));
    }

    void enable_cursor() {

    }

    void update_cursor() {
      move_cursor(offset);
    }

  private:
    int offset = 0;
};


template<size_t ptr>
constexpr vga_buffer<ptr>& operator<<(vga_buffer<ptr>& vga, const char * str) {
  unsigned short color = vga.background << 4 | vga.foreground;
  while(*str != '\0')
    vga.write(color << 8  | *str++);
  vga.update_cursor();
  return vga;
}

template<size_t ptr, typename T>
constexpr vga_buffer<ptr>& operator<<(vga_buffer<ptr>& vga, const T* obj) {
  unsigned short color = vga.background << 4 | vga.foreground;
  u64 address = reinterpret_cast<u64>(obj);
  auto length = 0;
  char buffer[16] = {0x00};
  vga << "0x";

  while(address != 0) {
    auto rem = address % 16;
    address /= 16;
    buffer[length] = rem < 10 ? rem + '0' : rem  + ('A' - 10);
    length++;
  }

  for(auto i = length - 1; i >= 0; i--)
    vga.write(color << 8 | buffer[i]);

  vga.update_cursor();
  return vga;
}

//template<size_t ptr>
//constexpr vga_buffer<ptr>& operator<<(vga_buffer<ptr>& vga, float f) {
//  return vga;
//}

} // namespace kos::io

