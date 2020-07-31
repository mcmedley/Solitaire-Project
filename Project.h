#ifndef PROJECT_H
#define PROJECT_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct card {
	int state;
	char suit;
	char value;
};

struct foundations {
	struct card stacks[4][13];
	int suitpos;
	int valuepos;
};

struct tableau {
	struct card columns[7][52];
	int columnpos;

};

struct stock {
	struct card waste[52];
	int wpos;
	struct card stock[52];
	int spos;
};

struct game {
	struct foundations f;
	struct tableau t;
	struct stock s;
	int depth;
	char* move;
};

struct game g;

int covered = 0;
int uncovered = 0;
int stock = 0;
int waste = 0;
int lineNum = 1;

int progress = 0;
char keywords[7][20] = { "RULES:", "turn", "1", "unlimited", "FOUNDATIONS:", "TABLEAU:", "STOCK:" };
char cards[] = {'A', '2', '3', '4' , '5' , '6' , '7' , '8' , '9' , 'T' , 'J' , 'Q' ,'K' };
char suits[] = "cdhs";

//Project Part 1 functions!
void initialize(struct game* test) {
	test->f.suitpos = 0;
	test->f.valuepos = 0;
	test->s.wpos = 0;
	test->s.spos = 0;
	test->t.columnpos = 0;
	test->depth = 1;
	test->move = "";
}

void set_foundation(char* card, struct game* test) {
	struct card c;
	c.value = card[0];
	c.suit = card[1];
	c.state = 0;

	initialize(test);

	if (c.value != '_') {
		while (c.value != cards[test->f.valuepos]) {
			test->f.valuepos++;
		}
	}
	

	while (c.suit != suits[test->f.suitpos]) {
		test->f.suitpos++;
	}

	for (int j = test->f.valuepos; j >= 0; j--) {
		test->f.stacks[test->f.suitpos][j] = c;
		c.value = cards[j - 1];
	}
}

void add_to_column(struct card card, struct game* test, int column) {
	struct card c = card;

	test->t.columns[7-column][test->t.columnpos] = c;
}

void add_to_column2(struct card card, struct game* test, int column, int columnpos) {
	struct card c = card;

	test->t.columns[column-1][columnpos] = c;
}

void add_to_waste(char* card, struct game* test) {
	struct card c;
	c.value = card[0];
	c.suit = card[1];
	c.state = 0;

	test->s.waste[test->s.wpos] = c;
	test->s.wpos++;
}

void add_to_stock(char* card, struct game* test) {
	struct card c;
	c.value = card[0];
	c.suit = card[1];
	c.state = 1;

	test->s.stock[test->s.spos] = c;
	test->s.spos++;
}


void parse(FILE* fr) {
	char line[100];
	fgets(line, 100, fr);
	int pos = 0;
	int fcheck = 0;
	int tcheck = 0;
	int scheck = 0;
	int cov = 0;
	int column = 0;
	while (strcmp(line, "MOVES:\n") != 0 && strcmp(line, "MOVES:\r\n") != 0) {
		char* temp = strtok(line, " \r\n");
		while (temp != NULL) {
			if (temp[0] == '#') {//check for comments
				break;
			}
			if (fcheck == 0 && tcheck == 0 && scheck == 0) {//if not currently checking the foundation cards
				if (strcmp(temp, keywords[progress]) == 0) {//check for next keyword
					progress++;
				}
				else {
					printf("Invalid input: word didn't match keyword %s\ntemp is %s", keywords[progress], temp);
					exit(0);
				}
			}

			if (strcmp(temp, "FOUNDATIONS:") == 0) {//start checking for foundation cards
				fcheck++;
			}
			else if (fcheck == 1 && pos <= 3) {//checking for foundation cards
				if (temp[0] != '_') {
					if (strchr(cards, temp[0]) == NULL || temp[1] != suits[pos]) {
						printf("Invalid input: invalid foundation");
						exit(0);
					}
				}
				if (pos == 3) {//last foundation card is checked, stop checking for foundation cards
					fcheck--;
				}
				set_foundation(temp, &g);
				pos++;
			}

			if (strcmp(temp, "TABLEAU:") == 0) {
				tcheck++;
			}
			else if (tcheck == 1 && column <= 7 && cov == 0) {//begin checking for tableau cards
				if (temp[0] != '|') {//count covered cards
					if (strchr(cards, temp[0]) == NULL || strchr(suits, temp[1]) == NULL) {
						printf("Invalid input: invalid covered card %d", covered);
						exit(0);
					}
					covered++;
					struct card c;
					c.value = temp[0];
					c.suit = temp[1];
					c.state = 1;
					add_to_column(c, &g, column);
					g.t.columnpos++;
				}
				else {//count uncovered cards
					cov++;
				}
			}
			else if (tcheck == 1 && column <= 7 && cov == 1) {
				if (strchr(cards, temp[0]) == NULL || strchr(suits, temp[1]) == NULL) {
					printf("Invalid input: invalid uncovered card %d", uncovered);
					exit(0);
				}
				uncovered++;
				struct card c;
				c.value = temp[0];
				c.suit = temp[1];
				c.state = 0;
				add_to_column(c, &g, column);
				g.t.columnpos++;
			}

			if (strcmp(temp, "STOCK:") == 0) {
				scheck++;
			}
			else if (scheck == 1 && cov == 0) {//begin checking for tableau cards
				if (strcmp(temp, "|") != 0) {//count covered cards
					if (strchr(cards, temp[0]) == NULL || strchr(suits, temp[1]) == NULL) {
						printf("Invalid input: invalid waste card");
						exit(0);
					}
					waste++;
					add_to_waste(temp, &g);
				}
				else {
					cov++;
				}
			}
			else if (scheck == 1 && cov == 1) {
				if (strchr(cards, temp[0]) == NULL || strchr(suits, temp[1]) == NULL) {
					printf("Invalid input: invalid stock card");
					exit(0);
				}
				stock++;
				add_to_stock(temp, &g);
			}


			temp = strtok(NULL, " \r\n");//get new word in line
		}
		if (tcheck == 1) {
			column++;
			cov = 0;
			g.t.columnpos = 0;
		}
		if (column == 8) {
			tcheck--;
			g.t.columnpos = 0;
			cov = 0;
		}
		fgets(line, 100, fr);//get next line of input
		lineNum++;
	}
}


//Project Part 2 functions!

void human_readable(struct game* test) {
	printf("Foundations\n");
	int i = 0;
	while (test->f.stacks[0][i].value != '\0') {
		i++;
	}
	int j = 0;
	while (test->f.stacks[1][j].value != '\0') {
		j++;
	}
	int k = 0;
	while (test->f.stacks[2][k].value != '\0') {
		k++;
	}
	int l = 0;
	while (test->f.stacks[3][l].value != '\0') {
		l++;
	}
	printf("%c%c %c%c %c%c %c%c\n", test->f.stacks[0][i - 1].value, test->f.stacks[0][i - 1].suit, test->f.stacks[1][j - 1].value, test->f.stacks[1][j - 1].suit, test->f.stacks[2][k - 1].value, test->f.stacks[2][k - 1].suit, test->f.stacks[3][l - 1].value, test->f.stacks[3][l - 1].suit);
	printf("Tableau\n");

	int longest = 0;
	for (int i = 0; i < 7; i++) {
		int j = 0;
		while (test->t.columns[i][j].value != '\0') {
			j++;
		}
		j--;
		if (j > longest) {
			longest = j;
		}

	}
	for (int j = 0; j <= longest; j++) {
		for (int i = 0; i < 7; i++) {
			if (test->t.columns[i][j].state == 1) {
				printf("%c%c ", '#', '#');
			}
			else if (test->t.columns[i][j].value == '\0') {
				printf("%c%c ", '.', '.');
			}
			else {
				printf("%c%c ", test->t.columns[i][j].value, test->t.columns[i][j].suit);
			}

		}
		printf("\n");
	}
	printf("Waste top\n");
	if (test->s.wpos != 0) {
		printf("%c%c\n", test->s.waste[test->s.wpos - 1].value, test->s.waste[test->s.wpos - 1].suit);
	}
	else {
		printf("(empty)\n");
	}
}

void file_input(struct game* test) {
	printf("RULES:\n");
	printf("turn 1\n");
	printf("unlimited\n");
	printf("FOUNDATIONS:\n");
	int i = 0;
	while (test->f.stacks[0][i].value != '\0') {
		i++;
	}
	int j = 0;
	while (test->f.stacks[1][j].value != '\0') {
		j++;
	}
	int k = 0;
	while (test->f.stacks[2][k].value != '\0') {
		k++;
	}
	int l = 0;
	while (test->f.stacks[3][l].value != '\0') {
		l++;
	}
	printf("%c%c\n", test->f.stacks[0][i - 1].value, test->f.stacks[0][i - 1].suit);
	printf("%c%c\n", test->f.stacks[1][j - 1].value, test->f.stacks[1][j - 1].suit);
	printf("%c%c\n", test->f.stacks[2][k - 1].value, test->f.stacks[2][k - 1].suit);
	printf("%c%c\n", test->f.stacks[3][l - 1].value, test->f.stacks[3][l - 1].suit);
	printf("TABLEAU:\n");
	for (int i = 6; i >= 0; i--) {
		int j = 0;
		int change = 0;
		if (test->t.columns[i][0].value == '\0') {
			printf("|\n");
		}
		else {
			while (test->t.columns[i][j].value != '\0') {
				if (test->t.columns[i][j].state == 0 && change == 0) {
					printf("| ");
					change = 1;
				}
				printf("%c%c ", test->t.columns[i][j].value, test->t.columns[i][j].suit);
				j++;
			}
			if (change == 0) {
				printf("| ");
			}
			printf("\n");
		}
	}
	printf("STOCK:\n");
	int m = 0;
	while (test->s.waste[m].value != '\0') {
		printf("%c%c ", test->s.waste[m].value, test->s.waste[m].suit);
		m++;
	}
	printf("| ");
	m = 0;
	while (test->s.stock[m].value != '\0') {
		printf("%c%c ", test->s.stock[m].value, test->s.stock[m].suit);
		m++;
	}
	printf("\n");
	printf("MOVES:\n");
}
void add_foundation(struct card c, int suitposition, struct game* test) {
	int i = 0;
	while (test->f.stacks[suitposition][i].value != '\0') {
		i++;
	}
	test->f.stacks[suitposition][i] = c;
}

int waste_to_foundations(int moveNum, struct game* test) {
	if (test->s.waste[0].value == '\0') {
		return 0;
	}
	int j = 0;
	while (test->s.waste[test->s.wpos - 1].suit != suits[j]) {//find the correct foundation
		j++;
	}
	if (test->s.waste[test->s.wpos - 1].value == 'A') {//if top waste card is an ace
		add_foundation(test->s.waste[test->s.wpos - 1], j, test);//add to proper foundation
		test->s.waste[test->s.wpos - 1].value = '\0';
		test->s.waste[test->s.wpos - 1].suit = '\0';
		test->s.waste[test->s.wpos - 1].state = 0;
		test->s.wpos--;
	}
	else {
		int k = 0;
		while (test->s.waste[test->s.wpos - 1].value != cards[k]) {
			k++;
		}
		int l = 0;
		while (test->f.stacks[j][l].value != '\0') {
			l++;
		}

		int m = 0;
		if (test->f.stacks[j][l - 1].value != '_') {
			while (test->f.stacks[j][l - 1].value != cards[m]) {//find the index of its value(rank)
				m++;
			}
		}

		if (test->f.stacks[j][l - 1].value != '_' && k - m != 1) {
			return 0;
		}
		else if (test->f.stacks[j][l - 1].value == '_') {
			if (k == 0) {
				add_foundation(test->s.waste[test->s.wpos - 1], j, test);
				test->s.waste[test->s.wpos - 1].value = '\0';
				test->s.waste[test->s.wpos - 1].suit = '\0';
				test->s.waste[test->s.wpos - 1].state = 0;
				test->s.wpos--;
				return 1;
			}
			else {
				return 0;
			}
		}
		else {
			add_foundation(test->s.waste[test->s.wpos - 1], j, test);
			test->s.waste[test->s.wpos - 1].value = '\0';
			test->s.waste[test->s.wpos - 1].suit = '\0';
			test->s.waste[test->s.wpos - 1].state = 0;
			test->s.wpos--;
			return 1;
		}
	}
	return 1;
}

int tableau_to_foundations(int moveNum, int col, struct game* test) {
	while (test->t.columns[col - 1][test->t.columnpos].value != '\0') {
		test->t.columnpos++;
	}
	test->t.columnpos--;
	if (test->t.columns[col - 1][0].value == '\0') {
		test->t.columnpos = 0;
		return 0;
	}
	else {
		int j = 0;
		while (test->t.columns[col - 1][test->t.columnpos].suit != suits[j]) {//find the correct foundation
			j++;
		}

		int k = 0;
		while (test->t.columns[col - 1][test->t.columnpos].value != cards[k]) {//index of current card's value
			k++;
		}
		int l = 0;
		while (test->f.stacks[j][l].value != '\0') {//go to top card in foundation stack
			l++;
		}

		int m = 0;
		if (test->f.stacks[j][l-1].value != '_') {
			while (test->f.stacks[j][l - 1].value != cards[m]) {//find the index of its value(rank)
				m++;
			}
		}


		if (test->f.stacks[j][l-1].value != '_' && k - m != 1) {//compare values
			test->t.columnpos = 0;
			return 0;
		}
		else if (test->f.stacks[j][l - 1].value == '_') {
			if (k == 0) {
				add_foundation(test->t.columns[col - 1][test->t.columnpos], j, test);
				test->t.columns[col - 1][test->t.columnpos].value = '\0';
				test->t.columns[col - 1][test->t.columnpos].suit = '\0';
				test->t.columns[col - 1][test->t.columnpos].state = 0;
				if (test->t.columnpos > 0) {
					test->t.columnpos--;
					test->t.columns[col - 1][test->t.columnpos].state = 0;
				}
				test->t.columnpos = 0;
				return 1;
			}
			else {
				test->t.columnpos = 0;
				return 0;
			}
		}

		else {
			add_foundation(test->t.columns[col - 1][test->t.columnpos], j, test);
			test->t.columns[col - 1][test->t.columnpos].value = '\0';
			test->t.columns[col - 1][test->t.columnpos].suit = '\0';
			test->t.columns[col - 1][test->t.columnpos].state = 0;
			if (test->t.columnpos > 0) {
				test->t.columnpos--;
				test->t.columns[col - 1][test->t.columnpos].state = 0;
			}
			test->t.columnpos = 0;
			return 1;
		}

	}
	test->t.columnpos = 0;
	return 1;
}

int tableau_to_tableau(int moveNum, int col1, int col2, struct game* test) {
	int columnpos2 = 0;
	test->t.columnpos = 0;
	if (test->t.columns[col1 - 1][0].value == '\0') {
		return 0;
	}
	while (test->t.columns[col1 - 1][test->t.columnpos].state != 0) {//advance to the first uncovered card in the column
		test->t.columnpos++;
	}
	while (test->t.columns[col2 - 1][columnpos2].value != '\0') {
		columnpos2++;
	}
	
	int valpos2 = 0;
	while (test->t.columns[col2 - 1][columnpos2-1].value != cards[valpos2]) {
		valpos2++;
	}
	if (test->t.columns[col1 - 1][test->t.columnpos].value == 'K') {//if first column card is a King
		if (columnpos2 == 0) {//and the second column is empty
			int i = test->t.columnpos;
			while (test->t.columns[col1 - 1][i].value != '\0') {
				struct card c;
				c.value = test->t.columns[col1 - 1][i].value;
				c.suit = test->t.columns[col1 - 1][i].suit;
				c.state = test->t.columns[col1 - 1][i].state;

				test->t.columns[col2 - 1][i - test->t.columnpos] = c;//add to second column

				test->t.columns[col1 - 1][i].value = '\0';
				test->t.columns[col1 - 1][i].suit = '\0';
				test->t.columns[col1 - 1][i].state = 0;

				i++;
			}
			test->t.columnpos = 0;
			return 1;
		}
	}
	if (test->t.columns[col1 - 1][test->t.columnpos].value != 'K' && columnpos2 == 0) {
		test->t.columnpos = 0;
		return 0;
	}
	int newpos = test->t.columnpos;
	int works = 0;
	while (test->t.columns[col1 - 1][newpos].value != '\0') {
		int valpos = 0;
		while (test->t.columns[col1 - 1][newpos].value != cards[valpos]) {
			valpos++;
		}
		int j = 0;
		while (test->t.columns[col1 - 1][newpos].suit != suits[j]) {//find the correct suit of first column card
			j++;
		}
		int k = 0;
		while (test->t.columns[col2 - 1][columnpos2-1].suit != suits[k]) {//find the correct suit of second column card
			k++;
		}

		if (j == 0 || j == 3) {
			if ((k == 1 || k == 2) && valpos2 - valpos == 1) {

				int i = columnpos2;
				int i2 = newpos;
				while (test->t.columns[col1 - 1][i2].value != '\0') {//add all cards in that column
					struct card c;
					c.value = test->t.columns[col1 - 1][i2].value;
					c.suit = test->t.columns[col1 - 1][i2].suit;
					c.state = test->t.columns[col1 - 1][i2].state;

					test->t.columns[col2 - 1][i] = c;//add to second column

					test->t.columns[col1 - 1][i2].value = '\0';
					test->t.columns[col1 - 1][i2].suit = '\0';
					test->t.columns[col1 - 1][i2].state = 0;

					i++;
					i2++;
				}
				works = 1;
				break;
			}
		}
		else {
			if ((k == 0 || k == 3) && valpos2 - valpos == 1) {
				int i = columnpos2;
				int i2 = newpos;
				while (test->t.columns[col1 - 1][i2].value != '\0') {//add all cards in that column
					struct card c;
					c.value = test->t.columns[col1 - 1][i2].value;
					c.suit = test->t.columns[col1 - 1][i2].suit;
					c.state = test->t.columns[col1 - 1][i2].state;

					test->t.columns[col2 - 1][i] = c;//add to second column

					test->t.columns[col1 - 1][i2].value = '\0';
					test->t.columns[col1 - 1][i2].suit = '\0';
					test->t.columns[col1 - 1][i2].state = 0;

					i++;
					i2++;
				}
				works = 1;
				break;
			}
		}
		newpos++;

	}
	if (works == 0) {
		test->t.columnpos = 0;
		return 0;
	}
	if (test->t.columns[col1 - 1][newpos-1].state == 1) {
		test->t.columns[col1 - 1][newpos-1].state = 0;
	}
	test->t.columnpos = 0;
	columnpos2 = 0;
	return 1;
}


int waste_to_tableau(int moveNum, int col, struct game* test) {
	if (test->s.waste[0].value == '\0') {
		/*printf("Move %d is illegal: %s%d", moveNum, "w->", col);
		printf("\n");
		file_input(test);
		exit(0);*/
		return 0;
	}
	int j = 0;
	while (test->s.waste[test->s.wpos - 1].suit != suits[j]) {//find the correct foundation
		j++;
	}

	if (test->s.waste[test->s.wpos - 1].value == 'K') {//if the top waste card is a K
		if (test->t.columns[col - 1][0].value == '\0') {//if the destination column is empty
			struct card c;
			c.value = 'K';
			c.suit = test->s.waste[test->s.wpos - 1].suit;
			c.state = 0;
			test->t.columns[col - 1][0] = c;//add that card to the empty column

			test->s.waste[test->s.wpos - 1].value = '\0';
			test->s.waste[test->s.wpos - 1].suit = '\0';
			test->s.waste[test->s.wpos - 1].state = 0;
			test->s.wpos--;

		}
		else {
			/*printf("Move %d is illegal: %s%d", moveNum, "w->", col);
			printf("\n");
			file_input(test);
			exit(0);*/
			return 0;
		}
	}
	else {
		while (test->t.columns[col - 1][test->t.columnpos].value != '\0') {
			test->t.columnpos++;
		}
		int k = 0;
		while (test->t.columns[col - 1][test->t.columnpos - 1].suit != suits[k]) {
			k++;
		}

		int valpos = 0;
		while (test->s.waste[test->s.wpos - 1].value != cards[valpos]) {
			valpos++;
		}
		int valpos2 = 0;
		while (test->t.columns[col - 1][test->t.columnpos - 1].value != cards[valpos2]) {
			valpos2++;
		}
		if (j == 0 || j == 3) {
			if ((k == 1 || k == 2) && valpos2 - valpos == 1) {
				struct card c;
				c.value = test->s.waste[test->s.wpos - 1].value;
				c.suit = test->s.waste[test->s.wpos - 1].suit;
				c.state = 0;

				test->t.columns[col - 1][test->t.columnpos] = c;

				test->s.waste[test->s.wpos - 1].value = '\0';
				test->s.waste[test->s.wpos - 1].suit = '\0';
				test->s.waste[test->s.wpos - 1].state = 0;
				test->s.wpos--;
				test->t.columnpos = 0;
			}
			else {
				/*printf("Move %d is illegal: %s%d", moveNum, "w->", col);
				human_readable(test);
				exit(0);*/
				return 0;
			}
		}
		if (j == 1 || j == 2) {
			if ((k == 0 || k == 3) && valpos2 - valpos == 1) {
				struct card c;
				c.value = test->s.waste[test->s.wpos - 1].value;
				c.suit = test->s.waste[test->s.wpos - 1].suit;
				c.state = 0;

				test->t.columns[col - 1][test->t.columnpos] = c;

				test->s.waste[test->s.wpos - 1].value = '\0';
				test->s.waste[test->s.wpos - 1].suit = '\0';
				test->s.waste[test->s.wpos - 1].state = 0;
				test->s.wpos--;
				test->t.columnpos = 0;
			}
			else {
				/*printf("Move %d is illegal: %s%d", moveNum, "w->", col);
				printf("\n");
				file_input(test);
				exit(0);*/
				return 0;
			}
		}
	}
	return 1;
}

int reset_stock(struct game* test) {
	if (test->s.stock[0].value != '\0') {
		/*printf("Move %d is illegal: r", j);
		printf("\n");
		file_input(&g);
		exit(0);*/
		return 0;
	}
	for (int i = 0; i <= test->s.wpos; i++) {
		struct card c;
		c.value = test->s.waste[i].value;
		c.suit = test->s.waste[i].suit;
		c.state = 1;
		test->s.stock[i] = c;
	}
	for (int i = 0; i <= test->s.wpos; i++) {
		test->s.waste[i].value = '\0';
		test->s.waste[i].suit = '\0';
		test->s.waste[i].state = 0;
	}
	test->s.wpos = 0;
	return 1;

}

int new_waste(struct game* test) {
	if (test->s.stock[0].value == '\0') {
		/*printf("Move %d is illegal: .", j);
		printf("\n");
		file_input(&g);
		exit(0);*/
		return 0;
	}
	struct card c;
	c.value = test->s.stock[0].value;
	c.suit = test->s.stock[0].suit;
	c.state = 0;
	test->s.waste[test->s.wpos] = c;
	test->s.wpos++;

	for (int i = 0; i < (sizeof(test->s.stock) / sizeof(test->s.stock[0])); i++) {
		test->s.stock[i] = test->s.stock[i + 1];
	}
	return 1;
}

int turns = 99999;
int j = 0;
void update(FILE* fr) {
	char line[100];
	char numbers[] = "1234567";
	fgets(line, 100, fr);
	lineNum++;
	char done = 0;
	char* temp = strtok(line, " \r\n");
	if (temp != NULL) {
		if (strcmp(temp, "MOVES:") == 0) {
			done = 1;
		}
	}
	while (done == 0 && j < turns) {

		while (temp == NULL || temp[0] == '#') {
			fgets(line, 100, fr);
			if (line[0] == '\r') {
				done = 1;
				break;
			}
			temp = strtok(line, " \r\n");
			lineNum++;
		}
		if (done == 1) {
			continue;
		}

		if (strlen(temp) != 4 && strlen(temp) != 1) {
			fprintf(stderr, "Invalid input on line %d", lineNum);
			exit(0);
		}
		else if (strlen(temp) == 4) {//check for proper src->dest format
			if (strchr(numbers, temp[0]) == NULL && temp[0] != 'w') {
				fprintf(stderr, "Invalid input on line %d", lineNum);
				exit(0);
			}
			if (temp[1] != '-' || temp[2] != '>') {
				fprintf(stderr, "Invalid input on line %d", lineNum);
				exit(0);
			}
			if (strchr(numbers, temp[3]) == NULL && temp[3] != 'f') {
				fprintf(stderr, "Invalid input on line %d", lineNum);
				exit(0);
			}

			if (strcmp(temp, "w->f") == 0) {//waste to foundations
				waste_to_foundations(j, &g);
			}

			if (strchr(numbers, temp[0]) != NULL && strchr(numbers, temp[3]) != NULL) {//tableau to tableau
				tableau_to_tableau(j, atoi(&temp[0]), atoi(&temp[3]), &g);
			}
			if (strchr(numbers, temp[0]) != NULL && temp[3] == 'f') {//tableau to foundations
				tableau_to_foundations(j, atoi(&temp[0]), &g);
			}

			if (strchr(numbers, temp[3]) != NULL && temp[0] == 'w') {//waste to tableau
				waste_to_tableau(j, atoi(&temp[3]), &g);
			}
		}
		else {
			if (temp[0] == '.') {//Get a new waste card
				new_waste(&g);
			}
			if (temp[0] == 'r') {//reset stock pile
				reset_stock(&g);
			}
		}
		temp = strtok(NULL, " \r\n");
		j++;
	}
}

#endif
