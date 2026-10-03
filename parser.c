#include<stdio.h>
#include<regex.h>
#include<dirent.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include"scanparse.h"
// Maximum number of tokens that can be parsed
#define MAXTOKENS 5000
struct node* inp;

int main()
{
	// regex for checking for text files
	regex_t regexp;
	int val = regcomp(&regexp, "[a-zA-Z0-9_-].txt$", 0); 
	DIR *direc;
	direc = opendir("./");
	struct dirent *nextfile = readdir(direc);

	while(nextfile != NULL){
		val = regexec(&regexp, nextfile -> d_name, 0, NULL, 0);
		if(val == 0){ // regex match
			// tempStorage is an array that contains the nodes of a linked list of tokens
			struct node tempStorage[MAXTOKENS]; 
			char *newFilename = (char *) calloc((strlen(nextfile -> d_name) + 11), sizeof(char));
			char *oldFilename = (char *) calloc(strlen(nextfile -> d_name), sizeof(char));
			oldFilename = strcpy(oldFilename, nextfile -> d_name);
			int i = 0;
			while(oldFilename[i] != '.'){
				newFilename[i] = oldFilename[i];
				i++;
			}
			newFilename[i] = '\0';
			newFilename = strcat(newFilename, "_parse.txt");
			openfile(oldFilename, newFilename);
			i = 1;
			struct token t = gettoken(false);
			// head of the linked list
			tempStorage[0].value = t;
			// inserting new nodes in this linked list involves creating a new node containing the next 
			// token, then adding a pointer from the "tail" to the new node
			struct node *curr = &tempStorage[0];
			while(t.id != TokenEOF){
				t = gettoken(false);
				tempStorage[i].value = t;
				curr -> next = &tempStorage[i];
				tempStorage[i].linenum = getlinenumber();
				curr = &tempStorage[i];
				i++;
			}
			
			// calling the parser; start with a pointer to the head
			inp = &tempStorage[0];
			prg(&inp);
			// reset the line number variable for next file to be parsed
			resetlinenumber();
			if(isSuccessful() == false){
				nextfile = readdir(direc);
				closefiles();
				free(oldFilename);
				free(newFilename);
				continue;
			}
			else{
				confirmsuccess(oldFilename);
				closefiles();
				free(oldFilename);
				free(newFilename);
			}

		}
		// read next file
		nextfile = readdir(direc);
	}
	return 0;
}
