#include <kos/idt.hpp>
#include <kos/types.hpp>

namespace kos::interrupts {

  u16 current_code_segment() {
    u16 cs = 0;
    __asm__ volatile("mov %%cs, %0" : "=r" (cs));
    return cs;
  }

  void idt_entry::set_handler(interrupt_handler handler) {
    u64 address = reinterpret_cast<u64>(handler);
    lower_funptr = address;
    middle_funptr = address >> 16;
    upper_funptr = address >> 32;
    segment_selector = current_code_segment();
  }
}
