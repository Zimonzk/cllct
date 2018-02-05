VERSION		= 4.2
CC				= gcc
AR				= ar
DEFINES		=
CFLAGS		= -Wall
LDFLAGS		= 

OBJ				= cllct.o

cllct.a: $(OBJ)
	make -C logger/
	$(AR) rcs cllct.a $(OBJ) logger/*.o


%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
