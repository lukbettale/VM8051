/* Copyright (C) 2014, 2015, 2016 Luk Bettale

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
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <vm/lib8051.h>
#include <vm/lib8051coprocessors.h>
#include <copros/copro_RNG.h>
#include <print/lib8051print.h>
#include <utils/libhexbin.h>

/* simulate global variables for a struct vm8051 *vm */
#include <vm/lib8051globals.h>

unsigned int inbuf_idx = 0;
unsigned int inbuf_len = 0;
char inbuf[1024];
unsigned int outbuf_len = 0;
char outbuf[1024];

/* convert hexadecimal char to int */
static int dhx (char c)
{
  if ('0' <= c && c <= '9')
    return (c - '0');
  if ('a' <= c && c <= 'f')
    return 10 + (c - 'a');
  if ('A' <= c && c <= 'F')
    return 10 + (c - 'A');
  return -1;
}

static void dump8051_data (struct vm8051 *vm)
{
  printf ("\nIDATA:\n");
  for (int i = 0; i < 256; i++)
    {
      if ((i & 0x0F) == 0x00)
        printf ("  0x%02X: ", i);
      printf ("%02X ", _data[i]);
      if ((i & 0x0F) == 0x0F)
        printf ("\n");
    }
}

static void dump8051_sfr (struct vm8051 *vm)
{
  printf ("\nSFR:\n");
  for (int i = 0; i < 128; i++)
    {
      if ((i & 0x0F) == 0x00)
        printf ("  0x%02X: ", 0x80 + i);
      printf ("%02X ", _sfr[i]);
      if ((i & 0x0F) == 0x0F)
        printf ("\n");
    }
}

static void dump8051_xdata (struct vm8051 *vm, unsigned int page)
{
  printf ("XDATA page %d (0x%04X - 0x%04X):\n", page,
          512 * page, 512 * (page + 1) - 1);
  for (unsigned int i = 512 * page; i < 512 * (page + 1); i++)
    {
      if ((i & 0x0F) == 0x00)
        printf ("0x%04X: ", i);
      printf ("%02X ", _xdata[i]);
      if ((i & 0x0F) == 0x0F)
        printf ("\n");
    }
}

static void dump8051 (struct vm8051 *vm, int minimal)
{
  uint8_t next_IR[4];

  printf ("Regs");
  if (!minimal)
    printf ("                                        Ports");
  printf ("\n");
  printf ("R0   : 0x%02X    %x%x%x%x%x%x%x%xb", regs[0],
          (regs[0]&0x80)>>7, (regs[0]&0x40)>>6,
          (regs[0]&0x20)>>5, (regs[0]&0x10)>>4,
          (regs[0]&0x08)>>3, (regs[0]&0x04)>>2,
          (regs[0]&0x02)>>1, (regs[0]&0x01)>>0);
  if (isprint (regs[0]))
    printf ("    '%c'", regs[0]);
  if (!minimal)
    {
      if (!isprint (regs[0]))
        printf ("       ");
      printf ("             ");
      printf ("P0   : 0x%02X    %x%x%x%x%x%x%x%xb", P0,
              (P0&0x80)>>7, (P0&0x40)>>6, (P0&0x20)>>5, (P0&0x10)>>4,
              (P0&0x08)>>3, (P0&0x04)>>2, (P0&0x02)>>1, (P0&0x01)>>0);
      if (isprint (P0))
        printf ("    '%c'", P0);
    }
  printf ("\n");
  printf ("R1   : 0x%02X    %x%x%x%x%x%x%x%xb", regs[1],
          (regs[1]&0x80)>>7, (regs[1]&0x40)>>6,
          (regs[1]&0x20)>>5, (regs[1]&0x10)>>4,
          (regs[1]&0x08)>>3, (regs[1]&0x04)>>2,
          (regs[1]&0x02)>>1, (regs[1]&0x01)>>0);
  if (isprint (regs[1]))
    printf ("    '%c'", regs[1]);
  if (!minimal)
    {
      if (!isprint (regs[1]))
        printf ("       ");
      printf ("             ");
      printf ("P1   : 0x%02X    %x%x%x%x%x%x%x%xb", P1,
              (P1&0x80)>>7, (P1&0x40)>>6, (P1&0x20)>>5, (P1&0x10)>>4,
              (P1&0x08)>>3, (P1&0x04)>>2, (P1&0x02)>>1, (P1&0x01)>>0);
      if (isprint (P1))
        printf ("    '%c'", P1);
    }
  printf ("\n");
  printf ("R2   : 0x%02X    %x%x%x%x%x%x%x%xb", regs[2],
          (regs[2]&0x80)>>7, (regs[2]&0x40)>>6,
          (regs[2]&0x20)>>5, (regs[2]&0x10)>>4,
          (regs[2]&0x08)>>3, (regs[2]&0x04)>>2,
          (regs[2]&0x02)>>1, (regs[2]&0x01)>>0);
  if (isprint (regs[2]))
    printf ("    '%c'", regs[2]);
  if (!minimal)
    {
      if (!isprint (regs[2]))
        printf ("       ");
      printf ("             ");
      printf ("P2   : 0x%02X    %x%x%x%x%x%x%x%xb", P2,
              (P2&0x80)>>7, (P2&0x40)>>6, (P2&0x20)>>5, (P2&0x10)>>4,
              (P2&0x08)>>3, (P2&0x04)>>2, (P2&0x02)>>1, (P2&0x01)>>0);
      if (isprint (P2))
        printf ("    '%c'", P2);
    }
  printf ("\n");
  printf ("R3   : 0x%02X    %x%x%x%x%x%x%x%xb", regs[3],
          (regs[3]&0x80)>>7, (regs[3]&0x40)>>6,
          (regs[3]&0x20)>>5, (regs[3]&0x10)>>4,
          (regs[3]&0x08)>>3, (regs[3]&0x04)>>2,
          (regs[3]&0x02)>>1, (regs[3]&0x01)>>0);
  if (isprint (regs[3]))
    printf ("    '%c'", regs[3]);
  if (!minimal)
    {
      if (!isprint (regs[3]))
        printf ("       ");
      printf ("             ");
      printf ("P3   : 0x%02X    %x%x%x%x%x%x%x%xb", P3,
              (P3&0x80)>>7, (P3&0x40)>>6, (P3&0x20)>>5, (P3&0x10)>>4,
              (P3&0x08)>>3, (P3&0x04)>>2, (P3&0x02)>>1, (P3&0x01)>>0);
      if (isprint (P3))
        printf ("    '%c'", P3);
    }
  printf ("\n");

  printf ("R4   : 0x%02X    %x%x%x%x%x%x%x%xb", regs[4],
          (regs[4]&0x80)>>7, (regs[4]&0x40)>>6,
          (regs[4]&0x20)>>5, (regs[4]&0x10)>>4,
          (regs[4]&0x08)>>3, (regs[4]&0x04)>>2,
          (regs[4]&0x02)>>1, (regs[4]&0x01)>>0);
  if (isprint (regs[4]))
    printf ("    '%c'", regs[4]);
  printf ("\n");
  printf ("R5   : 0x%02X    %x%x%x%x%x%x%x%xb", regs[5],
          (regs[5]&0x80)>>7, (regs[5]&0x40)>>6,
          (regs[5]&0x20)>>5, (regs[5]&0x10)>>4,
          (regs[5]&0x08)>>3, (regs[5]&0x04)>>2,
          (regs[5]&0x02)>>1, (regs[5]&0x01)>>0);
  if (isprint (regs[5]))
    printf ("    '%c'", regs[5]);
  printf ("\n");
  printf ("R6   : 0x%02X    %x%x%x%x%x%x%x%xb", regs[6],
          (regs[6]&0x80)>>7, (regs[6]&0x40)>>6,
          (regs[6]&0x20)>>5, (regs[6]&0x10)>>4,
          (regs[6]&0x08)>>3, (regs[6]&0x04)>>2,
          (regs[6]&0x02)>>1, (regs[6]&0x01)>>0);
  if (isprint (regs[6]))
    printf ("    '%c'", regs[6]);
  printf ("\n");
  printf ("R7   : 0x%02X    %x%x%x%x%x%x%x%xb", regs[7],
          (regs[7]&0x80)>>7, (regs[7]&0x40)>>6,
          (regs[7]&0x20)>>5, (regs[7]&0x10)>>4,
          (regs[7]&0x08)>>3, (regs[7]&0x04)>>2,
          (regs[7]&0x02)>>1, (regs[7]&0x01)>>0);
  if (isprint (regs[7]))
    printf ("    '%c'", regs[7]);
  printf ("\n");
  printf ("\n");
  printf ("Sys");
  if (interrupted)
    printf (" (interrupted, %d)", interrupted & HIGH ? 1 : 0);
  printf ("\n");
  printf ("A    : 0x%02X    %x%x%x%x%x%x%x%xb", A,
          (A&0x80)>>7, (A&0x40)>>6, (A&0x20)>>5, (A&0x10)>>4,
          (A&0x08)>>3, (A&0x04)>>2, (A&0x02)>>1, (A&0x01)>>0);
  if (isprint (A))
    printf ("    '%c'", A);
  printf ("\n");
  printf ("B    : 0x%02X    %x%x%x%x%x%x%x%xb", B,
          (B&0x80)>>7, (B&0x40)>>6, (B&0x20)>>5, (B&0x10)>>4,
          (B&0x08)>>3, (B&0x04)>>2, (B&0x02)>>1, (B&0x01)>>0);
  if (isprint (B))
    printf ("    '%c'", B);
  printf ("\n");
  printf ("SP   : 0x%02X\n", SP);
  printf ("DPTR : 0x%04X\n", DPTR);
  printf ("PC   : 0x%04X  (", PC);
  inst8051 (vm, next_IR, PC);
  print_op (next_IR, PC);
  printf (" ; ");
  print_opcode (next_IR);
  printf (")\n");
  printf ("PSW  : 0x%02X    %s %s %s  RS = %d %s    %s\n", PSW,
          CY?" CY":"   ", AC?" AC":"   ", F0?" F0":"   ",
          RS, OV?" OV":"   ",  P?" P ":"   ");
  printf ("states: %u\n", cycles);
  printf ("\n");
  if (!minimal)
    {
      printf ("IE   : 0x%02X    %s %s %s %s %s %s %s %s\n", IE,
              EA?" EA":"   ", IE&0x40?" # ":"   ", IE&0x20?" # ":"   ",
              ES?" ES":"   ",
              ET1?"ET1":"   ", EX1?"EX1":"   ",
              ET0?"ET0":"   ", EX0?"EX0":"   ");
      printf ("IP   : 0x%02X    %s %s %s %s %s %s %s %s\n", IP,
              IP&0x40?" # ":"   ", IP&0x40?" # ":"   ", IP&0x20?" # ":"   ",
              PS?" PS":"   ",
              PT1?"PT1":"   ", PX1?"PX1":"   ",
              PT0?"PT0":"   ", PX0?"PX0":"   ");
      printf ("PCON : 0x%02X   %s %s %s %s %s %s %s %s\n", PCON,
              PCON&0x80?"SMOD":"    ", PCON&0x40?" # ":"   ",
              PCON&0x20?" # ":"   ", PCON&0x10?" # ":"   ",
              PCON&0x08?" # ":"   ", PCON&0x04?" # ":"   ",
              PCON&0x02?" # ":"   ", PCON&0x01?" # ":"   ");
      printf ("TCON : 0x%02X    %s %s %s %s %s %s %s %s\n", TCON,
              TF1?"TF1":"   ", TR1?"TR1":"   ",
              TF0?"TF0":"   ", TR0?"TR0":"   ",
              IE1?"IE1":"   ", IT1?"IT1":"   ",
              IE0?"IE0":"   ", IT0?"IT0":"   ");
      printf ("\n");
      printf ("SCON : 0x%02X    %s %s %s %s %s %s %s %s\n", SCON,
              SM0?"SM0":"   ", SM1?"SM1":"   ", SM2?"SM2":"   ",
              REN?"REN":"   ",
              TB8?"TB8":"   ", RB8?"RB8":"   ",
              TI?" TI":"   ", RI?" RI":"   ");
      printf ("SBUF : 0x%02X    %x%x%x%x%x%x%x%xb", SBUF,
              (SBUF&0x80)>>7, (SBUF&0x40)>>6, (SBUF&0x20)>>5, (SBUF&0x10)>>4,
              (SBUF&0x08)>>3, (SBUF&0x04)>>2, (SBUF&0x02)>>1, (SBUF&0x01)>>0);
      if (isprint (SBUF))
        printf ("    '%c'", SBUF);
      printf ("\n");
      printf ("\n");
      printf ("TMOD : 0x%02X\n", TMOD);
      if ((TMOD & 0x03) == 0x03)              /* split mode */
        {
          printf ("TL0  : 0x%02X    Timer0a %s: % 6d%c     8-bit %s\n",
                  TL0, TR0?"(on) ":"(off)",
                  TL0, TF0?'*':' ',
                  (TMOD & 0x04) ? "counter" : "timer");
          printf ("TH0  : 0x%02X    Timer0b %s: % 6d%c     8-bit %s\n",
                  TH0, TR1?"(on) ":"(off)",
                  TH0, TF1?'*':' ',
                  "timer");
          printf ("TL1  : 0x%02X    Timer1  %s: % 6d%c    %s %s\n",
                  TL1, (TMOD & 0x030) != 0x30?"(on) ":"(off)",
                  get_timer1 (vm), ' ',
                  (TMOD & 0x30) == 0x00 ? "13-bit" :
                  (TMOD & 0x30) == 0x10 ? "16-bit" :
                  (TMOD & 0x30) == 0x20 ? " 8-bit auto-reload" :
                  (TMOD & 0x30) == 0x30 ? "stopped" : "",
                  "timer");
          printf ("TH1  : 0x%02X                      \n", TH1);
        }
      else
        {
          printf ("TL0  : 0x%02X    Timer0  %s: % 6d%c    %s %s\n",
                  TL0, TR0?"(on) ":"(off)",
                  get_timer0 (vm), TF0?'*':' ',
                  (TMOD & 0x03) == 0x00 ? "13-bit" :
                  (TMOD & 0x03) == 0x01 ? "16-bit" :
                  (TMOD & 0x03) == 0x02 ? " 8-bit auto-reload" : "",
                  (TMOD & 0x04) ? "counter" : "timer");
          printf ("TH0  : 0x%02X                      \n", TH0);
          printf ("TL1  : 0x%02X    Timer1  %s: % 6d%c    %s %s\n",
                  TL1, TR1?"(on) ":"(off)",
                  get_timer1 (vm), TF1?'*':' ',
                  (TMOD & 0x30) == 0x00 ? "13-bit" :
                  (TMOD & 0x30) == 0x10 ? "16-bit" :
                  (TMOD & 0x30) == 0x20 ? " 8-bit auto-reload" :
                  (TMOD & 0x30) == 0x30 ? "stopped" : "",
                  (TMOD & 0x40) ? "counter" : "timer");
          printf ("TH1  : 0x%02X                      \n", TH1);
        }
      printf ("\n");
    }
#ifndef PURE_8051
  print_coprocessors (vm);
#endif
  printf ("----------------------------------------"
          "----------------------------------------\n");
  for (unsigned int i = 0; i < outbuf_len; i++)
    printf ("%c", outbuf[i]);
  printf ("\n");
  printf ("----------------------------------------"
          "----------------------------------------\n");
}

static int array_contains (int len, unsigned int *array, unsigned int elt)
{
  for (int i = 0; i < len; i++)
    {
      if (array[i] == elt)
        return 1;
    }
  return 0;
}

static int array_remove (int len, unsigned int *array, unsigned int elt)
{
  for (int i = 0; i < len; i++)
    {
      if (array[i] == elt)
        {
          array[i] = -1;
          return i;
        }
    }
  return -1;
}

static void wrap_operate8051 (struct vm8051 *vm)
{
  int prev_TI;

  if (REN && !RI && inbuf_idx < inbuf_len)
    {
      SBUF = inbuf[inbuf_idx++];
      SCON |= RI_MASK;
      if (inbuf_idx == inbuf_len)
        {
          printf ("reset here\n");
          inbuf_idx = 0;
          inbuf_len = 0;
          inbuf[0] = 0;
        }
    }

  prev_TI = TI;
  operate8051 (vm);

  if (prev_TI == 0 && TI == 1)
    outbuf[outbuf_len++] = SBUF;
}

static void run8051 (struct vm8051 *vm, int minimal)
{
  int i;
  char info[80] = "";
  int ind_stack[256];
  unsigned int breakpoints[256];
  unsigned int nbp = 0;
  int command = 0;
  int end = 0;

  for (i = 0; i < 256; i++)
    {
      ind_stack[i] = i;
      breakpoints[i] = -1;
    }

  while (!end)
    {
      int ret = -1;
      int value = -1;
      unsigned int address = -1;
      unsigned int ncy = 0;
      char opcode[6];
      uint8_t next_IR[4];

      if (command != 'i' && command != 'x' && command != 'f')
        {
          dump8051 (vm, minimal);
        }
      printf ("%s\n> ", info);
      info[0] = 0;

      command = fgetc (stdin);
      switch (command)
        {
        case 's':
        case '\n':
          /* step instruction */
          fetch8051 (vm);
          wrap_operate8051 (vm);
          break;
        case EOF:
          printf ("\n");
        case 'q':
          /* quit */
          end = 1;
        case 'p':
          /* do nothing (re-print only) */
          break;
        case 'r':
          /* reset VM */
          inbuf_idx = 0;
          inbuf_len = 0;
          outbuf_len = 0;
          reset8051 (vm);
          sprintf (info, "vm reset");
          break;
        case 'z':
          /* reset states to zero */
          cycles = 0;
          break;
        case 'b':
          /* add breakpoint */
          ret = scanf ("%x", &address);
          if (ret != 1)
            {
              sprintf (info, "%c: hexadecimal address required", command);
              break;
            }
          if (!array_contains (256, breakpoints, address))
            {
              breakpoints[ind_stack[nbp++]] = address;
              sprintf (info, "new breakpoint: 0x%04X", address);
            }
          break;
        case 'd':
          /* delete breakpoint */
          ret = scanf ("%x", &address);
          if (ret != 1)
            {
              sprintf (info, "%c: hexadecimal address required", command);
              break;
            }
          i = array_remove (256, breakpoints, address);
          if (i > 0)
            {
              ind_stack[--nbp] = i;
              sprintf (info, "breakpoint removed: 0x%04X", address);
            }
          break;
        case 'j':
          /* jump to address (goto) */
          ret = scanf ("%x", &address);
          if (ret != 1)
            {
              sprintf (info, "%c: hexadecimal address required", command);
              break;
            }
          PC = (uint16_t) address;
          sprintf (info, "PC set to 0x%04X", address);
          break;
        case 'k':
          /* skip instruction */
          fetch8051 (vm);
          sprintf (info, "instruction skipped");
          break;
        case 'n':
          /* run to next line (do not step into function) */
          address = PC + inst8051 (vm, next_IR, PC);
          do
            {
              fetch8051 (vm);
              wrap_operate8051 (vm);
            }
          while (PC != address && !array_contains (256, breakpoints, PC));
          if (PC == address)
            sprintf (info, "next line");
          else
            sprintf (info, "breakpoint reached: 0x%04X", PC);
          break;
        case 'g':
          /* run until address is reached */
          ret = scanf ("%x", &address);
          if (ret != 1)
            {
              sprintf (info, "%c: hexadecimal address required", command);
              break;
            }
        case 'c':
          /* continue execution */
          do
            {
              fetch8051 (vm);
              wrap_operate8051 (vm);
            }
          while (PC != address && !array_contains (256, breakpoints, PC));
          if (PC == address)
            sprintf (info, "run to 0x%04X", address);
          else
            sprintf (info, "breakpoint reached: 0x%04X", PC);
          break;
        case 'w':
          /* wait the given amount of cycles */
          ret = scanf ("%u", &ncy);
          if (ret != 1)
            {
              sprintf (info, "%c: unsigned decimal value required", command);
              break;
            }
          sprintf (info, "%u cycles ellapsed", ncy);
          ncy += cycles;
          do
            {
              fetch8051 (vm);
              wrap_operate8051 (vm);
            }
          while (cycles < ncy && !array_contains (256, breakpoints, PC));
          if (cycles < ncy)
            sprintf (info, "breakpoint reached: 0x%04X", PC);
          break;
        case 'e':
          /* inject an opcode */
          scanf ("%s", opcode);
          ret = sscanf (opcode, "%6[0-9a-fA-F]", opcode);
          if (ret != 1 || strlen (opcode) & 1)
            {
              sprintf (info, "%c: valid opcode required", command);
              break;
            }
          IR[0] = (dhx (opcode[0]) << 4) | dhx (opcode[1]);
          IR[1] = (dhx (opcode[2]) << 4) | dhx (opcode[3]);
          IR[2] = (dhx (opcode[4]) << 4) | dhx (opcode[5]);
          IR[3] = strlen (opcode) >> 1;
          sprintf (info, "instruction injected: ");
          sprint_op (info + strlen (info), IR, PC-IR[3]);
          wrap_operate8051 (vm);
          break;
        case '?':
          /* send data (serial port) */
          scanf ("%1024[^\n]", inbuf + inbuf_len);
          inbuf_len += strlen (inbuf + inbuf_len);
          sprintf (info, "string bufferized");
          break;
        case '!':
          /* flush received data */
          outbuf_len = 0;
          sprintf (info, "received data cleared");
          break;
        case 'P':
          /* change value of port P */
          ret = scanf ("%u %i", &address, &value);
          if (ret != 2)
            {
              sprintf (info, "%c: invalid arguments", command);
              break;
            }
          if (address >= 4)
            {
              sprintf (info, "%c: invalid port number %u", command, address);
              break;
            }
          _sfr[address << 4] = value;
          sprintf (info, "Port P%d affected to 0x%02X",
                   address, _sfr[address << 4]);
          break;
        case 'i':
          /* print contents of idata */
          dump8051_data (vm);
          break;
        case 'f':
          /* print contents of SFRs */
          dump8051_sfr (vm);
          break;
        case 'x':
          /* print contents of xdata (specified page) */
          ret = scanf ("%u", &ncy);
          if (ret != 1)
            {
              sprintf (info, "%c: unsigned decimal value required", command);
              command = 'p';    /* force state printing next */
              break;
            }
          if (ncy >= 128)
            {
              sprintf (info, "%c: page out of bounds", command);
              command = 'p';    /* force state printing next */
              break;
            }
          dump8051_xdata (vm, ncy);
          break;
        default:
          sprintf (info, "invalid command");
        }
      if (command != '\n' && command != EOF)
        while (fgetc (stdin) != '\n');
      if (end)
        break;
    }
  dump8051 (vm, minimal);
}

int main (int argc, char *argv[])
{
  int minimal = 0;
  struct vm8051 *vm;
  FILE *program;

  if (argc > 1)
    {
      if (strcmp (argv[1], "-m") == 0)
        {
          minimal = 1;
          argc--;
          argv++;
        }
    }
  if (argc < 2)
    {
      fprintf (stderr, "Usage: %s [-m] input\n", argv[0]);
      return -1;
    }
  vm = malloc (sizeof (struct vm8051));
  assert (vm != NULL);
  vm->coprocessors = NULL;

#ifndef PURE_8051
  add_copro_RNG (vm);
#endif

  program = fopen (argv[1], "rb");
  if (program != NULL && read_hex (_code, program) > 0)
    {
      reset8051 (vm);
      run8051 (vm, minimal);
      fclose (program);
    }
  else
    fprintf (stderr, "%s: empty program\n", argv[1]);

#ifndef PURE_8051
  free_coprocessors (vm);
#endif

  free (vm);
  return 0;
}
