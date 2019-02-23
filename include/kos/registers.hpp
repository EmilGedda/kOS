#pragma once
#include <kos/types.hpp>

namespace kos::registers {

  // Segment Registers
  u16 code_segment();
  u16 data_segment();
  u16 stack_segment();
  u16 extra_segment(); // used for string operations
  u16 fs();            // general purpose
  u16 gs();            // general purpose

  // Control Registers
  u64 cr0();
  u64 cr2(); // contains the linear (virtual) address which triggered a page fault
  u64 cr3();
  u64 cr4();
  // CR1, CR5-7, CR9-15 are all reserved

  // CR8 is a new register accessible in 64-bit mode using the REX prefix.
  // CR8 is used to prioritize external interrupts and is referred to as the task-priority register (TPR).
  u64 cr8();

  void pop_flags();
  void push_flags();
} // namespace kos::registers
