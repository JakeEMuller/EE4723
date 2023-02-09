#include "MTUHashLib.h"


int main( int argc, char *argv[]){

    if(argc != 2){
        printf("Please input one argment: ./MTUHash <InputFile.txt> \n");
        exit(1);
    }
    
    int numBlocks = 0;  
    //char* fileName = "Hashin(32 bit).txt";
    char* fileName = argv[1];
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

    createOutputFile("OutFinal.txt", result);

    if(debug == 1){
        //print final result
        printf("Final Result: ");
        for(int i = 0; i < 32; i++){
            printf("%c", result[i]);
        }
        printf("\n");
    }


    for(int i = 0; i < numBlocks; i++){
        free(blocks[i]);
    }
    free(blocks);
    free(result);

}

