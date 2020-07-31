#include <ncurses.h>
#include <string.h>
#include <locale.h>
#include "Project2.h"


char* filename;


void display_suit(int pos1, int pos2) {
	if(g.t.columns[pos1][pos2].suit == 'c') {
		printw("♣");
	}
	if(g.t.columns[pos1][pos2].suit == 'd') {
		printw("♦");
	}
	if(g.t.columns[pos1][pos2].suit == 'h') {
		printw("♥");
	}
	if(g.t.columns[pos1][pos2].suit == 's') {
		printw("♠");
	}
}

void display_suit2(int pos) {
	if(g.s.waste[pos].suit == 'c') {
		printw("♣");
	}
	if(g.s.waste[pos].suit == 'd') {
		printw("♦");
	}
	if(g.s.waste[pos].suit == 'h') {
		printw("♥");
	}
	if(g.s.waste[pos].suit == 's') {
		printw("♠");
	}
}

void display_column(int pos, int colNum) {
	move(1,pos);
	int j = 0;
	while(g.t.columns[colNum][j].value != '\0') {
		j++;
	}
	int col = 1;
	for(int k = 0; k < j; k++) {
		if(g.t.columns[colNum][k].state == 1) {
			attron(COLOR_PAIR(3));
			mvhline(col, pos, '#', 7);
			attroff(COLOR_PAIR(3));
		}
		else {
			if(g.t.columns[colNum][k].suit == 'c' || g.t.columns[colNum][k].suit == 's') {
				attron(COLOR_PAIR(4));
			}
			else {
				attron(COLOR_PAIR(1));
			}
			printw("%c", g.t.columns[colNum][k].value);
			display_suit(colNum, k);
			mvhline(col, pos+2, ' ', 5);
		}
		col++;
		move(col, pos);
	}
	for(int i = j; i < j+5; i++) {
		mvhline(col, pos, ' ', 7);
		col++;
	}
	move(col-2, pos+5);
	display_suit(colNum, j-1);
	printw("%c", g.t.columns[colNum][j-1].value);
	if(g.t.columns[colNum][j-1].suit == 'c' || g.t.columns[colNum][j-1].suit == 's') {
		attroff(COLOR_PAIR(4));
	}
	else {
		attroff(COLOR_PAIR(1));
	}
	for(int m = j+5; m < 30; m++) {
		attron(COLOR_PAIR(6));
		mvhline(m, pos, ' ', 7);
		attroff(COLOR_PAIR(6));
	}
	
}

void display_foundation(int pos) {
	int index = 15;
	if(pos == 15) {
		move(pos, 3);
		int m = 0;
		while(g.f.stacks[0][m].value != '\0') {
			m++;
		}
		if(g.f.stacks[0][m-1].value == '_') {
			attron(COLOR_PAIR(5));
			for(int i = 0; i < 5; i++) {
				mvprintw(index, 3, "♣♣♣♣♣♣♣");
				index++;
			}
			attroff(COLOR_PAIR(5));
		}
		else {
			attron(COLOR_PAIR(4));
			for(int i = 0; i < 5; i++) {
				mvhline(index, 3, ' ', 7);
				index++;
			}
			mvprintw(15, 3, "%c", g.f.stacks[0][m-1].value);
			mvprintw(15, 4, "♣");
			mvprintw(19, 8, "♣");
			mvprintw(19, 9, "%c", g.f.stacks[0][m-1].value);
			attroff(COLOR_PAIR(4));
		}
	}
	 else if(pos == 25) {
		move(15, 13);
		int m = 0;
		while(g.f.stacks[1][m].value != '\0') {
			m++;
		}
		if(g.f.stacks[1][m-1].value == '_') {
			attron(COLOR_PAIR(5));
			for(int i = 0; i < 5; i++) {
				mvprintw(index, 13, "♦♦♦♦♦♦♦");
				index++;
			}
			attroff(COLOR_PAIR(5));
		}
		else {
			attron(COLOR_PAIR(1));
			for(int i = 0; i < 5; i++) {
				mvhline(index, 13, ' ', 7);
				index++;
			}
			mvprintw(15, 13, "%c", g.f.stacks[1][m-1].value);
			mvprintw(15, 14, "♦");
			mvprintw(19, 18, "♦");
			mvprintw(19, 19, "%c", g.f.stacks[1][m-1].value);
			attroff(COLOR_PAIR(1));
		}
	 }
	 else if(pos == 35) {
		move(15, 23);
		int m = 0;
		while(g.f.stacks[2][m].value != '\0') {
			m++;
		}
		if(g.f.stacks[2][m-1].value == '_') {
			attron(COLOR_PAIR(5));
			for(int i = 0; i < 5; i++) {
				mvprintw(index, 23, "♥♥♥♥♥♥♥");
				index++;
			}
			attroff(COLOR_PAIR(5));
		}
		else {
			attron(COLOR_PAIR(1));
			for(int i = 0; i < 5; i++) {
				mvhline(index, 23, ' ', 7);
				index++;
			}
			mvprintw(15, 23, "%c", g.f.stacks[2][m-1].value);
			mvprintw(15, 24, "♥");
			mvprintw(19, 28, "♥");
			mvprintw(19, 29, "%c", g.f.stacks[2][m-1].value);
			attroff(COLOR_PAIR(1));
		}
	 }
	 else {
		 move(15, 33);
		int m = 0;
		while(g.f.stacks[3][m].value != '\0') {
			m++;
		}
		if(g.f.stacks[3][m-1].value == '_') {
			attron(COLOR_PAIR(5));
			for(int i = 0; i < 5; i++) {
				mvprintw(index, 33, "♠♠♠♠♠♠♠");
				index++;
			}
			attroff(COLOR_PAIR(5));
		}
		else {
			attron(COLOR_PAIR(4));
			for(int i = 0; i < 5; i++) {
				mvhline(index, 33, ' ', 7);
				index++;
			}
			mvprintw(15, 33, "%c", g.f.stacks[3][m-1].value);
			mvprintw(15, 34, "♠");
			mvprintw(19, 38, "♠");
			mvprintw(19, 39, "%c", g.f.stacks[3][m-1].value);
			attroff(COLOR_PAIR(4));
		}
	 }
}

void display_game() {
	setlocale(LC_ALL, "");
	initscr();
	raw();
	noecho();
	
	start_color();
	init_pair(1, COLOR_RED, COLOR_WHITE);
	init_pair(2, COLOR_BLACK, COLOR_GREEN);
	init_pair(3, COLOR_BLACK, COLOR_CYAN);
	init_pair(4, COLOR_BLACK, COLOR_WHITE);
	init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(6, COLOR_BLACK, COLOR_BLACK);
	
	attron(COLOR_PAIR(2));
	move(0, 64);
	printw("1");
	move(0, 72);
	printw("2");
	move(0, 80);
	printw("3");
	move(0, 88);
	printw("4");
	move(0, 96);
	printw("5");
	move(0, 104);
	printw("6");
	move(0, 112);
	printw("7");
	attroff(COLOR_PAIR(2));
	int start = 61;
	int inc = 8;
	for(int i = 0; i <=6; i++) {
		display_column(start, i);
		start = start + inc;
	}
	
	move(0, 0);
	attron(COLOR_PAIR(5));
	hline(' ', 45);
	vline(' ', 10);
	mvhline(10, 0, ' ', 45);
	mvvline(0, 45, ' ', 11);
	move(1, 2);
	attron(COLOR_PAIR(2));
	printw("Q");
	attroff(COLOR_PAIR(2));
	printw(": quit");
	move(3, 2);
	attron(COLOR_PAIR(2));
	printw("R");
	attroff(COLOR_PAIR(2));
	printw(": restart");
	move(5, 2);
	attron(COLOR_PAIR(2));
	printw(".");
	attroff(COLOR_PAIR(2));
	printw(": next card");
	move(1, 18);
	attron(COLOR_PAIR(2));
	printw("U");
	attroff(COLOR_PAIR(2));
	printw(": undo last move");
	move(5, 18);
	attron(COLOR_PAIR(2));
	printw(">");
	attroff(COLOR_PAIR(2));
	printw(": reset stock");
	move(7, 2);
	printw("To move, type the source and then the");
	mvprintw(8, 2, "destination (one of W, F, 1, 2, 3, 4, 5,");
	mvprintw(9, 2, "6, 7)");
	attron(COLOR_PAIR(5));
	mvprintw(0, 12, "File: %s", filename);
	attroff(COLOR_PAIR(5));
	
	attron(COLOR_PAIR(2));
	mvprintw(13, 12, "F");
	attroff(COLOR_PAIR(2));
	mvprintw(13, 13, "oundations");
	
	display_foundation(15);
	display_foundation(25);
	display_foundation(35);
	display_foundation(45);
	
	
	attron(COLOR_PAIR(2));
	mvprintw(22, 15, "W");
	attroff(COLOR_PAIR(2));
	mvprintw(22, 16, "aste");
	
	int j = 0;
	while(g.s.waste[j].value != '\0') {
		j++;
	}
	if(j == 0) {
		attron(COLOR_PAIR(6));
		move(24, 9);
		int ind = 24;
		for(int i = 0; i < 5; i++) {
			mvhline(ind, 9, ' ', 7);
			ind++;
		}
		
		
		
	}
	if(j > 0) {
		if(g.s.waste[j-1].suit == 'c' || g.s.waste[j-1].suit == 's') {
			attron(COLOR_PAIR(4));
		}
		else {
			attron(COLOR_PAIR(1));
		}
		move(24, 9);
		int ind2 = 24;
		for(int i = 0; i < 5; i++) {
			mvhline(ind2, 9, ' ', 7);
			ind2++;
		}
		move(24, 9);
		printw("%c", g.s.waste[j-1].value);
		display_suit2(j-1);
		move(28, 14);
		display_suit2(j-1);
		printw("%c", g.s.waste[j-1].value);
		if(g.s.waste[j-1].suit == 'c' || g.s.waste[j-1].suit == 's') {
			attroff(COLOR_PAIR(4));
		}
		else {
			attroff(COLOR_PAIR(1));
		}
	}
	move(29, 11);
	printw("%d", j);
	
	move(24, 19);
	int k = 0;
	while(g.s.stock[k].value != '\0') {
		k++;
	}
	
	if(k == 0) {
		attron(COLOR_PAIR(6));
		for(int i = 0; i < 5; i++) {
			mvhline(i + 24, 19, ' ', 7);
		}
		attroff(COLOR_PAIR(6));
		
	}
	if(k > 0) {
		attron(COLOR_PAIR(3));
		for(int i = 0; i < 5; i++) {
			mvhline(i + 24, 19, '#', 7);
			
		}
		attroff(COLOR_PAIR(3));
	}
	
	move(29, 21);
	printw("%d", k);
	
}


int main(int argc, char* argv[]) {
	filename = NULL;
	FILE* fr = NULL;
	int i = 0;
	while (i != argc) {
		if (strcmp(argv[i], "-f") == 0) {// -f switch
			filename = argv[i+1];
			fr = fopen(argv[i+1], "r");
			if (fr == NULL) {
				fprintf(stderr, "Unable to open file!");
				exit(0);
			}
		}
		i++;
	}
	if (fr == NULL) {
		fprintf(stderr, "The -f switch was not typed!");
		exit(0);
	}
	
	parse(fr); //part1 stuff
	update(fr); //part2 stuff
	
	struct game prev;
	struct game reset = g;
	display_game();
	int c;
	while(c != 'q' && c != 'Q') {
		c = getch();
		if(c == '>') {
			prev = g;
			reset_stock(&g);
			display_game();
		}
		else if(c == '.') {
			prev = g;
			new_waste(&g);
			display_game();
		}
		else if(c == 'w' || c == 'W') {
			c = getch();
			if(c == 'f' || c == 'F') {
				prev = g;
				waste_to_foundations(3, &g);
				display_game();
			}
			else if(c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7') {
				char x = (char) c;
				int temp = x - '0';
				prev = g;
				waste_to_tableau(3, temp, &g);
				display_game();
			}
		}
		
		else if(c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7') {
			char y = (char) c;
			int temp = y - '0';
			c = getch();
			if(c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7') {
				char z = (char) c;
				int temp2 = z - '0';
				prev = g;
				tableau_to_tableau(3, temp, temp2, &g);
				display_game();
			}
			else if(c == 'f' || c == 'F') {
				prev = g;
				tableau_to_foundations(3, temp, &g);
				display_game();
			}
		}
		
		else if (c == 'u' || c == 'U') {
			g = prev;
			display_game();
		}
		
		else if(c == 'r' || c == 'R') {
			g = reset;
			display_game();
		}
		
	}
	endwin();
	
	
	return 0;
}