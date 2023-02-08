#define BLOCK_LENGTH 32
#define EXPANSION_TABLE_OUTPUT 48
#define S_BOX_INPUT 6
#define S_BOX_OUTPUT 4

#include <stdio.h>
#include <stdlib.h>
//#include <iostream>
#include <stdint.h>

char** getInputFile(char* fileName, int* NumBlocks);

void createOutputFile(char* outputFileName, char* output);

void expansionFunction(char* input, char* result);

void separateAfterExpansion(char* input, char** s_blocks);

void substitutionFunction(char* input, char* result);

void combineAfterSubstitution(char** input, char* result);

void ES_Operation(char* block, char* result);

void XOR_Function(char** values, int numBlocks, char** result);

void final_XOR_Function(char** values, int numBlocks, char* result);

void MTUHash(char** blocks, int numBlocks, char* result);

// debug functions

void printBlocks(char** blocks, int numBlocks);

void test_Sub();

void test_expansion();

void test_separate();

void test_ES_operation_function();