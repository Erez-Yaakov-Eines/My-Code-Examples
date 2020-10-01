#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {printf("incorrect arguments!"); exit(EXIT_FAILURE);}
    char c;
    int counter = 0;
    FILE* toPrint;
    toPrint = fopen(argv[1],"rt");
    while((c = fgetc(toPrint)) != EOF) {
        if (c == '\n') ++counter;
        printf("%c",c);
        if (counter == 10) {
            counter = 0;
            system("pause"); // windows only
        }
    }
    fclose(toPrint);
    system("pause");// windows only.
exit(EXIT_SUCCESS);
}
