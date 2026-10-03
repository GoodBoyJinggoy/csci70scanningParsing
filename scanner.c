#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <regex.h>
#include "scanparse.h"

// int main(int argc, char** argv)
int main()
{
	regex_t regexp;
	int val = regcomp(&regexp, "[a-zA-Z0-9_-].txt$", 0); 
	DIR *direc;
	direc = opendir("./");
	struct dirent *nextfile = readdir(direc);
	while(nextfile != NULL){
		val = regexec(&regexp, nextfile -> d_name, 0, NULL, 0);
		if(val == 0){
			char *newFilename = malloc((strlen(nextfile -> d_name) + 10) * sizeof(char));
			char *oldFilename = malloc(strlen(nextfile -> d_name) * sizeof(char));
			oldFilename = strcpy(oldFilename, nextfile -> d_name);
			printf("Scanning %s\n", oldFilename);
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
			free(oldFilename);
			free(newFilename);
		}
		nextfile = readdir(direc);
	}
	printf("worked\n");
	return 0;
	/*
	char filename[50];
	if(argc >= 2){
		strcpy(filename,argv[1]);
	}
	openfile(filename);
	struct token t = gettoken(true);
	while(t.id != TokenEOF)
	{
		t = gettoken(true);
	}*/
}
