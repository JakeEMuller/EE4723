

#include "MTUHash.h"

// --------------------------
// Expansion table
// --------------------------
int expansionTable[48] =   { 32,  1,  2,  3,  4,  5,
                              4,  5,  6,  7,  8,  9,
                              8,  9, 10, 11, 12, 13,
                             12, 13, 14, 15, 16, 17,
                             16, 17, 18, 19, 20, 21,
                             20, 21, 22, 23, 24, 25,
                             24, 25, 26, 27, 28, 29,
                             28, 29, 30, 31, 32,  1}; 

// --------------------------
// Substitution table
// --------------------------
int substitutionTable[4][16] = {{ 14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
                                {  0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
                                {  4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
                                { 15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}};


// -----------------------------
// File Input
// -----------------------------
char** getInputFile(char* fileName, int* NumBlocks){
    FILE* fp;

    fp = fopen(fileName, "r+");

    // get file size
    fseek(fp, 0L, SEEK_END);
    int sz = ftell(fp);
    rewind(fp);

    if((sz % BLOCK_LENGTH) != 0){
        printf("ERROR: file size is not multiple of 32 (sz % 32 = %d)\n", (sz % 32));
    }

    // calculate and store num of blocks
    *NumBlocks = (sz / BLOCK_LENGTH);

    // allocate size for block array
    char** result = (char**) malloc(sizeof(*result)* (*NumBlocks) );

    for(int i = 0; i < *NumBlocks; i++){
        
        //allocate size for block
        result[i] = (char*) malloc(sizeof(char) * BLOCK_LENGTH);
        for(int j = 0; j < BLOCK_LENGTH; j++){
            //get character
            char character = fgetc(fp);

            //printf("Character: %c \n", character);
            //printf("HEX: %x \n", character);
            //check if EOF 
            if (character == EOF){

                printf("ERROR: EOF Reached at Unexpected Time \n");
                return 0;
            // check if not 0 or 1
            }else if(character != '0' && character != '1' ){
                
                printf("ERROR: Non 0 or 1 character detected\n");
                return 0;
            }

            // put character in result
            result[i][j] = character;
            
        }
    }

    fclose(fp);
    return result;

}


// -----------------------------
// File Output 
// -----------------------------

void createOutputFile(char* outputFileName, char* output){
    FILE *fp;
    fp = fopen(outputFileName, "w+");
    for(int i = 0; i < BLOCK_LENGTH; i++){
        fprintf(fp, "%c", output[i]);
    }
    fclose(fp);
    return;
}

// --------------------------
// Expansion Function
// --------------------------

char* expansionFunction(char input[32]){
    char result[48];

    for(int i = 0; i < EXPANSION_TABLE_OUTPUT; i++){
        char x = expansionTable[i];
        result[i] = input[x];
    }

    return result;
}

// -----------------------------------
// Separate after Expansion Function
// -----------------------------------



// --------------------------
// Substitution Function
// --------------------------

char* substitutionFunction(char input[S_BOX_INPUT]){
    
    //calculate i
    int i = 2 * (input[0] - 0x30) + (input[5] - 0x30);
    //calculate j
    int j = input[1] - 0x30;
    j = (j << 1) | (input[2] - 0x30);
    j = (j << 1) | (input[3] - 0x30);
    j = (j << 1) | (input[4] - 0x30);

    int result_asInt = substitutionTable[i][j]; 

    //convert int to array
    int result_asInt = 0xF & result_asInt;
    char result[S_BOX_OUTPUT];
    for(int k = 0; k < S_BOX_OUTPUT; k++){
        int temp = 0x8 & result_asInt;
        int temp = temp >> 3;
        result[k] = temp + 0x30;
        result_asInt = result_asInt << 1;
    }

    return result;
}

// ----------------------------------------
// Combine after substitution Function
// ----------------------------------------

// --------------------------
// XOR Operation Function
// --------------------------

char* XOR_Function(char* values[32], int blocks){

}

// --------------------------
// Final XOR
// --------------------------

char* final_XOR_Function(char* values[32], int blocks){

}

// --------------------------
// MTUHash Function
// --------------------------

int* MTUHash(int* bitstream){
    
}



// --------------------------
// Main function
// --------------------------
int main(){

    //get input
    printf("hello world \n");
    int numBlocks = 0;  
    char** blocks = getInputFile("Hashin(96 bit).txt", &numBlocks);


    printf("Number of blocks: %d \n", numBlocks);
    printBlocks(blocks, numBlocks);
    
    createOutputFile("Hashout_TEST1.txt", blocks[0]);

    //exit
    return 0;

}



// debug functions

void printBlocks(char** blocks, int numBlocks){
    
    for(int i = 0; i < numBlocks; i++){
        printf("Block %d: ", i);
        for(int j = 0; j < 32; j++){
            printf("%c", blocks[i][j]);
        }
        printf("\n");
    }       
}
