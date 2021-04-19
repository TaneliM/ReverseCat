#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#define MAX_PATH 4096

// List node struct
struct lines
{
	char* string;
	struct lines * prev;
	struct lines * next;
};


// Reading input to the list
int read_lines(struct lines** line, FILE* stream) {
	
	char* buffer = NULL;
	size_t n = 0;
	struct lines* lp;
	
	// reading input stream line by line
	while (getline(&buffer, &n , stream) >= 0) {
		if (buffer == NULL) {
			fprintf(stderr, "getline failed\n");
		}
		
		// Allocating memory for the first node
		if (*line == NULL) { 
			if ((*line = lp = malloc(sizeof(struct lines))) == NULL) {
				fprintf(stderr, "malloc failed\n");
				return -1;
			}
			
			lp->prev = lp->next = NULL;
		}
		// Allocating memory for a new node
		else { 
			if ( (lp->next = malloc(sizeof(struct lines))) == NULL) {
				fprintf(stderr, "malloc failed\n");
				return -1;
			}

			lp->next->prev = lp;
			lp = lp->next;
		}
		
		// Allocating memory for the line to store it in the list
		if ( (lp->string = malloc(strlen(buffer) + 1)) == NULL) {
			fprintf(stderr, "malloc failed\n");
			return -1;
		}
		strcpy(lp->string, buffer);
	}
	return 0;
}


// Printing the list in reverce
void print_lines(struct lines * line, FILE * stream) {
	struct lines * lp;
	lp = line;
	
	// Going through the linked list to find the last node
	while (lp->next != NULL) {
		lp = lp->next;
	}
	// Writing the list to the "stream" in reverse
	while (lp != NULL) {
		fprintf(stream, "%s", lp->string);
		lp = lp->prev;
	}
}


// Deleting the linked list
void delete_lines(struct lines * line) {
	struct lines * lp;
	lp = line;
	
	// Freeing allocated memory
	while (lp != NULL) {
		line = lp->next;
		free(lp->string);
		free(lp);
		lp = line;
	}
}


// Handling the arguments
int main(int argc, char** argv) {
	struct lines * line = NULL;
	char *inputFile, *outputFile;
	char inputFilePath[MAX_PATH], outputFilePath[MAX_PATH];
	FILE* inputStream = stdin;
	FILE* outputStream = stdout;
	
	// Checking if argument count is correct
	if (argc > 3) {
		fprintf(stderr, "usage: reverse <input> <output>\n");
		exit(1);
	}
	
	// Use input file if specified
	if (argc > 1) {	
		if (((inputFile = realpath(argv[1], inputFilePath)) == NULL) || ((inputStream = fopen(("%s", argv[1]), "r")) == NULL)) {
			fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
			exit(1);
		}
	}
	
	// Use or create output file if specified
	if (argc > 2) {
		outputFile = realpath(argv[2], outputFilePath);
		
		if (strcmp(basename(inputFile), basename(outputFile)) == 0) {
			fprintf(stderr, "reverse: input and output file must differ\n");
			exit(1);
		}
		
		if ((outputStream = fopen(("%s", argv[2]), "w+")) == NULL) {
			fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
			exit(1);
		}
	}
	
	if (read_lines(&line, inputStream) == -1) {
		exit(1);
	}
		
	print_lines(line, outputStream);
	delete_lines(line);
	fclose(inputStream);
	fclose(outputStream);
	
	return 0;
}


