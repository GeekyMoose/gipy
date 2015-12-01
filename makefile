##
# GIPY Library makefile
##

# Define variables
CC			= $(CROSS_COMPILER)gcc
CF_FLAG		= -Wall -g

BUILD		= build
TARGET		= execGipy
SRC			= src

#SOURCES		= $(wildcard $(SRC)/**/*.c src/*.c) #Get all .c from src dir
#OBJECTS		= $(patsubst %.c, %.o, $(SOURCES)) #Create the .o name from .c
#build/$(TARGET): $(OBJECTS)
#	$(CC) $(CF_FLAG) -o $@ $(SOURCES)

# Build Rules
$(BUILD)/$(TARGET): createBuild build/gipy.o build/main.o
	$(CC) $(CF_FLAG) -o $(BUILD)/$(TARGET) $(BUILD)/main.o

$(BUILD)/gipy.o : $(SRC)/gipy.c src/gipy.h
	$(CC) $(CF_FLAG) -c -o $(BUILD)/gipy.o $(SRC)/gipy.c

$(BUILD)/main.o : $(SRC)/main.c src/gipy.c src/gipy.h
	$(CC) $(CF_FLAG) -c -o $(BUILD)/main.o $(SRC)/main.c

$(BUILD)/errman.o : $(SRC)/errman.h
	$(CC) $(CF_FLAG) -c -o $(BUILD)/errman.o $(SRC)/errman.h

# Other rules

# Create the generated folders
createBuild:
	@mkdir -p $(BUILD)
	@mkdir -p bin

clean:
	-rm -rf $(BUILD) bin
	#$(RM) *.o *.out execGipy
