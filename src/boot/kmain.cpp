#include <kos/boot/multiboot.hpp>
#include <kos/idt.hpp>
#include <kos/instructions/cpuid.hpp>
#include <kos/io/uart.hpp>
#include <kos/io/vga.hpp>
#include <kos/mem.hpp>
#include <kos/types.hpp>

#include <algorithm>
#include <array>
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


    auto vendor = cpuid::vendor_string();
    char s[13];

    std::copy(vendor.begin(), vendor.end(), s);

    vga << "Vendor string: " << s;

    return 0;
  }

} // namespace kos::boot
