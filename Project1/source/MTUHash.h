#define BLOCK_LENGTH 32
#define EXPANSION_TABLE_OUTPUT 48
#define S_BOX_INPUT 6
#define S_BOX_OUTPUT 4

#include <stdio.h>
#include <stdlib.h>
//#include <iostream>
#include <stdint.h>


//using namespace std;

//int expansionTable[8][6];

//int substitutionTable[4][16];

char** getInputFile(char* fileName, int* NumBlocks);



char* expansionFunction(char input[48]);

char* substitutionFunction(char input[S_BOX_INPUT]);

char* XOR_Function(char* values[32], int blocks);

char* final_XOR_Function(char* values[32], int blocks);

int* MTUHash(int* bitstream);

// debug functions

void printBlocks(char** blocks, int numBlocks);