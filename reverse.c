#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 1024


// List node struct
struct lines
{
	char* string;
	struct lines * prev;
	struct lines * next;
};


// Reading input to the list
int read_lines(struct lines** line, FILE* stream) {
	char tmp[MAXLEN];
	struct lines* lp;
	
	while (fgets(tmp, MAXLEN, stream) != NULL) {
		
		if (*line == NULL) {
			if ((*line = lp = malloc(sizeof(struct lines))) == NULL) {
				fprintf(stderr, "Malloc failed.\n");
				return -1;
			}
			
			lp->prev = lp->next = NULL;
			
		}
		else {
			if ( (lp->next = malloc(sizeof(struct lines))) == NULL) {
				fprintf(stderr, "Malloc failed.\n");
				return -1;
			}

			lp->next->prev = lp;
			lp = lp->next;
			
		}
		
		if ( (lp->string = malloc(strlen(tmp) + 1)) == NULL) {
			fprintf(stderr, "Malloc failed.\n");
			return -1;
		}
		strcpy(lp->string, tmp);
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
	FILE* input = stdin;
	FILE* output = stdout;
	
	// Chenking argument count
	if (argc > 3) {
		fprintf(stderr, "usage: reverse <input> <output>");
		exit(1);
	}
	
	// Using input file if specified
	if (argc > 1) {
		if ((input = fopen(("%s", argv[1]), "r")) == NULL) {
			fprintf(stderr, "error: Cannot open file '%s'., argv[1]\n");
			exit(1);
		}
	}
	
	// Using output file is specified
	if (argc > 2) {
		if (strcmp(argv[1], argv[2]) == 0) {
			fprintf(stderr, "Input and output files must differ.\n");
			exit(1);
		}
		
		if ((output = fopen(("%s", argv[2]), "w")) == NULL) {
			fprintf(stderr, "error: Cannot open file '%s'.\n", argv[2]);
			exit(1);
		}
	}
	
	if (read_lines(&line, input) == -1)
		exit(1);
		
	print_lines(line, output);
	delete_lines(line);
	fclose(input);
	fclose(output);
	return 0;
}


