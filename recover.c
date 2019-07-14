#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct{
    uint8_t data[512];
} BUFFER;

void findSOI(BUFFER *bf, FILE *iFile, int *n, char *fileName);
int findEOI(BUFFER *bf, FILE *iFile, FILE *oFile, int *n);

int main(int argc, char *argv[]){

    if(argc != 2){
        fprintf(stderr, "No file to scan provided\n");
        return 1;
    }

    //Open input file, Define a buffer of type BUFFER to store 512 bytes per read, Define a pointer to the buffer.
    FILE *iFile = fopen(argv[1], "r");
    BUFFER *bf = (BUFFER *) malloc(512);

    //Define Filename and incrementor variable for each found file.
    int n = 000;
    char fileName[9] = { };

    //While 512 Bytes are successfully read execute the findStart function.
    while(fread(bf, 1, 512, iFile) == 512){
        findSOI(bf, iFile, &n, fileName);
    }
    return 0;
    free(bf);
}
//During each execution determine if the SOI is present, If dound execute findEOI - If not discontinue execution.
void findSOI(BUFFER *bf, FILE *iFile, int *n, char *fileName){
    if(bf->data[0] == 0xFF && bf->data[1] == 0xD8){
        sprintf(fileName,"%03i.jpg",*n);
        FILE *oFile = fopen(fileName, "w+");
        fwrite(bf, sizeof(BUFFER), 1, oFile);
        findEOI(bf, iFile, oFile, n);
    }
}
/*During execution, Define a while loop per each successful 512 byte read check each 2 bytes for the EOI,
If ofund close oFile increment n and return 0, If not found write current buffer and loop around reading another 512 bytes.*/
int findEOI(BUFFER *bf, FILE *iFile, FILE *oFile, int *n){
    while(fread(bf, 1, 512, iFile) == 512){
        for(int i = 0; i < 511; i++){
            if(bf->data[i] == 0xFF && bf->data[i+1] == 0xD9){
                fwrite(bf, sizeof(BUFFER), 1, oFile);
                fclose(oFile);
                fprintf(stdout, "Image %03i restored\n",*n);
                *n += 1;
                return 0;
            }
        }
        fwrite(bf, sizeof(BUFFER), 1, oFile);
    }
    return 0;
}