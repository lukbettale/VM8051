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

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "lib8051.h"

/* simulate global variables for a struct vm8051 *vm */
#include "lib8051globals.h"

#ifndef PURE_8051
extern void operate_coprocessors (struct vm8051 *vm);
#else
#define operate_coprocessors(vm)
#endif

#define GO_ISR(I,E,P) ((E) && (I) && (!interrupted || (P)))

int32_t get_timer0 (struct vm8051 *vm)
{
  switch (TMOD & 0x03)
    {
    case 0x00: return (TH0 << 5) + TL0;
    case 0x01: return (TH0 << 8) + TL0;
    case 0x02: return TL0;
    case 0x03: return (TH0 << 8) + TL0;
    }
  return -1;
}

int32_t get_timer1 (struct vm8051 *vm)
{
  switch (TMOD & 0x30)
    {
    case 0x00: return (TH1 << 5) + TL1;
    case 0x10: return (TH1 << 8) + TL1;
    case 0x20: return TL1;
    case 0x30: return (TH1 << 8) + TL1;
    }
  return -1;
}

/* get the instruction at addr in inst and return inst length */
size_t inst8051 (struct vm8051 *vm, uint8_t *inst, uint16_t addr)
{
  size_t inst_len = 0;

  inst[0] = _code[addr+inst_len++];
  /* opcodes with register argument */
  if (inst[0] & 0x08)
    {
      switch (inst[0] & 0xF0)
        {
        case 0x70:
        case 0x80:
        case 0xA0:
        case 0xD0:
          inst[1] = _code[addr+inst_len++];
          break;
        case 0xB0:
          inst[1] = _code[addr+inst_len++];
          inst[2] = _code[addr+inst_len++];
          break;
        }
    }
  /* opcodes with indirect register argument */
  else if ((inst[0] & 0x06) == 0x06)
    {
      switch (inst[0] & 0xF0)
        {
        case 0x70:
        case 0x80:
        case 0xA0:
          inst[1] = _code[addr+inst_len++];
          break;
        case 0xB0:
          inst[1] = _code[addr+inst_len++];
          inst[2] = _code[addr+inst_len++];
          break;
        }
    }
  else if ((inst[0] & 0x0F) == 0x01)
    {
      inst[1] = _code[addr+inst_len++];
    }
  else
    {
      switch (inst[0])
        {
        case 0x10:
        case 0x20:
        case 0x30:
        case 0x90:
        case 0x02:
        case 0x12:
        case 0x43:
        case 0x53:
        case 0x63:
        case 0xB4:
        case 0x75:
        case 0x85:
        case 0xB5:
        case 0xD5:
          inst[1] = _code[addr+inst_len++];
          inst[2] = _code[addr+inst_len++];
          break;
        case 0x40:
        case 0x50:
        case 0x60:
        case 0x70:
        case 0x80:
        case 0xA0:
        case 0xB0:
        case 0xC0:
        case 0xD0:
        case 0x42:
        case 0x52:
        case 0x62:
        case 0x72:
        case 0x82:
        case 0x92:
        case 0xA2:
        case 0xB2:
        case 0xC2:
        case 0xD2:
        case 0x24:
        case 0x34:
        case 0x44:
        case 0x54:
        case 0x64:
        case 0x74:
        case 0x94:
        case 0x05:
        case 0x15:
        case 0x25:
        case 0x35:
        case 0x45:
        case 0x55:
        case 0x65:
        case 0x95:
        case 0xC5:
        case 0xE5:
        case 0xF5:
          inst[1] = _code[addr+inst_len++];
          break;
        }
    }
  inst[3] = (uint8_t) inst_len;
  return inst_len;
}

/* fetch the next instruction */
void fetch8051 (struct vm8051 *vm)
{
  PC += inst8051 (vm, IR, PC);
}

/* reset the virtual machine in vm with the code in progname */
void reset8051 (struct vm8051 *vm)
{
  uint32_t i;

  PC = 0;
  IR[0] = 0;
  IR[1] = 0;
  IR[2] = 0;
  IR[3] = 0;

  for (i = 0; i < 256; i++)
    _data[i] = 0;
  for (i = 0; i < 128; i++)
    _sfr[i] = 0;
  SP = 0x07;
  P0 = 0xFF;
  P1 = 0xFF;
  P2 = 0xFF;
  P3 = 0xFF;

  for (i = 0; i < 65536; i++)
    _xdata[i] = 0;

  interrupted = 0;
  interrupts_blocked = 0;

  cycles = 0;
}

/* run the current instruction */
void operate8051 (struct vm8051 *vm)
{
  uint8_t n;
  int32_t timer;
  uint32_t cycles_prev = cycles;

  /* set external interrupts */
  if (!IT0)
    {
      if (P3 & (1<<2))
        {
          TCON &= ~IE0_MASK;
          /* external interrupt */;
        }
      else
        TCON |= IE0_MASK;
    }
  if (!IT1)
    {
      if (P3 & (1<<3))
        {
          TCON &= ~IE1_MASK;
          /* external interrupt */;
        }
      else
        TCON |= IE1_MASK;
    }
  /* opcodes with register argument */
  if (IR[0] & 0x08)
    {
      n = IR[0] & 0x07;
      switch (IR[0] & 0xF0)
        {
        case 0x00:
          inst_inc_Rn (vm, n);
          break;
        case 0x10:
          inst_dec_Rn (vm, n);
          break;
        case 0x20:
          inst_add_Rn (vm, n);
          break;
        case 0x30:
          inst_addc_Rn (vm, n);
          break;
        case 0x40:
          inst_orl_Rn (vm, n);
          break;
        case 0x50:
          inst_anl_Rn (vm, n);
          break;
        case 0x60:
          inst_xrl_Rn (vm, n);
          break;
        case 0x70:
          inst_mov_data_to_Rn (vm, n, IR[1]);
          break;
        case 0x80:
          inst_mov_Rn_to_direct (vm, n, IR[1]);
          break;
        case 0x90:
          inst_subb_Rn (vm, n);
          break;
        case 0xA0:
          inst_mov_direct_to_Rn (vm, n, IR[1]);
          break;
        case 0xB0:
          inst_cjne_data_with_Rn (vm, n, IR[1], IR[2]);
          break;
        case 0xC0:
          inst_xch_Rn (vm, n);
          break;
        case 0xD0:
          inst_djnz_Rn (vm, n, IR[1]);
          break;
        case 0xE0:
          inst_mov_Rn (vm, n);
          break;
        case 0xF0:
          inst_mov_to_Rn (vm, n);
          break;
        }
    }
  /* opcodes with indirect register argument */
  else if ((IR[0] & 0x06) == 0x06)
    {
      n = IR[0] & 0x01;
      switch (IR[0] & 0xF0)
        {
        case 0x00:
          inst_inc_atRi (vm, n);
          break;
        case 0x10:
          inst_dec_atRi (vm, n);
          break;
        case 0x20:
          inst_add_atRi (vm, n);
          break;
        case 0x30:
          inst_addc_atRi (vm, n);
          break;
        case 0x40:
          inst_orl_atRi (vm, n);
          break;
        case 0x50:
          inst_anl_atRi (vm, n);
          break;
        case 0x60:
          inst_xrl_atRi (vm, n);
          break;
        case 0x70:
          inst_mov_data_to_atRi (vm, n, IR[1]);
          break;
        case 0x80:
          inst_mov_atRi_to_direct (vm, n, IR[1]);
          break;
        case 0x90:
          inst_subb_atRi (vm, n);
          break;
        case 0xA0:
          inst_mov_direct_to_atRi (vm, n, IR[1]);
          break;
        case 0xB0:
          inst_cjne_data_with_atRi (vm, n, IR[1], IR[2]);
          break;
        case 0xC0:
          inst_xch_atRi (vm, n);
          break;
        case 0xD0:
          inst_xchd_atRi (vm, n);
          break;
        case 0xE0:
          inst_mov_atRi (vm, n);
          break;
        case 0xF0:
          inst_mov_to_atRi (vm, n);
          break;
        }
    }
  else if ((IR[0] & 0x0F) == 0x01)
    {
      n = (IR[0] & 0xE0) >> 5;
      if (IR[0] & 0x10)
        inst_acall (vm, n, IR[1]);
      else
        inst_ajmp (vm, n, IR[1]);
    }
  else if ((IR[0] & 0xEE) == 0xE2)
    {
      n = IR[0] & 0x01;
      if (IR[0] & 0x10)
        inst_movx_to_atRi (vm, n);
      else
        inst_movx_atRi (vm, n);
    }
  else
    {
      switch (IR[0])
        {
        case 0x00:
          inst_nop (vm);
          break;
        case 0x10:
          inst_jbc (vm, IR[1], IR[2]);
          break;
        case 0x20:
          inst_jb (vm, IR[1], IR[2]);
          break;
        case 0x30:
          inst_jnb (vm, IR[1], IR[2]);
          break;
        case 0x40:
          inst_jc (vm, IR[1]);
          break;
        case 0x50:
          inst_jnc (vm, IR[1]);
          break;
        case 0x60:
          inst_jz (vm, IR[1]);
          break;
        case 0x70:
          inst_jnz (vm, IR[1]);
          break;
        case 0x80:
          inst_sjmp (vm, IR[1]);
          break;
        case 0x90:
          inst_mov_to_DPTR (vm, IR[1], IR[2]);
          break;
        case 0xA0:
          inst_orl_not_bit (vm, IR[1]);
          break;
        case 0xB0:
          inst_anl_not_bit (vm, IR[1]);
          break;
        case 0xC0:
          inst_push (vm, IR[1]);
          break;
        case 0xD0:
          inst_pop (vm, IR[1]);
          break;
        case 0xE0:
          inst_movx_atDPTR (vm);
          break;
        case 0xF0:
          inst_movx_to_atDPTR (vm);
          break;
        case 0x02:
          inst_ljmp (vm, IR[1], IR[2]);
          break;
        case 0x12:
          inst_lcall (vm, IR[1], IR[2]);
          break;
        case 0x22:
          inst_ret (vm);
          break;
        case 0x32:
          inst_reti (vm);
          break;
        case 0x42:
          inst_orl_to_direct (vm, IR[1]);
          break;
        case 0x52:
          inst_anl_to_direct (vm, IR[1]);
          break;
        case 0x62:
          inst_xrl_to_direct (vm, IR[1]);
          break;
        case 0x72:
          inst_orl_bit (vm, IR[1]);
          break;
        case 0x82:
          inst_anl_bit (vm, IR[1]);
          break;
        case 0x92:
          inst_mov_to_bit (vm, IR[1]);
          break;
        case 0xA2:
          inst_mov_bit (vm, IR[1]);
          break;
        case 0xB2:
          inst_cpl_bit (vm, IR[1]);
          break;
        case 0xC2:
          inst_clr_bit (vm, IR[1]);
          break;
        case 0xD2:
          inst_setb_bit (vm, IR[1]);
          break;
        case 0x03:
          inst_rr (vm);
          break;
        case 0x13:
          inst_rrc (vm);
          break;
        case 0x23:
          inst_rl (vm);
          break;
        case 0x33:
          inst_rlc (vm);
          break;
        case 0x43:
          inst_orl_data_to_direct (vm, IR[1], IR[2]);
          break;
        case 0x53:
          inst_anl_data_to_direct (vm, IR[1], IR[2]);
          break;
        case 0x63:
          inst_xrl_data_to_direct (vm, IR[1], IR[2]);
          break;
        case 0x73:
          inst_jmp_DPTR (vm);
          break;
        case 0x83:
          inst_movc_PC (vm);
          break;
        case 0x93:
          inst_movc_DPTR (vm);
          break;
        case 0xA3:
          inst_inc_DPTR (vm);
          break;
        case 0xB3:
          inst_cpl_C (vm);
          break;
        case 0xC3:
          inst_clr_C (vm);
          break;
        case 0xD3:
          inst_setb_C (vm);
          break;
        case 0x04:
          inst_inc (vm);
          break;
        case 0x14:
          inst_dec (vm);
          break;
        case 0x24:
          inst_add_data (vm, IR[1]);
          break;
        case 0x34:
          inst_addc_data (vm, IR[1]);
          break;
        case 0x44:
          inst_orl_data (vm, IR[1]);
          break;
        case 0x54:
          inst_anl_data (vm, IR[1]);
          break;
        case 0x64:
          inst_xrl_data (vm, IR[1]);
          break;
        case 0x74:
          inst_mov_data (vm, IR[1]);
          break;
        case 0x84:
          inst_div (vm);
          break;
        case 0x94:
          inst_subb_data (vm, IR[1]);
          break;
        case 0xA4:
          inst_mul (vm);
          break;
        case 0xB4:
          inst_cjne_data (vm, IR[1], IR[2]);
          break;
        case 0xC4:
          inst_swap (vm);
          break;
        case 0xD4:
          inst_da (vm);
          break;
        case 0xE4:
          inst_clr (vm);
          break;
        case 0xF4:
          inst_cpl (vm);
          break;

        case 0x05:
          inst_inc_direct (vm, IR[1]);
          break;
        case 0x15:
          inst_dec_direct (vm, IR[1]);
          break;
        case 0x25:
          inst_add_direct (vm, IR[1]);
          break;
        case 0x35:
          inst_addc_direct (vm, IR[1]);
          break;
        case 0x45:
          inst_orl_direct (vm, IR[1]);
          break;
        case 0x55:
          inst_anl_direct (vm, IR[1]);
          break;
        case 0x65:
          inst_xrl_direct (vm, IR[1]);
          break;
        case 0x75:
          inst_mov_data_to_direct (vm, IR[1], IR[2]);
          break;
        case 0x85:
          inst_mov_direct_to_direct (vm, IR[1], IR[2]);
          break;
        case 0x95:
          inst_subb_direct (vm, IR[1]);
          break;
        case 0xB5:
          inst_cjne_direct (vm, IR[1], IR[2]);
          break;
        case 0xC5:
          inst_xch_direct (vm, IR[1]);
          break;
        case 0xD5:
          inst_djnz_direct (vm, IR[1], IR[2]);
          break;
        case 0xE5:
          inst_mov_direct (vm, IR[1]);
          break;
        case 0xF5:
          inst_mov_to_direct (vm, IR[1]);
          break;
        default:
          assert (IR[0] == 0xA5);
          break;
        }
    }

  /* ask the coprocessors to do their thing */
  operate_coprocessors (vm);

  /* timer(s) increment */
  if (TR0)
    {
      timer = TL0;
      if ((TMOD & 0x04))
        /* external event */;
      else
        timer += cycles - cycles_prev;
      switch (TMOD & 0x03)
        {
        case 0:
          timer += TH0 << 5;
          TL0 = timer & 0x1F;
          TH0 = (timer & 0x1FE0) >> 5;
          if (timer & 0x2000)
            TCON |= TF0_MASK;
          break;
        case 1:
          timer += TH0 << 8;
          TL0 = timer & 0xFF;
          TH0 = (timer & 0xFF00) >> 8;
          if (timer & 0x10000)
            TCON |= TF0_MASK;
          break;
        case 2:
          TL0 = timer & 0xFF;
          if (timer & 0x100)
            {
              TL0 += TH0;
              TCON |= TF0_MASK;
            }
          break;
        case 3:
          TL0 = timer & 0xFF;
          if (timer & 0x100)
            TCON |= TF0_MASK;
          break;
        }
    }
  if (((TMOD & 0x03) == 0x03) && TR1)
    {
      timer = TH0;
      if ((TMOD & 0x40))
        /* external event */;
      else
        timer += cycles - cycles_prev;
      TH0 = timer & 0xFF;
      if (timer & 0x100)
        TCON |= TF1_MASK;
    }
  if (((TMOD & 0x03) == 0x03) || TR1)
    {
      timer = TL1;
      if ((TMOD & 0x03) != 0x03 && (TMOD & 0x40))
        ;
      else
        timer += cycles - cycles_prev;
      switch ((TMOD & 0x30) >> 4)
        {
        case 0:
          timer += TH1 << 5;
          TL1 = timer & 0x1F;
          TH1 = (timer & 0x1FE0) >> 5;
          if (timer & 0x2000)
            TCON |= TF1_MASK;
          break;
        case 1:
          timer += TH1 << 8;
          TL1 = timer & 0xFF;
          TH1 = (timer & 0xFF00) >> 8;
          if (timer & 0x10000)
            TCON |= TF1_MASK;
          break;
        case 2:
          TL1 = timer & 0xFF;
          if (timer & 0x100)
            {
              TL1 += TH1;
              TCON |= TF1_MASK;
            }
          break;
        case 3:
          break;
        }
    }
  /* interrupts handling */
  if (EA && !(interrupted & HIGH) && !(interrupts_blocked))
    {
      uint8_t prioritary = IE & IP &
        (((RI|TI) << 4) | (TF1 << 3) | (IE1 << 2) | (TF0 << 1) | (IE0 << 0));
        
      if (GO_ISR (IE0, EX0, PX0) && !(prioritary & 0x1E))
        {
          if (IT0)
            TCON &= ~IE0_MASK;
          interrupted |= PX0 ? HIGH : LOW;
          inst_lcall (vm, 0x00, 0x03);
        }
      else if (GO_ISR (TF0, ET0, PT0) && !(prioritary & 0x1D))
        {
          TCON &= ~TF0_MASK;
          interrupted |= PT0 ? HIGH : LOW;
          inst_lcall (vm, 0x00, 0x0B);
        }
      else if (GO_ISR (IE1, EX1, PX1) && !(prioritary & 0x1B))
        {
          if (IT1)
            TCON &= ~IE1_MASK;
          interrupted |= PX1 ? HIGH : LOW;
          inst_lcall (vm, 0x00, 0x13);
        }
      else if (GO_ISR (TF1, ET1, PT1) && !(prioritary & 0x17))
        {
          TCON &= ~TF1_MASK;
          interrupted |= PT1 ? HIGH : LOW;
          inst_lcall (vm, 0x00, 0x1B);
        }
      else if (GO_ISR (RI|TI, ES, PS) && !(prioritary & 0x0F))
        {
          interrupted |= PS ? HIGH : LOW;
          inst_lcall (vm, 0x00, 0x23);
        }
    }
  /* unblock interrupts for next cycle */
  interrupts_blocked = 0;
}

void sim8051 (struct vm8051 *vm, uint16_t address, unsigned int ncy)
{
  do
    {
      fetch8051 (vm);
      operate8051 (vm);
    }
  while (PC != address && cycles < ncy);
}
