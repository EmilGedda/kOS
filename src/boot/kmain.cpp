#include <kos/io/vga.hpp>

extern "C" int kmain() {
  // Welcome, to kOS.
  

  using namespace kos::io;
  vga_buffer<> vga;
  vga << "Hello, welcome to kOS. This is a 64-bit OS, in C++17.";

  return 0;
}
