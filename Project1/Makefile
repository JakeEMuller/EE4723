CC = g++
CFLAGS = -pedantic -Wall -Wextra -O1
LDFLAGS = -lm

default: clean MTUHash.o

MTUHash.o: MTUHash.cpp MTUHash.h
			g++ -pedantic -Wall -Wextra -O1 -c MTUHash.cpp -o MTUHash ${LDFLAGS}

clean: 
			rm -f MTUHash