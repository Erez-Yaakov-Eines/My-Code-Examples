#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH 260

int main() {
    char fName1[MAX_PATH]; char fName2[MAX_PATH];
    printf("Enter file name 1:"); scanf("%s",fName1);
    printf("\nEnter file name 2:"); scanf("%s",fName2);
    //initializers
    FILE* file1 = NULL, *file2 = NULL;
    char c1 = '\0', c2 = '\0';
    file1 = fopen(fName1,"rt"); file2 = fopen(fName2,"rt");
    //read write success check
    if (!file1 || !file2)
        {printf("\nfile read error!"); exit(EXIT_FAILURE); }
    //while simultaneous reads are equal and end of files not reached.
    while (c1 != EOF && c2 != EOF ) {
        c1 = fgetc(file1);
        c2 = fgetc(file2);
        if (c1 != c2) {
            printf("\nfiles contents are different. ");
            system("pause");
            exit(EXIT_SUCCESS);
        }
    }
    printf("\nfiles are identical.  ");
    system("pause");
exit(EXIT_SUCCESS);
}
