VERSION		= 4.2
CC				= gcc
AR				= ar
DEFINES		=
CFLAGS		= -Wall -I. -Ilogger/include -g
LDFLAGS		= -L. -lcllct
OBJ				= cllct.o

libcllct.a: $(OBJ)
	make -C logger/
	$(AR) rcs libcllct.a $(OBJ) logger/*.o

tests: libcllct.a tests.c
	$(CC) tests.c $(CFLAGS) $(LDFLAGS) -o tests
	tests

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
