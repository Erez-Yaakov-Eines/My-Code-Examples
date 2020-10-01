#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    if (argc != 4) {printf("incorrect arguments!"); exit(EXIT_FAILURE);}
    //initializers
    FILE* file1 = NULL, *file2 = NULL,*target = NULL;
    char c1 = '\0', c2 = '\0';
    file1 = fopen(argv[1],"rt"); file2 = fopen(argv[2],"rt"); target = fopen(argv[3],"wt");
    //read write success check
    if (!file1 || !file2 || !target)
        { printf("file read error!"); exit(EXIT_FAILURE); }
    // check if file1 is empty
    fseek(file1, 0, SEEK_END);
    if (ftell(file1) == 0) c1 = EOF;
    else rewind(file1);
    //check if file2 is empty
    fseek(file2, 0, SEEK_END);
    if (ftell(file2) == 0) c2 = EOF;
    else rewind(file2);
    //while file1 AND file2 are both full
    while (c1 != EOF && c2 != EOF) {
        while ( (c1 = fgetc(file1)) != '\n' ) {
            if (c1 == EOF) break;
            fputc(c1,target);
        }
        fputc('\n',target);
        while ( (c2 = fgetc(file2)) != '\n' ) {
            if (c2 == EOF) break;
            fputc(c2,target);
        }
        fputc('\n',target);
    }
    // if file1 is not empty
    if (c1 != EOF) {
        while ( (c1 = fgetc(file1)) != EOF ) {
            fputc(c1,target);
        }
    }
    //if file2 is not empty
    if (c2 != EOF) {
        while ( (c2 = fgetc(file2)) != EOF ) {
            fputc(c2,target);
        }
    }

    system("pause");
exit(EXIT_SUCCESS);
}
