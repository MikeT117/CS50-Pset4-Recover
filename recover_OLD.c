#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct{
    uint8_t data[512];
} BUFFER;

int main(int argc, char *argv[]){

    if(argc != 2){
        fprintf(stderr, "No file to scan provided\n");
        return 1;
    }

    FILE *iFile = fopen(argv[1], "r");
    BUFFER buffer;
    BUFFER *bf = &buffer;

    //Define Filename and increement for each found file.
    int n = 000;
    char fileName[9] = { };

    //Find start of image and write if found - Assumes 512 Byte block type(FAT).
    while(fread(bf, 1, 512, iFile) == 512){
        int found = 0;

        for(int i = 0; i < 511; i++){
            if(bf->data[i] == 0xFF && bf->data[i+1] == 0xD8){
                sprintf(fileName,"%03i.jpg",n);
                FILE *oFile = fopen(fileName, "w+");
                fwrite(bf, sizeof(BUFFER), 1, oFile);
                found = 1;
                for(;;){
                    fread(bf, 1, 512, iFile);
                    for(int j = 0; j < 511; j++){
                        if(bf->data[j] == 0xFF && bf->data[j+1] == 0xD9){
                        fwrite(bf, sizeof(BUFFER), 1, oFile);
                        fclose(oFile);
                        fprintf(stdout, "Image %03i restored\n",n);
                        n++;
                        found = 2;
                        }
                    }
                    if (found == 2){
                        break;
                    }else{
                        fwrite(bf, sizeof(BUFFER), 1, oFile);
                    }
                }
            }
            if (found == 2){
                break;
            }
        }
    }
    return 0;
}