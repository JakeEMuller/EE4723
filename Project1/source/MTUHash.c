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
        printf("ERROR: file size is not multiple of 32 (sz %% 32 = %d)\n", (sz % 32));
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

char* expansionFunction(char* input){
    char* result = malloc(sizeof(char) * 48);

    for(int i = 0; i < EXPANSION_TABLE_OUTPUT; i++){
        int x = expansionTable[i] - 1;
        result[i] = input[x];
    }

    return result;
}

// -----------------------------------
// Separate after Expansion Function
// -----------------------------------

char** separateAfterExpansion(char input[EXPANSION_TABLE_OUTPUT]){
    char** s_blocks = (char**) malloc(sizeof(char*) * 8);
    
 
    for(int i = 0; i < 8; i++){
        s_blocks[i] = (char*) malloc(sizeof(char) * 6);
        for(int j = 0; j < 6; j++){
            int index = (i * 6) + j;
            s_blocks[i][j] = input[index];
        }
    }

    return s_blocks;
}

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

    //printf("I: %d J: %d \n", i, j);

    int result_asInt = substitutionTable[i][j]; 

    //convert int to array
    result_asInt = 0xF & result_asInt;
    char* result = malloc(sizeof(char) * S_BOX_OUTPUT);
    for(int k = 0; k < S_BOX_OUTPUT; k++){
        int temp = 0x8 & result_asInt;
        temp = temp >> 3;
        result[k] = temp + 0x30;
        result_asInt = result_asInt << 1;
    }

    return result;
}

// ----------------------------------------
// Combine after substitution Function
// ----------------------------------------

char* combineAfterSubstitution(char** input){

    char* result = malloc(sizeof(char) * 32);

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 4; j++){
            int index = (i * 4) + j;
            result[index] = input[i][j]; 
        }
    }

    return result;
}



// --------------------------
// XOR Operation Function
// --------------------------

char** XOR_Function(char** values, int blocks){
    
    // if only one block return
    if(blocks == 1){
        return values;
    }

    char** result = (char**) calloc(blocks, sizeof(char*));

    for(int i = 0; i < blocks; i++){
        result[i] = calloc(32, sizeof(char*));
        for(int j = 0; j < blocks; j++){
            if( i != j ){
                for(int k = 0; k < 32; k++){
                    result[i][k] = result[i][k] ^ (values[j][k]);
                }
            }
            
        }
    }


    return result;
}

// --------------------------
// Final XOR
// --------------------------

char* final_XOR_Function(char** values, int blocks){

    if( blocks == 1 ){
        return values[0];
    }

    char* result =  calloc(32, sizeof(char));

    for(int i = 0; i < blocks; i++){
        for(int j = 0; j < 32; j++){
            result[j] = result[j] ^ values[i][j];
        }
    }

    return result;
}

// --------------------------
// MTUHash Function
// --------------------------

char* MTUHash(char** blocks, int numBlocks){

    for(int rounds = 0; rounds < 16; rounds++){
        //printf("round %d \n", rounds);
        //do E-S on all blocks
        for(int i = 0; i < numBlocks; i++){
            //printf("Block %d \n", i);

            char* expanded = expansionFunction(blocks[i]);
            //printf("after expand \n");

            char** sBlocks = separateAfterExpansion(expanded);
            free(expanded);
            //printf("after seperation \n");


            //go through all S blocks and run substitution 
            for(int j = 0; j < 8; j++){
                //printf("subblock %d \n", j);
                sBlocks[j] = substitutionFunction(sBlocks[j]);
            }
            //printf("after substitutions \n");

            blocks[i] = combineAfterSubstitution(sBlocks);
            //printf("after combine \n");
            free(sBlocks);
        }

        blocks = XOR_Function(blocks, numBlocks);


    }

    char* result = final_XOR_Function(blocks, numBlocks);
    

    return result;
}



// --------------------------
// Main function
// --------------------------
int main(){

    //get input
    //printf("hello world \n");
    int numBlocks = 0;  
    char* fileName = "Hashin(32 bit).txt";
    char** blocks = getInputFile(fileName, &numBlocks);

    //char* result = MTUHash(blocks, numBlocks);
    //free(blocks);

    //printf("Number of blocks: %d \n", numBlocks);
    //printBlocks(blocks, numBlocks);
    //test_Sub();    
    //test_expansion();
    //test_separate();
    test_ES_operation();
    //createOutputFile("Hashout_TEST1.txt", result);

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





void test_expansion(){
    char input[32] = "10000100001000010000100001000011";

    char* result = expansionFunction(input);
     printf("Expanstion expected: 110000001000000100000010100001010000001000000111\n");
    printf("Actual: ");
    for(int i = 0; i < 48; i++){
        printf("%c", result[i]);
    }
    printf("\n");

}

void test_separate(){
    char input[48] = "110000001000000100000010100001010000001000000111";
    printf("110000 001000 000100 000010 100001 010000 001000 000111\n");
    char** s_blocks = separateAfterExpansion(input);

    for(int i = 0; i < 8; i++){
        printf("Block 1: ");
        for(int j = 0; j < 6; j++){
            printf("%c", s_blocks[i][j]);
        }
        printf("\n");
    }

}


void test_Sub(){
    
    char input[6] = {'1','0','1','0','1','0'};

    char* result = substitutionFunction(input);

    printf("Substitution expected: 0110\n");
    printf("Actual: ");
    for(int i = 0; i < 4; i++){
        printf("%c", result[i]);
    }
    printf("\n");

}


void test_ES_operation(){
    char input[48] = "110000001000000100000010100001010000001000000111";
    printf("110000 001000 000100 000010 100001 010000 001000 000111\n");
    
    printf("Spliting\n");
    char** s_blocks = separateAfterExpansion(input);

    for(int i = 0; i < 8; i++){
        printf("Block 1: ");
        for(int j = 0; j < 6; j++){
            printf("%c", s_blocks[i][j]);
        }
        printf("\n");
    }

    printf("Substitution\n");
    for(int j = 0; j < 8; j++){
        //printf("subblock %d \n", j);
        s_blocks[j] = substitutionFunction(s_blocks[j]);
    }

    for(int i = 0; i < 8; i++){
        printf("sBlock 1: ");
        for(int j = 0; j < 4; j++){
            printf("%c", s_blocks[i][j]);
        }
        printf("\n");
    }

    printf("Combine\n");
    char* combined = combineAfterSubstitution(s_blocks);
    printf("Expected: 11110010110101001111001100100100\n");
    printf("Actual:   ");
    for(int i = 0; i < 32; i++){
        printf("%c", combined[i]);
    }
    printf("\n");

}