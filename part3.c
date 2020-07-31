#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include "Project2.h"

int r = 0;
int total = 0;
int winnable = 0;
int p = 0;//used for -v switch

char* reset = "r";//reset stock
char* new_waste2 = ".";//get new waste card
char* moves1[7][6] = {{"1->2", "1->3", "1->4", "1->5", "1->6", "1->7"},//tableau to tableau
					 {"2->1", "2->3", "2->4", "2->5", "2->6", "2->7"},
					 {"3->1", "3->2", "3->4", "3->5", "3->6", "3->7"},
					 {"4->1", "4->2", "4->3", "4->5", "4->6", "4->7"},
					 {"5->1", "5->2", "5->3", "5->4", "5->6", "5->7"},
					 {"6->1", "6->2", "6->3", "6->4", "6->5", "6->7"},
					 {"7->1", "7->2", "7->3", "7->4", "7->5", "7->6"}};
char* wastef = "w->f";//waste to foundations
char* moves2[] = { "1->f", "2->f", "3->f", "4->f", "5->f", "6->f", "7->f" };//tableau to foundations
char* moves3[] = { "w->1", "w->2", "w->3", "w->4", "w->5", "w->6", "w->7" };//waste to tableau




int cindex = 0;
char* combo[100000];//many winning combinations may be stored here

int win(struct game* test, int limit) {
	int pass1 = 0;//becomes 1 if there are no covered cards in this configuration
	int pass2 = 0;//becomes 1 if there are no stock cards in this configuration
	int pass3 = 0;//becomes 1 if there are less than 2 waste cards in this configuration

	int v1 = 0;//these are used to evaluate a winning path at the end of the function
	int v2 = 0;
	int v3 = 0;
	int v4 = 0;
	int v5 = 0;
	int v6 = 0;
	
	for (int j = 0; j < 7; j++) {
		int k = 0;
		while (test->t.columns[j][k].value != '\0') {
			if (test->t.columns[j][k].state == 1) {
				j = 7;
				break;
			}
			k++;
		}

		if (j == 6) {//no covered cards
			pass1 = 1;
		}
	}
	
	int i = 0;
	while (test->s.stock[i].value != '\0') {
		i++;
	}
	if (i == 0) {//no stock cards
		pass2 = 1;
	}

	int j = 0;
	while (test->s.waste[j].value != '\0') {
		j++;
	}
	if (j < 2) {//less than 2 waste cards
		pass3 = 1;
	}




	if(test->depth > limit) {
		if (pass1 == 1 && pass2 == 1 && pass3 == 1) {
			winnable = 1;
			combo[cindex] = test->move;
			cindex++;
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		total++;
		if (p == 1) {//only displays progress if -v switch is used
			if (total % 1000000 == 0 && total > 0) {
				printf("%d,000,000 configurations checked so far\n", total / 1000000);
			}
		}
		struct game temp6;
		temp6.f = test->f;//deep copy
		temp6.t = test->t;
		temp6.s = test->s;
		temp6.depth = (test->depth) + 1;
		temp6.move = "empty";
		if (new_waste(&temp6) == 1) {
			//total++;
			temp6.move = ".";
			if (win(&temp6, limit) == 1) {
				v6 = 1;
			}
		}

		struct game temp5;
		temp5.f = test->f;//deep copy
		temp5.t = test->t;
		temp5.s = test->s;
		temp5.depth = (test->depth) + 1;
		temp5.move = "empty";
		if (reset_stock(&temp5) == 1) {
			//total++;
			temp5.move = "r";
			if (win(&temp5, limit) == 1) {
				v5 = 1;
			}
		}
		struct game temp2;
		for (int i = 0; i <= 6; i++) {//get valid tableau to foundations moves
			temp2.f = test->f;//deep copy
			temp2.t = test->t;
			temp2.s = test->s;
			temp2.depth = (test->depth) + 1;
			temp2.move = "empty";
			if (tableau_to_foundations(r, atoi(&moves2[i][0]), &temp2) == 1) {
				temp2.move = moves2[i];
				//total++;
				if (win(&temp2, limit) == 1) {
					v2 = 1;
				}
			}
		}

		struct game temp;
		for (int i = 0; i <= 6; i++) {//get valid tableau to tableau moves
			for (int j = 0; j <= 5; j++) {
				temp.f = test->f;//deep copy
				temp.t = test->t;
				temp.s = test->s;
				temp.depth = (test->depth) + 1;
				temp.move = "empty";
				if (tableau_to_tableau(r, atoi(&moves1[i][j][0]), atoi(&moves1[i][j][3]), &temp) == 1) {
					temp.move = moves1[i][j];
					//total++;
					if (win(&temp, limit) == 1) {
						v1 = 1;
					}
				}
			}
		}


		struct game temp4;
		temp4.f = test->f;//deep copy
		temp4.t = test->t;
		temp4.s = test->s;
		temp4.depth = (test->depth) + 1;
		temp4.move = "empty";
		if (waste_to_foundations(r, &temp4) == 1) {//get waste to foundations move
			//total++;
			temp4.move = "w->f";
			if (win(&temp4, limit) == 1) {
				v4 = 1;
			}
		}

		struct game temp3;
		for (int i = 0; i <= 6; i++) {//waste to tableau
			temp3.f = test->f;//deep copy
			temp3.t = test->t;
			temp3.s = test->s;
			temp3.depth = (test->depth) + 1;
			temp3.move = "empty";
			if (waste_to_tableau(r, atoi(&moves3[i][3]), &temp3) == 1) {
				temp3.move = moves3[i];
				//total++;
				if (win(&temp3, limit) == 1) {
					v3 = 1;
				}
			}
		}

		if ((pass1 == 1 && pass2 == 1 && pass3 == 1) || v6 == 1 || v5 == 1 || v2 == 1 || v1 == 1 || v4 == 1 || v3 == 1) {
			winnable = 1;
			combo[cindex] = test->move;
			cindex++;
			return 1;
		}
		return 0;
	}

}

int main(int argc, char* argv[]) {
	FILE* fr = NULL;
	int limit = 1000;
	int i = 0;
	while (i != argc) {
		if (strcmp(argv[i], "-m") == 0) {// -m switch
			limit = atoi(argv[i + 1]);
		}
		if (strcmp(argv[i], "-v") == 0) {// -v switch
			p = 1;
		}
		if (strstr(argv[i], ".txt") != NULL) {//reads from a file
			fr = fopen(argv[i], "r");
			if (fr == NULL) {
				fprintf(stderr, "Unable to open file!");
				exit(0);
			}
		}
		i++;
	}
	if (fr == NULL) {
		fr = stdin;//reads from stdin if no filename is present
	}
	parse(fr);//executes part1
	update(fr);//executes part2
	
	printf("\n");
	printf("Using DFS search");
	if (p == 1) {
		printf(", progress will be displayed for every 1,000,000 configurations\n");
	}
	else {
		printf(", progress will not be displayed\n");
	}

	win(&g, limit);//recursive function

	setlocale(LC_NUMERIC, "");
	printf("%'d configurations checked.\n", total);
	if (winnable == 0) {
		printf("# Game is not winnable within %d moves\n", limit);
	}
	else {
		printf("# Game is winnable in %d moves\n", limit);
		for (int i = 1; i <= limit+1; i++) {
			printf("%s\n", combo[cindex-i]);//output displayed to stdout
		}
	}
	return 0;
}