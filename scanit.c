#include <stdio.h>
#include <string.h>
#include "scan.h"

int main(int argc, char** argv)
{
	char filename[50];
	if(argc >= 2){
		strcpy(filename,argv[1]);
	}
	openfile(filename);
	struct token t = gettoken();
	while(t.id != TokenEOF)
	{
		printf("%s %s\n", tokennames[t.id], t.lexeme);
		t = gettoken();
	}
	return 0;
}
