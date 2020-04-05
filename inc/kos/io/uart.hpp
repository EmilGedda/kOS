// UART 16550
#pragma once
#include <kos/types.hpp>

namespace kos::io::serial {

  class UART {

    public:
    UART(u64 p)
        : port(reinterpret_cast<u8*>(p)) {
    }

    void init();
    void write(u8 x);
    void set_divisor(int divisor);

    private:
    unsigned char* port;
    const int      baud    = 115200;
    const int      divisor = 1;
  };

  UART COM1(0x3F8);
  UART COM2(0x2F8);
  UART COM3(0x3E8);
  UART COM4(0x2E8);
} // namespace kos::io::serial
