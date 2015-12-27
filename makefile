##
###############################################################################
# GIPY Library makefile
###############################################################################
#
##

# Define variables
CC			= $(CROSS_COMPILER)gcc
CF_FLAG		= -Wall -g
VPATH		= src build

TARGET		= execGipy
BUILD		= build
BIN			= bin


#SOURCES		= $(wildcard $(SRC)/**/*.c src/*.c) #Get all .c from src dir
#OBJECTS		= $(patsubst %.c, %.o, $(SOURCES)) #Create the .o name from .c
#build/$(TARGET): $(OBJECTS)
#	$(CC) $(CF_FLAG) -o $@ $(SOURCES)


###############################################################################
# Build Rules
###############################################################################
all: $(BUILD)/$(TARGET)

$(BUILD)/$(TARGET): createBuild main.o gipy.o errman.o debug.o
	$(CC) $(CF_FLAG) -o $(BIN)/$(TARGET) main.o gipy.o errman.o debug.o -pthread

$(BUILD)/main.o: main.c gipy.h errman.h debug.h
	$(CC) $(CF_FLAG) -c main.c

$(BUILD)/gipy.o: gipy.c gipy.h errman.h debug.h
	$(CC) $(CF_FLAG) -c gipy.c

$(BUILD)/errman.o: errman.c errman.h
	$(CC) $(CF_FLAG) -c errman.c

$(BUILD)/debug.o: debug.c debug.h
	$(CC) $(CF_FLAG) -c debug.c


###############################################################################
# Comp / exc rules
###############################################################################
# Create the generated folders
createBuild:
	@mkdir -p $(BUILD)
	@mkdir -p bin

clean:
	-rm -rf $(BUILD) bin
	-rm -r *.o
	-rm -f *.exe

run:
	bin/$(TARGET)

