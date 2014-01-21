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

#include "lib8051print.h"

static int32_t PC;


/* fprint primitives */

/* add A, Rn              1       1 */
static void fprint_add_Rn (FILE *stream, unsigned char n)
{
  fprintf (stream, "ADD   A, R%X                     ", n);
}

/* add A, direct          2       1 */
static void fprint_add_direct (FILE *stream, uint8_t direct)
{
  fprintf (stream, "ADD   A, " FORMAT_BYTE "                   ", direct);
}

/* add A, @Ri             1       1 */
static void fprint_add_atRi (FILE *stream, unsigned char i)
{
  fprintf (stream, "ADD   A, @R%X                    ", i);
}

/* add A, #data           2       1 */
static void fprint_add_data (FILE *stream, uint8_t data)
{
  fprintf (stream, "ADD   A, #" FORMAT_BYTE "                  ", data);
}

/* addc A, Rn             1       1 */
static void fprint_addc_Rn (FILE *stream, unsigned char n)
{
  fprintf (stream, "ADDC  A, R%X                     ", n);
}

/* addc A, direct         2       1 */
static void fprint_addc_direct (FILE *stream, uint8_t direct)
{
  fprintf (stream, "ADDC  A, " FORMAT_BYTE "                   ", direct);
}

/* addc A, @Ri            1       1 */
static void fprint_addc_atRi (FILE *stream, unsigned char i)
{
  fprintf (stream, "ADDC  A, @R%X                    ", i);
}

/* addc A, #data          2       1 */
static void fprint_addc_data (FILE *stream, uint8_t data)
{
  fprintf (stream, "ADDC  A, #" FORMAT_BYTE "                  ", data);
}

/* subb A, Rn             1       1 */
static void fprint_subb_Rn (FILE *stream, unsigned char n)
{
  fprintf (stream, "SUBB  A, R%X                     ", n);
}

/* subb A, direct         2       1 */
static void fprint_subb_direct (FILE *stream, uint8_t direct)
{
  fprintf (stream, "SUBB  A, " FORMAT_BYTE "                   ", direct);
}

/* subb A, @Ri            1       1 */
static void fprint_subb_atRi (FILE *stream, unsigned char i)
{
  fprintf (stream, "SUBB  A, @R%X                    ", i);
}

/* subb A, #data          2       1 */
static void fprint_subb_data (FILE *stream, uint8_t data)
{
  fprintf (stream, "SUBB  A, #" FORMAT_BYTE "                  ", data);
}

/* inc A                  1       1 */
static void fprint_inc (FILE *stream)
{
  fprintf (stream, "INC   A                         ");
}

/* inc Rn                 1       1 */
static void fprint_inc_Rn (FILE *stream, unsigned char n)
{
  fprintf (stream, "INC   R%X                        ", n);
}

/* inc direct             2       1 */
static void fprint_inc_direct (FILE *stream, uint8_t direct)
{
  fprintf (stream, "INC   " FORMAT_BYTE "                      ", direct);
}

/* inc @Ri                1       1 */
static void fprint_inc_atRi (FILE *stream, unsigned char i)
{
  fprintf (stream, "INC   @R%X                       ", i);
}

/* dec A                  1       1 */
static void fprint_dec (FILE *stream)
{
  fprintf (stream, "DEC   A                         ");
}

/* dec Rn                 1       1 */
static void fprint_dec_Rn (FILE *stream, unsigned char n)
{
  fprintf (stream, "DEC   R%X                        ", n);
}

/* dec direct             2       1 */
static void fprint_dec_direct (FILE *stream, uint8_t direct)
{
  fprintf (stream, "DEC   " FORMAT_BYTE "                      ", direct);
}

/* dec @Ri                1       1 */
static void fprint_dec_atRi (FILE *stream, unsigned char i)
{
  fprintf (stream, "DEC   @R%X                       ", i);
}

/* inc DPTR               1       2 */
static void fprint_inc_DPTR (FILE *stream)
{
  fprintf (stream, "INC   DPTR                      ");
}

/* anl A, Rn              1       1 */
static void fprint_anl_Rn (FILE *stream, unsigned char n)
{
  fprintf (stream, "ANL   A, R%X                     ", n);
}

/* anl A, direct          2       1 */
static void fprint_anl_direct (FILE *stream, uint8_t direct)
{
  fprintf (stream, "ANL   A, " FORMAT_BYTE "                   ", direct);
}

/* anl A, @Ri             1       1 */
static void fprint_anl_atRi (FILE *stream, unsigned char i)
{
  fprintf (stream, "ANL   A, @R%X                    ", i);
}

/* anl A, #data           2       1 */
static void fprint_anl_data (FILE *stream, uint8_t data)
{
  fprintf (stream, "ANL   A, #" FORMAT_BYTE "                  ", data);
}

/* anl direct, A          2       1 */
static void fprint_anl_to_direct (FILE *stream, uint8_t direct)
{
  fprintf (stream, "ANL   " FORMAT_BYTE ", A                   ", direct);
}

/* anl direct, #data      3       2 */
static void fprint_anl_data_to_direct (FILE *stream, uint8_t direct, uint8_t data)
{
  fprintf (stream, "ANL   " FORMAT_BYTE ", #" FORMAT_BYTE "               ", direct, data);
}

/* orl A, Rn              1       1 */
static void fprint_orl_Rn (FILE *stream, unsigned char n)
{
  fprintf (stream, "ORL   A, R%X                     ", n);
}

/* orl A, direct          2       1 */
static void fprint_orl_direct (FILE *stream, uint8_t direct)
{
  fprintf (stream, "ORL   A, " FORMAT_BYTE "                   ", direct);
}

/* orl A, @Ri             1       1 */
static void fprint_orl_atRi (FILE *stream, unsigned char i)
{
  fprintf (stream, "ORL   A, @R%X                    ", i);
}

/* orl A, #data           2       1 */
static void fprint_orl_data (FILE *stream, uint8_t data)
{
  fprintf (stream, "ORL   A, #" FORMAT_BYTE "                  ", data);
}

/* orl direct, A          2       1 */
static void fprint_orl_to_direct (FILE *stream, uint8_t direct)
{
  fprintf (stream, "ORL   " FORMAT_BYTE ", A                   ", direct);
}

/* orl direct, #data      3       2 */
static void fprint_orl_data_to_direct (FILE *stream, uint8_t direct, uint8_t data)
{
  fprintf (stream, "ORL   " FORMAT_BYTE ", #" FORMAT_BYTE "               ", direct, data);
}

/* xrl A, Rn              1       1 */
static void fprint_xrl_Rn (FILE *stream, unsigned char n)
{
  fprintf (stream, "XRL   A, R%X                     ", n);
}

/* xrl A, direct          2       1 */
static void fprint_xrl_direct (FILE *stream, uint8_t direct)
{
  fprintf (stream, "XRL   A, " FORMAT_BYTE "                   ", direct);
}

/* xrl A, @Ri             1       1 */
static void fprint_xrl_atRi (FILE *stream, unsigned char i)
{
  fprintf (stream, "XRL   A, @R%X                    ", i);
}

/* xrl A, #data           2       1 */
static void fprint_xrl_data (FILE *stream, uint8_t data)
{
  fprintf (stream, "XRL   A, #" FORMAT_BYTE "                  ", data);
}

/* xrl direct, A          2       1 */
static void fprint_xrl_to_direct (FILE *stream, uint8_t direct)
{
  fprintf (stream, "XRL   " FORMAT_BYTE ", A                   ", direct);
}

/* xrl direct, #data      3       2 */
static void fprint_xrl_data_to_direct (FILE *stream, uint8_t direct, uint8_t data)
{
  fprintf (stream, "XRL   " FORMAT_BYTE ", #" FORMAT_BYTE "               ", direct, data);
}

/* clr A                  1       1 */
static void fprint_clr (FILE *stream)
{
  fprintf (stream, "CLR   A                         ");
}

/* cpl A                  1       1 */
static void fprint_cpl (FILE *stream)
{
  fprintf (stream, "CPL   A                         ");
}

/* rl A                   1       1 */
static void fprint_rl (FILE *stream)
{
  fprintf (stream, "RL    A                         ");
}

/* rlc A                  1       1 */
static void fprint_rlc (FILE *stream)
{
  fprintf (stream, "RLC   A                         ");
}

/* rr A                   1       1 */
static void fprint_rr (FILE *stream)
{
  fprintf (stream, "RR    A                         ");
}

/* rrc A                  1       1 */
static void fprint_rrc (FILE *stream)
{
  fprintf (stream, "RRC   A                         ");
}

/* swap A                 1       1 */
static void fprint_swap (FILE *stream)
{
  fprintf (stream, "SWAP  A                         ");
}

/* mul AB                 1       4 */
static void fprint_mul (FILE *stream)
{
  fprintf (stream, "MUL   AB                        ");
}

/* div AB                 1       4 */
static void fprint_div (FILE *stream)
{
  fprintf (stream, "DIV   AB                        ");
}

/* da A                   1       1 */
static void fprint_da (FILE *stream)
{
  fprintf (stream, "DA    A                         ");
}

/* mov A, Rn              1       1 */
static void fprint_mov_Rn (FILE *stream, unsigned char n)
{
  fprintf (stream, "MOV   A, R%X                     ", n);
}

/* mov A, direct          2       1 */
static void fprint_mov_direct (FILE *stream, uint8_t direct)
{
  fprintf (stream, "MOV   A, " FORMAT_BYTE "                   ", direct);
}

/* mov A, @Ri             1       1 */
static void fprint_mov_atRi (FILE *stream, unsigned char i)
{
  fprintf (stream, "MOV   A, @R%X                    ", i);
}

/* mov A, #data           2       1 */
static void fprint_mov_data (FILE *stream, uint8_t data)
{
  fprintf (stream, "MOV   A, #" FORMAT_BYTE "                  ", data);
}

/* mov Rn, A              1       1 */
static void fprint_mov_to_Rn (FILE *stream, unsigned char n)
{
  fprintf (stream, "MOV   R%X, A                     ", n);
}

/* mov Rn, direct         2       2 */
static void fprint_mov_direct_to_Rn (FILE *stream, unsigned char n, uint8_t direct)
{
  fprintf (stream, "MOV   R%X, " FORMAT_BYTE "                  ", n, direct);
}

/* mov Rn, #data          2       1 */
static void fprint_mov_data_to_Rn (FILE *stream, unsigned char n, uint8_t data)
{
  fprintf (stream, "MOV   R%X, #" FORMAT_BYTE "                 ", n, data);
}

/* mov direct, A          2       1 */
static void fprint_mov_to_direct (FILE *stream, uint8_t direct)
{
  fprintf (stream, "MOV   " FORMAT_BYTE ", A                   ", direct);
}

/* mov direct, Rn         2       2 */
static void fprint_mov_Rn_to_direct (FILE *stream, unsigned char n, uint8_t direct)
{
  fprintf (stream, "MOV   " FORMAT_BYTE ", R%X                  ", direct, n);
}

/* mov direct, direct     3       2 */
static void fprint_mov_direct_to_direct (FILE *stream, uint8_t direct_src, uint8_t direct_dst)
{
  fprintf (stream, "MOV   " FORMAT_BYTE ", " FORMAT_BYTE "                ",
           direct_dst, direct_src);
}

/* mov direct, @Ri        2       2 */
static void fprint_mov_atRi_to_direct (FILE *stream, unsigned char i, uint8_t direct)
{
  fprintf (stream, "MOV   " FORMAT_BYTE ", @R%X                 ", direct, i);
}

/* mov direct, #data      3       2 */
static void fprint_mov_data_to_direct (FILE *stream, uint8_t direct, uint8_t data)
{
  fprintf (stream, "MOV   " FORMAT_BYTE ", #" FORMAT_BYTE "               ", direct, data);
}

/* mov @Ri, A             1       1 */
static void fprint_mov_to_atRi (FILE *stream, unsigned char i)
{
  fprintf (stream, "MOV   @R%X, A                    ", i);
}

/* mov @Ri, direct        2       2 */
static void fprint_mov_direct_to_atRi (FILE *stream, unsigned char i, uint8_t direct)
{
  fprintf (stream, "MOV   @R%X, " FORMAT_BYTE "                 ", i, direct);
}

/* mov @Ri, #data         2       1 */
static void fprint_mov_data_to_atRi (FILE *stream, unsigned char i, uint8_t data)
{
  fprintf (stream, "MOV   @R%X, #" FORMAT_BYTE "                ", i, data);
}

/* mov DPTR, #data16      3       2 */
static void fprint_mov_to_DPTR (FILE *stream, uint8_t data16_high, uint8_t data16_low)
{
  fprintf (stream, "MOV   DPTR, #" FORMAT_WORD "             ",
           (data16_high << 8) + data16_low);
}

/* movc A, @A+DPTR        1       2 */
static void fprint_movc_DPTR (FILE *stream)
{
  fprintf (stream, "MOVC  A, @A+DPTR                ");
}

/* movc A, @A+PC          1       2 */
static void fprint_movc_PC (FILE *stream)
{
  fprintf (stream, "MOVC  A, @A+PC                  ");
}

/* movx A, @Ri            1       2 */
static void fprint_movx_atRi (FILE *stream, unsigned char i)
{
  fprintf (stream, "MOVX  A, @R%X                    ", i);
}

/* movx A, @DPTR          1       2 */
static void fprint_movx_atDPTR (FILE *stream)
{
  fprintf (stream, "MOVX  A, @DPTR                  ");
}

/* movx @Ri, A            1       2 */
static void fprint_movx_to_atRi (FILE *stream, unsigned char i)
{
  fprintf (stream, "MOVX  @R%X, A                    ", i);
}

/* movx @DPTR, A          1       2 */
static void fprint_movx_to_atDPTR (FILE *stream)
{
  fprintf (stream, "MOVX  @DPTR, A                  ");
}

/* clr C                  1       1 */
static void fprint_clr_C (FILE *stream)
{
  fprintf (stream, "CLR   C                         ");
}

/* clr bit                2       1 */
static void fprint_clr_bit (FILE *stream, uint8_t bit)
{
  if (bit & 0x80)
    fprintf (stream, "CLR   " FORMAT_BYTE ".%X                    ",
             bit & 0xF8, bit & 0x07);
  else
    fprintf (stream, "CLR   " FORMAT_BYTE "                      ", bit);
}

/* setb C                 1       1 */
static void fprint_setb_C (FILE *stream)
{
  fprintf (stream, "SETB  C                         ");
}

/* setb bit               2       1 */
static void fprint_setb_bit (FILE *stream, uint8_t bit)
{
  if (bit & 0x80)
    fprintf (stream, "SETB  " FORMAT_BYTE ".%X                    ",
             bit & 0xF8, bit & 0x07);
  else
    fprintf (stream, "SETB  " FORMAT_BYTE "                      ", bit);
}

/* cpl C                  1       1 */
static void fprint_cpl_C (FILE *stream)
{
  fprintf (stream, "CPL   C                         ");
}

/* cpl bit                2       1 */
static void fprint_cpl_bit (FILE *stream, uint8_t bit)
{
  if (bit & 0x80)
    fprintf (stream, "CPL   " FORMAT_BYTE ".%X                    ",
             bit & 0xF8, bit & 0x07);
  else
    fprintf (stream, "CPL   " FORMAT_BYTE "                      ", bit);
}

/* anl C, bit             2       2 */
static void fprint_anl_bit (FILE *stream, uint8_t bit)
{
  if (bit & 0x80)
    fprintf (stream, "ANL   C, " FORMAT_BYTE ".%X                 ",
             bit & 0xF8, bit & 0x07);
  else
    fprintf (stream, "ANL   C, " FORMAT_BYTE "                   ", bit);
}

/* anl C, /bit            2       2 */
static void fprint_anl_not_bit (FILE *stream, uint8_t bit)
{
  if (bit & 0x80)
    fprintf (stream, "ANL   C, /" FORMAT_BYTE ".%X                ",
             bit & 0xF8, bit & 0x07);
  else
    fprintf (stream, "ANL   C, /" FORMAT_BYTE "                  ", bit);
}

/* orl C, bit             2       2 */
static void fprint_orl_bit (FILE *stream, uint8_t bit)
{
  if (bit & 0x80)
    fprintf (stream, "ORL   C, " FORMAT_BYTE ".%X                 ",
             bit & 0xF8, bit & 0x07);
  else
    fprintf (stream, "ORL   C, " FORMAT_BYTE "                   ", bit);
}

/* orl C, /bit            2       2 */
static void fprint_orl_not_bit (FILE *stream, uint8_t bit)
{
  if (bit & 0x80)
    fprintf (stream, "ORL   C, /" FORMAT_BYTE ".%X                ",
             bit & 0xF8, bit & 0x07);
  else
    fprintf (stream, "ORL   C, /" FORMAT_BYTE "                  ", bit);
}

/* mov C, bit             2       1 */
static void fprint_mov_bit (FILE *stream, uint8_t bit)
{
  if (bit & 0x80)
    fprintf (stream, "MOV   C, " FORMAT_BYTE ".%X                 ",
             bit & 0xF8, bit & 0x07);
  else
    fprintf (stream, "MOV   C, " FORMAT_BYTE "                   ", bit);
}

/* mov bit, C             2       2 */
static void fprint_mov_to_bit (FILE *stream, uint8_t bit)
{
  if (bit & 0x80)
    fprintf (stream, "MOV   " FORMAT_BYTE ".%X, C                 ",
             bit & 0xF8, bit & 0x07);
  else
    fprintf (stream, "MOV   " FORMAT_BYTE ", C                   ", bit);
}

/* xch A, Rn              1       1 */
static void fprint_xch_Rn (FILE *stream, unsigned char n)
{
  fprintf (stream, "XCH   A, R%X                     ", n);
}

/* xch A, direct          2       1 */
static void fprint_xch_direct (FILE *stream, uint8_t direct)
{
  fprintf (stream, "XCH   A, " FORMAT_BYTE "                   ", direct);
}

/* xch A, @Ri             1       1 */
static void fprint_xch_atRi (FILE *stream, unsigned char i)
{
  fprintf (stream, "XCH   A, @R%X                    ", i);
}

/* xchd A, @Ri            1       1 */
static void fprint_xchd_atRi (FILE *stream, unsigned char i)
{
  fprintf (stream, "XCHD  A, @R%X                    ", i);
}

/* push direct            2       2 */
static void fprint_push (FILE *stream, uint8_t direct)
{
  fprintf (stream, "PUSH  " FORMAT_BYTE "                      ", direct);
}

/* pop direct             2       2 */
static void fprint_pop (FILE *stream, uint8_t direct)
{
  fprintf (stream, "POP   " FORMAT_BYTE "                      ", direct);
}

/* jz rel                 2       2 */
static void fprint_jz (FILE *stream, uint8_t rel)
{
  if (PC == -1)
    fprintf (stream, "JZ    ($ + 2) + % 4d            ",
             (int8_t) rel);
  else
    fprintf (stream, "JZ    " FORMAT_WORD "                    ",
             (uint16_t) PC + 2 + (int8_t) rel);
}

/* jnz rel                2       2 */
static void fprint_jnz (FILE *stream, uint8_t rel)
{
  if (PC == -1)
    fprintf (stream, "JNZ   ($ + 2) + % 4d            ",
             (int8_t) rel);
  else
    fprintf (stream, "JNZ   " FORMAT_WORD "                    ",
             (uint16_t) PC + 2 + (int8_t) rel);
}

/* jc rel                 2       2 */
static void fprint_jc (FILE *stream, uint8_t rel)
{
  if (PC == -1)
    fprintf (stream, "JC    ($ + 2) + % 4d            ",
             (int8_t) rel);
  else
    fprintf (stream, "JC    " FORMAT_WORD "                    ",
             (uint16_t) PC + 2 + (int8_t) rel);
}

/* jnc rel                2       2 */
static void fprint_jnc (FILE *stream, uint8_t rel)
{
  if (PC == -1)
    fprintf (stream, "JNC   ($ + 2) + % 4d            ",
             (int8_t) rel);
  else
    fprintf (stream, "JNC   " FORMAT_WORD "                    ",
             (uint16_t) PC + 2 + (int8_t) rel);
}

/* jb bit, rel            3       2 */
static void fprint_jb (FILE *stream, uint8_t bit, uint8_t rel)
{
  if (PC == -1)
    fprintf (stream, "JB    " FORMAT_BYTE ".%X, ($ + 3) + % 4d    ",
             (bit & 0x80) ? (bit & 0xF8) : (0x20 + (bit >> 3)), bit & 0x07,
             (int8_t) rel);
  else
    fprintf (stream, "JB    " FORMAT_BYTE ".%X, " FORMAT_WORD "            ",
             (bit & 0x80) ? (bit & 0xF8) : (0x20 + (bit >> 3)), bit & 0x07,
             (uint16_t) PC + 3 + (int8_t) rel);
}

/* jnb bit, rel           3       2 */
static void fprint_jnb (FILE *stream, uint8_t bit, uint8_t rel)
{
  if (PC == -1)
    fprintf (stream, "JNB   " FORMAT_BYTE ".%X, ($ + 3) + % 4d    ",
             (bit & 0x80) ? (bit & 0xF8) : (0x20 + (bit >> 3)), bit & 0x07,
             (int8_t) rel);
  else
    fprintf (stream, "JNB   " FORMAT_BYTE ".%X, " FORMAT_WORD "            ",
             (bit & 0x80) ? (bit & 0xF8) : (0x20 + (bit >> 3)), bit & 0x07,
             (uint16_t) PC + 3 + (int8_t) rel);
}

/* jbc bit, rel           3       2 */
static void fprint_jbc (FILE *stream, uint8_t bit, uint8_t rel)
{
  if (PC == -1)
    fprintf (stream, "JBC   " FORMAT_BYTE ".%X, ($ + 3) + % 4d    ",
             (bit & 0x80) ? (bit & 0xF8) : (0x20 + (bit >> 3)), bit & 0x07,
             (int8_t) rel);
  else
    fprintf (stream, "JBC   " FORMAT_BYTE ".%X, " FORMAT_WORD "            ",
             (bit & 0x80) ? (bit & 0xF8) : (0x20 + (bit >> 3)), bit & 0x07,
             (uint16_t) PC + 3 + (int8_t) rel);
}

/* cjne A, direct, rel    3       2 */
static void fprint_cjne_direct (FILE *stream, uint8_t direct, uint8_t rel)
{
  if (PC == -1)
    fprintf (stream, "CJNE  A, " FORMAT_BYTE ", ($ + 3) + % 4d   ",
             direct, (int8_t) rel);
  else
    fprintf (stream, "CJNE  A, " FORMAT_BYTE ", " FORMAT_WORD "           ",
             direct, (uint16_t) PC + 3 + (int8_t) rel);
}

/* cjne A, #data, rel     3       2 */
static void fprint_cjne_data (FILE *stream, uint8_t data, uint8_t rel)
{
  if (PC == -1)
    fprintf (stream, "CJNE  A, #" FORMAT_BYTE ", ($ + 3) + % 4d  ",
             data, (int8_t) rel);
  else
    fprintf (stream, "CJNE  A, #" FORMAT_BYTE ", " FORMAT_WORD "          ",
             data, (uint16_t) PC + 3 + (int8_t) rel);
}

/* cjne Rn, #data, rel    3       2 */
static void fprint_cjne_data_with_Rn (FILE *stream, unsigned char n, uint8_t data, uint8_t rel)
{
  if (PC == -1)
    fprintf (stream, "CJNE  R%X, #" FORMAT_BYTE ", ($+3) + % 4d   ",
             n, data, (int8_t) rel);
  else
    fprintf (stream, "CJNE  R%X, #" FORMAT_BYTE ", " FORMAT_WORD "         ",
             n, data, (uint16_t) PC + 3 + (int8_t) rel);
}

/* cjne @Ri, #data, rel   3       2 */
static void fprint_cjne_data_with_atRi (FILE *stream, unsigned char i, uint8_t data, uint8_t rel)
{
  if (PC == -1)
    fprintf (stream, "CJNE  @R%X, #" FORMAT_BYTE ", ($ + 3) + % 4d",
             i, data, (int8_t) rel);
  else
    fprintf (stream, "CJNE  @R%X, #" FORMAT_BYTE ", " FORMAT_WORD "        ",
             i, data, (uint16_t) PC + 3 + (int8_t) rel);
}

/* djnz Rn, rel           2       2 */
static void fprint_djnz_Rn (FILE *stream, unsigned char n, uint8_t rel)
{
  if (PC == -1)
    fprintf (stream, "DJNZ  R%X, ($ + 2) + % 4d        ",
             n, (int8_t) rel);
  else
    fprintf (stream, "DJNZ  R%X, " FORMAT_WORD "                ",
             n, (uint16_t) PC + 2 + (int8_t) rel);
}

/* djnz direct, rel       3       2 */
static void fprint_djnz_direct (FILE *stream, uint8_t direct, uint8_t rel)
{
  if (PC == -1)
    fprintf (stream, "DJNZ  " FORMAT_BYTE ", ($ + 3) + % 4d      ",
             direct, (int8_t) rel);
  else
    fprintf (stream, "DJNZ  " FORMAT_BYTE ", " FORMAT_WORD "              ",
             direct, (uint16_t) PC + 3 + (int8_t) rel);
}

/* ajmp addr11            2       2 */
static void fprint_ajmp (FILE *stream, unsigned char prefix, uint8_t addr11)
{
  if (PC == -1)
    fprintf (stream, "AJMP  ($ & " FORMAT_WORD ") | " FORMAT_WORD "     ",
             0xF800, (prefix << 8) | addr11);
  else
    fprintf (stream, "AJMP  " FORMAT_WORD "                    ",
             (PC & 0xF800) | (prefix << 8) | addr11);
}

/* ljmp addr16            3       2 */
static void fprint_ljmp (FILE *stream, uint8_t addr16_high, uint8_t addr16_low)
{
  fprintf (stream, "LJMP  " FORMAT_WORD "                    ",
           (addr16_high << 8) + addr16_low);
}

/* sjmp rel               2       2 */
static void fprint_sjmp (FILE *stream, uint8_t rel)
{
  if (PC == -1)
    fprintf (stream, "SJMP  ($ + 2) + % 4d            ",
             (int8_t) rel);
  else
    fprintf (stream, "SJMP  " FORMAT_WORD "                    ",
             (uint16_t) PC + 2 + (int8_t) rel);
}

/* jmp @A+DPTR            1       2 */
static void fprint_jmp_DPTR (FILE *stream)
{
  fprintf (stream, "JMP   @A+DPTR                   ");
}

/* nop                    1       1 */
static void fprint_nop (FILE *stream)
{
  fprintf (stream, "NOP                             ");
}

/* acall addr11           2       2 */
static void fprint_acall (FILE *stream, unsigned char prefix, uint8_t addr11)
{
  if (PC == -1)
    fprintf (stream, "ACALL ($ & " FORMAT_WORD ") | " FORMAT_WORD "     ",
             0xF800, (prefix << 8) | addr11);
  else
    fprintf (stream, "ACALL " FORMAT_WORD "                    ",
             (PC & 0xF800) | (prefix << 8) | addr11);
}

/* lcall addr16           3       2 */
static void fprint_lcall (FILE *stream, uint8_t addr16_high, uint8_t addr16_low)
{
  fprintf (stream, "LCALL " FORMAT_WORD "                    ",
           (addr16_high << 8) + addr16_low);
}

/* ret                    1       2 */
static void fprint_ret (FILE *stream)
{
  fprintf (stream, "RET                             ");
}

/* reti                   1       2 */
static void fprint_reti (FILE *stream)
{
  fprintf (stream, "RETI                            ");
}

void fprint_opcode (FILE *stream, uint8_t *IR)
{
  if (IR[3] > 0)
    fprintf (stream, "%02X", IR[0]);
  if (IR[3] > 1)
    fprintf (stream, "%02X", IR[1]);
  if (IR[3] > 2)
    fprintf (stream, "%02X", IR[2]);
}

void fprint_op (FILE *stream, uint8_t *IR, int32_t current_PC)
{
  unsigned char n;

  PC = current_PC;

  /* opcodes with register argument */
  if (IR[0] & 0x08)
    {
      n = IR[0] & 0x07;
      switch (IR[0] & 0xF0)
        {
        case 0x00:
          fprint_inc_Rn (stream, n);
          break;
        case 0x10:
          fprint_dec_Rn (stream, n);
          break;
        case 0x20:
          fprint_add_Rn (stream, n);
          break;
        case 0x30:
          fprint_addc_Rn (stream, n);
          break;
        case 0x40:
          fprint_orl_Rn (stream, n);
          break;
        case 0x50:
          fprint_anl_Rn (stream, n);
          break;
        case 0x60:
          fprint_xrl_Rn (stream, n);
          break;
        case 0x70:
          fprint_mov_data_to_Rn (stream, n, IR[1]);
          break;
        case 0x80:
          fprint_mov_Rn_to_direct (stream, n, IR[1]);
          break;
        case 0x90:
          fprint_subb_Rn (stream, n);
          break;
        case 0xA0:
          fprint_mov_direct_to_Rn (stream, n, IR[1]);
          break;
        case 0xB0:
          fprint_cjne_data_with_Rn (stream, n, IR[1], IR[2]);
          break;
        case 0xC0:
          fprint_xch_Rn (stream, n);
          break;
        case 0xD0:
          fprint_djnz_Rn (stream, n, IR[1]);
          break;
        case 0xE0:
          fprint_mov_Rn (stream, n);
          break;
        case 0xF0:
          fprint_mov_to_Rn (stream, n);
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
          fprint_inc_atRi (stream, n);
          break;
        case 0x10:
          fprint_dec_atRi (stream, n);
          break;
        case 0x20:
          fprint_add_atRi (stream, n);
          break;
        case 0x30:
          fprint_addc_atRi (stream, n);
          break;
        case 0x40:
          fprint_orl_atRi (stream, n);
          break;
        case 0x50:
          fprint_anl_atRi (stream, n);
          break;
        case 0x60:
          fprint_xrl_atRi (stream, n);
          break;
        case 0x70:
          fprint_mov_data_to_atRi (stream, n, IR[1]);
          break;
        case 0x80:
          fprint_mov_atRi_to_direct (stream, n, IR[1]);
          break;
        case 0x90:
          fprint_subb_atRi (stream, n);
          break;
        case 0xA0:
          fprint_mov_direct_to_atRi (stream, n, IR[1]);
          break;
        case 0xB0:
          fprint_cjne_data_with_atRi (stream, n, IR[1], IR[2]);
          break;
        case 0xC0:
          fprint_xch_atRi (stream, n);
          break;
        case 0xD0:
          fprint_xchd_atRi (stream, n);
          break;
        case 0xE0:
          fprint_mov_atRi (stream, n);
          break;
        case 0xF0:
          fprint_mov_to_atRi (stream, n);
          break;
        }
    }
  else if ((IR[0] & 0x0F) == 0x01)
    {
      n = (IR[0] & 0xE0) >> 5;
      if (IR[0] & 0x10)
        fprint_acall (stream, n, IR[1]);
      else
        fprint_ajmp (stream, n, IR[1]);
    }
  else if ((IR[0] & 0xEE) == 0xE2)
    {
      n = IR[0] & 0x01;
      if (IR[0] & 0x10)
        fprint_movx_to_atRi (stream, n);
      else
        fprint_movx_atRi (stream, n);
    }
  else
    {
      switch (IR[0])
        {
        case 0x00:
          fprint_nop (stream);
          break;
        case 0x10:
          fprint_jbc (stream, IR[1], IR[2]);
          break;
        case 0x20:
          fprint_jb (stream, IR[1], IR[2]);
          break;
        case 0x30:
          fprint_jnb (stream, IR[1], IR[2]);
          break;
        case 0x40:
          fprint_jc (stream, IR[1]);
          break;
        case 0x50:
          fprint_jnc (stream, IR[1]);
          break;
        case 0x60:
          fprint_jz (stream, IR[1]);
          break;
        case 0x70:
          fprint_jnz (stream, IR[1]);
          break;
        case 0x80:
          fprint_sjmp (stream, IR[1]);
          break;
        case 0x90:
          fprint_mov_to_DPTR (stream, IR[1], IR[2]);
          break;
        case 0xA0:
          fprint_orl_not_bit (stream, IR[1]);
          break;
        case 0xB0:
          fprint_anl_not_bit (stream, IR[1]);
          break;
        case 0xC0:
          fprint_push (stream, IR[1]);
          break;
        case 0xD0:
          fprint_pop (stream, IR[1]);
          break;
        case 0xE0:
          fprint_movx_atDPTR (stream);
          break;
        case 0xF0:
          fprint_movx_to_atDPTR (stream);
          break;
        case 0x02:
          fprint_ljmp (stream, IR[1], IR[2]);
          break;
        case 0x12:
          fprint_lcall (stream, IR[1], IR[2]);
          break;
        case 0x22:
          fprint_ret (stream);
          break;
        case 0x32:
          fprint_reti (stream);
          break;
        case 0x42:
          fprint_orl_to_direct (stream, IR[1]);
          break;
        case 0x52:
          fprint_anl_to_direct (stream, IR[1]);
          break;
        case 0x62:
          fprint_xrl_to_direct (stream, IR[1]);
          break;
        case 0x72:
          fprint_orl_bit (stream, IR[1]);
          break;
        case 0x82:
          fprint_anl_bit (stream, IR[1]);
          break;
        case 0x92:
          fprint_mov_to_bit (stream, IR[1]);
          break;
        case 0xA2:
          fprint_mov_bit (stream, IR[1]);
          break;
        case 0xB2:
          fprint_cpl_bit (stream, IR[1]);
          break;
        case 0xC2:
          fprint_clr_bit (stream, IR[1]);
          break;
        case 0xD2:
          fprint_setb_bit (stream, IR[1]);
          break;
        case 0x03:
          fprint_rr (stream);
          break;
        case 0x13:
          fprint_rrc (stream);
          break;
        case 0x23:
          fprint_rl (stream);
          break;
        case 0x33:
          fprint_rlc (stream);
          break;
        case 0x43:
          fprint_orl_data_to_direct (stream, IR[1], IR[2]);
          break;
        case 0x53:
          fprint_anl_data_to_direct (stream, IR[1], IR[2]);
          break;
        case 0x63:
          fprint_xrl_data_to_direct (stream, IR[1], IR[2]);
          break;
        case 0x73:
          fprint_jmp_DPTR (stream);
          break;
        case 0x83:
          fprint_movc_PC (stream);
          break;
        case 0x93:
          fprint_movc_DPTR (stream);
          break;
        case 0xA3:
          fprint_inc_DPTR (stream);
          break;
        case 0xB3:
          fprint_cpl_C (stream);
          break;
        case 0xC3:
          fprint_clr_C (stream);
          break;
        case 0xD3:
          fprint_setb_C (stream);
          break;
        case 0x04:
          fprint_inc (stream);
          break;
        case 0x14:
          fprint_dec (stream);
          break;
        case 0x24:
          fprint_add_data (stream, IR[1]);
          break;
        case 0x34:
          fprint_addc_data (stream, IR[1]);
          break;
        case 0x44:
          fprint_orl_data (stream, IR[1]);
          break;
        case 0x54:
          fprint_anl_data (stream, IR[1]);
          break;
        case 0x64:
          fprint_xrl_data (stream, IR[1]);
          break;
        case 0x74:
          fprint_mov_data (stream, IR[1]);
          break;
        case 0x84:
          fprint_div (stream);
          break;
        case 0x94:
          fprint_subb_data (stream, IR[1]);
          break;
        case 0xA4:
          fprint_mul (stream);
          break;
        case 0xB4:
          fprint_cjne_data (stream, IR[1], IR[2]);
          break;
        case 0xC4:
          fprint_swap (stream);
          break;
        case 0xD4:
          fprint_da (stream);
          break;
        case 0xE4:
          fprint_clr (stream);
          break;
        case 0xF4:
          fprint_cpl (stream);
          break;

        case 0x05:
          fprint_inc_direct (stream, IR[1]);
          break;
        case 0x15:
          fprint_dec_direct (stream, IR[1]);
          break;
        case 0x25:
          fprint_add_direct (stream, IR[1]);
          break;
        case 0x35:
          fprint_addc_direct (stream, IR[1]);
          break;
        case 0x45:
          fprint_orl_direct (stream, IR[1]);
          break;
        case 0x55:
          fprint_anl_direct (stream, IR[1]);
          break;
        case 0x65:
          fprint_xrl_direct (stream, IR[1]);
          break;
        case 0x75:
          fprint_mov_data_to_direct (stream, IR[1], IR[2]);
          break;
        case 0x85:
          fprint_mov_direct_to_direct (stream, IR[1], IR[2]);
          break;
        case 0x95:
          fprint_subb_direct (stream, IR[1]);
          break;
        case 0xB5:
          fprint_cjne_direct (stream, IR[1], IR[2]);
          break;
        case 0xC5:
          fprint_xch_direct (stream, IR[1]);
          break;
        case 0xD5:
          fprint_djnz_direct (stream, IR[1], IR[2]);
          break;
        case 0xE5:
          fprint_mov_direct (stream, IR[1]);
          break;
        case 0xF5:
          fprint_mov_to_direct (stream, IR[1]);
          break;
        }
    }
}


/* sprint primitives */

/* add A, Rn              1       1 */
static void sprint_add_Rn (char *str, unsigned char n)
{
  sprintf (str, "ADD   A, R%X                     ", n);
}

/* add A, direct          2       1 */
static void sprint_add_direct (char *str, uint8_t direct)
{
  sprintf (str, "ADD   A, " FORMAT_BYTE "                   ", direct);
}

/* add A, @Ri             1       1 */
static void sprint_add_atRi (char *str, unsigned char i)
{
  sprintf (str, "ADD   A, @R%X                    ", i);
}

/* add A, #data           2       1 */
static void sprint_add_data (char *str, uint8_t data)
{
  sprintf (str, "ADD   A, #" FORMAT_BYTE "                  ", data);
}

/* addc A, Rn             1       1 */
static void sprint_addc_Rn (char *str, unsigned char n)
{
  sprintf (str, "ADDC  A, R%X                     ", n);
}

/* addc A, direct         2       1 */
static void sprint_addc_direct (char *str, uint8_t direct)
{
  sprintf (str, "ADDC  A, " FORMAT_BYTE "                   ", direct);
}

/* addc A, @Ri            1       1 */
static void sprint_addc_atRi (char *str, unsigned char i)
{
  sprintf (str, "ADDC  A, @R%X                    ", i);
}

/* addc A, #data          2       1 */
static void sprint_addc_data (char *str, uint8_t data)
{
  sprintf (str, "ADDC  A, #" FORMAT_BYTE "                  ", data);
}

/* subb A, Rn             1       1 */
static void sprint_subb_Rn (char *str, unsigned char n)
{
  sprintf (str, "SUBB  A, R%X                     ", n);
}

/* subb A, direct         2       1 */
static void sprint_subb_direct (char *str, uint8_t direct)
{
  sprintf (str, "SUBB  A, " FORMAT_BYTE "                   ", direct);
}

/* subb A, @Ri            1       1 */
static void sprint_subb_atRi (char *str, unsigned char i)
{
  sprintf (str, "SUBB  A, @R%X                    ", i);
}

/* subb A, #data          2       1 */
static void sprint_subb_data (char *str, uint8_t data)
{
  sprintf (str, "SUBB  A, #" FORMAT_BYTE "                  ", data);
}

/* inc A                  1       1 */
static void sprint_inc (char *str)
{
  sprintf (str, "INC   A                         ");
}

/* inc Rn                 1       1 */
static void sprint_inc_Rn (char *str, unsigned char n)
{
  sprintf (str, "INC   R%X                        ", n);
}

/* inc direct             2       1 */
static void sprint_inc_direct (char *str, uint8_t direct)
{
  sprintf (str, "INC   " FORMAT_BYTE "                      ", direct);
}

/* inc @Ri                1       1 */
static void sprint_inc_atRi (char *str, unsigned char i)
{
  sprintf (str, "INC   @R%X                       ", i);
}

/* dec A                  1       1 */
static void sprint_dec (char *str)
{
  sprintf (str, "DEC   A                         ");
}

/* dec Rn                 1       1 */
static void sprint_dec_Rn (char *str, unsigned char n)
{
  sprintf (str, "DEC   R%X                        ", n);
}

/* dec direct             2       1 */
static void sprint_dec_direct (char *str, uint8_t direct)
{
  sprintf (str, "DEC   " FORMAT_BYTE "                      ", direct);
}

/* dec @Ri                1       1 */
static void sprint_dec_atRi (char *str, unsigned char i)
{
  sprintf (str, "DEC   @R%X                       ", i);
}

/* inc DPTR               1       2 */
static void sprint_inc_DPTR (char *str)
{
  sprintf (str, "INC   DPTR                      ");
}

/* anl A, Rn              1       1 */
static void sprint_anl_Rn (char *str, unsigned char n)
{
  sprintf (str, "ANL   A, R%X                     ", n);
}

/* anl A, direct          2       1 */
static void sprint_anl_direct (char *str, uint8_t direct)
{
  sprintf (str, "ANL   A, " FORMAT_BYTE "                   ", direct);
}

/* anl A, @Ri             1       1 */
static void sprint_anl_atRi (char *str, unsigned char i)
{
  sprintf (str, "ANL   A, @R%X                    ", i);
}

/* anl A, #data           2       1 */
static void sprint_anl_data (char *str, uint8_t data)
{
  sprintf (str, "ANL   A, #" FORMAT_BYTE "                  ", data);
}

/* anl direct, A          2       1 */
static void sprint_anl_to_direct (char *str, uint8_t direct)
{
  sprintf (str, "ANL   " FORMAT_BYTE ", A                   ", direct);
}

/* anl direct, #data      3       2 */
static void sprint_anl_data_to_direct (char *str, uint8_t direct, uint8_t data)
{
  sprintf (str, "ANL   " FORMAT_BYTE ", #" FORMAT_BYTE "               ", direct, data);
}

/* orl A, Rn              1       1 */
static void sprint_orl_Rn (char *str, unsigned char n)
{
  sprintf (str, "ORL   A, R%X                     ", n);
}

/* orl A, direct          2       1 */
static void sprint_orl_direct (char *str, uint8_t direct)
{
  sprintf (str, "ORL   A, " FORMAT_BYTE "                   ", direct);
}

/* orl A, @Ri             1       1 */
static void sprint_orl_atRi (char *str, unsigned char i)
{
  sprintf (str, "ORL   A, @R%X                    ", i);
}

/* orl A, #data           2       1 */
static void sprint_orl_data (char *str, uint8_t data)
{
  sprintf (str, "ORL   A, #" FORMAT_BYTE "                  ", data);
}

/* orl direct, A          2       1 */
static void sprint_orl_to_direct (char *str, uint8_t direct)
{
  sprintf (str, "ORL   " FORMAT_BYTE ", A                   ", direct);
}

/* orl direct, #data      3       2 */
static void sprint_orl_data_to_direct (char *str, uint8_t direct, uint8_t data)
{
  sprintf (str, "ORL   " FORMAT_BYTE ", #" FORMAT_BYTE "               ", direct, data);
}

/* xrl A, Rn              1       1 */
static void sprint_xrl_Rn (char *str, unsigned char n)
{
  sprintf (str, "XRL   A, R%X                     ", n);
}

/* xrl A, direct          2       1 */
static void sprint_xrl_direct (char *str, uint8_t direct)
{
  sprintf (str, "XRL   A, " FORMAT_BYTE "                   ", direct);
}

/* xrl A, @Ri             1       1 */
static void sprint_xrl_atRi (char *str, unsigned char i)
{
  sprintf (str, "XRL   A, @R%X                    ", i);
}

/* xrl A, #data           2       1 */
static void sprint_xrl_data (char *str, uint8_t data)
{
  sprintf (str, "XRL   A, #" FORMAT_BYTE "                  ", data);
}

/* xrl direct, A          2       1 */
static void sprint_xrl_to_direct (char *str, uint8_t direct)
{
  sprintf (str, "XRL   " FORMAT_BYTE ", A                   ", direct);
}

/* xrl direct, #data      3       2 */
static void sprint_xrl_data_to_direct (char *str, uint8_t direct, uint8_t data)
{
  sprintf (str, "XRL   " FORMAT_BYTE ", #" FORMAT_BYTE "               ", direct, data);
}

/* clr A                  1       1 */
static void sprint_clr (char *str)
{
  sprintf (str, "CLR   A                         ");
}

/* cpl A                  1       1 */
static void sprint_cpl (char *str)
{
  sprintf (str, "CPL   A                         ");
}

/* rl A                   1       1 */
static void sprint_rl (char *str)
{
  sprintf (str, "RL    A                         ");
}

/* rlc A                  1       1 */
static void sprint_rlc (char *str)
{
  sprintf (str, "RLC   A                         ");
}

/* rr A                   1       1 */
static void sprint_rr (char *str)
{
  sprintf (str, "RR    A                         ");
}

/* rrc A                  1       1 */
static void sprint_rrc (char *str)
{
  sprintf (str, "RRC   A                         ");
}

/* swap A                 1       1 */
static void sprint_swap (char *str)
{
  sprintf (str, "SWAP  A                         ");
}

/* mul AB                 1       4 */
static void sprint_mul (char *str)
{
  sprintf (str, "MUL   AB                        ");
}

/* div AB                 1       4 */
static void sprint_div (char *str)
{
  sprintf (str, "DIV   AB                        ");
}

/* da A                   1       1 */
static void sprint_da (char *str)
{
  sprintf (str, "DA    A                         ");
}

/* mov A, Rn              1       1 */
static void sprint_mov_Rn (char *str, unsigned char n)
{
  sprintf (str, "MOV   A, R%X                     ", n);
}

/* mov A, direct          2       1 */
static void sprint_mov_direct (char *str, uint8_t direct)
{
  sprintf (str, "MOV   A, " FORMAT_BYTE "                   ", direct);
}

/* mov A, @Ri             1       1 */
static void sprint_mov_atRi (char *str, unsigned char i)
{
  sprintf (str, "MOV   A, @R%X                    ", i);
}

/* mov A, #data           2       1 */
static void sprint_mov_data (char *str, uint8_t data)
{
  sprintf (str, "MOV   A, #" FORMAT_BYTE "                  ", data);
}

/* mov Rn, A              1       1 */
static void sprint_mov_to_Rn (char *str, unsigned char n)
{
  sprintf (str, "MOV   R%X, A                     ", n);
}

/* mov Rn, direct         2       2 */
static void sprint_mov_direct_to_Rn (char *str, unsigned char n, uint8_t direct)
{
  sprintf (str, "MOV   R%X, " FORMAT_BYTE "                  ", n, direct);
}

/* mov Rn, #data          2       1 */
static void sprint_mov_data_to_Rn (char *str, unsigned char n, uint8_t data)
{
  sprintf (str, "MOV   R%X, #" FORMAT_BYTE "                 ", n, data);
}

/* mov direct, A          2       1 */
static void sprint_mov_to_direct (char *str, uint8_t direct)
{
  sprintf (str, "MOV   " FORMAT_BYTE ", A                   ", direct);
}

/* mov direct, Rn         2       2 */
static void sprint_mov_Rn_to_direct (char *str, unsigned char n, uint8_t direct)
{
  sprintf (str, "MOV   " FORMAT_BYTE ", R%X                  ", direct, n);
}

/* mov direct, direct     3       2 */
static void sprint_mov_direct_to_direct (char *str, uint8_t direct_src, uint8_t direct_dst)
{
  sprintf (str, "MOV   " FORMAT_BYTE ", " FORMAT_BYTE "                ",
           direct_dst, direct_src);
}

/* mov direct, @Ri        2       2 */
static void sprint_mov_atRi_to_direct (char *str, unsigned char i, uint8_t direct)
{
  sprintf (str, "MOV   " FORMAT_BYTE ", @R%X                 ", direct, i);
}

/* mov direct, #data      3       2 */
static void sprint_mov_data_to_direct (char *str, uint8_t direct, uint8_t data)
{
  sprintf (str, "MOV   " FORMAT_BYTE ", #" FORMAT_BYTE "               ", direct, data);
}

/* mov @Ri, A             1       1 */
static void sprint_mov_to_atRi (char *str, unsigned char i)
{
  sprintf (str, "MOV   @R%X, A                    ", i);
}

/* mov @Ri, direct        2       2 */
static void sprint_mov_direct_to_atRi (char *str, unsigned char i, uint8_t direct)
{
  sprintf (str, "MOV   @R%X, " FORMAT_BYTE "                 ", i, direct);
}

/* mov @Ri, #data         2       1 */
static void sprint_mov_data_to_atRi (char *str, unsigned char i, uint8_t data)
{
  sprintf (str, "MOV   @R%X, #" FORMAT_BYTE "                ", i, data);
}

/* mov DPTR, #data16      3       2 */
static void sprint_mov_to_DPTR (char *str, uint8_t data16_high, uint8_t data16_low)
{
  sprintf (str, "MOV   DPTR, #" FORMAT_WORD "             ",
           (data16_high << 8) + data16_low);
}

/* movc A, @A+DPTR        1       2 */
static void sprint_movc_DPTR (char *str)
{
  sprintf (str, "MOVC  A, @A+DPTR                ");
}

/* movc A, @A+PC          1       2 */
static void sprint_movc_PC (char *str)
{
  sprintf (str, "MOVC  A, @A+PC                  ");
}

/* movx A, @Ri            1       2 */
static void sprint_movx_atRi (char *str, unsigned char i)
{
  sprintf (str, "MOVX  A, @R%X                    ", i);
}

/* movx A, @DPTR          1       2 */
static void sprint_movx_atDPTR (char *str)
{
  sprintf (str, "MOVX  A, @DPTR                  ");
}

/* movx @Ri, A            1       2 */
static void sprint_movx_to_atRi (char *str, unsigned char i)
{
  sprintf (str, "MOVX  @R%X, A                    ", i);
}

/* movx @DPTR, A          1       2 */
static void sprint_movx_to_atDPTR (char *str)
{
  sprintf (str, "MOVX  @DPTR, A                  ");
}

/* clr C                  1       1 */
static void sprint_clr_C (char *str)
{
  sprintf (str, "CLR   C                         ");
}

/* clr bit                2       1 */
static void sprint_clr_bit (char *str, uint8_t bit)
{
  if (bit & 0x80)
    sprintf (str, "CLR   " FORMAT_BYTE ".%X                    ",
             bit & 0xF8, bit & 0x07);
  else
    sprintf (str, "CLR   " FORMAT_BYTE "                      ", bit);
}

/* setb C                 1       1 */
static void sprint_setb_C (char *str)
{
  sprintf (str, "SETB  C                         ");
}

/* setb bit               2       1 */
static void sprint_setb_bit (char *str, uint8_t bit)
{
  if (bit & 0x80)
    sprintf (str, "SETB  " FORMAT_BYTE ".%X                    ",
             bit & 0xF8, bit & 0x07);
  else
    sprintf (str, "SETB  " FORMAT_BYTE "                      ", bit);
}

/* cpl C                  1       1 */
static void sprint_cpl_C (char *str)
{
  sprintf (str, "CPL   C                         ");
}

/* cpl bit                2       1 */
static void sprint_cpl_bit (char *str, uint8_t bit)
{
  if (bit & 0x80)
    sprintf (str, "CPL   " FORMAT_BYTE ".%X                    ",
             bit & 0xF8, bit & 0x07);
  else
    sprintf (str, "CPL   " FORMAT_BYTE "                      ", bit);
}

/* anl C, bit             2       2 */
static void sprint_anl_bit (char *str, uint8_t bit)
{
  if (bit & 0x80)
    sprintf (str, "ANL   C, " FORMAT_BYTE ".%X                 ",
             bit & 0xF8, bit & 0x07);
  else
    sprintf (str, "ANL   C, " FORMAT_BYTE "                   ", bit);
}

/* anl C, /bit            2       2 */
static void sprint_anl_not_bit (char *str, uint8_t bit)
{
  if (bit & 0x80)
    sprintf (str, "ANL   C, /" FORMAT_BYTE ".%X                ",
             bit & 0xF8, bit & 0x07);
  else
    sprintf (str, "ANL   C, /" FORMAT_BYTE "                  ", bit);
}

/* orl C, bit             2       2 */
static void sprint_orl_bit (char *str, uint8_t bit)
{
  if (bit & 0x80)
    sprintf (str, "ORL   C, " FORMAT_BYTE ".%X                 ",
             bit & 0xF8, bit & 0x07);
  else
    sprintf (str, "ORL   C, " FORMAT_BYTE "                   ", bit);
}

/* orl C, /bit            2       2 */
static void sprint_orl_not_bit (char *str, uint8_t bit)
{
  if (bit & 0x80)
    sprintf (str, "ORL   C, /" FORMAT_BYTE ".%X                ",
             bit & 0xF8, bit & 0x07);
  else
    sprintf (str, "ORL   C, /" FORMAT_BYTE "                  ", bit);
}

/* mov C, bit             2       1 */
static void sprint_mov_bit (char *str, uint8_t bit)
{
  if (bit & 0x80)
    sprintf (str, "MOV   C, " FORMAT_BYTE ".%X                 ",
             bit & 0xF8, bit & 0x07);
  else
    sprintf (str, "MOV   C, " FORMAT_BYTE "                   ", bit);
}

/* mov bit, C             2       2 */
static void sprint_mov_to_bit (char *str, uint8_t bit)
{
  if (bit & 0x80)
    sprintf (str, "MOV   " FORMAT_BYTE ".%X, C                 ",
             bit & 0xF8, bit & 0x07);
  else
    sprintf (str, "MOV   " FORMAT_BYTE ", C                   ", bit);
}

/* xch A, Rn              1       1 */
static void sprint_xch_Rn (char *str, unsigned char n)
{
  sprintf (str, "XCH   A, R%X                     ", n);
}

/* xch A, direct          2       1 */
static void sprint_xch_direct (char *str, uint8_t direct)
{
  sprintf (str, "XCH   A, " FORMAT_BYTE "                   ", direct);
}

/* xch A, @Ri             1       1 */
static void sprint_xch_atRi (char *str, unsigned char i)
{
  sprintf (str, "XCH   A, @R%X                    ", i);
}

/* xchd A, @Ri            1       1 */
static void sprint_xchd_atRi (char *str, unsigned char i)
{
  sprintf (str, "XCHD  A, @R%X                    ", i);
}

/* push direct            2       2 */
static void sprint_push (char *str, uint8_t direct)
{
  sprintf (str, "PUSH  " FORMAT_BYTE "                      ", direct);
}

/* pop direct             2       2 */
static void sprint_pop (char *str, uint8_t direct)
{
  sprintf (str, "POP   " FORMAT_BYTE "                      ", direct);
}

/* jz rel                 2       2 */
static void sprint_jz (char *str, uint8_t rel)
{
  if (PC == -1)
    sprintf (str, "JZ    ($ + 2) + % 4d            ",
             (int8_t) rel);
  else
    sprintf (str, "JZ    " FORMAT_WORD "                    ",
             (uint16_t) PC + 2 + (int8_t) rel);
}

/* jnz rel                2       2 */
static void sprint_jnz (char *str, uint8_t rel)
{
  if (PC == -1)
    sprintf (str, "JNZ   ($ + 2) + % 4d            ",
             (int8_t) rel);
  else
    sprintf (str, "JNZ   " FORMAT_WORD "                    ",
             (uint16_t) PC + 2 + (int8_t) rel);
}

/* jc rel                 2       2 */
static void sprint_jc (char *str, uint8_t rel)
{
  if (PC == -1)
    sprintf (str, "JC    ($ + 2) + % 4d            ",
             (int8_t) rel);
  else
    sprintf (str, "JC    " FORMAT_WORD "                    ",
             (uint16_t) PC + 2 + (int8_t) rel);
}

/* jnc rel                2       2 */
static void sprint_jnc (char *str, uint8_t rel)
{
  if (PC == -1)
    sprintf (str, "JNC   ($ + 2) + % 4d            ",
             (int8_t) rel);
  else
    sprintf (str, "JNC   " FORMAT_WORD "                    ",
             (uint16_t) PC + 2 + (int8_t) rel);
}

/* jb bit, rel            3       2 */
static void sprint_jb (char *str, uint8_t bit, uint8_t rel)
{
  if (PC == -1)
    sprintf (str, "JB    " FORMAT_BYTE ".%X, ($ + 3) + % 4d    ",
             (bit & 0x80) ? (bit & 0xF8) : (0x20 + (bit >> 3)), bit & 0x07,
             (int8_t) rel);
  else
    sprintf (str, "JB    " FORMAT_BYTE ".%X, " FORMAT_WORD "            ",
             (bit & 0x80) ? (bit & 0xF8) : (0x20 + (bit >> 3)), bit & 0x07,
             (uint16_t) PC + 3 + (int8_t) rel);
}

/* jnb bit, rel           3       2 */
static void sprint_jnb (char *str, uint8_t bit, uint8_t rel)
{
  if (PC == -1)
    sprintf (str, "JNB   " FORMAT_BYTE ".%X, ($ + 3) + % 4d    ",
             (bit & 0x80) ? (bit & 0xF8) : (0x20 + (bit >> 3)), bit & 0x07,
             (int8_t) rel);
  else
    sprintf (str, "JNB   " FORMAT_BYTE ".%X, " FORMAT_WORD "            ",
             (bit & 0x80) ? (bit & 0xF8) : (0x20 + (bit >> 3)), bit & 0x07,
             (uint16_t) PC + 3 + (int8_t) rel);
}

/* jbc bit, rel           3       2 */
static void sprint_jbc (char *str, uint8_t bit, uint8_t rel)
{
  if (PC == -1)
    sprintf (str, "JBC   " FORMAT_BYTE ".%X, ($ + 3) + % 4d    ",
             (bit & 0x80) ? (bit & 0xF8) : (0x20 + (bit >> 3)), bit & 0x07,
             (int8_t) rel);
  else
    sprintf (str, "JBC   " FORMAT_BYTE ".%X, " FORMAT_WORD "            ",
             (bit & 0x80) ? (bit & 0xF8) : (0x20 + (bit >> 3)), bit & 0x07,
             (uint16_t) PC + 3 + (int8_t) rel);
}

/* cjne A, direct, rel    3       2 */
static void sprint_cjne_direct (char *str, uint8_t direct, uint8_t rel)
{
  if (PC == -1)
    sprintf (str, "CJNE  A, " FORMAT_BYTE ", ($ + 3) + % 4d   ",
             direct, (int8_t) rel);
  else
    sprintf (str, "CJNE  A, " FORMAT_BYTE ", " FORMAT_WORD "           ",
             direct, (uint16_t) PC + 3 + (int8_t) rel);
}

/* cjne A, #data, rel     3       2 */
static void sprint_cjne_data (char *str, uint8_t data, uint8_t rel)
{
  if (PC == -1)
    sprintf (str, "CJNE  A, #" FORMAT_BYTE ", ($ + 3) + % 4d  ",
             data, (int8_t) rel);
  else
    sprintf (str, "CJNE  A, #" FORMAT_BYTE ", " FORMAT_WORD "          ",
             data, (uint16_t) PC + 3 + (int8_t) rel);
}

/* cjne Rn, #data, rel    3       2 */
static void sprint_cjne_data_with_Rn (char *str, unsigned char n, uint8_t data, uint8_t rel)
{
  if (PC == -1)
    sprintf (str, "CJNE  R%X, #" FORMAT_BYTE ", ($+3) + % 4d   ",
             n, data, (int8_t) rel);
  else
    sprintf (str, "CJNE  R%X, #" FORMAT_BYTE ", " FORMAT_WORD "         ",
             n, data, (uint16_t) PC + 3 + (int8_t) rel);
}

/* cjne @Ri, #data, rel   3       2 */
static void sprint_cjne_data_with_atRi (char *str, unsigned char i, uint8_t data, uint8_t rel)
{
  if (PC == -1)
    sprintf (str, "CJNE  @R%X, #" FORMAT_BYTE ", ($ + 3) + % 4d",
             i, data, (int8_t) rel);
  else
    sprintf (str, "CJNE  @R%X, #" FORMAT_BYTE ", " FORMAT_WORD "        ",
             i, data, (uint16_t) PC + 3 + (int8_t) rel);
}

/* djnz Rn, rel           2       2 */
static void sprint_djnz_Rn (char *str, unsigned char n, uint8_t rel)
{
  if (PC == -1)
    sprintf (str, "DJNZ  R%X, ($ + 2) + % 4d        ",
             n, (int8_t) rel);
  else
    sprintf (str, "DJNZ  R%X, " FORMAT_WORD "                ",
             n, (uint16_t) PC + 2 + (int8_t) rel);
}

/* djnz direct, rel       3       2 */
static void sprint_djnz_direct (char *str, uint8_t direct, uint8_t rel)
{
  if (PC == -1)
    sprintf (str, "DJNZ  " FORMAT_BYTE ", ($ + 3) + % 4d      ",
             direct, (int8_t) rel);
  else
    sprintf (str, "DJNZ  " FORMAT_BYTE ", " FORMAT_WORD "              ",
             direct, (uint16_t) PC + 3 + (int8_t) rel);
}

/* ajmp addr11            2       2 */
static void sprint_ajmp (char *str, unsigned char prefix, uint8_t addr11)
{
  if (PC == -1)
    sprintf (str, "AJMP  ($ & " FORMAT_WORD ") | " FORMAT_WORD "     ",
             0xF800, (prefix << 8) | addr11);
  else
    sprintf (str, "AJMP  " FORMAT_WORD "                    ",
             (PC & 0xF800) | (prefix << 8) | addr11);
}

/* ljmp addr16            3       2 */
static void sprint_ljmp (char *str, uint8_t addr16_high, uint8_t addr16_low)
{
  sprintf (str, "LJMP  " FORMAT_WORD "                    ",
           (addr16_high << 8) + addr16_low);
}

/* sjmp rel               2       2 */
static void sprint_sjmp (char *str, uint8_t rel)
{
  if (PC == -1)
    sprintf (str, "SJMP  ($ + 2) + % 4d            ",
             (int8_t) rel);
  else
    sprintf (str, "SJMP  " FORMAT_WORD "                    ",
             (uint16_t) PC + 2 + (int8_t) rel);
}

/* jmp @A+DPTR            1       2 */
static void sprint_jmp_DPTR (char *str)
{
  sprintf (str, "JMP   @A+DPTR                   ");
}

/* nop                    1       1 */
static void sprint_nop (char *str)
{
  sprintf (str, "NOP                             ");
}

/* acall addr11           2       2 */
static void sprint_acall (char *str, unsigned char prefix, uint8_t addr11)
{
  if (PC == -1)
    sprintf (str, "ACALL ($ & " FORMAT_WORD ") | " FORMAT_WORD "     ",
             0xF800, (prefix << 8) | addr11);
  else
    sprintf (str, "ACALL " FORMAT_WORD "                    ",
             (PC & 0xF800) | (prefix << 8) | addr11);
}

/* lcall addr16           3       2 */
static void sprint_lcall (char *str, uint8_t addr16_high, uint8_t addr16_low)
{
  sprintf (str, "LCALL " FORMAT_WORD "                    ",
           (addr16_high << 8) + addr16_low);
}

/* ret                    1       2 */
static void sprint_ret (char *str)
{
  sprintf (str, "RET                             ");
}

/* reti                   1       2 */
static void sprint_reti (char *str)
{
  sprintf (str, "RETI                            ");
}

void sprint_opcode (char *str, uint8_t *IR)
{
  if (IR[3] > 0)
    sprintf (str, "%02X", IR[0]);
  if (IR[3] > 1)
    sprintf (str, "%02X", IR[1]);
  if (IR[3] > 2)
    sprintf (str, "%02X", IR[2]);
}

void sprint_op (char *str, uint8_t *IR, int32_t current_PC)
{
  unsigned char n;

  PC = current_PC;

  /* opcodes with register argument */
  if (IR[0] & 0x08)
    {
      n = IR[0] & 0x07;
      switch (IR[0] & 0xF0)
        {
        case 0x00:
          sprint_inc_Rn (str, n);
          break;
        case 0x10:
          sprint_dec_Rn (str, n);
          break;
        case 0x20:
          sprint_add_Rn (str, n);
          break;
        case 0x30:
          sprint_addc_Rn (str, n);
          break;
        case 0x40:
          sprint_orl_Rn (str, n);
          break;
        case 0x50:
          sprint_anl_Rn (str, n);
          break;
        case 0x60:
          sprint_xrl_Rn (str, n);
          break;
        case 0x70:
          sprint_mov_data_to_Rn (str, n, IR[1]);
          break;
        case 0x80:
          sprint_mov_Rn_to_direct (str, n, IR[1]);
          break;
        case 0x90:
          sprint_subb_Rn (str, n);
          break;
        case 0xA0:
          sprint_mov_direct_to_Rn (str, n, IR[1]);
          break;
        case 0xB0:
          sprint_cjne_data_with_Rn (str, n, IR[1], IR[2]);
          break;
        case 0xC0:
          sprint_xch_Rn (str, n);
          break;
        case 0xD0:
          sprint_djnz_Rn (str, n, IR[1]);
          break;
        case 0xE0:
          sprint_mov_Rn (str, n);
          break;
        case 0xF0:
          sprint_mov_to_Rn (str, n);
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
          sprint_inc_atRi (str, n);
          break;
        case 0x10:
          sprint_dec_atRi (str, n);
          break;
        case 0x20:
          sprint_add_atRi (str, n);
          break;
        case 0x30:
          sprint_addc_atRi (str, n);
          break;
        case 0x40:
          sprint_orl_atRi (str, n);
          break;
        case 0x50:
          sprint_anl_atRi (str, n);
          break;
        case 0x60:
          sprint_xrl_atRi (str, n);
          break;
        case 0x70:
          sprint_mov_data_to_atRi (str, n, IR[1]);
          break;
        case 0x80:
          sprint_mov_atRi_to_direct (str, n, IR[1]);
          break;
        case 0x90:
          sprint_subb_atRi (str, n);
          break;
        case 0xA0:
          sprint_mov_direct_to_atRi (str, n, IR[1]);
          break;
        case 0xB0:
          sprint_cjne_data_with_atRi (str, n, IR[1], IR[2]);
          break;
        case 0xC0:
          sprint_xch_atRi (str, n);
          break;
        case 0xD0:
          sprint_xchd_atRi (str, n);
          break;
        case 0xE0:
          sprint_mov_atRi (str, n);
          break;
        case 0xF0:
          sprint_mov_to_atRi (str, n);
          break;
        }
    }
  else if ((IR[0] & 0x0F) == 0x01)
    {
      n = (IR[0] & 0xE0) >> 5;
      if (IR[0] & 0x10)
        sprint_acall (str, n, IR[1]);
      else
        sprint_ajmp (str, n, IR[1]);
    }
  else if ((IR[0] & 0xEE) == 0xE2)
    {
      n = IR[0] & 0x01;
      if (IR[0] & 0x10)
        sprint_movx_to_atRi (str, n);
      else
        sprint_movx_atRi (str, n);
    }
  else
    {
      switch (IR[0])
        {
        case 0x00:
          sprint_nop (str);
          break;
        case 0x10:
          sprint_jbc (str, IR[1], IR[2]);
          break;
        case 0x20:
          sprint_jb (str, IR[1], IR[2]);
          break;
        case 0x30:
          sprint_jnb (str, IR[1], IR[2]);
          break;
        case 0x40:
          sprint_jc (str, IR[1]);
          break;
        case 0x50:
          sprint_jnc (str, IR[1]);
          break;
        case 0x60:
          sprint_jz (str, IR[1]);
          break;
        case 0x70:
          sprint_jnz (str, IR[1]);
          break;
        case 0x80:
          sprint_sjmp (str, IR[1]);
          break;
        case 0x90:
          sprint_mov_to_DPTR (str, IR[1], IR[2]);
          break;
        case 0xA0:
          sprint_orl_not_bit (str, IR[1]);
          break;
        case 0xB0:
          sprint_anl_not_bit (str, IR[1]);
          break;
        case 0xC0:
          sprint_push (str, IR[1]);
          break;
        case 0xD0:
          sprint_pop (str, IR[1]);
          break;
        case 0xE0:
          sprint_movx_atDPTR (str);
          break;
        case 0xF0:
          sprint_movx_to_atDPTR (str);
          break;
        case 0x02:
          sprint_ljmp (str, IR[1], IR[2]);
          break;
        case 0x12:
          sprint_lcall (str, IR[1], IR[2]);
          break;
        case 0x22:
          sprint_ret (str);
          break;
        case 0x32:
          sprint_reti (str);
          break;
        case 0x42:
          sprint_orl_to_direct (str, IR[1]);
          break;
        case 0x52:
          sprint_anl_to_direct (str, IR[1]);
          break;
        case 0x62:
          sprint_xrl_to_direct (str, IR[1]);
          break;
        case 0x72:
          sprint_orl_bit (str, IR[1]);
          break;
        case 0x82:
          sprint_anl_bit (str, IR[1]);
          break;
        case 0x92:
          sprint_mov_to_bit (str, IR[1]);
          break;
        case 0xA2:
          sprint_mov_bit (str, IR[1]);
          break;
        case 0xB2:
          sprint_cpl_bit (str, IR[1]);
          break;
        case 0xC2:
          sprint_clr_bit (str, IR[1]);
          break;
        case 0xD2:
          sprint_setb_bit (str, IR[1]);
          break;
        case 0x03:
          sprint_rr (str);
          break;
        case 0x13:
          sprint_rrc (str);
          break;
        case 0x23:
          sprint_rl (str);
          break;
        case 0x33:
          sprint_rlc (str);
          break;
        case 0x43:
          sprint_orl_data_to_direct (str, IR[1], IR[2]);
          break;
        case 0x53:
          sprint_anl_data_to_direct (str, IR[1], IR[2]);
          break;
        case 0x63:
          sprint_xrl_data_to_direct (str, IR[1], IR[2]);
          break;
        case 0x73:
          sprint_jmp_DPTR (str);
          break;
        case 0x83:
          sprint_movc_PC (str);
          break;
        case 0x93:
          sprint_movc_DPTR (str);
          break;
        case 0xA3:
          sprint_inc_DPTR (str);
          break;
        case 0xB3:
          sprint_cpl_C (str);
          break;
        case 0xC3:
          sprint_clr_C (str);
          break;
        case 0xD3:
          sprint_setb_C (str);
          break;
        case 0x04:
          sprint_inc (str);
          break;
        case 0x14:
          sprint_dec (str);
          break;
        case 0x24:
          sprint_add_data (str, IR[1]);
          break;
        case 0x34:
          sprint_addc_data (str, IR[1]);
          break;
        case 0x44:
          sprint_orl_data (str, IR[1]);
          break;
        case 0x54:
          sprint_anl_data (str, IR[1]);
          break;
        case 0x64:
          sprint_xrl_data (str, IR[1]);
          break;
        case 0x74:
          sprint_mov_data (str, IR[1]);
          break;
        case 0x84:
          sprint_div (str);
          break;
        case 0x94:
          sprint_subb_data (str, IR[1]);
          break;
        case 0xA4:
          sprint_mul (str);
          break;
        case 0xB4:
          sprint_cjne_data (str, IR[1], IR[2]);
          break;
        case 0xC4:
          sprint_swap (str);
          break;
        case 0xD4:
          sprint_da (str);
          break;
        case 0xE4:
          sprint_clr (str);
          break;
        case 0xF4:
          sprint_cpl (str);
          break;

        case 0x05:
          sprint_inc_direct (str, IR[1]);
          break;
        case 0x15:
          sprint_dec_direct (str, IR[1]);
          break;
        case 0x25:
          sprint_add_direct (str, IR[1]);
          break;
        case 0x35:
          sprint_addc_direct (str, IR[1]);
          break;
        case 0x45:
          sprint_orl_direct (str, IR[1]);
          break;
        case 0x55:
          sprint_anl_direct (str, IR[1]);
          break;
        case 0x65:
          sprint_xrl_direct (str, IR[1]);
          break;
        case 0x75:
          sprint_mov_data_to_direct (str, IR[1], IR[2]);
          break;
        case 0x85:
          sprint_mov_direct_to_direct (str, IR[1], IR[2]);
          break;
        case 0x95:
          sprint_subb_direct (str, IR[1]);
          break;
        case 0xB5:
          sprint_cjne_direct (str, IR[1], IR[2]);
          break;
        case 0xC5:
          sprint_xch_direct (str, IR[1]);
          break;
        case 0xD5:
          sprint_djnz_direct (str, IR[1], IR[2]);
          break;
        case 0xE5:
          sprint_mov_direct (str, IR[1]);
          break;
        case 0xF5:
          sprint_mov_to_direct (str, IR[1]);
          break;
        }
    }
}

void print_opcode (uint8_t *IR)
{
  fprint_opcode (stdout, IR);
}

void print_op (uint8_t *IR, int32_t current_PC)
{
  fprint_op (stdout, IR, current_PC);
}
