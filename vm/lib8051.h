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

#ifndef LIB8051_H
#define LIB8051_H

#include <stdio.h>
#include <stdint.h>

#include "lib8051defs.h"

extern int init8051 (struct vm8051 *vm, char *progname);
extern size_t inst8051 (struct vm8051 *vm, uint8_t *inst, uint16_t addr);
extern void reset8051 (struct vm8051 *vm);
extern void fetch8051 (struct vm8051 *vm);
extern void operate8051 (struct vm8051 *vm);
extern void sim8051 (struct vm8051 *vm, uint16_t address, unsigned int ncy);

extern int32_t get_timer0 (struct vm8051 *vm);
extern int32_t get_timer1 (struct vm8051 *vm);

/* add A, Rn              1       1 */
extern void inst_add_Rn (struct vm8051 *vm, unsigned char n);
/* add A, direct          2       1 */
extern void inst_add_direct (struct vm8051 *vm, uint8_t direct);
/* add A, @Ri             1       1 */
extern void inst_add_atRi (struct vm8051 *vm, unsigned char i);
/* add A, #data           2       1 */
extern void inst_add_data (struct vm8051 *vm, uint8_t data);

/* addc A, Rn             1       1 */
extern void inst_addc_Rn (struct vm8051 *vm, unsigned char n);
/* addc A, direct         2       1 */
extern void inst_addc_direct (struct vm8051 *vm, uint8_t direct);
/* addc A, @Ri            1       1 */
extern void inst_addc_atRi (struct vm8051 *vm, unsigned char i);
/* addc A, #data          2       1 */
extern void inst_addc_data (struct vm8051 *vm, uint8_t data);

/* subb A, Rn             1       1 */
extern void inst_subb_Rn (struct vm8051 *vm, unsigned char n);
/* subb A, direct         2       1 */
extern void inst_subb_direct (struct vm8051 *vm, uint8_t direct);
/* subb A, @Ri            1       1 */
extern void inst_subb_atRi (struct vm8051 *vm, unsigned char i);
/* subb A, #data          2       1 */
extern void inst_subb_data (struct vm8051 *vm, uint8_t data);

/* inc A                  1       1 */
extern void inst_inc (struct vm8051 *vm);
/* inc Rn                 1       1 */
extern void inst_inc_Rn (struct vm8051 *vm, unsigned char n);
/* inc direct             2       1 */
extern void inst_inc_direct (struct vm8051 *vm, uint8_t direct);
/* inc @Ri                1       1 */
extern void inst_inc_atRi (struct vm8051 *vm, unsigned char i);

/* dec A                  1       1 */
extern void inst_dec (struct vm8051 *vm);
/* dec Rn                 1       1 */
extern void inst_dec_Rn (struct vm8051 *vm, unsigned char n);
/* dec direct             2       1 */
extern void inst_dec_direct (struct vm8051 *vm, uint8_t direct);
/* dec @Ri                1       1 */
extern void inst_dec_atRi (struct vm8051 *vm, unsigned char i);

/* inc DPTR               1       2 */
extern void inst_inc_DPTR (struct vm8051 *vm);

/* anl A, Rn              1       1 */
extern void inst_anl_Rn (struct vm8051 *vm, unsigned char n);
/* anl A, direct          2       1 */
extern void inst_anl_direct (struct vm8051 *vm, uint8_t direct);
/* anl A, @Ri             1       1 */
extern void inst_anl_atRi (struct vm8051 *vm, unsigned char i);
/* anl A, #data           2       1 */
extern void inst_anl_data (struct vm8051 *vm, uint8_t data);

/* anl direct, A          2       1 */
extern void inst_anl_to_direct (struct vm8051 *vm, uint8_t direct);
/* anl direct, #data      3       2 */
extern void inst_anl_data_to_direct (struct vm8051 *vm, uint8_t direct, uint8_t data);

/* orl A, Rn              1       1 */
extern void inst_orl_Rn (struct vm8051 *vm, unsigned char n);
/* orl A, direct          2       1 */
extern void inst_orl_direct (struct vm8051 *vm, uint8_t direct);
/* orl A, @Ri             1       1 */
extern void inst_orl_atRi (struct vm8051 *vm, unsigned char i);
/* orl A, #data           2       1 */
extern void inst_orl_data (struct vm8051 *vm, uint8_t data);

/* orl direct, A          2       1 */
extern void inst_orl_to_direct (struct vm8051 *vm, uint8_t direct);
/* orl direct, #data      3       2 */
extern void inst_orl_data_to_direct (struct vm8051 *vm, uint8_t direct, uint8_t data);

/* xrl A, Rn              1       1 */
extern void inst_xrl_Rn (struct vm8051 *vm, unsigned char n);
/* xrl A, direct          2       1 */
extern void inst_xrl_direct (struct vm8051 *vm, uint8_t direct);
/* xrl A, @Ri             1       1 */
extern void inst_xrl_atRi (struct vm8051 *vm, unsigned char i);
/* xrl A, #data           2       1 */
extern void inst_xrl_data (struct vm8051 *vm, uint8_t data);

/* xrl direct, A          2       1 */
extern void inst_xrl_to_direct (struct vm8051 *vm, uint8_t direct);
/* xrl direct, #data      3       2 */
extern void inst_xrl_data_to_direct (struct vm8051 *vm, uint8_t direct, uint8_t data);

/* clr A                  1       1 */
extern void inst_clr (struct vm8051 *vm);
/* cpl A                  1       1 */
extern void inst_cpl (struct vm8051 *vm);

/* rl A                   1       1 */
extern void inst_rl (struct vm8051 *vm);
/* rlc A                  1       1 */
extern void inst_rlc (struct vm8051 *vm);
/* rr A                   1       1 */
extern void inst_rr (struct vm8051 *vm);
/* rrc A                  1       1 */
extern void inst_rrc (struct vm8051 *vm);

/* swap A                 1       1 */
extern void inst_swap (struct vm8051 *vm);

/* mul AB                 1       4 */
extern void inst_mul (struct vm8051 *vm);
/* div AB                 1       4 */
extern void inst_div (struct vm8051 *vm);
/* da A                   1       1 */
extern void inst_da (struct vm8051 *vm);

/* mov A, Rn              1       1 */
extern void inst_mov_Rn (struct vm8051 *vm, unsigned char n);
/* mov A, direct          2       1 */
extern void inst_mov_direct (struct vm8051 *vm, uint8_t direct);
/* mov A, @Ri             1       1 */
extern void inst_mov_atRi (struct vm8051 *vm, unsigned char i);
/* mov A, #data           2       1 */
extern void inst_mov_data (struct vm8051 *vm, uint8_t data);

/* mov Rn, A              1       1 */
extern void inst_mov_to_Rn (struct vm8051 *vm, unsigned char n);
/* mov Rn, direct         2       2 */
extern void inst_mov_direct_to_Rn (struct vm8051 *vm, unsigned char n, uint8_t direct);
/* mov Rn, #data          2       1 */
extern void inst_mov_data_to_Rn (struct vm8051 *vm, unsigned char n, uint8_t data);

/* mov direct, A          2       1 */
extern void inst_mov_to_direct (struct vm8051 *vm, uint8_t direct);
/* mov direct, Rn         2       2 */
extern void inst_mov_Rn_to_direct (struct vm8051 *vm, unsigned char n, uint8_t direct);
/* mov direct, direct     3       2 */
extern void inst_mov_direct_to_direct (struct vm8051 *vm, uint8_t direct_src, uint8_t direct);
/* mov direct, @Ri        2       2 */
extern void inst_mov_atRi_to_direct (struct vm8051 *vm, unsigned char i, uint8_t direct);
/* mov direct, #data      3       2 */
extern void inst_mov_data_to_direct (struct vm8051 *vm, uint8_t direct, uint8_t data);

/* mov @Ri, A             1       1 */
extern void inst_mov_to_atRi (struct vm8051 *vm, unsigned char i);
/* mov @Ri, direct        2       2 */
extern void inst_mov_direct_to_atRi (struct vm8051 *vm, unsigned char i, uint8_t direct);
/* mov @Ri, #data         2       1 */
extern void inst_mov_data_to_atRi (struct vm8051 *vm, unsigned char i, uint8_t data);

/* mov DPTR, #data16      3       2 */
extern void inst_mov_to_DPTR (struct vm8051 *vm, uint8_t data16_high, uint8_t data16_low);

/* movc A, @A+DPTR        1       2 */
extern void inst_movc_DPTR (struct vm8051 *vm);
/* movc A, @A+PC          1       2 */
extern void inst_movc_PC (struct vm8051 *vm);

/* movx A, @Ri            1       2 */
extern void inst_movx_atRi (struct vm8051 *vm, unsigned char i);
/* movx A, @DPTR          1       2 */
extern void inst_movx_atDPTR (struct vm8051 *vm);
/* movx @Ri, A            1       2 */
extern void inst_movx_to_atRi (struct vm8051 *vm, unsigned char i);
/* movx @DPTR, A          1       2 */
extern void inst_movx_to_atDPTR (struct vm8051 *vm);

/* clr C                  1       1 */
extern void inst_clr_C (struct vm8051 *vm);
/* clr bit                2       1 */
extern void inst_clr_bit (struct vm8051 *vm, uint8_t bit);

/* setb C                 1       1 */
extern void inst_setb_C (struct vm8051 *vm);
/* setb bit               2       1 */
extern void inst_setb_bit (struct vm8051 *vm, uint8_t bit);

/* cpl C                  1       1 */
extern void inst_cpl_C (struct vm8051 *vm);
/* cpl bit                2       1 */
extern void inst_cpl_bit (struct vm8051 *vm, uint8_t bit);

/* anl C, bit             2       2 */
extern void inst_anl_bit (struct vm8051 *vm, uint8_t bit);
/* anl C, /bit            2       2 */
extern void inst_anl_not_bit (struct vm8051 *vm, uint8_t bit);

/* orl C, bit             2       2 */
extern void inst_orl_bit (struct vm8051 *vm, uint8_t bit);
/* orl C, /bit            2       2 */
extern void inst_orl_not_bit (struct vm8051 *vm, uint8_t bit);

/* mov C, bit             2       1 */
extern void inst_mov_bit (struct vm8051 *vm, uint8_t bit);
/* mov bit, C             2       2 */
extern void inst_mov_to_bit (struct vm8051 *vm, uint8_t bit);

/* xch A, Rr              1       1 */
extern void inst_xch_Rn (struct vm8051 *vm, unsigned char n);
/* xch A, direct          2       1 */
extern void inst_xch_direct (struct vm8051 *vm, uint8_t direct);
/* xch A, @Ri             1       1 */
extern void inst_xch_atRi (struct vm8051 *vm, unsigned char i);
/* xchd A, @Ri            1       1 */
extern void inst_xchd_atRi (struct vm8051 *vm, unsigned char i);

/* push direct            2       2 */
extern void inst_push (struct vm8051 *vm, uint8_t direct);
/* pop direct             2       2 */
extern void inst_pop (struct vm8051 *vm, uint8_t direct);

/* jz rel                 2       2 */
extern void inst_jz (struct vm8051 *vm, uint8_t rel);
/* jnz rel                2       2 */
extern void inst_jnz (struct vm8051 *vm, uint8_t rel);

/* jc rel                 2       2 */
extern void inst_jc (struct vm8051 *vm, uint8_t rel);
/* jnc rel                2       2 */
extern void inst_jnc (struct vm8051 *vm, uint8_t rel);

/* jb bit, rel            3       2 */
extern void inst_jb (struct vm8051 *vm, uint8_t bit, uint8_t rel);
/* jnb bit, rel           3       2 */
extern void inst_jnb (struct vm8051 *vm, uint8_t bit, uint8_t rel);
/* jbc bit, rel           3       2 */
extern void inst_jbc (struct vm8051 *vm, uint8_t bit, uint8_t rel);

/* cjne A, direct, rel    3       2 */
extern void inst_cjne_direct (struct vm8051 *vm, uint8_t direct, uint8_t rel);
/* cjne A, #data, rel     3       2 */
extern void inst_cjne_data (struct vm8051 *vm, uint8_t data, uint8_t rel);
/* cjne Rn, #data, rel    3       2 */
extern void inst_cjne_data_with_Rn (struct vm8051 *vm, unsigned char n, uint8_t data, uint8_t rel);
/* cjne @Ri, #data, rel   3       2 */
extern void inst_cjne_data_with_atRi (struct vm8051 *vm, unsigned char i, uint8_t data, uint8_t rel);

/* djnz Rn, rel           2       2 */
extern void inst_djnz_Rn (struct vm8051 *vm, unsigned char n, uint8_t rel);
/* djnz direct, rel       3       2 */
extern void inst_djnz_direct (struct vm8051 *vm, uint8_t direct, uint8_t rel);

/* ajmp addr11            2       2 */
extern void inst_ajmp (struct vm8051 *vm, unsigned char prefix, uint8_t addr11);
/* ljmp addr16            3       2 */
extern void inst_ljmp (struct vm8051 *vm, uint8_t addr16_high, uint8_t addr16_low);
/* sjmp rel               2       2 */
extern void inst_sjmp (struct vm8051 *vm, uint8_t rel);

/* jmp @A+DPTR            1       2 */
extern void inst_jmp_DPTR (struct vm8051 *vm);

/* nop                    1       1 */
extern void inst_nop (struct vm8051 *vm);

/* acall addr11           2       2 */
extern void inst_acall (struct vm8051 *vm, unsigned char prefix, uint8_t addr11);
/* lcall addr16           3       2 */
extern void inst_lcall (struct vm8051 *vm, uint8_t addr16_high, uint8_t addr16_low);

/* ret                    1       2 */
extern void inst_ret (struct vm8051 *vm);
/* reti                   1       2 */
extern void inst_reti (struct vm8051 *vm);

#endif  /* LIB8051_H */
