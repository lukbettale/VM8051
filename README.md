VM8051
======

Virtual Machine for 8051.

This program is intentended to simulate and study the
execution of a 8051 micro-controller.

VM8051 provides:

 - a program called `vm8051` which executes a 8051
   virtual machine on a binary file;

 - a library called `lib8051` which allows simulate a 8051 in software.


Installation: `make && make install`
 - You should run make install as root in order to install the
   program and library in `/usr/local`.
 - You may specify another
   installation program by assigning the environment variable `PREFIX`:

`PREFIX="/my/own/path" make install`
              

Usage: `vm8051 [-m] input clock`

runs vm8051 on the code provided in `input`. If `clock` is set to 0,
run in interactive mode, else, use `clock` as the time period between
two instructions.

`-m`     only show the minimal interface
