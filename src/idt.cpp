#include <kos/idt.hpp>
#include <kos/types.hpp>

namespace kos::interrupts {

  u16 current_code_segment() {
    u16 cs = 0;
    __asm__ volatile("mov %%cs, %0" : "=r" (cs));
    return cs;
  }
}
