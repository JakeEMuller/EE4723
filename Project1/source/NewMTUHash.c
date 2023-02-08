#include "MTUHash.h"
#include "AECalculator.h"

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
        printf("Warning: file size is not multiple of 32 (sz %% 32 = %d).\n", (sz % 32));
        printf(" -- Likely line character at end of file\n");
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
                printf("Exiting...\n");
                exit(1);
                return 0;
            // check if not 0 or 1
            }else if(character != '0' && character != '1' ){
                
                printf("ERROR: Non 0 or 1 character detected\n");
                printf("Exiting...\n");
                exit(1);
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

void expansionFunction(char* input, char* result){
    //char* result = malloc(sizeof(char) * 48);

    for(int i = 0; i < EXPANSION_TABLE_OUTPUT; i++){
        int x = expansionTable[i] - 1;
        result[i] = input[x];
    }

    //return result;
}

// -----------------------------------
// Separate after Expansion Function
// -----------------------------------

void separateAfterExpansion(char* input, char** s_blocks){
    //char** s_blocks = (char**) malloc(sizeof(char*) * 8);
    
 
    for(int i = 0; i < 8; i++){
        //s_blocks[i] = (char*) malloc(sizeof(char) * 6);
        for(int j = 0; j < 6; j++){
            int index = (i * 6) + j;
            s_blocks[i][j] = input[index];
        }
    }

    //return s_blocks;
}



// --------------------------
// Substitution Function
// --------------------------

void substitutionFunction(char* input, char* result){
    
    //calculate i
    int i = 2 * (input[0] - 0x30) + (input[5] - 0x30);
    //calculate j
    int j = input[1] - 0x30;
    j = (j << 1) | (input[2] - 0x30);
    j = (j << 1) | (input[3] - 0x30);
    j = (j << 1) | (input[4] - 0x30);

    //printf("I: %d J: %d \n", i, j);

    int result_asInt = substitutionTable[i][j]; 

    //convert int to array
    result_asInt = 0xF & result_asInt;
    //char* result = malloc(sizeof(char) * S_BOX_OUTPUT);
    for(int k = 0; k < S_BOX_OUTPUT; k++){
        int temp = 0x8 & result_asInt;
        temp = temp >> 3;
        result[k] = temp + 0x30;
        result_asInt = result_asInt << 1;
    }

    //return result;
}

// ----------------------------------------
// Combine after substitution Function
// ----------------------------------------

void combineAfterSubstitution(char** input, char* result){

    //char* result = malloc(sizeof(char) * 32);

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 4; j++){
            int index = (i * 4) + j;
            result[index] = input[i][j]; 
        }
    }

    //return result;
}


// ---------------------
// ES Operation
// ---------------------

void ES_Operation(char* block, char* result){
    
    char* expanded = malloc(sizeof(char) * 48);
    expansionFunction(block, expanded);
    
    // allocate memory
    char** sBlocks = (char**) malloc(sizeof(char*) * 8);
    for(int i = 0; i < 8; i++){
        sBlocks[i] = (char*) malloc(sizeof(char) * 6);
    }

    separateAfterExpansion(expanded, sBlocks);

    char** cBlocks = (char**) malloc(sizeof(char*) * 8);
    for(int j = 0; j < 8; j++){
        substitutionFunction(sBlocks[j], cBlocks[j]);

    }

    //char* result = malloc(sizeof(char) * 32);
    combineAfterSubstitution(cBlocks, result);

    // free memory
    free(expanded);
    for(int i = 0; i < 8; i++){
        free(sBlocks[i]);
        free(cBlocks[i]);
    }
    free(sBlocks);
    free(cBlocks);

}

void XOR_Function(char** values, int numBlocks, char** result){
    if(numBlocks == 1){
        //copy values[0] to results[0]

    }
}