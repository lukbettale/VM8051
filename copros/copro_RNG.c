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

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <vm/lib8051.h>
#include <vm/lib8051coprocessors.h>

#include "copro_RNG.h"

#define RNG_INDEX 0

#define RNG_CTRL vm->_sfr[0xC0 ^ 0x80]
#define RNG_RND  vm->_sfr[0xC1 ^ 0x80]

#define RNG_CTRL_RUN_MASK  0x01
#define RNG_DURATION 10

struct copro_RNG
{
  uint32_t cycles_start;
};

void print_copro_RNG (struct vm8051 *vm, void *copro)
{
  struct copro_RNG *rng = copro;

  printf ("copro RNG:              \tstatus: ");
  if (!rng->cycles_start)
    printf ("available (0x%02X)\n", RNG_RND);
  else
    printf ("generating (%u cy left)\n",
            RNG_DURATION - (vm->cycles - rng->cycles_start));
  printf ("\n");

}

void operate_copro_RNG (struct vm8051 *vm, void *copro)
{
  struct copro_RNG *rng = copro;

  /* no software clear of RUN bit */
  if (rng->cycles_start)
    RNG_CTRL |= RNG_CTRL_RUN_MASK;

  /* launch operation */
  if ((RNG_CTRL & RNG_CTRL_RUN_MASK) && !rng->cycles_start)
    rng->cycles_start = vm->cycles;

  /* currently running */
  if (rng->cycles_start)
    {
      /* random generated */
      if (vm->cycles - rng->cycles_start >= RNG_DURATION)
        {
          RNG_RND = (uint8_t) rand ();
          rng->cycles_start = 0;
        }
    }

  /* always clear RUN bit if not running */
  if (!rng->cycles_start)
    RNG_CTRL &= ~RNG_CTRL_RUN_MASK;
}

void add_copro_RNG (struct vm8051 *vm)
{
  struct copro_RNG *rng;

  srand (time (NULL));

  rng = malloc (sizeof (struct copro_RNG));
  rng->cycles_start = 0;

  operate_copro_table[RNG_INDEX] = &operate_copro_RNG;
  print_copro_table[RNG_INDEX] = &print_copro_RNG;
  add_coprocessor (vm, rng, RNG_INDEX);
}
