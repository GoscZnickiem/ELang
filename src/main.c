#include <stdio.h>
#include <stdlib.h>

static void printHelp(char* progName) {
	printf("Enter %s -h for help\n", progName);
}

int main(int argc, char *argv[]) {

	if(argc < 2) {
		printHelp(argv[0]);
	}

	// Parse arguments:
	
	char* filename = argv[1];

	FILE* inputFile = fopen(filename, "r");

	if(inputFile == NULL) {
		fprintf(stderr, "Unable to open file %s\n", filename);
		return 1;
	}

	fclose(inputFile);

	return 0;
}
