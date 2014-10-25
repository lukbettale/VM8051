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
#include <stdlib.h>
#include <time.h>

#include <vm/lib8051.h>
#include <vm/lib8051coprocessors.h>

#include "copro_RNG.h"

#define RNG_INDEX 0

#define RNG_CTRL _sfr[0xC0 ^ 0x80]
#define RNG_RND  _sfr[0xC1 ^ 0x80]

#define RNG_DURATION 10

struct copro_RNG
{
  uint32_t cycles_start;
};

extern void (*operate_table[8]) (struct vm8051 *, void *);

static void operate_RNG (struct vm8051 *vm, void *copro)
{
  struct copro_RNG *rng = copro;

  if (vm->RNG_CTRL & 0x01 && !rng->cycles_start)
    rng->cycles_start = vm->cycles;

  if (vm->RNG_CTRL & 0x01 && vm->cycles - rng->cycles_start >= RNG_DURATION)
    {
      rng->cycles_start = 0;
      vm->RNG_CTRL &= ~0x01;
      vm->RNG_RND = (uint8_t) rand ();
    }
}

void add_copro_RNG (struct vm8051 *vm)
{
  struct copro_RNG *rng;

  srand (time (NULL));

  rng = malloc (sizeof (struct copro_RNG));
  rng->cycles_start = 0;

  operate_table[RNG_INDEX] = &operate_RNG;
  add_coprocessor (vm, rng, RNG_INDEX);
}
