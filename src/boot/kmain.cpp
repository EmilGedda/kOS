#include <kos/io/vga.hpp>
#include <kos/io/uart.hpp>
#include <kos/mem.hpp>
#include <kos/boot/multiboot.hpp>
#include <kos/idt.hpp>
#include <array>
#include <algorithm>
#include <numeric>
#include <optional>

using namespace kos::io;
using namespace kos::io::serial;

vga_buffer<> vga;

__attribute__((interrupt)) void breakpoint_handler(kos::interrupts::interrupt_exception_frame*)
{
  vga << "Caught breakpoint!\n";
}; 

namespace kos::boot {

extern "C" int kmain(multiboot_information*) {

  vga << "Booting kOS... \n"
      << "Loading interrupts...\n";

  interrupts::idt[3].set_handler(&breakpoint_handler);
  interrupts::load_idt(interrupts::idt);

  vga << "Triggering interrupt...\n";
  
  //__asm__ volatile("int3");

  //try {
  //  vga << "Throwing exception...\n";
  //  throw 1;
  //} catch(float) {
  //  vga << "Should not catch float\n";
  //} catch(int x) {
  //  vga << "Caught " << typeid(x).name() << "\n";
  //}
  vga << "Successfully booted kOS.\n";
  int* x = reinterpret_cast<int*>(0xDEADBEEF);
  vga << x << "\n";

  std::array<char, 3> tmp = {'!', '!', '!'};
  std::for_each(std::begin(tmp), std::end(tmp), 
      [](auto val) {
        char str[2] = {val, 0x00};
        return vga << str;
      });
  std::optional<int> opt = 0;
  std::optional<int> opt2 = std::nullopt;
  return !opt2 ? *opt : 0;
}

} // namespace kos::boot
