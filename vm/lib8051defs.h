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

#ifndef LIB8051DEF_H
#define LIB8051DEF_H

#include <stdint.h>

/* 8051 virtual machine */
struct vm8051
{
  uint32_t cycles;
  uint8_t _data[256];
  uint8_t _sfr[128];
  uint8_t _xdata[65536];
  uint8_t _code[65536];
  uint8_t IR[4];
  uint16_t PC;
  uint8_t interrupted;
  uint8_t interrupts_blocked;
};

#define LOW  (1 << 0)
#define HIGH (1 << 1)

/* SFRs */
#define SP   _sfr[0x81 ^ 0x80]
#define DPL  _sfr[0x82 ^ 0x80]
#define DPH  _sfr[0x83 ^ 0x80]
#define PCON _sfr[0x87 ^ 0x80]

#define TCON _sfr[0x88 ^ 0x80]
#define TMOD _sfr[0x89 ^ 0x80]
#define TL0  _sfr[0x8A ^ 0x80]
#define TL1  _sfr[0x8B ^ 0x80]
#define TH0  _sfr[0x8C ^ 0x80]
#define TH1  _sfr[0x8D ^ 0x80]

#define SCON _sfr[0x98 ^ 0x80]
#define SBUF _sfr[0x99 ^ 0x80]

#define IE   _sfr[0xA8 ^ 0x80]
#define IP   _sfr[0xB8 ^ 0x80]

#define P0   _sfr[0x80 ^ 0x80]
#define P1   _sfr[0x90 ^ 0x80]
#define P2   _sfr[0xA0 ^ 0x80]
#define P3   _sfr[0xB0 ^ 0x80]

#define PSW  _sfr[0xD0 ^ 0x80]
#define A    _sfr[0xE0 ^ 0x80]
#define B    _sfr[0xF0 ^ 0x80]

/* interrupts */
#define IE0_POS 1
#define IE0_MASK (0x01 << IE0_POS)
#define IE0(vm) ((vm->TCON & IE0_MASK) >> IE0_POS)

#define TF0_POS 5
#define TF0_MASK (0x01 << TF0_POS)
#define TF0(vm) ((vm->TCON & TF0_MASK) >> TF0_POS)

#define IE1_POS 3
#define IE1_MASK (0x01 << IE1_POS)
#define IE1(vm) ((vm->TCON & IE1_MASK) >> IE1_POS)

#define TF1_POS 7
#define TF1_MASK (0x01 << TF1_POS)
#define TF1(vm) ((vm->TCON & TF1_MASK) >> TF1_POS)

#define TI_POS 1
#define TI_MASK (0x01 << TI_POS)
#define TI(vm) ((vm->SCON & TI_MASK) >> TI_POS)

#define RI_POS 0
#define RI_MASK (0x01 << RI_POS)
#define RI(vm) ((vm->SCON & RI_MASK) >> RI_POS)

/* program status word */
#define CY_POS 7
#define CY_MASK (0x01 << CY_POS)
#define CY(vm) ((vm->PSW & CY_MASK) >> CY_POS)

#define AC_POS 6
#define AC_MASK (0x01 << AC_POS)
#define AC(vm) ((vm->PSW & AC_MASK) >> AC_POS)

#define F0_POS 5
#define F0_MASK (0x01 << F0_POS)
#define F0(vm) ((vm->PSW & F0_MASK) >> F0_POS)

#define RS1_POS 4
#define RS1_MASK (0x01 << RS1_POS)
#define RS1(vm) ((vm->PSW & RS1_MASK) >> RS1_POS)

#define RS0_POS 3
#define RS0_MASK (0x01 << RS0_POS)
#define RS0(vm) ((vm->PSW & RS0_MASK) >> RS0_POS)

#define RS(vm) ((RS1(vm) << 1) | RS0(vm))

#define OV_POS 2
#define OV_MASK (0x01 << OV_POS)
#define OV(vm) ((vm->PSW & OV_MASK) >> OV_POS)

#define P_POS 0
#define P_MASK (0x01 << P_POS)
#define P(vm) ((vm->PSW & P_MASK) >> P_POS)

#define DPTR(vm) (vm->DPL + (vm->DPH << 8))

#define regs(vm) (vm->_data+(RS(vm) << 3))
#define bit_addressable(vm) (vm->_data+0x20)

#endif  /* LIB8051DEF_H */
