#include "token.h"
#include <stdbool.h>

int openfile(char *inputFilename, char *outputFilename);
void closefiles();
void printtoken(struct token t);
struct token gettoken(bool isScanner);
int getlinenumber();
