#include <kos/io/vga.hpp>
#include <kos/boot/multiboot.hpp>


namespace kos::boot {

extern "C" int kmain(multiboot_information* ) {
  // Welcome, to kOS.
  using namespace kos::io;
  vga_buffer<> vga;
  vga << "Hello, welcome to kOS.\n" 
      << "This is a 64-bit OS, in C++17.\n";

  return 0;
}

} // namespace kos::boot
