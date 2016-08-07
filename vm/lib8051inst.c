/* Copyright (C) 2014, 2016 Luk Bettale

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

#include <stdio.h>
#include <assert.h>

/* simulate global variables for a struct vm8051 *vm */
#include "lib8051globals.h"

static void parity_check (struct vm8051 *vm)
{
  uint8_t n;
  n = A;
  n ^= n >> 4;
  n ^= n >> 2;
  n ^= n >> 1;
  PSW &= ~P_MASK;
  PSW |= (n & 1) << P_POS;
}

static void adder (struct vm8051 *vm, uint8_t data, uint8_t use_CY)
{
  uint16_t res;

  res = (A & 0x0F) + (data & 0x0F) + (CY & use_CY);
  PSW &= ~AC_MASK;
  PSW |= (((res >> 4) & 1) << AC_POS);

  res += (A & 0xF0) + (data & 0xF0);
  PSW &= ~(CY_MASK|OV_MASK);
  PSW |= ((res >> 8) & 1) << CY_POS;
  PSW |= (((A ^ data ^ res ^ (CY << 7)) >> 7) & 1) << OV_POS;

  A = res & 0xFF;
}

static void SFR_check (struct vm8051 *vm, uint8_t direct)
{
  if (direct == 0xE0)           /* ACC */
    parity_check (vm);
  if (direct == 0xA8)           /* IE */
    {
      IE &= 0x9F;
      interrupts_blocked = 1;
    }
  if (direct == 0xB8)           /* IP */
    {
      IP &= 0x1F;
      interrupts_blocked = 1;
    }
  if (direct == 0x87)           /* PCON */
    PCON &= 0x8F;
  if (direct == 0x99)           /* SBUF */
    SCON |= TI_MASK;
}

static void assign_direct (struct vm8051 *vm, uint8_t direct, uint8_t val)
{
  if (direct & 0x80)
    {
      _sfr[direct ^ 0x80] = val;
      SFR_check (vm, direct);
    }
  else
    _data[direct] = val;
}

static uint8_t get_direct (struct vm8051 *vm, uint8_t direct)
{
  return ((direct & 0x80) ? _sfr[direct ^ 0x80] : _data[direct]);
}

#ifndef PURE_8051
#define is_valid_direct(direct) 1
#else
static int is_valid_direct (uint8_t direct)
{
  if (direct & 0x80)            /* SFR memory area */
    {
      switch (direct)
        {
        case 0x81:              /* SP */
        case 0x82:              /* DPL */
        case 0x83:              /* DPH */
        case 0x87:              /* PCON */

        case 0x88:              /* TCON */
        case 0x89:              /* TMOD */
        case 0x8A:              /* TL0 */
        case 0x8B:              /* TL1 */
        case 0x8C:              /* TH0 */
        case 0x8D:              /* TH1 */

        case 0x98:              /* SCON */
        case 0x99:              /* SBUF */

        case 0xA8:              /* IE */
        case 0xB8:              /* IP */

        case 0x80:              /* P0 */
        case 0x90:              /* P1 */
        case 0xA0:              /* P2 */
        case 0xB0:              /* P3 */

        case 0xD0:              /* PSW */
        case 0xE0:              /* ACC */
        case 0xF0:              /* B */
          break;

        default:
          return 0;
        }
    }
  return 1;
}
#endif

#ifndef PURE_8051
#define is_valid_bit(bit) 1
#else
static int is_valid_bit (uint8_t bit)
{
  if (bit & 0x80)               /* SFR memory area */
    {
      switch (bit & 0xF8)
        {
        case 0x88:              /* TCON */
        case 0x98:              /* SCON */

        case 0x80:              /* P0 */
        case 0x90:              /* P1 */
        case 0xA0:              /* P2 */
        case 0xB0:              /* P3 */

        case 0xD0:              /* PSW */
        case 0xE0:              /* ACC */
        case 0xF0:              /* B */
          break;

        case 0xA8:              /* IE */
          if ((bit & 0x07) == 6 || (bit & 0x07) == 5)
            return 0;
          break;
        case 0xB8:              /* IP */
          if ((bit & 0x07) > 4)
            return 0;
          break;

        default:
          return 0;
        }
    }
  return 1;
}
#endif

/* add A, Rn              1       1 */
void inst_add_Rn (struct vm8051 *vm, unsigned char n)
{
  assert (!(n & 0xF8));
  adder (vm, regs[n], 0);
  parity_check (vm);
  cycles += 1;
}

/* add A, direct          2       1 */
void inst_add_direct (struct vm8051 *vm, uint8_t direct)
{
  assert (is_valid_direct (direct));
  adder (vm, get_direct (vm, direct), 0);
  parity_check (vm);
  cycles += 1;
}

/* add A, @Ri             1       1 */
void inst_add_atRi (struct vm8051 *vm, unsigned char i)
{
  assert (!(i & 0xFE));
#ifdef STRICT_8051
  assert (!(regs[i] & 0x80));
#endif
  adder (vm, _data[regs[i]], 0);
  parity_check (vm);
  cycles += 1;
}

/* add A, #data           2       1 */
void inst_add_data (struct vm8051 *vm, uint8_t data)
{
  adder (vm, data, 0);
  parity_check (vm);
  cycles += 1;
}


/* addc A, Rn             1       1 */
void inst_addc_Rn (struct vm8051 *vm, unsigned char n)
{
  assert (!(n & 0xF8));
  adder (vm, regs[n], 1);
  parity_check (vm);
  cycles += 1;
}

/* addc A, direct         2       1 */
void inst_addc_direct (struct vm8051 *vm, uint8_t direct)
{
  assert (is_valid_direct (direct));
  adder (vm, get_direct (vm, direct), 1);
  parity_check (vm);
  cycles += 1;
}

/* addc A, @Ri            1       1 */
void inst_addc_atRi (struct vm8051 *vm, unsigned char i)
{
  assert (!(i & 0xFE));
#ifdef STRICT_8051
  assert (!(regs[i] & 0x80));
#endif
  adder (vm, _data[regs[i]], 1);
  parity_check (vm);
  cycles += 1;
}

/* addc A, #data          2       1 */
void inst_addc_data (struct vm8051 *vm, uint8_t data)
{
  adder (vm, data, 1);
  parity_check (vm);
  cycles += 1;
}


/* subb A, Rn             1       1 */
void inst_subb_Rn (struct vm8051 *vm, unsigned char n)
{
  assert (!(n & 0xF8));
  PSW ^= CY_MASK;
  adder (vm, ~regs[n], 1);
  PSW ^= CY_MASK;
  parity_check (vm);
  cycles += 1;
}

/* subb A, direct         2       1 */
void inst_subb_direct (struct vm8051 *vm, uint8_t direct)
{
  assert (is_valid_direct (direct));
  PSW ^= CY_MASK;
  adder (vm, ~get_direct (vm, direct), 1);
  PSW ^= CY_MASK;
  parity_check (vm);
  cycles += 1;
}

/* subb A, @Ri            1       1 */
void inst_subb_atRi (struct vm8051 *vm, unsigned char i)
{
  assert (!(i & 0xFE));
#ifdef STRICT_8051
  assert (!(regs[i] & 0x80));
#endif
  PSW ^= CY_MASK;
  adder (vm, ~_data[regs[i]], 1);
  PSW ^= CY_MASK;
  parity_check (vm);
  cycles += 1;
}

/* subb A, #data          2       1 */
void inst_subb_data (struct vm8051 *vm, uint8_t data)
{
  PSW ^= CY_MASK;
  adder (vm, ~data, 1);
  PSW ^= CY_MASK;
  parity_check (vm);
  cycles += 1;
}


/* inc A                  1       1 */
void inst_inc (struct vm8051 *vm)
{
  A++;
  parity_check (vm);
  cycles += 1;
}

/* inc Rn                 1       1 */
void inst_inc_Rn (struct vm8051 *vm, unsigned char n)
{
  assert (!(n & 0xF8));
  regs[n]++;
  cycles += 1;
}

/* inc direct             2       1 */
void inst_inc_direct (struct vm8051 *vm, uint8_t direct)
{
  assert (is_valid_direct (direct));
  assign_direct (vm, direct, get_direct (vm, direct) + 1);
  cycles += 1;
}

/* inc @Ri                1       1 */
void inst_inc_atRi (struct vm8051 *vm, unsigned char i)
{
  assert (!(i & 0xFE));
#ifdef STRICT_8051
  assert (!(regs[i] & 0x80));
#endif
  _data[regs[i]]++;
  cycles += 1;
}


/* dec A                  1       1 */
void inst_dec (struct vm8051 *vm)
{
  A--;
  parity_check (vm);
  cycles += 1;
}

/* dec Rn                 1       1 */
void inst_dec_Rn (struct vm8051 *vm, unsigned char n)
{
  assert (!(n & 0xF8));
  regs[n]--;
  cycles += 1;
}

/* dec direct             2       1 */
void inst_dec_direct (struct vm8051 *vm, uint8_t direct)
{
  assert (is_valid_direct (direct));
  assign_direct (vm, direct, get_direct (vm, direct) - 1);
  cycles += 1;
}

/* dec @Ri                1       1 */
void inst_dec_atRi (struct vm8051 *vm, unsigned char i)
{
  assert (!(i & 0xFE));
#ifdef STRICT_8051
  assert (!(regs[i] & 0x80));
#endif
  _data[regs[i]]--;
  cycles += 1;
}


/* inc DPTR               1       2 */
void inst_inc_DPTR (struct vm8051 *vm)
{
  DPL++;
  if (!DPL)
    DPH++;
  cycles += 2;
}


/* anl A, Rn              1       1 */
void inst_anl_Rn (struct vm8051 *vm, unsigned char n)
{
  assert (!(n & 0xF8));
  A &= regs[n];
  parity_check (vm);
  cycles += 1;
}

/* anl A, direct          2       1 */
void inst_anl_direct (struct vm8051 *vm, uint8_t direct)
{
  assert (is_valid_direct (direct));
  A &= get_direct (vm, direct);
  parity_check (vm);
  cycles += 1;
}

/* anl A, @Ri             1       1 */
void inst_anl_atRi (struct vm8051 *vm, unsigned char i)
{
  assert (!(i & 0xFE));
#ifdef STRICT_8051
  assert (!(regs[i] & 0x80));
#endif
  A &= _data[regs[i]];
  parity_check (vm);
  cycles += 1;
}

/* anl A, #data           2       1 */
void inst_anl_data (struct vm8051 *vm, uint8_t data)
{
  A &= data;
  parity_check (vm);
  cycles += 1;
}

/* anl direct, A          2       1 */
void inst_anl_to_direct (struct vm8051 *vm, uint8_t direct)
{
  assert (is_valid_direct (direct));
  assign_direct (vm, direct, get_direct (vm, direct) & A);
  cycles += 1;
}

/* anl direct, #data      3       2 */
void inst_anl_data_to_direct (struct vm8051 *vm, uint8_t direct, uint8_t data)
{
  assert (is_valid_direct (direct));
  assign_direct (vm, direct, get_direct (vm, direct) & data);
  cycles += 2;
}


/* orl A, Rn              1       1 */
void inst_orl_Rn (struct vm8051 *vm, unsigned char n)
{
  assert (!(n & 0xF8));
  A |= regs[n];
  parity_check (vm);
  cycles += 1;
}

/* orl A, direct          2       1 */
void inst_orl_direct (struct vm8051 *vm, uint8_t direct)
{
  assert (is_valid_direct (direct));
  A |= get_direct (vm, direct);
  parity_check (vm);
  cycles += 1;
}

/* orl A, @Ri             1       1 */
void inst_orl_atRi (struct vm8051 *vm, unsigned char i)
{
  assert (!(i & 0xFE));
#ifdef STRICT_8051
  assert (!(regs[i] & 0x80));
#endif
  A |= _data[regs[i]];
  parity_check (vm);
  cycles += 1;
}

/* orl A, #data           2       1 */
void inst_orl_data (struct vm8051 *vm, uint8_t data)
{
  A |= data;
  parity_check (vm);
  cycles += 1;
}

/* orl direct, A          2       1 */
void inst_orl_to_direct (struct vm8051 *vm, uint8_t direct)
{
  assert (is_valid_direct (direct));
  assign_direct (vm, direct, get_direct (vm, direct) | A);
  cycles += 1;
}

/* orl direct, #data      3       2 */
void inst_orl_data_to_direct (struct vm8051 *vm, uint8_t direct, uint8_t data)
{
  assert (is_valid_direct (direct));
  assign_direct (vm, direct, get_direct (vm, direct) | data);
  cycles += 2;
}


/* xrl A, Rn              1       1 */
void inst_xrl_Rn (struct vm8051 *vm, unsigned char n)
{
  assert (!(n & 0xF8));
  A ^= regs[n];
  parity_check (vm);
  cycles += 1;
}

/* xrl A, direct          2       1 */
void inst_xrl_direct (struct vm8051 *vm, uint8_t direct)
{
  assert (is_valid_direct (direct));
  A ^= get_direct (vm, direct);
  parity_check (vm);
  cycles += 1;
}

/* xrl A, @Ri             1       1 */
void inst_xrl_atRi (struct vm8051 *vm, unsigned char i)
{
  assert (!(i & 0xFE));
#ifdef STRICT_8051
  assert (!(regs[i] & 0x80));
#endif
  A ^= _data[regs[i]];
  parity_check (vm);
  cycles += 1;
}

/* xrl A, #data           2       1 */
void inst_xrl_data (struct vm8051 *vm, uint8_t data)
{
  A ^= data;
  parity_check (vm);
  cycles += 1;
}

/* xrl direct, A          2       1 */
void inst_xrl_to_direct (struct vm8051 *vm, uint8_t direct)
{
  assert (is_valid_direct (direct));
  assign_direct (vm, direct, get_direct (vm, direct) ^ A);
  cycles += 1;
}

/* xrl direct, #data      3       2 */
void inst_xrl_data_to_direct (struct vm8051 *vm, uint8_t direct, uint8_t data)
{
  assert (is_valid_direct (direct));
  assign_direct (vm, direct, get_direct (vm, direct) ^ data);
  cycles += 2;
}


/* clr A                  1       1 */
void inst_clr (struct vm8051 *vm)
{
  A = 0;
  PSW &= ~P_MASK;
  parity_check (vm);
  cycles += 1;
}

/* cpl A                  1       1 */
void inst_cpl (struct vm8051 *vm)
{
  A = ~A;
  parity_check (vm);
  cycles += 1;
}


/* rl A                   1       1 */
void inst_rl (struct vm8051 *vm)
{
  A = (A << 1) | (A >> 7);
  parity_check (vm);
  cycles += 1;
}

/* rlc A                  1       1 */
void inst_rlc (struct vm8051 *vm)
{
  A = (A << 1) | (A >> 7);
  PSW ^= ((A << CY_POS) & CY_MASK);
  A ^= ((PSW & CY_MASK) >> CY_POS);
  PSW ^= ((A << CY_POS) & CY_MASK);
  parity_check (vm);
  cycles += 1;
}

/* rr A                   1       1 */
void inst_rr (struct vm8051 *vm)
{
  A = (A >> 1) | (A << 7);
  parity_check (vm);
  cycles += 1;
}

/* rrc A                  1       1 */
void inst_rrc (struct vm8051 *vm)
{
  PSW ^= ((A << CY_POS) & CY_MASK);
  A ^= ((PSW & CY_MASK) >> CY_POS);
  PSW ^= ((A << CY_POS) & CY_MASK);
  A = (A >> 1) | (A << 7);
  parity_check (vm);
  cycles += 1;
}


/* swap A                 1       1 */
void inst_swap (struct vm8051 *vm)
{
  A = (A << 4) | (A >> 4);
  parity_check (vm);
  cycles += 1;
}


/* mul AB                 1       4 */
void inst_mul (struct vm8051 *vm)
{
  uint16_t res;
  PSW &= ~(CY_MASK|OV_MASK);
  res = A * B;
  A = res & 0xFF;
  B = res >> 8;
  if (B)
    PSW |= OV_MASK;
  parity_check (vm);
  cycles += 4;
}

/* div AB                 1       4 */
void inst_div (struct vm8051 *vm)
{
  uint8_t rem;
  PSW &= ~(CY_MASK|OV_MASK);
  if (!B)
    PSW |= OV_MASK;
  else
    {
      rem = A % B;
      A /= B;
      B = rem;
    }
  parity_check (vm);
  cycles += 4;
}

/* da A                   1       1 */
void inst_da (struct vm8051 *vm)
{
  uint16_t res = A;
  if ((res & 0x0F) > 0x09 || AC)
    res += 0x06;
  if ((res & 0xF0) > 0x90 || CY)
    res += 0x60;
  A = res & 0xFF;
  PSW |= (res >> 8) << CY_POS;
  parity_check (vm);
  cycles += 1;
}


/* mov A, Rn              1       1 */
void inst_mov_Rn (struct vm8051 *vm, unsigned char n)
{
  assert (!(n & 0xF8));
  A = regs[n];
  parity_check (vm);
  cycles += 1;
}

/* mov A, direct          2       1 */
void inst_mov_direct (struct vm8051 *vm, uint8_t direct)
{
  assert (is_valid_direct (direct));
  A = get_direct (vm, direct);
  parity_check (vm);
  cycles += 1;
}

/* mov A, @Ri             1       1 */
void inst_mov_atRi (struct vm8051 *vm, unsigned char i)
{
  assert (!(i & 0xFE));
#ifdef STRICT_8051
  assert (!(regs[i] & 0x80));
#endif
  A = _data[regs[i]];
  parity_check (vm);
  cycles += 1;
}

/* mov A, #data           2       1 */
void inst_mov_data (struct vm8051 *vm, uint8_t data)
{
  A = data;
  parity_check (vm);
  cycles += 1;
}


/* mov Rn, A              1       1 */
void inst_mov_to_Rn (struct vm8051 *vm, unsigned char n)
{
  assert (!(n & 0xF8));
  regs[n] = A;
  cycles += 1;
}

/* mov Rn, direct         2       2 */
void inst_mov_direct_to_Rn (struct vm8051 *vm, unsigned char n, uint8_t direct)
{
  assert (is_valid_direct (direct));
  assert (!(n & 0xF8));
  regs[n] = get_direct (vm, direct);
  cycles += 2;
}

/* mov Rn, #data          2       1 */
void inst_mov_data_to_Rn (struct vm8051 *vm, unsigned char n, uint8_t data)
{
  assert (!(n & 0xF8));
  regs[n] = data;
  cycles += 1;
}


/* mov direct, A          2       1 */
void inst_mov_to_direct (struct vm8051 *vm, uint8_t direct)
{
  assert (is_valid_direct (direct));
  assign_direct (vm, direct, A);
  cycles += 1;
}

/* mov direct, Rn         2       2 */
void inst_mov_Rn_to_direct (struct vm8051 *vm, unsigned char n, uint8_t direct)
{
  assert (is_valid_direct (direct));
  assert (!(n & 0xF8));
  assign_direct (vm, direct, regs[n]);
  cycles += 2;
}

/* mov direct, direct     3       2 */
void inst_mov_direct_to_direct (struct vm8051 *vm, uint8_t direct_src, uint8_t direct)
{
  assert (is_valid_direct (direct_src));
  assert (is_valid_direct (direct));
  assign_direct (vm, direct, get_direct (vm, direct_src));
  cycles += 2;
}

/* mov direct, @Ri        2       2 */
void inst_mov_atRi_to_direct (struct vm8051 *vm, unsigned char i, uint8_t direct)
{
  assert (is_valid_direct (direct));
  assert (!(i & 0xFE));
#ifdef STRICT_8051
  assert (!(regs[i] & 0x80));
#endif
  assign_direct (vm, direct, _data[regs[i]]);
  cycles += 2;
}

/* mov direct, #data      3       2 */
void inst_mov_data_to_direct (struct vm8051 *vm, uint8_t direct, uint8_t data)
{
  assert (is_valid_direct (direct));
  assign_direct (vm, direct, data);
  cycles += 2;
}


/* mov @Ri, A             1       1 */
void inst_mov_to_atRi (struct vm8051 *vm, unsigned char i)
{
  assert (!(i & 0xFE));
#ifdef STRICT_8051
  assert (!(regs[i] & 0x80));
#endif
  _data[regs[i]] = A;
  cycles += 1;
}

/* mov @Ri, direct        2       2 */
void inst_mov_direct_to_atRi (struct vm8051 *vm, unsigned char i, uint8_t direct)
{
  assert (is_valid_direct (direct));
  assert (!(i & 0xFE));
#ifdef STRICT_8051
  assert (!(regs[i] & 0x80));
#endif
  _data[regs[i]] = get_direct (vm, direct);
  cycles += 2;
}

/* mov @Ri, #data         2       1 */
void inst_mov_data_to_atRi (struct vm8051 *vm, unsigned char i, uint8_t data)
{
  assert (!(i & 0xFE));
#ifdef STRICT_8051
  assert (!(regs[i] & 0x80));
#endif
  _data[regs[i]] = data;
  cycles += 1;
}


/* mov DPTR, #data16      3       2 */
void inst_mov_to_DPTR (struct vm8051 *vm, uint8_t data16_high, uint8_t data16_low)
{
  DPL = data16_low;
  DPH = data16_high;
  cycles += 2;
}


/* movc A, @A+DPTR        1       2 */
void inst_movc_DPTR (struct vm8051 *vm)
{
  A = _code[(uint16_t)(A+DPTR)];
  parity_check (vm);
  cycles += 2;
}

/* movc A, @A+PC          1       2 */
void inst_movc_PC (struct vm8051 *vm)
{
  A = _code[(uint16_t)(A+PC)];
  parity_check (vm);
  cycles += 2;
}


/* movx A, @Ri            1       2 */
void inst_movx_atRi (struct vm8051 *vm, unsigned char i)
{
  assert (!(i & 0xFE));
  A = _xdata[(P2 << 8) + regs[i]];
  P0 = 0xFF;
  parity_check (vm);
  cycles += 2;
}

/* movx A, @DPTR          1       2 */
void inst_movx_atDPTR (struct vm8051 *vm)
{
  A = _xdata[DPTR];
  P0 = 0xFF;
  parity_check (vm);
  cycles += 2;
}

/* movx @Ri, A            1       2 */
void inst_movx_to_atRi (struct vm8051 *vm, unsigned char i)
{
  assert (!(i & 0xFE));
  P0 = 0xFF;
  _xdata[(P2 << 8) + regs[i]] = A;
  cycles += 2;
}

/* movx @DPTR, A          1       2 */
void inst_movx_to_atDPTR (struct vm8051 *vm)
{
  _xdata[DPTR] = A;
  P0 = 0xFF;
  cycles += 2;
}


/* clr C                  1       1 */
void inst_clr_C (struct vm8051 *vm)
{
  PSW &= ~CY_MASK;
  cycles += 1;
}

/* clr bit                2       1 */
void inst_clr_bit (struct vm8051 *vm, uint8_t bit)
{
  assert (is_valid_bit (bit));
  if (bit & 0x80)
    {
      _sfr[bit & 0x78] &= ~(1 << (bit & 0x07));
      SFR_check (vm, bit & 0xF8);
    }
  else
    bit_addressable[bit >> 3] &= ~(1 << (bit & 0x07));
  cycles += 1;
}


/* setb C                 1       1 */
void inst_setb_C (struct vm8051 *vm)
{
  PSW |= CY_MASK;
  cycles += 1;
}

/* setb bit               2       1 */
void inst_setb_bit (struct vm8051 *vm, uint8_t bit)
{
  assert (is_valid_bit (bit));
  if (bit & 0x80)
    {
      _sfr[bit & 0x78] |= 1 << (bit & 0x07);
      SFR_check (vm, bit & 0xF8);
    }
  else
    bit_addressable[bit >> 3] |= 1 << (bit & 0x07);
  cycles += 1;
}


/* cpl C                  1       1 */
void inst_cpl_C (struct vm8051 *vm)
{
  PSW ^= CY_MASK;
  cycles += 1;
}

/* cpl bit                2       1 */
void inst_cpl_bit (struct vm8051 *vm, uint8_t bit)
{
  assert (is_valid_bit (bit));
  if (bit & 0x80)
    {
      _sfr[bit & 0x78] ^= 1 << (bit & 0x07);
      SFR_check (vm, bit & 0xF8);
    }
  else
    bit_addressable[bit >> 3] ^= 1 << (bit & 0x07);
  cycles += 1;
}


/* anl C, bit             2       2 */
void inst_anl_bit (struct vm8051 *vm, uint8_t bit)
{
  assert (is_valid_bit (bit));
  if (bit & 0x80)
    PSW &= ((_sfr[bit & 0x78] >> (bit & 0x07)) << CY_POS) | ~CY_MASK;
  else
    PSW &= ((bit_addressable[bit >> 3] >> (bit & 0x07)) << CY_POS) | ~CY_MASK;
  cycles += 2;
}

/* anl C, /bit            2       2 */
void inst_anl_not_bit (struct vm8051 *vm, uint8_t bit)
{
  assert (is_valid_bit (bit));
  if (bit & 0x80)
    PSW &= ~((_sfr[bit & 0x78] >> (bit & 0x07)) << CY_POS) | ~CY_MASK;
  else
    PSW &= ~((bit_addressable[bit >> 3] >> (bit & 0x07)) << CY_POS) | ~CY_MASK;
  cycles += 2;
}


/* orl C, bit             2       2 */
void inst_orl_bit (struct vm8051 *vm, uint8_t bit)
{
  assert (is_valid_bit (bit));
  if (bit & 0x80)
    PSW |= ((_sfr[bit & 0x78] >> (bit & 0x07)) << CY_POS) & CY_MASK;
  else
    PSW |= ((bit_addressable[bit >> 3] >> (bit & 0x07)) << CY_POS) & CY_MASK;
  cycles += 2;
}

/* orl C, /bit            2       2 */
void inst_orl_not_bit (struct vm8051 *vm, uint8_t bit)
{
  assert (is_valid_bit (bit));
  if (bit & 0x80)
    PSW |= ~((_sfr[bit & 0x78] >> (bit & 0x07)) << CY_POS) & CY_MASK;
  else
    PSW |= ~((bit_addressable[bit >> 3] >> (bit & 0x07)) << CY_POS) & CY_MASK;
  cycles += 2;
}


/* mov C, bit             2       1 */
void inst_mov_bit (struct vm8051 *vm, uint8_t bit)
{
  assert (is_valid_bit (bit));
  PSW &= ~CY_MASK;
  if (bit & 0x80)
    PSW |= ((_sfr[bit & 0x78] >> (bit & 0x07)) << CY_POS) & CY_MASK;
  else
    PSW |= ((bit_addressable[bit >> 3] >> (bit & 0x07)) << CY_POS) & CY_MASK;
  cycles += 1;
}

/* mov bit, C             2       2 */
void inst_mov_to_bit (struct vm8051 *vm, uint8_t bit)
{
  assert (is_valid_bit (bit));
  if (bit & 0x80)
    {
      _sfr[bit & 0x78] &= ~(1 << (bit & 0x07));
      _sfr[bit & 0x78] |= CY << (bit & 0x07);
      SFR_check (vm, bit & 0xF8);
    }
  else
    {
      bit_addressable[bit >> 3] &= ~(1 << (bit & 0x07));
      bit_addressable[bit >> 3] |= CY << (bit & 0x07);
    }
  cycles += 2;
}


/* xch A, Rr              1       1 */
void inst_xch_Rn (struct vm8051 *vm, unsigned char n)
{
  assert (!(n & 0xF8));
  A ^= regs[n];
  regs[n] ^= A;
  A ^= regs[n];
  parity_check (vm);
  cycles += 1;
}

/* xch A, direct          2       1 */
void inst_xch_direct (struct vm8051 *vm, uint8_t direct)
{
  uint8_t tmp;
  assert (is_valid_direct (direct));
  tmp = A;
  A = get_direct (vm, direct);
  assign_direct (vm, direct, tmp);
  parity_check (vm);
  cycles += 1;
}

/* xch A, @Ri             1       1 */
void inst_xch_atRi (struct vm8051 *vm, unsigned char i)
{
  assert (!(i & 0xFE));
#ifdef STRICT_8051
  assert (!(regs[i] & 0x80));
#endif
  A ^= _data[regs[i]];
  _data[regs[i]] ^= A;
  A ^= _data[regs[i]];
  cycles += 1;
}

/* xchd A, @Ri            1       1 */
void inst_xchd_atRi (struct vm8051 *vm, unsigned char i)
{
  assert (!(i & 0xFE));
#ifdef STRICT_8051
  assert (!(regs[i] & 0x80));
#endif
  A ^= _data[regs[i]] & 0x0F;
  _data[regs[i]] ^= A & 0x0F;
  A ^= _data[regs[i]] & 0x0F;
  parity_check (vm);
  cycles += 1;
}


/* push direct            2       2 */
void inst_push (struct vm8051 *vm, uint8_t direct)
{
  assert (is_valid_direct (direct));
  _data[++SP] = get_direct (vm, direct);
  cycles += 2;
}

/* pop direct             2       2 */
void inst_pop (struct vm8051 *vm, uint8_t direct)
{
  assert (is_valid_direct (direct));
  assign_direct (vm, direct, _data[SP--]);
  cycles += 2;
}


/* jz rel                 2       2 */
void inst_jz (struct vm8051 *vm, uint8_t rel)
{
  if (!A)
    PC += (int8_t) rel;
  cycles += 2;
}

/* jnz rel                2       2 */
void inst_jnz (struct vm8051 *vm, uint8_t rel)
{
  if (A)
    PC += (int8_t) rel;
  cycles += 2;
}


/* jc rel                 2       2 */
void inst_jc (struct vm8051 *vm, uint8_t rel)
{
  if (CY)
    PC += (int8_t) rel;
  cycles += 2;
}

/* jnc rel                2       2 */
void inst_jnc (struct vm8051 *vm, uint8_t rel)
{
  if (!CY)
    PC += (int8_t) rel;
  cycles += 2;
}


/* jb bit, rel            3       2 */
void inst_jb (struct vm8051 *vm, uint8_t bit, uint8_t rel)
{
  assert (is_valid_bit (bit));
  if (bit & 0x80)
    {
      if (_sfr[bit & 0x78] & (1 << (bit & 0x07)))
        PC += (int8_t) rel;
    }
  else
    {
      if (bit_addressable[bit >> 3] & (1 << (bit & 0x07)))
        PC += (int8_t) rel;
    }
  cycles += 2;
}

/* jnb bit, rel           3       2 */
void inst_jnb (struct vm8051 *vm, uint8_t bit, uint8_t rel)
{
  assert (is_valid_bit (bit));
  if (bit & 0x80)
    {
      if (!(_sfr[bit & 0x78] & (1 << (bit & 0x07))))
        PC += (int8_t) rel;
    }
  else
    {
      if (!(bit_addressable[bit >> 3] & (1 << (bit & 0x07))))
        PC += (int8_t) rel;
    }
  cycles += 2;
}

/* jbc bit, rel           3       2 */
void inst_jbc (struct vm8051 *vm, uint8_t bit, uint8_t rel)
{
  assert (is_valid_bit (bit));
  if (bit & 0x80)
    {
      if (_sfr[bit & 0x78] & (1 << (bit & 0x07)))
        {
          _sfr[bit & 0x78] &= ~(1 << (bit & 0x07));
          SFR_check (vm, bit & 0xF8);
          PC += (int8_t) rel;
        }
    }
  else
    {
      if (bit_addressable[bit >> 3] & (1 << (bit & 0x07)))
        {
          bit_addressable[bit >> 3] &= ~(1 << (bit & 0x07));
          PC += (int8_t) rel;
        }
    }
}


/* cjne A, direct, rel    3       2 */
void inst_cjne_direct (struct vm8051 *vm, uint8_t direct, uint8_t rel)
{
  uint8_t tmp;
  assert (is_valid_direct (direct));
  tmp = get_direct (vm, direct);
  PSW &= ~CY_MASK;
  if (A < tmp)
    PSW |= CY_MASK;
  if (A != tmp)
    PC += (int8_t) rel;
  cycles += 2;
}

/* cjne A, #data, rel     3       2 */
void inst_cjne_data (struct vm8051 *vm, uint8_t data, uint8_t rel)
{
  PSW &= ~CY_MASK;
  if (A < data)
    PSW |= CY_MASK;
  if (A != data)
    PC += (int8_t) rel;
  cycles += 2;
}

/* cjne Rn, #data, rel    3       2 */
void inst_cjne_data_with_Rn (struct vm8051 *vm, unsigned char n, uint8_t data, uint8_t rel)
{
  assert (!(n & 0xF8));
  PSW &= ~CY_MASK;
  if (regs[n] < data)
    PSW |= CY_MASK;
  if (regs[n] != data)
    PC += (int8_t) rel;
  cycles += 2;
}

/* cjne @Ri, #data, rel   3       2 */
void inst_cjne_data_with_atRi (struct vm8051 *vm, unsigned char i, uint8_t data, uint8_t rel)
{
  assert (!(i & 0xFE));
#ifdef STRICT_8051
  assert (!(regs[i] & 0x80));
#endif
  PSW &= ~CY_MASK;
  if (_data[regs[i]] < data)
    PSW |= CY_MASK;
  if (_data[regs[i]] != data)
    PC += (int8_t) rel;
  cycles += 2;
}


/* djnz Rn, rel           2       2 */
void inst_djnz_Rn (struct vm8051 *vm, unsigned char n, uint8_t rel)
{
  assert (!(n & 0xF8));
  if (--regs[n])
    PC += (int8_t) rel;
  cycles += 2;
}

/* djnz direct, rel       3       2 */
void inst_djnz_direct (struct vm8051 *vm, uint8_t direct, uint8_t rel)
{
  assert (is_valid_direct (direct));
  assign_direct (vm, direct, get_direct (vm, direct) - 1);
  if (get_direct (vm, direct))
    PC += (int8_t) rel;
  cycles += 2;
}


/* ajmp addr11            2       2 */
void inst_ajmp (struct vm8051 *vm, unsigned char prefix, uint8_t addr11)
{
  assert (!(prefix & 0xF8));
  PC &= 0xF800;
  PC |= (prefix << 8) | addr11;
  cycles += 2;
}

/* ljmp addr16            3       2 */
void inst_ljmp (struct vm8051 *vm, uint8_t addr16_high, uint8_t addr16_low)
{
  PC = addr16_low | (addr16_high << 8);
  cycles += 2;
}

/* sjmp rel               2       2 */
void inst_sjmp (struct vm8051 *vm, uint8_t rel)
{
  PC += (int8_t) rel;
  cycles += 2;
}


/* jmp @A+DPTR            1       2 */
void inst_jmp_DPTR (struct vm8051 *vm)
{
  PC = A+DPTR;
  cycles += 2;
}


/* nop                    1       1 */
void inst_nop (struct vm8051 *vm)
{
  cycles += 1;
}

/* acall addr11           2       2 */
void inst_acall (struct vm8051 *vm, unsigned char prefix, uint8_t addr11)
{
  assert (!(prefix & 0xF8));
  _data[++SP] = PC & 0xFF;
  _data[++SP] = PC >> 8;
  PC &= 0xF800;
  PC |= (prefix << 8) | addr11;
  cycles += 2;
}

/* lcall addr16           3       2 */
void inst_lcall (struct vm8051 *vm, uint8_t addr16_high, uint8_t addr16_low)
{
  _data[++SP] = PC & 0xFF;
  _data[++SP] = PC >> 8;
  PC = addr16_low | (addr16_high << 8);
  cycles += 2;
}


/* ret                    1       2 */
void inst_ret (struct vm8051 *vm)
{
  PC = _data[SP--] << 8;
  PC |= _data[SP--];
  cycles += 2;
}

/* reti                   1       2 */
void inst_reti (struct vm8051 *vm)
{
  if (interrupted & HIGH)
    interrupted &= ~HIGH;
  else
    interrupted = 0;
  interrupts_blocked = 1;
  PC = _data[SP--] << 8;
  PC |= _data[SP--];
  cycles += 2;
}
