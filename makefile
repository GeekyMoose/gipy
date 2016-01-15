##
###############################################################################
# GIPY Library makefile
###############################################################################
#
##

# Define variables
CC			= $(CROSS_COMPILER)gcc
CF_FLAG		= -Wall -g
VPATH		= src examples

TARGET		= execGipy
BIN			= bin


###############################################################################
# Launcher rules
###############################################################################
all: growthTree $(TARGET)

$(TARGET): main.o gipy.o errman.o debug.o
	$(CC) $(CF_FLAG) -o $(BIN)/$(TARGET) $^ -pthread


###############################################################################
# Build Rules for GIPY Lib
###############################################################################
main.o: main.c gipy.h
	$(CC) $(CF_FLAG) -c $<

gipy.o: gipy.c gipy.h errman.h debug.h
	$(CC) $(CF_FLAG) -c $< -pthread

errman.o: errman.c errman.h
	$(CC) $(CF_FLAG) -c $<

debug.o: debug.c debug.h
	$(CC) $(CF_FLAG) -c $<


###############################################################################
# Annexe functions
###############################################################################
# Create the generated folders
growthTree:
	@mkdir -p bin

clean:
	-rm -rf bin
	-rm -r *.o
	-rm -f *.exe
	-rm *.out

