.SUFFIXES:
.SUFFIXES: .c .cpp .o

GPP=g++
GCC=gcc
CPPFLAGS=-g -Wall -ansi -pedantic
CFLAGS=  -g -Wall -ansi -pedantic

all: hex rm_cr ascii2bin parsefloat32

HEX_OBJS       = hex.o
RM_CR_OBJS     = rm_cr.o
ASCII_OBJS     = ascii2bin.o
PARSEFP32_OBJS = parsefloat32.o

hex: $(HEX_OBJS)
	$(GCC) $< -o $@

rm_cr: $(RM_CR_OBJS)
	$(GCC) $< -o $@

ascii2bin: $(ASCII_OBJS)
	$(GCC) $< -o $@

parsefloat32: $(PARSEFP32_OBJS)
	$(GPP) $< -o $@

.cpp.o:
	$(GPP) -c $(CPPFLAGS) src/$< -o $@

.c.o:
	$(GCC) -c $(CFLAGS) src/$< -o $@

clean:
	rm -f *.o
	rm -f hex
	rm -f rm_cr
	rm -f ascii2bin
	rm -f parsefloat32
