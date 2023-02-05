CC = g++
CFLAGS = -pedantic -Wall -Wextra -O1
LDFLAGS = -lm

default: clean MTUHash.o AECalculator.o

MTUHash.o: MTUHash.cpp MTUHash.h
			g++ -pedantic -Wall -Wextra -O1 MTUHash.cpp -o MTUHash ${LDFLAGS}

AECalculator.o: AECalculator.cpp
			g++ -pedantic -Wall -Wextra -O1 AECalculator.cpp -o AECalculator ${LDFLAGS}

clean: 
			rm -f MTUHash AECalculator