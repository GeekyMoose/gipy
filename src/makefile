##
# GIPY Library makefile
#
#
##

# Define variables
CC		= $(CROSS_COMPILER)gcc
CF_FLAG	= -Wall -g

execGipy: gipy.o main.o
	$(CC) $(CF_FLAG) -o execGipy main.o

gipy.o : gipy.c gipy.h
	$(CC) $(CF_FLAG) -c gipy.c

main.o : main.c
	$(CC) $(CF_FLAG) -c main.c

clean:
	$(RM) *.o *.out execGipy
