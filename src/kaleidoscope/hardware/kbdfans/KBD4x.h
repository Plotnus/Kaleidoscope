/* -*- mode: c++ -*-
 * Kaleidoscope-Hardware-KBDFans-KBD4x -- KBD4x hardware support for Kaleidoscope
 * Copyright (C) 2019  Keyboard.io, Inc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of version 3 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#ifdef ARDUINO_AVR_KBD4X

#include <Arduino.h>
#define HARDWARE_IMPLEMENTATION kaleidoscope::hardware::kbdfans::KBD4x
#include "Kaleidoscope-HIDAdaptor-KeyboardioHID.h"

#include "kaleidoscope/macro_helpers.h"

struct cRGB {
  uint8_t g;
  uint8_t r;
  uint8_t b;
};

#define CRGB(r, g, b) (cRGB){g, r, b}

#include "kaleidoscope/hardware/ATMegaKeyboard.h"
#include "kaleidoscope/driver/led/WS2812.h"

using Color = kaleidoscope::driver::led::color::GRB;

namespace kaleidoscope {
namespace hardware {
namespace kbdfans {
class KBD4x: public kaleidoscope::hardware::ATMegaKeyboard {
 public:
  KBD4x(void) {
    /* These two lines here are the result of many hours spent chasing ghosts.
     * These are great lines, and we love them dearly, for they make a set of
     * pins that would otherwise be reserved for JTAG accessible from the
     * firmware.
     *
     * Most AVR chips that get put into keyboards have the JTAG port disabled in
     * fuses, but some do not. When they're used for JTAG, then no matter what
     * we do in the firmware, they will not be affected. So in case JTAG is not
     * disabled in fuses, we want to do that in the firmware. Luckily for us,
     * that's doable, we just have to write the JTD bit into MCUCR twice within
     * four cycles. These two lines do just that.
     *
     * For more information, see the ATMega16U4/ATMega32U4 datasheet, the
     * following sections:
     *  - 2.2.7 (PIN Descriptions; PIN F)
     *  - 7.8.7 (On-chip Debug System)
     *  - 26.5.1 (MCU Control Register – MCUCR)
     */
    MCUCR |= (1 << JTD);
    MCUCR |= (1 << JTD);

    // Disable Clock division
    CLKPR = (1 << CLKPCE);
    CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
  }

  ATMEGA_KEYBOARD_CONFIG(
    ROW_PIN_LIST({ PIN_D0, PIN_D1, PIN_D2, PIN_D3 }),
    COL_PIN_LIST({ PIN_F0, PIN_F1, PIN_F4, PIN_F5, PIN_F6, PIN_F7, PIN_B3, PIN_B1, PIN_B0, PIN_D5, PIN_B7, PIN_C7 })
  );

  static constexpr int8_t led_count = 0;

  static kaleidoscope::driver::led::WS2812<PIN_E2, Color, 6> underglow;

  void resetDevice();
};

#define KEYMAP(                                                                    \
         R0C0, R0C1, R0C2, R0C3, R0C4, R0C5, R0C6, R0C7, R0C8, R0C9, R0C10, R0C11, \
         R1C0, R1C1, R1C2, R1C3, R1C4, R1C5, R1C6, R1C7, R1C8, R1C9, R1C10, R1C11, \
         R2C0, R2C1, R2C2, R2C3, R2C4, R2C5, R2C6, R2C7, R2C8, R2C9, R2C10, R2C11, \
         R3C0, R3C1, R3C2, R3C3, R3C4,    R3C5,    R3C7, R3C8, R3C9, R3C10, R3C11 \
  ) {                                                                                  \
         { R0C0, R0C1, R0C2, R0C3, R0C4, R0C5, R0C6, R0C7, R0C8, R0C9, R0C10, R0C11, }, \
         { R1C0, R1C1, R1C2, R1C3, R1C4, R1C5, R1C6, R1C7, R1C8, R1C9, R1C10, R1C11, }, \
         { R2C0, R2C1, R2C2, R2C3, R2C4, R2C5, R2C6, R2C7, R2C8, R2C9, R2C10, R2C11, }, \
         { R3C0, R3C1, R3C2, R3C3, R3C4, R3C5, R3C5, R3C7, R3C8, R3C9, R3C10, R3C11, } \
  }
}

}
}

#include "kaleidoscope/hardware/key_indexes.h"

extern kaleidoscope::hardware::kbdfans::KBD4x &KBD4x;

#endif
