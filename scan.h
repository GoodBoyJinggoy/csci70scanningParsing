#include "token.h"
#include <stdbool.h>

int openfile(char *filename);
struct token gettoken(bool isScanner);
int getlinenumber();
