#include <algorithm>
#include <array>
#include <kos/boot/multiboot.hpp>
#include <kos/idt.hpp>
#include <kos/io/uart.hpp>
#include <kos/io/vga.hpp>
#include <kos/mem.hpp>
#include <kos/types.hpp>
#include <numeric>
#include <optional>

using namespace kos::io;
using namespace kos::io::serial;

vga_buffer<> vga;

__attribute__((interrupt)) void breakpoint_handler(kos::interrupts::interrupt_exception_frame*) {
  vga << "Caught breakpoint!\n";
};

namespace kos::boot {

  extern "C" int kmain(u32*) {
    int dummy = 0;

    physical_address p(0x12u);
    virtual_address  a(0x11u);
    vga.clear_screen();

    vga << "Booting kOS... \n"
        << "Loading interrupts...\n";

    interrupts::idt[3].set_handler(&breakpoint_handler);
    interrupts::load_idt(interrupts::idt);

    vga << "Triggering breakpoint...\n";

    __asm__ volatile("int3");

    // try {
    //  vga << "Throwing exception...\n";
    //  throw 1;
    //} catch(float) {
    //  vga << "Should not catch float\n";
    //} catch(int x) {
    //  vga << "Caught " << typeid(x).name() << "\n";
    //}
    vga << "Successfully booted kOS.\n";
    vga << "kmain entry point: " << reinterpret_cast<u64*>(&kmain) << "\n";
    vga << "Stack address entry point: " << reinterpret_cast<u64*>(&dummy) << "\n";

    std::array<char, 3> tmp = {'!', '!', '!'};
    std::for_each(std::begin(tmp), std::end(tmp), [](auto val) {
      char str[2] = {val, 0x00};
      return vga << str;
    });

    std::optional<int> opt  = 0;
    std::optional<int> opt2 = std::nullopt;
    return !opt2 ? *opt : 0;
  }

} // namespace kos::boot
