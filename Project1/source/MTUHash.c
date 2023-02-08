#include "MTUHash.h"
#include "AECalculator.h"

int debug = 0;

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
// Expansion Function: 
// Input: 32 Bit Block
// Output: 48 Bit Block
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
// Input: 48 Bit block
// Output: Array of bits 8 * 6 bits
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
// Inputs: 6 bits
// Output: 4 bits
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
// Inputs: 8 * 4 bits matrix
// Output: 32 Bit
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
// Inputs: 32 Bit block
// Output: 32 Bit E-S Block  
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
        cBlocks[j] = malloc(sizeof(char) * 4);
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

// -----------------------
// XOR Function: 
// Input: Array of 32 bit blocks 
//        Number of blocks
// Output: Array of 32 bit blocks (same length as inputed)
// -----------------------

void XOR_Function(char** values, int numBlocks, char** result){
    
    //if one block return
    if(numBlocks == 1){
        //copy values[0] to results[0]
        for(int i = 0; i < 32; i++){
            result[0][i] = values[0][i]; 
        }
        return;
    }

    for(int i = 0; i < numBlocks; i++){
        //fill with 0s 
        for(int fill = 0; fill < 32; fill++){
            result[i][fill] = '0';
        }

        //perform XOR
        for(int j = 0; j < numBlocks; j++){
            if(i != j){
                for(int k = 0; k < 32; k++){
                    if(result[i][k] == values[j][k]){
                        result[i][k] = '0';
                    } else {
                        result[i][k] = '1';
                    }
                }
            }
        }
    }
    
}


// ----------------------------
// final_XOR_function
// Inputs: Array of 32 Bit blocks
//         Number of blocks
// Outputs: 32 bit final output
// ----------------------------
void final_XOR_Function(char** values, int numBlocks, char* result){
    
    //if one block return
    if(numBlocks == 1){
        //copy values[0] to results
        for(int i = 0; i < 32; i++){
            result[i] = values[0][i]; 
        }
        return;
    }

    //fill result with 0s
    for(int i = 0; i < 32; i++){
        result[i] = '0';
    }

    for(int i = 0; i < numBlocks; i++){
        for(int j = 0; j < 32; j++){
            // do xor
            //result[j] = result[j] ^ values[i][j];
            if(result[j] == values[i][j]){
                result[j] = '0';
            } else {
                result[j] = '1';
            }
            
        }
    }


}


// -------------------------------------
// MTU Hash 
// Inputs: Array of 32 bit blocks
//         Number of blocks
// Outputs: 32 bit result
// -------------------------------------
void MTUHash(char** blocks, int numBlocks, char* result){

    //allocate memory of copied blocks
    char** modBlocks = (char**) malloc(sizeof(char*) * numBlocks);
    for(int i = 0; i < numBlocks; i++){
        modBlocks[i] = malloc(sizeof(char)* 32);
        for(int j = 0; j < 32; j++){
            modBlocks[i][j] = blocks[i][j];
        }
    }
    //perform 15 normal rounds
    for(int rounds = 0; rounds < 15; rounds++){
        for(int i = 0; i < numBlocks; i++){
            ES_Operation(modBlocks[i], modBlocks[i]);
        }
        if(debug == 1){
            printf("round: %d\n", rounds);
            printf("Before XOR\n");
            printBlocks(modBlocks, numBlocks);
        }
        

        XOR_Function(modBlocks, numBlocks, modBlocks);

        if(debug == 1){
            printf("After XOR\n");
            printBlocks(modBlocks, numBlocks);
        }
        

        //check for 1st iteration and output round one hash

    }

    for(int i = 0; i < numBlocks; i++){
        ES_Operation(modBlocks[i], modBlocks[i]);
    }

    if(debug == 1){
        printf("Before Final XOR\n");
        printBlocks(modBlocks, numBlocks);
    }
    
    final_XOR_Function(modBlocks, numBlocks, result);

    //free memory
    for(int i = 0; i < numBlocks; i++){
        free(modBlocks[i]);
    }
    free(modBlocks);

}

int main(){

    
    int numBlocks = 0;  
    char* fileName = "Hashin(32 bit).txt";
    char** blocks = getInputFile(fileName, &numBlocks);
    

    if(debug == 2){
        printf("Print blocks \n");
        printBlocks(blocks, numBlocks);
        printf("--\nTest Sub\n--\n");
        test_Sub();    
        printf("--\nTest Expand\n--\n");
        test_expansion();
        printf("--\nTest Separate\n--\n");
        test_separate();
        printf("--\nTest ES\n--\n");
        test_ES_operation_function();
    }


    char* result = malloc(sizeof(char) * 32);
    MTUHash(blocks, numBlocks, result);

    createOutputFile("Hashout_TEST1.txt", result);

    if(debug == 1){
        //print final result
        printf("Final Result: ");
        for(int i = 0; i < 32; i++){
            printf("%c", result[i]);
        }
        printf("\n");
    }

    AECalculator();


    for(int i = 0; i < numBlocks; i++){
        free(blocks[i]);
    }
    free(blocks);
    free(result);

}

// Debug functions

void printBlocks(char** blocks, int numBlocks){
    for(int i = 0; i < numBlocks; i++){
        printf("Block %d: ", i);
        for(int j = 0; j < 32; j++){
            printf("%c", blocks[i][j]);
        }
        printf("\n");
    }   
}

void test_Sub(){
    char input[6] = {'1','0','1','0','1','0'};
    
    char* result = malloc(sizeof(char) * 4);
    substitutionFunction(input, result);

    printf("Substitution expected: 0110\n");
    printf("Actual: ");
    for(int i = 0; i < 4; i++){
        printf("%c", result[i]);
    }
    printf("\n");

    free(result);
}

void test_expansion(){
    char input[32] = "10000100001000010000100001000011";

    char* result = malloc(sizeof(char) * 48);
    expansionFunction(input, result);
    printf("Expansion expected: 110000001000000100000010100001010000001000000111\n");
    printf("Expansoin Actual:   ");
    for(int i = 0; i < 48; i++){
        printf("%c", result[i]);
    }
    printf("\n");

    free(result);
}

void test_separate(){
    char input[48] = "110000001000000100000010100001010000001000000111";
    printf("110000 001000 000100 000010 100001 010000 001000 000111\n");
    
    char** s_blocks = (char**) malloc(sizeof(char*) * 8);
    for(int i = 0; i < 8; i++){
        s_blocks[i] = malloc(sizeof(char) * 6);
    }

    separateAfterExpansion(input, s_blocks);

    for(int i = 0; i < 8; i++){
        printf("Block 1: ");
        for(int j = 0; j < 6; j++){
            printf("%c", s_blocks[i][j]);
        }
        printf("\n");
    }

    //free memory
    for(int i = 0; i < 8; i++){
        free(s_blocks[i]);
    }
    free(s_blocks);
}

void test_ES_operation_function(){
    char input[48] = "10000100001000010000100001000011";

    char* result = malloc(sizeof(char) * 32);
    ES_Operation(input, result);

    printf("Expected: 11110010110101001111001100100100\n");
    printf("Actual:   ");
    for(int i = 0; i < 32; i++){
        printf("%c", result[i]);
    }
    printf("\n");

    free(result);
}