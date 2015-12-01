##
# GIPY Library makefile
##

# Define variables
CC			= $(CROSS_COMPILER)gcc
CF_FLAG		= -Wall -g

SOURCES		= $(wildcard src/**/*.c src/*.c) #Get all .c from src dir
OBJECTS		= $(patsubst %.c, %.o, $(SOURCES)) #Create the .o name from .c

BUILD		= build
TARGET		= execGipy
SRC			= src/

#build/$(TARGET): $(OBJECTS)
#	$(CC) $(CF_FLAG) -o $@ $(SOURCES)


buil/execGipy: build/gipy.o build/main.o
	$(CC) $(CF_FLAG) -o build/execGipy build/main.o

build/gipy.o : src/gipy.c src/gipy.h
	$(CC) $(CF_FLAG) -o build/gipy.o src/gipy.c

build/main.o : src/main.c
	$(CC) $(CF_FLAG) -o build/main.o src/main.c

build/errman.o : src/errman.h
	$(CC) $(CF_FLAG) -o build/errman.o src/errman.h


# Create the generated folders
createBuild:
	@mkdir -p build
	@mkdir -p bin


clean:
	-rm -rf buil bin
	#$(RM) *.o *.out execGipy
