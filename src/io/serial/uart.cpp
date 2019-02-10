#include <kos/io/uart.hpp>
#include <kos/mem.hpp>

namespace kos::io::serial {

  using namespace mem;
  void UART::init() {
    out(port + 1, u8(0x00));    // Disable all interrupts
    out(port + 3, u8(0x80));    // Enable DLAB (set baud rate divisor)
    out(port + 0, u8(divisor));
    out(port + 1, u8(0x00));    //                  (hi byte)
    out(port + 3, u8(0x03));    // 8 bits, no parity, one stop bit
    out(port + 2, u8(0xC7));    // Enable FIFO, clear them, with 14-byte threshold
    out(port + 4, u8(0x0B));    // IRQs enabled, RTS/DSR set
  }

  void UART::write(u8 x) {
    while((in<u8>(port + 5) & 0x20) == 0);
    out(port, x);
  }

  void UART::set_divisor(int divisor) {
    // Set the most significant bit of the Line Control Register. This is the DLAB bit, and allows access to the divisor registers.
    // Send the least significant byte of the divisor value to [PORT + 0].
    // Send the most significant byte of the divisor value to [PORT + 1].
    // Clear the most significant bit of the Line Control Register.
  }

} // namespace kos::io::serial
