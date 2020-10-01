#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {printf("incorrect arguments!"); exit(EXIT_FAILURE);}
    int countLeft = 0, countRight = 0, generalCount = 0;
    bool isBalanced = true;
    char c;
    FILE* checkFile = fopen(argv[1],"rt");
    if (!checkFile) { printf("file read error!"); exit(EXIT_FAILURE); }

    while ( (c = fgetc(checkFile)) != EOF ) {
        if (c == '{') { ++countLeft; ++generalCount; }
        if (c == '}') { ++countRight; --generalCount; }
        if (generalCount < 0) isBalanced = false;
    }

    if (countLeft == countRight) {
        printf("number of '{' characters in argument EQUALS number of '}'");
        if (isBalanced == true) printf(" and is balanced\n");
        else printf(" BUT is not a balanced series of parentheses\n");
    }
    else
    printf("number of '{' characters does NOT EQUAL number of '}'\n");

    if ( (fclose(checkFile)) == EOF ) {
        printf("file close error!");
        exit(EXIT_FAILURE);
    }
    system("pause");
exit(EXIT_SUCCESS);
}
