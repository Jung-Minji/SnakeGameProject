#include <iostream>
#include <locale.h>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <ncurses.h>
using namespace std;


int map[40][70] = { 0, };
int stage = 1;
int run=0;
WINDOW* screen;
int inputKey;
int currentHead = 2;  // 초기 snake의 방향 : 왼쪽
bool checkFail = false;
bool checkPrey = false;
bool checkPoison = false;
int preyX, preyY, poisonX, poisonY;

int headDirection[4][2] = {
  {-1, 0}, {1, 0}, {0, -1}, {0, 1}
};  // snake head가 움직이는 방향; 위, 아래, 왼, 오

vector <int> xPos;  // snake의 x좌표가 있는 벡터
vector <int> yPos;  // snake의 y좌표가 있는 벡터

bool gameOver();
void growthItem();

//stage별 맵 구현
void gmap(int stage,int runtime) {
	srand((unsigned int)time(NULL));
	int maxX, maxY;
	if (stage == 1) {
		int counter = 0;
		wclear(screen);
		screen = newwin(40, 70, 2, 2);
		getmaxyx(stdscr, maxY, maxX);
		for (int y = 0; y < 40; y++) {
			for (int x = 0; x < 70; x++) {
				if (y == 0 && (x == 0 || x == 69) || (y == 39 && (x == 0 || x == 69))) {
					map[y][x] = 2;
					wmove(screen, y, x);
					waddch(screen, '+');
				}
				else if (x == 0 || x == 69) {
					map[y][x] = 1;
					wmove(screen, y, x);
					waddch(screen, '|');
				}
				else if (y == 0 || y == 39) {
					map[y][x] = 1;
					wmove(screen, y, x);
					waddch(screen, '-');
				}
			}
		}
		wrefresh(screen);
	}
	else if(stage == 2) {
		int counter = 0;
		wclear(screen);
		int map[36][63] = { 0, };
		screen = newwin(36, 63, 5,5);
		getmaxyx(stdscr, maxY, maxX);
		for (int y = 0; y < 36; y++) {
			for (int x = 0; x < 63; x++) {
				if (y == 0 && (x == 0 || x == 63) || (y == 35 && (x == 0 || x == 63))) {
					map[y][x] = 2;
					wmove(screen, y, x);
					waddch(screen, '+');
				}
				else if (x == 0 || x == 63) {
					map[y][x] = 1;
					wmove(screen, y, x);
					waddch(screen, '|');
				}
				else if (y == 0 || y == 35) {
					map[y][x] = 1;
					wmove(screen, y, x);
					waddch(screen, '-');
				}
			}
		}

		wrefresh(screen);
	}
	else if (stage == 3) {
		int counter = 0;
		wclear(screen);
		int map[32][56] = { 0, };
		screen = newwin(32, 56, 7, 7);
		getmaxyx(stdscr, maxY, maxX);
		for (int y = 0; y < 32; y++) {
			for (int x = 0; x < 56; x++) {
				if (y == 0 && (x == 0 || x == 55) || (y == 31 && (x == 0 || x == 55))) {
					map[y][x] = 2;
					wmove(screen, y, x);
					waddch(screen, '+');
				}
				else if (x == 0 || x == 55) {
					map[y][x] = 1;
					wmove(screen, y, x);
					waddch(screen, '|');
				}
				else if (y == 0 || y == 31) {
					map[y][x] = 1;
					wmove(screen, y, x);
					waddch(screen, '-');
				}
			}
		}

		wrefresh(screen);
	}
	else if (stage == 4) {
		int gp1, gp2;
		int gp3, gp4;
		int counter = 0;
		int test = 0;
		wclear(screen);
		int map[32][56] = { 0, };

		screen = newwin(32, 56, 7, 7);
		getmaxyx(stdscr, maxY, maxX);
		for (int y = 0; y < 32; y++) {
			for (int x = 0; x < 56; x++) {
				if (y == 0 && (x == 0 || x == 55) || (y == 31 && (x == 0 || x == 55))) {
					map[y][x] = 2;
					wmove(screen, y, x);
					waddch(screen, '+');
				}
				else if (x == 0 || x == 55) {
					map[y][x] = 1;
					wmove(screen, y, x);
					waddch(screen, '|');
				}
				else if (y == 0 || y == 31) {
					map[y][x] = 1;
					wmove(screen, y, x);
					waddch(screen, '-');
				}
			}
		}
		/*if (runtime % 30 == 1) {
			while (counter < 2) {
				int g1 = rand() % 32;
				int g2 = rand() % 56;
				if (map[g1][g2] == 1) {
					if (counter == 0) {
						map[g1][g2] = 7;
						wmove(screen, g1, g2);
						waddch(screen, 'G');
					}
					else {
						map[g1][g2] = 8;
						wmove(screen, g1, g2);
						waddch(screen, 'G');
					}
					counter++;
				}
			}
		}*/


		wrefresh(screen);
	}
  // for (int y = 0; y < 32; y++) {
  //   for (int x = 0; x < 56; x++) {
  //     printw("%d", map[y][x]);
  //   }}
}

// snake 좌표 이동
void move(int direction) {
	inputKey = wgetch(stdscr);
	// 그 전 방향에 따라  제약조건 추가
	if (inputKey == KEY_UP && currentHead != 1) {
		currentHead = 0;
	}
	else if (inputKey == KEY_DOWN && currentHead != 0) {
		currentHead = 1;
	}
	else if (inputKey == KEY_LEFT && currentHead != 3) {
		currentHead = 2;
	}
	else if (inputKey == KEY_RIGHT && currentHead != 2) {
		currentHead = 3;
	}
	for (int i = 0; i < xPos.size(); i++) {  // 이전의 snake를 지움
		mvprintw(xPos[i], yPos[i], " ");
    map[xPos[i]][yPos[i]] = 1;
	}

	for (int i = xPos.size(); i > 0; i--) {  // head를 제외한 body를 한 칸씩 옮김
		xPos[i] = xPos[i - 1];
		yPos[i] = yPos[i - 1];
	}

	xPos[0] = xPos[0] + headDirection[direction][0]; // 방향키에 따라 head의 좌표 재설정
	yPos[0] = yPos[0] + headDirection[direction][1];


  if(!checkPrey) growthItem();

  // 먹이를 먹은 경우
  if((xPos[0] == preyX) && (yPos[0] == preyY)){
    // 몸의 길이가 진행방향으로 증가
    xPos.insert(xPos.begin(), preyX);
    yPos.insert(yPos.begin(), preyY);
    checkPrey = true;
  }
  else{
    mvprintw(preyX, preyY, " ");
    checkPrey = false;
  }
  printf("%d", checkPrey);

	for (int i = 0; i < xPos.size(); i++) {  // 화면상에 snake출력
		if (i == 0) mvprintw(xPos[i], yPos[i], "O");
		else mvprintw(xPos[i], yPos[i], "o");
	}

  // 몸의 길이가 3보다 작아진 경우 -> game over
  if(xPos.size() < 3) checkFail = true;

  //벽과 출돌한 경우 -> game over
  //if((map[xPos[0]][yPos[0]] == 1) || (map[xPos[0]][yPos[0]] == 2)) checkFail = true;

  // Tail방향으로 움직인 경우 -> game over
  if ((inputKey == KEY_UP && currentHead == 1) || (inputKey == KEY_DOWN && currentHead == 0)
      || (inputKey == KEY_LEFT && currentHead == 3) || (inputKey == KEY_RIGHT && currentHead == 2))
    checkFail = true;

  if(checkFail){
     mvwprintw(16, 30, "**GAME OVER**");
     return;
  }

	for(int i=0; i<xPos.size(); i++){  // map에 snake값 할당
		if (i == 0) map[xPos[i]][yPos[i]] = 3;
		else map[xPos[i]][yPos[i]] = 4;
	}
}

// 먹이 생성 위치를 map의 값이 0인 곳 중 랜덤으로 지정
void growthItem(){
	int width = sizeof(map[0]) / sizeof(int);
	int height = (sizeof(map) / width) / sizeof(int);
	srand((unsigned int)time(NULL));

  preyX = (rand() % height - 1) + 1;
  preyY = (rand() % width - 1) + 1;

	while(map[preyX][preyY] != 0){
		preyX = (rand() % height - 1) + 1;
		preyY = (rand() % width - 1) + 1;
	}
	map[preyX][preyY] = 5;
	mvwprintw(preyX, preyY, "*");
}

// 독 생성 위치를 map의 값이 0인 곳 중 랜덤으로 지정
void poisonItem(){
  int width = sizeof(map[0]) / sizeof(int);
	int height = (sizeof(map) / width) / sizeof(int);
	srand((unsigned int)time(NULL));

  poisonX = (rand() % height - 1) + 1;
poisonY = (rand() % width - 1) + 1;

	while(map[poisonX][poisonY] != 0){
		poisonX = (rand() % height - 1) + 1;
		poisonY = (rand() % width - 1) + 1;
	}
	map[poisonX][poisonY] = 6;
	mvwprintw(poisonX, poisonY, "@");
}

void gate(int runtime) {

}
//mission창 score창 프린트
void mission_score() {
	WINDOW* score;
	WINDOW* mission;
	mission = newwin(20, 35, 22, 73);
	wborder(mission, '|', '|', '-', '-', '+', '+', '+', '+');
	mvwprintw(mission, 2, 2, "Mission");
	mvwprintw(mission, 5, 2, "B: ");
	mvwprintw(mission, 8, 2, "+: ");
	mvwprintw(mission, 11, 2, "-: ");
	mvwprintw(mission, 14, 2, "G: ");
	wrefresh(mission);
	score = newwin(20, 35, 2, 73);
	wborder(score, '|', '|', '-', '-', '+', '+', '+', '+');
	mvwprintw(score, 2, 2, "Score Board");
	mvwprintw(score, 5, 2, "B:");
	mvwprintw(score, 8, 2, "+:");
	mvwprintw(score, 11, 2, "-:");
	mvwprintw(score, 14, 2, "G:");
	wrefresh(score);
}

int main() {
  setlocale(LC_ALL, "");
	initscr();
	resize_term(45, 110);
	noecho();
	border('o', 'o', 'o', 'o', 'o', 'o', 'o', 'o');
	refresh();

	//initscr();
	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE);  // 키보드 입력 설정
	curs_set(0);  // 커서 지우기
	noecho();  // 입력한 키 값을 화면에 보이지 않기

	xPos.push_back(10);  // 초기 snake의 위치 지정
	xPos.push_back(10);
	xPos.push_back(10);
	yPos.push_back(55);
	yPos.push_back(56);
	yPos.push_back(57);
	map[10][55] = 3;
	map[10][56] = 4;
	map[10][57] = 4;

	// 초기 snake 화면에 출력
	for (int i = 0; i < xPos.size(); i++) {
		if (i == 0) mvprintw(xPos[i], yPos[i], "O");
		else mvprintw(xPos[i], yPos[i], "o");
	}
  gmap(1,run);
	while (true) {
  	mission_score();
		move(currentHead);
    if(checkFail) break;
		usleep(100000);  // 0.5초마다
		run++;
	}

	refresh();
	getch();
	endwin();
	return 0;

}
