#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdint.h>


using namespace std;

//int expansionTable[8][6];

//int substitutionTable[4][16];

char** getInputFile(char* fileName, int* NumBlocks);



int* expansionFunction(int input[48]);

int* substitutionFunction(int input[6]);

int* XOR_Function(int* values[32], int blocks);

int* final_XOR_Function(int* values[32], int blocks);

int* MTUHash(int* bitstream);

// debug functions

void printBlocks(char** blocks, int numBlocks);