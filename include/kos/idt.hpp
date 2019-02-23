#pragma once
#include <kos/types.hpp>

namespace kos::interrupts {

#pragma pack(push, 1)
  enum gate {
    INTERRUPT_GATE, // disable interrupts in handler
    TRAP_GATE       // enable interrupts in handler
  };

  enum privilege_level { RING0, RING1, RING2, RING3 };

  struct idt_options {
    u8              stack_table_index : 3 = 0;
    u8              reserved : 5          = 0;
    gate            gate_type : 1         = INTERRUPT_GATE;
    u8              must_be_one : 3       = ~0;    // vendor specific
    u8              must_be_zero : 1      = 0;     // vendor specific
    privilege_level priv : 2              = RING0; // privilege level required
    u8              present : 1           = 1;
  };

  using interrupt_exception_frame = void;
  using interrupt_handler         = void (*)(interrupt_exception_frame*);
  using errc_interrupt_handler    = void (*)(interrupt_exception_frame*, u64);

  struct idt_entry {
    u16         lower_funptr     = 0;
    u16         segment_selector = 0;
    idt_options options {};
    u16         middle_funptr = 0;
    u32         upper_funptr  = 0;
    u32         reserved      = 0; // vendor reserved

    void set_handler(interrupt_handler handler);
  };

  struct idtr {
    u16        limit = 0;
    idt_entry* base  = nullptr;
  };
#pragma pack(pop)

  static idt_entry idt[16];

  template <size_t N>
  void load_idt(idt_entry (&interrupt_table)[N]) {
    idtr descriptor = {N * sizeof(idt_entry) - 1, &interrupt_table[0]};
    __asm__ volatile("lidt (%0)" : : "r"(&descriptor));
  }

  u16 current_code_segment();

  static_assert(sizeof(idt_options) == 2);
  static_assert(sizeof(idt_entry) == 16);

} // namespace kos::interrupts
