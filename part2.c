#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Project.h"



int main(int argc, char* argv[]) {
	FILE* fr = NULL;
	int output = 0;
	int i = 0;
	int j = 0;
	while (i != argc) {
		if (strcmp(argv[i], "-m") == 0) {
			turns = atoi(argv[i + 1]);
		}
		if (strcmp(argv[i], "-x") == 0) {
			output++;
		}
		if (strstr(argv[i], ".txt") != NULL) {
			fr = fopen(argv[i], "r");
			if (fr == NULL) {
				fprintf(stderr, "Unable to open file!");
				exit(0);
			}
		}
		i++;
	}
	if (fr == NULL) {
		fr = stdin;
	}
	parse(fr);//This will do everything from Project Part 1!!!

	update(fr);//This will do everything from Project Part 2!!!

	if (output == 0) {
		printf("\n");
		printf("\n");
		printf("Processed %d moves, all valid\n", j);
		human_readable(&g);
	}
	else {
		printf("\n");
		printf("\n");
		printf("Processed %d moves, all valid\n", j);
		file_input(&g);
	}
	j = 0;
	return 0;
}