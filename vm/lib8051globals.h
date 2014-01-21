/* Copyright (C) 2014 Luk Bettale

   This file is part of VM8051.

   VM8051 is free software: you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with VM8051.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef LIB8051GLOBALS_H
#define LIB8051GLOBALS_H

#include "lib8051.h"

#undef CY
#undef AC
#undef F0
#undef RS1
#undef RS0
#undef RS
#undef OV
#undef P
#undef IE0
#undef TF0
#undef IE1
#undef TF1
#undef TI
#undef RI

#define cycles vm->cycles
#define _data vm->_data
#define _sfr vm->_sfr
#define _xdata vm->_xdata
#define _code vm->_code
#define IR vm->IR
#define PC vm->PC
#define interrupted vm->interrupted
#define interrupts_blocked vm->interrupts_blocked
#define leakages vm->leakages

/* TCON bits */
#define TF1 ((TCON & 0x80) >> 7)
#define TR1 ((TCON & 0x40) >> 6)
#define TF0 ((TCON & 0x20) >> 5)
#define TR0 ((TCON & 0x10) >> 4)
#define IE1 ((TCON & 0x08) >> 3)
#define IT1 ((TCON & 0x04) >> 2)
#define IE0 ((TCON & 0x02) >> 1)
#define IT0 ((TCON & 0x01) >> 0)

/* SCON bits */
#define SM0 ((SCON & 0x80) >> 7)
#define SM1 ((SCON & 0x40) >> 6)
#define SM2 ((SCON & 0x20) >> 5)
#define REN ((SCON & 0x10) >> 4)
#define TB8 ((SCON & 0x08) >> 3)
#define RB8 ((SCON & 0x04) >> 2)
#define TI  ((SCON & 0x02) >> 1)
#define RI  ((SCON & 0x01) >> 0)

/* IE bits */
#define EA  ((IE & 0x80) >> 7)
#define ES  ((IE & 0x10) >> 4)
#define ET1 ((IE & 0x08) >> 3)
#define EX1 ((IE & 0x04) >> 2)
#define ET0 ((IE & 0x02) >> 1)
#define EX0 ((IE & 0x01) >> 0)

/* IP bits */
#define PS  ((IP & 0x10) >> 4)
#define PT1 ((IP & 0x08) >> 3)
#define PX1 ((IP & 0x04) >> 2)
#define PT0 ((IP & 0x02) >> 1)
#define PX0 ((IP & 0x01) >> 0)

/* PSW bits */
#define CY ((PSW & CY_MASK) >> CY_POS)
#define AC ((PSW & AC_MASK) >> AC_POS)
#define F0 ((PSW & F0_MASK) >> F0_POS)
#define RS1 ((PSW & RS1_MASK) >> RS1_POS)
#define RS0 ((PSW & RS0_MASK) >> RS0_POS)
#define RS ((RS1 << 1) | RS0)
#define OV ((PSW & OV_MASK) >> OV_POS)
#define P ((PSW & P_MASK) >> P_POS)

#undef DPTR
#define DPTR (DPL + (DPH << 8))

#undef regs
#define regs (_data+(RS << 3))
#undef bit_addressable
#define bit_addressable (_data+0x20)

#endif  /* LIB8051GLOBALS_H */
