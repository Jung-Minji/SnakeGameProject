#include <iostream>
#include <cstdlib>
#include <ncurses.h>

void gmap(int stage) {
	WINDOW* screen;
	int x, y;
	int** map;
	if (stage == 1) {
		wclear(screen);
		screen = newwin(40, 70, 2, 2);
		wborder(screen,'|', '|', '-', '-', '+', '+', '+', '+');
		wrefresh(screen);
		getmaxyx(screen, y, x);
		map = new int*[y];
		for (int i = 0; i < y; i++) {
			map[i] = new int[x];
		}
		for (int i = 0; i < 50; i++) {
			for (int j = 0; j < 50; j++) {
				if (i==0 && (j ==0 || j ==49) || (i==49 && (j==0 || j==49))) {
					map[i][j] = 2;
				}
				else if (j == 0 || j==49) {
					map[i][j] = 1;
				}
				else if(i==0 || i==49){
					map[i][j] = 1;
				}
				else {
					map[i][j] = 0;
				}
			}
		}
	}
	else if(stage == 2) {

	}
	else if (stage == 3) {

	}
	else if (stage == 4) {

	}
	else {

	}

};
int main() {
	WINDOW* score;
	WINDOW* mission;
	char key;
	initscr(); 
	resize_term(45, 110);
	noecho();
	border('o', 'o', 'o', 'o', 'o', 'o', 'o', 'o');
	refresh();
	gmap(1);

	// Step 5 : mission, socre
	mission = newwin(20, 35, 22, 73);
	wborder(mission, '|', '|', '-', '-', '+', '+', '+', '+');
	wrefresh(mission);
	score = newwin(20, 35, 2, 73);
	wborder(score, '|', '|', '-', '-', '+', '+', '+', '+');
	wrefresh(score);

	getch();
	endwin();

	return 0;
}