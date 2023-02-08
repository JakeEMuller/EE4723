#include "AECalculator.h"
#include "MTUHash.h"

double compareOutputs(char* baseLineResult, char* result){
    int count = 0;
    for(int i = 0; i < 32; i++){
        //printf("base: %c\n", baseLineResult[i]);
        //printf("new: %c\n", result[i]);
        
        if(baseLineResult[i] != result[i]){

            count++;
        }
    }

    double percent = ((double)(count *100)) / 32.0;
    return percent;

}


void AECalculator(){

    //printf("Start AE Cacl\n");

    char* fileName = "Hashin(32 bit).txt";
    char* fileOut = "ARF.txt";
    FILE* fp; 

    //printf("Before File open\n");
    fp = fopen(fileOut, "w+");

    int numBlocks = 0;

    //printf("Before Input\n");
    char** blocks = getInputFile(fileName, &numBlocks);
    //printf("After Input\n");
    
    char* baseLineResult = malloc(sizeof(char) * 32);
    MTUHash(blocks, numBlocks, baseLineResult);

    double average = 0;

    for(int i = 0; i < numBlocks; i++){
        for(int j = 0; j < 32; j++){
            //printf("Bit [%d] [%d]\n", i, j);
            //flip bit



            char holder = blocks[i][j];
            if(blocks[i][j] == '0'){
                blocks[i][j] = '1';
            }else{
                blocks[i][j] = '0';
            }

            //run MTUHash
            char* result = malloc(sizeof(char) * 32);
            MTUHash(blocks, numBlocks, result); //blocks are getting super fucked up
            
            printf("BaseLine Hash: ");
            for(int i = 0; i < 32; i++) {
                printf("%c", baseLineResult[i]);
            }
            printf("\n"); 

            printf("AE Hash:       ");
            for(int i = 0; i < 32; i++) {
                printf("%c", result[i]);
            }
            printf("\n");

            //compare outputs
            double percent = compareOutputs(baseLineResult, result);
            
            fprintf(fp,"%.1f %%\n", percent);


            average = average + percent;

            //flip bit back
            blocks[i][j] = holder;


            free(result);
        }
    }

    average = average / (numBlocks * 32);
    fprintf(fp, "Average AFT: %.1f %%\n", average);


    for(int i = 0; i < numBlocks; i++){
        free(blocks[i]);
    }
    free(blocks);
    free(baseLineResult);
    fclose(fp);

    return;
}