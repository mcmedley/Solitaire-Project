#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Project.h"


int main(int argc, char* argv[]) {
	FILE* fr;
	if (argc >= 2) {//if a file has been passed as an argument
		fr = fopen(argv[1], "r");//input will come from the file given
		if (fr == NULL) {//if the file can't be opened
			printf("Error: unable to unopen file.");
			exit(0);
		}
	}
	else {
		fr = stdin;//use stdin for input instead
	}
	parse(fr);
	printf("Input file is valid\n");
	printf("%d covered cards\n", covered);
	printf("%d stock cards\n", stock);
	printf("%d waste cards\n", waste);
	return 0;
	
}