#include "include_header.h"
#include "parse.h"
#include "count.h"

void print_usage(){
	printf("Usage: efg -i input_file -b bound -s symbol_to_count\n");
}

void main(int argc, char** argv){
	char* filename = NULL;
	int bound = -1;
	char symbol_to_count = -1;

	for (int i=1; i<argc; i++){
		if (strcmp(argv[i], "-i") == 0 && argc > i+1){
			filename = malloc(sizeof(char) * strlen(argv[i+1]));
			strcpy(filename, argv[i+1]);
		}
		else if (strcmp(argv[i], "-b") == 0 && argc > i+1){
			bound = atoi(argv[i+1]);
		}
		else if (strcmp(argv[i], "-s") == 0 && argc > i+1){
			symbol_to_count = argv[i+1][0];
		}
	}

	// parse input file
	if (filename){
		if (parse(filename) == 1) return;
	}
	else{
		printf("Error: input file is not specified.\n");
		print_usage();
		return;
	}

	// check parameters
	if (bound == -1){
		printf("Error: bound is not specified.\n");
		print_usage();
		return;
	}

	if (symbol_to_count == -1){
		printf("Error: symbol to count is not specified.\n");
		print_usage();
		return;
	}

	// count models
	printf("Number of strings of symbol [%c] with length <= %d is: %d\n", symbol_to_count, bound, count(symbol_to_count, bound));

	free(filename);
	return;
}