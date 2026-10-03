#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <regex.h>
#include "scanparse.h"

int main()
{
	// regex for checking text files
	regex_t regexp;
	int val = regcomp(&regexp, "[a-zA-Z0-9_-].txt$", 0); 
	DIR *direc;
	direc = opendir("./");
	struct dirent *nextfile = readdir(direc);
	while(nextfile != NULL){
		val = regexec(&regexp, nextfile -> d_name, 0, NULL, 0);
		if(val == 0){ // regex match
			char *newFilename = (char *) calloc((strlen(nextfile -> d_name) + 10), sizeof(char));
			char *oldFilename = (char *) calloc(strlen(nextfile -> d_name), sizeof(char));
			oldFilename = strcpy(oldFilename, nextfile -> d_name);
			int i = 0;
			while(oldFilename[i] != '.'){
				newFilename[i] = oldFilename[i];
				i++;
			}
			newFilename[i] = '\0';
			newFilename = strcat(newFilename, "_scan.txt");

			openfile(oldFilename, newFilename);
			struct token t = gettoken(true);
			while(t.id != TokenEOF){
				printtoken(t);
				t = gettoken(true);
			}
			closefiles();
			resetlinenumber();
			free(oldFilename);
			free(newFilename);
		}
		nextfile = readdir(direc);
	}
	return 0;
}
