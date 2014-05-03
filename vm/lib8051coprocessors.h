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

#ifndef LIB8051COPROCESSORS_H
#define LIB8051COPROCESSORS_H

#include <stdio.h>
#include <stdint.h>

#include "lib8051.h"

extern void add_coprocessor (struct vm8051 *vm,
                             void *contents, unsigned int index);
extern void operate_coprocessors (struct vm8051 *vm);
extern void free_coprocessors (struct vm8051 *vm);

#endif  /* LIB8051COPROCESSORS_H */
