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

#ifndef LIB8051PRINT_H
#define LIB8051PRINT_H

#include <stdint.h>
#include <stdio.h>

#ifndef OLD
#define FORMAT_BYTE "0x%02X"
#define FORMAT_WORD "0x%04X"
#else
#define FORMAT_BYTE "0%02Xh"
#define FORMAT_WORD "0%04Xh"
#endif

void fprint_op (FILE *stream, uint8_t *IR, int32_t PC);
void fprint_opcode (FILE *stream, uint8_t *IR);

void sprint_op (char *str, uint8_t *IR, int32_t PC);
void sprint_opcode (char *str, uint8_t *IR);

void print_op (uint8_t *IR, int32_t PC);
void print_opcode (uint8_t *IR);

#endif  /* LIB8051PRINT_H */
