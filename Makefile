CC = gcc
CPPFLAGS = -D_POSIX_SOURCE -I.
CFLAGS = -Wall -Wextra -Wmissing-declarations -fPIC -std=c99 -pedantic -O3
LDFLAGS = -s -L.

PREFIX ?= /usr/local

LIBS = lib8051.a lib8051.so
EXE = vm8051

TARGETS = $(LIBS) $(EXE)

PRINTHEADERS = $(wildcard print/*.h)
PRINTSRC = $(wildcard print/*.c)
PRINTOBJ = $(PRINTSRC:.c=.o)

VMHEADERS = $(wildcard vm/*.h)
VMSRC = $(wildcard vm/*.c)
VMOBJ = $(VMSRC:.c=.o)

COPROSHEADERS = $(wildcard copros/*.h)
COPROSSRC = $(wildcard copros/*.c)
COPROSOBJ = $(COPROSSRC:.c=.o)

UTILSHEADERS = $(wildcard utils/*.h)
UTILSSRC = $(wildcard utils/*.c)
UTILSOBJ = $(UTILSSRC:.c=.o)

HEADERS = $(PRINTHEADERS) $(VMHEADERS) $(COPROSHEADERS) $(UTILSHEADERS)
OBJFILES = $(PRINTOBJ) $(VMOBJ) $(COPROSOBJ) $(UTILSOBJ)

all: $(TARGETS)

install: $(TARGETS)
	mkdir -p $(PREFIX)/lib $(PREFIX)/include $(PREFIX)/bin
	cp $(LIBS) $(PREFIX)/lib
	cp $(HEADERS) $(PREFIX)/include
	cp $(EXE) $(PREFIX)/bin

uninstall:
	rm -f $(foreach LIB, $(LIBS), $(PREFIX)/lib/$(LIB))
	rm -f $(foreach HEADER, $(HEADERS), $(PREFIX)/include/$(HEADER))
	rm -f $(foreach E, $(EXE), $(PREFIX)/bin/$(E))

lib8051.so: $(VMOBJ) $(COPROSOBJ) $(PRINTOBJ) $(UTILSOBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -o $@ $+

lib8051.a: $(VMOBJ) $(COPROSOBJ) $(PRINTOBJ) $(UTILSOBJ)
	ar cr $@ $+
	ranlib $@

vm8051: lib8051.a

clean:
	rm -f $(OBJFILES) $(TARGETS)

.PHONY: all clean install uninstall
