CC = gcc
CFLAGS = -pedantic -Wall -Wextra -O1
LDFLAGS = -lm

default: clean MTUHash.o AECalculator.o

MTUHash.o: MTUHash.c MTUHash.h
			gcc -pedantic -Wall -Wextra -O1 MTUHash.c -o MTUHash ${LDFLAGS}

AECalculator.o: AECalculator.c
			gcc -pedantic -Wall -Wextra -O1 AECalculator.c -o AECalculator ${LDFLAGS}

clean: 
			rm -f MTUHash AECalculator