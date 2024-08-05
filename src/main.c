#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"


static void printHelp(char* progName) {
	printf("Enter %s -h for help\n", progName);
}

static void compileUnit(FILE* file) {
	printf("Compiling\n");
	TokenList* tokens;
	tokenize(tokens, file);
}

int main(int argc, char *argv[]) {

	if(argc < 2) {
		printHelp(argv[0]);
	}

	// Parse arguments:
	
	int argumentCount = 0;
	for(int i = 1; i < argc; i++) {
		char* arg = argv[i];
		if(arg[0] == '-') {
			if(arg[1] == 'h') {
				printHelp(argv[0]);
			} else {
				printf("option %s\n", arg);
			}
		} else {
			printf("argument %s\n", arg);
			argumentCount++;
		}
	}

	if(argumentCount == 0) {
		printHelp(argv[0]);
	}

	FILE* files[argumentCount];
	for(int i = 0; i < argumentCount; i++) {
		files[i] = NULL;
	}

	int index = 0;
	for(int i = 1; i < argc; i++) {
		char* arg = argv[i];
		if(arg[0] != '-') {
			files[index] = fopen(arg, "r");

			if(files[index] == NULL) {
				fprintf(stderr, "Unable to open file %s\n", arg);
				return 1;
			}

			index++;
		}
	}

	for(int i = 0; i < argumentCount; i++) {
		compileUnit(files[i]);
		fclose(files[i]);
	}

	return 0;
}
