#include<stdio.h>
#include<regex.h>
#include<dirent.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include"scanparse.h"

struct node* tempStorage; 
struct node* inp;

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
			char *newFilename = malloc((strlen(nextfile -> d_name) + 11) * sizeof(char));
			char *oldFilename = malloc(strlen(nextfile -> d_name) * sizeof(char));
			oldFilename = strcpy(oldFilename, nextfile -> d_name);
			printf("Parsing %s\n", oldFilename);
			int i = 0;
			while(oldFilename[i] != '.'){
				newFilename[i] = oldFilename[i];
				i++;
			}
			newFilename[i] = '\0';
			newFilename = strcat(newFilename, "_parse.txt");
			openfile(oldFilename, newFilename);
			tempStorage = (struct node*) malloc(2 * sizeof(struct node));
			i = 1;
			struct token t = gettoken(false);
			// head
			tempStorage[0].value = t;
			struct node *curr = &tempStorage[0];
			while(t.id != TokenEOF){
				t = gettoken(false);
				tempStorage[i].value = t;
				curr -> next = &tempStorage[i];
				tempStorage[i].linenum = getlinenumber();
				curr = &tempStorage[i];
				i++;
				tempStorage = (struct node*) realloc(tempStorage,(i + 1) * sizeof(struct node));
			}
			
			/*struct token t = gettoken(true);
			while(t.id != TokenEOF){
				printtoken(t);
				t = gettoken(true);
			}
			*/
			inp = &tempStorage[0];
			prg(&inp);
			resetlinenumber();
			if(isSuccessful() == false){
				nextfile = readdir(direc);
				printf("invalid %s\n", oldFilename);
				closefiles();
				free(tempStorage);
				free(oldFilename);
				free(newFilename);
				continue;
			}
			else{
				printf("valid %s\n", oldFilename);
				confirmsuccess(oldFilename);
				closefiles();
				free(tempStorage);
				free(oldFilename);
				free(newFilename);
			}

		}
		nextfile = readdir(direc);
	}
	printf("worked\n");
	return 0;
}
