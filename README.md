VM8051
======

Virtual Machine for 8051.

This program is intentended to simulate and study the
execution of a 8051 micro-controller.

VM8051 provides:

 - a program called `vm8051` which executes a 8051
   virtual machine on a hex format file;

 - a library called `lib8051` which allows simulate a 8051 in software.


Installation: `make && make install`
 - You should run make install as root in order to install the
   program and library in `/usr/local`.
 - You may specify another
   installation program by assigning the environment variable `PREFIX`:

`PREFIX="/my/own/path" make install`
              

Usage: `vm8051 [-m] input.hex`

runs vm8051 on the code provided in `input` in an interactive mode.

`-m`     only show the minimal interface
