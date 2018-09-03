#include <kos/io/vga.hpp>
#include <kos/mem.hpp>
#include <kos/boot/multiboot.hpp>
#include <kos/idt.hpp>

using namespace kos::io;

vga_buffer<> vga;

__attribute__((interrupt)) void breakpoint_handler(kos::interrupts::interrupt_exception_frame*)
{
  vga << "Caught breakpoint!\n";
}; 

namespace kos::boot {

extern "C" int kmain(multiboot_information* ) {


  vga << "Booting kOS... \n"
      << "Loading interrupts...\n";

  interrupts::idt[3].set_handler(&breakpoint_handler);
  interrupts::load_idt(interrupts::idt);

  vga << "Triggering interrupt...\n";
  
  __asm__ volatile("int3");

  vga << "Successfully booted kOS.\n";
  return 0;
}

} // namespace kos::boot
