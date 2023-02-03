//Joan
#define BLOCK_MULTIPLE 32
#define EXPANSION_TABLE_OUTPUT 48
#define S_BOX_INPUT 6
#define S_BOX_OUTPUT 4

#include "MTUHash.h"

// --------------------------
// Expansion table
// --------------------------
int expansionTable[8][6] = {{ 32,  1,  2,  3,  4,  5},
                            {  4,  5,  6,  7,  8,  9},
                            {  8,  9, 10, 11, 12, 13},
                            { 12, 13, 14, 15, 16, 17},
                            { 16, 17, 18, 19, 20, 21},
                            { 20, 21, 22, 23, 24, 25},
                            { 24, 25, 26, 27, 28, 29},
                            { 28, 29, 30, 31, 32,  1}}; 

// --------------------------
// Substitution table
// --------------------------
int substitutionTable[4][16] = {{ 14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
                                {  0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
                                {  4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
                                { 15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}};


// --------------------------
// Expansion Function
// --------------------------

int* expansionFunction(int input[48]){

}

// --------------------------
// Substitution Function
// --------------------------

int* substitutionFunction(int input[6]){

}


// --------------------------
// XOR Operation Function
// --------------------------

int* XOR_Function(int* values[32], int blocks){

}

// --------------------------
// Final XOR
// --------------------------

int* final_XOR_Function(int* values[32], int blocks){

}

// --------------------------
// MTUHash Function
// --------------------------

int* MTUHash(int* bitstream){
    
}

// --------------------------
// Avalanche Effect Function
// --------------------------


// --------------------------
// Main function
// --------------------------
int main(){

    //get input

    //call mtu hash
    print("hello world");
    //output

    //exit
    return 0;

}
