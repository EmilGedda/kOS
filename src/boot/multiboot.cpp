#include <kos/boot/multiboot.hpp>
#include <kos/types.hpp>
#include <kos/debug.hpp>

namespace kos::multiboot {
  boot_information::boot_information(virtual_address<boot_information_inner> multiboot, u64 offset) 
    : multiboot(multiboot), offset(offset) {
      assert((static_cast<u64>(multiboot) & 0b111) == 0);
      assert((static_cast<u64>(offset) & 0b111) == 0);
      assert(multiboot->has_valid_end_tag());
  }
} // namespace kos::multiboot
