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

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "libhexbin.h"

static void fprint_line (FILE *stream, uint16_t nb, int i, uint8_t *line)
{
  int j;
  uint8_t ck = 0;
  fprintf (stream, ":%02X%04X00", i, nb);
  ck -= (uint8_t) i;
  ck -= nb & 0xFF;
  ck -= (nb >> 8) & 0xFF;
  for (j = 0; j < i; j++)
    {
      ck -= line[j];
      fprintf (stream, "%02X", line[j]);
    }
  fprintf (stream, "%02X\n", ck);
}

static uint8_t get_nibble (char c)
{
  uint8_t nibble;
  if (('0' <= c) && (c <= '9'))
    nibble = c - '0';
  else if (('A' <= c) && (c <= 'F'))
    nibble = 10 + (c - 'A');
  else
    assert (0);
  return nibble;
}

static uint8_t get_byte (char *buf)
{
  return (get_nibble (buf[0]) << 4) + (get_nibble (buf[1]));
}

size_t read_hex (uint8_t *code, FILE *stream)
{
  size_t i, nb = 0;
  char buffer[80];

  for (i = 0; i < MAX_LEN; i++)
    code[i] = 0;

  while (fgets (buffer, 80, stream))
    {
      size_t len;
      uint16_t address;

      assert (buffer[0] == ':');
      len = get_byte (buffer+1);
      address = (get_byte (buffer+3) << 8) + get_byte (buffer+5);
      assert (buffer[7] == '0');
      assert (buffer[8] == '0' || buffer[8] == '1');

      for (i = 0; i < len; i++)
        code[address+i] = get_byte (buffer+9+2*i);
      nb += len;
    }

  return nb;
}

size_t write_hex (uint8_t *code, FILE *stream)
{
  size_t i, nb;
  size_t last = 0;

  for (i = 0; i < MAX_LEN; i++)
    {
      if (code[i] != 0)
        last = i;
    }

  for (nb = 0; nb < last; nb += 16)
    fprint_line (stream, nb, 16, code+nb);
  nb -= 16;

  if (i)
    fprint_line (stream, nb, last - nb + 1, code+nb);

  fprintf (stream, ":00000001FF\n");

  return last;
}

size_t read_bin (uint8_t *code, FILE *stream)
{
  size_t nb;
  size_t i;

  for (i = 0; i < MAX_LEN; i++)
    {
      int c;
      c = fgetc (stream);
      if (c == EOF)
        break;
      code[i] = (uint8_t) c;
    }
  nb = i;

  for (; i < MAX_LEN; i++)
    code[i] = 0;

  return nb;
}

size_t write_bin (uint8_t *code, FILE *stream)
{
  size_t i;
  size_t last = 0;

  for (i = 0; i < MAX_LEN; i++)
    {
      if (code[i] != 0)
        last = i;
    }

  for (i = 0; i <= last; i++)
    fputc (code[i], stream);

  return i;
}
