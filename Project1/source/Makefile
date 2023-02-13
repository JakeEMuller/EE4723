CC = gcc
CFLAGS = -pedantic -Wall -Wextra -O1
LDFLAGS = -lm

default: clean MTUHash.o AECalculator.o

MTUHash.o: MTUHash.c MTUHashLib.h
			gcc -pedantic -Wall -Wextra -O1 MTUHash.c MTUHashLib.c -o MTUHash ${LDFLAGS}

AECalculator.o: AECalculator.c AECalculator.h MTUHashLib.h
			gcc -pedantic -Wall -Wextra -O1 AECalculator.c MTUHashLib.c -o AECalculator ${LDFLAGS}

clean: 
			rm -f MTUHash AECalculator