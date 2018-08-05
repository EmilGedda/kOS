#include <kos/io/vga.hpp>
#include <kos/mem.hpp>
#include <kos/boot/multiboot.hpp>
#include <kos/idt.hpp>

using namespace kos::io;

vga_buffer<> vga;

__attribute__((interrupt)) void div_by_zero_handler(kos::interrupts::interrupt_exception_frame*)
{
  vga << "Caught division by zero\n";
};

namespace kos::boot {

extern "C" int kmain(multiboot_information* ) {

  u64 addr = reinterpret_cast<u64>(&div_by_zero_handler);
  interrupts::idt[0].lower_funptr = addr;
  interrupts::idt[0].middle_funptr = addr >> 16;
  interrupts::idt[0].upper_funptr = addr >> 32;
  interrupts::idt[0].gdt_selector.index = interrupts::current_code_segment();

  vga << "Booting kOS... \n"
      << "Loading interrupts...\n";
  interrupts::load_idt(interrupts::idt);
  // Welcome, to kOS.

  vga << "Triggering interrupt...\n";
  
  int x = 0;
  __asm__ volatile("div %0" :: "r" (x));

  vga << "Successfully booted kOS.\n";
  return 0;
}

} // namespace kos::boot
