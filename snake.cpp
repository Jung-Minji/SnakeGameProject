#include <iostream>
#include <locale.h>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <ncurses.h>
#include <thread>
using namespace std;

WINDOW* screen;
int map[30][50] = { 0 };
int stage = 1 , currentstage = 1 , run=1;
int inputKey, start;
int currentHead =2;  // 초기 snake의 방향 : 왼쪽
bool checkFail = false , checkPrey = false, checkPoison = false;
int height =30, width =50;
int preyX = 1, preyY = 1, poisonX = 1, poisonY = 1;
int countPrey = 0, countPoison = 0;
int gateOneY, gateOneX, gateTwoY, gateTwoX; //gate 좌표
int mSizeX, mSizeY;
int countGate;								
int mission_B, mission_G, mission_P, mission_F;
char checkB[4], checkG[4], checkP[4], checkF[4];
bool checkMission1, checkMission2, checkMission3, checkMission4;

int headDirection[4][2] = {
  {-1, 0}, {1, 0}, {0, -1}, {0, 1}
};  // snake head가 움직이는 방향; 위, 아래, 왼, 오

vector <int> yPos;  // snake의 y좌표가 있는 벡터
vector <int> xPos;  // snake의 x좌표가 있는 벡터

void moveSnake(int);
bool gameOver();
void growthItem();
void poisonItem();
void moveSnake(int);
void gate(int);
void mission_score();
void initialize();
void nextStage(int);
void over();

void startGame(){
  while (true && stage !=5) {
	  currentstage = stage;
	  moveSnake(currentHead);
    //백번마다 Gate 위치 바꾸기
    if (run % 100 == 1) {
      gate(stage);
    }
	mission_score();
    drawmap(stage);
	if (currentstage != stage) {
		if (stage == 5) {			
			break;
		}
		else {
			nextStage(stage);		// 다음 스테이지 넘어가기 전 문구
			initialize();			// 스테이지 마다 snake 초기 위치 설정
			gate(stage);
			drawmap(stage);
		}
	}
	if (checkFail) {
		over();
		break;
	}
    usleep(100000);  // 0.25초마다
    run++;
  }
}
// snake 좌표 이동
void moveSnake(int direction) {
	inputKey = getch();
	// 방향 설정
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
	for (int i = 0; i < yPos.size(); i++) {  // 이전의 snake를 지움
		mvprintw(yPos[i], xPos[i], " ");
		map[yPos[i]][xPos[i]] = 0;
	}

	for (int i = yPos.size(); i > 0; i--) {  // head를 제외한 body를 한 칸씩 옮김
		yPos[i] = yPos[i - 1];
		xPos[i] = xPos[i - 1];
	}

	yPos[0] = yPos[0] + headDirection[direction][0]; // 방향키에 따라 head의 좌표 재설정
	xPos[0] = xPos[0] + headDirection[direction][1];


  // 먹이를 먹은 경우
  if(map[yPos[0]][xPos[0]] == 5){
    // 몸의 길이가 진행방향으로 증가
    yPos.insert(yPos.begin(), preyX);
    xPos.insert(xPos.begin(), preyY);
    countPrey++;
    checkPrey = true;
  }
  else{
    checkPrey = false;
  }

  // 독을 먹은 경우
  if(map[yPos[0]][xPos[0]] == 6){
    // 꼬리 1 감소
    yPos.pop_back();
    xPos.pop_back();
    countPoison++;
    checkPoison = true;
  }
  else{
    checkPoison = false;
  }

  //gate 이동
  if (stage == 1) {
	  mSizeY = 30;
	  mSizeX = 50;
  }
  else if (stage == 2) {
	  mSizeY = 26;
	  mSizeX = 45;
  }
  else if (stage == 3) {
	  mSizeY = 22;
	  mSizeX = 40;
  }
  else if (stage == 4) {
	  mSizeY = 21;
	  mSizeX = 36;
  }

  if (map[yPos[0]][xPos[0]] == 7) {
	  if (gateTwoY == 0 ) {
		  currentHead = 1;
		  yPos[0] = gateTwoY+1;
		  xPos[0] = gateTwoX ;

	  }
	  else if (gateTwoY == mSizeY-1) {
		  currentHead = 0;
		  yPos[0] = gateTwoY-1;
		  xPos[0] = gateTwoX;
	  }
	  if (gateTwoX == 0) {
		  currentHead = 3;
		  yPos[0] = gateTwoY ;
		  xPos[0] = gateTwoX +1;
	  }
	  else if (gateTwoX == mSizeX-1) {
		  currentHead = 2;
		  yPos[0] = gateTwoY;
		  xPos[0] = gateTwoX - 1;
	  }
	  countGate++;
  }
  else if (map[yPos[0]][xPos[0]] == 8) {
	  if (gateOneY == 0) {
		  currentHead = 1;
		  yPos[0] = gateOneY +1;
		  xPos[0] = gateOneX ;
	  }
	  else if (gateOneY == mSizeY - 1) {
		  currentHead = 0;
		  yPos[0] = gateOneY -1;
		  xPos[0] = gateOneX ;
	  }
	  if (gateOneX == 0) {
		  currentHead = 3;
		  yPos[0] = gateOneY;
		  xPos[0] = gateOneX + 1;
	  }
	  else if (gateOneX == mSizeX-1) {
		  currentHead = 2;
		  yPos[0] = gateOneY;
		  xPos[0] = gateOneX - 1;
	  }
	  countGate++;
  }

  // 몸의 길이가 3보다 작아진 경우 -> game over
  if(yPos.size() < 3) checkFail = true;

  //벽과 출돌한 경우 -> game over
  if(map[yPos[0]][xPos[0]] == 1) checkFail = true;
  
  
  // Tail방향으로 움직인 경우 -> game over
  if ((inputKey == KEY_UP && currentHead == 1) || (inputKey == KEY_DOWN && currentHead == 0)
      || (inputKey == KEY_LEFT && currentHead == 3) || (inputKey == KEY_RIGHT && currentHead == 2))
    checkFail = true;


  for(int i=0; i<yPos.size(); i++){  // map에 snake값 할당
    if (i == 0) map[yPos[i]][xPos[i]] = 3;
    else map[yPos[i]][xPos[i]] = 4;
  }
}

void setItem(){
  while(!checkFail){
    sleep(5);
    thread t1(growthItem);
    thread t2(poisonItem);
    t1.join();
    t2.join();
  }
}

// 먹이 생성 위치를 map의 값이 0인 곳 중 랜덤으로 지정
void growthItem(){
  mvprintw(preyX, preyY, " ");
  if(!checkPrey){  // 이전 먹이를 먹지 못했다면 다시 map의 값을 0로 바꾼다.
    map[preyX][preyY] = 0;
  }
  if (stage == 1) {
	  height = 30;
	  width = 50;
  }
  else if (stage == 2) {
	  height = 26;
	  width = 45;
  }
  else if (stage == 3) {
	  height = 22;
	  width = 40;
  }
  else if (stage == 4) {
	  height = 21;
	  width = 36;
  }
	srand((unsigned int)time(NULL));

  preyX = rand() % height;
  preyY = rand() % width;

	while(map[preyX][preyY] != 0){
		preyX = rand() % height;
		preyY = rand() % width;
	}
	map[preyX][preyY] = 5;
}

// 독 생성 위치를 mapx의 값이 0인 곳 중 랜덤으로 지정
void poisonItem(){
  mvprintw(poisonX, poisonY, " ");
  if(!checkPoison){
    map[poisonX][poisonY] = 0;
  }
  if (stage == 1) {
	  height = 30;
	  width = 50;
  }
  else if (stage == 2) {
	  height = 26;
	  width = 45;
  }
  else if (stage == 3) {
	  height = 22;
	  width = 40;
  }
  else if (stage == 4) {
	  height = 21;
	  width = 36;
  }
	srand((unsigned int)time(NULL));

  poisonX = rand() % height;
  poisonY = rand() % width;

	while(map[poisonX][poisonY] != 0){
		poisonX = rand() % height;
		poisonY = rand() % width;
	}
	map[poisonX][poisonY] = 6;
	
}

//stage별 맵 구현
void drawmap(int stage) {
	if (stage == 1) {
		screen = newwin(30, 50, 7, 7);
		//배열의 값을 이용해서 map을 그리는 식
		for (int y = 0; y < 30; y++) {
			for (int x = 0; x < 50; x++) {
				if (map[y][x] == 2) {
					wmove(screen, y, x);
					waddch(screen, '+');
				}
				else if (map[y][x] == 1) {
					if (x == 0 || x == 49) {
						wmove(screen, y, x);
						waddch(screen, '|');
					}
					else if (y == 0 || y == 29) {
						wmove(screen, y, x);
						waddch(screen, '-');
					}
				}
				//gate 그리기
				else if (map[y][x] == 7 || map[y][x] == 8) {
					wmove(screen, y, x);
					waddch(screen, 'G');
				}
				else if (map[y][x] == 3) {
					wmove(screen, y, x);
					waddch(screen, 'O');
				}
				else if (map[y][x] == 4) {
					wmove(screen, y, x);
					waddch(screen, 'o');
				}
				else if (map[y][x] == 5) {
					wmove(screen, y, x);
					waddch(screen, '@');
				}
				else if (map[y][x] == 6) {
					wmove(screen, y, x);
					waddch(screen, '#');
				}
				else if (map[y][x] == 0) {
					wmove(screen, y, x);
					waddch(screen, ' ');
				}
			}
		}
		wrefresh(screen);
	}
	else if (stage == 2) {
		wclear(screen);
		wrefresh(screen);
		screen = newwin(26, 45, 7, 7);
		for (int y = 0; y < 26; y++) {
			for (int x = 0; x < 45; x++) {
				if (map[y][x] == 2) {
					wmove(screen, y, x);
					waddch(screen, '+');
				}
				else if (map[y][x] == 1) {
					if (x == 0 || x == 44) {
						wmove(screen, y, x);
						waddch(screen, '|');
					}
					else if (y == 0 || y == 25) {
						wmove(screen, y, x);
						waddch(screen, '-');
					}
				}
				//gate 그리기
				else if (map[y][x] == 7 || map[y][x] == 8) {
					wmove(screen, y, x);
					waddch(screen, 'G');
				}
				else if (map[y][x] == 3) {
					wmove(screen, y, x);
					waddch(screen, 'O');
				}
				else if (map[y][x] == 4) {
					wmove(screen, y, x);
					waddch(screen, 'o');
				}
				else if (map[y][x] == 5) {
					wmove(screen, y, x);
					waddch(screen, '@');
				}
				else if (map[y][x] == 6) {
					wmove(screen, y, x);
					waddch(screen, '#');
				}
			}
		}
		wrefresh(screen);
	}
	else if (stage == 3) {
		wclear(screen);
		wrefresh(screen);
		screen = newwin(22, 40, 7, 7);
		for (int y = 0; y < 22; y++) {
			for (int x = 0; x < 40; x++) {
				if (map[y][x] == 2) {
					wmove(screen, y, x);
					waddch(screen, '+');
				}
				else if (map[y][x] == 1) {
					if (x == 0 || x == 39) {
						wmove(screen, y, x);
						waddch(screen, '|');
					}
					else if (y == 0 || y == 21) {
						wmove(screen, y, x);
						waddch(screen, '-');
					}

				}
				//gate 그리기
				else if (map[y][x] == 7 || map[y][x] == 8) {
					wmove(screen, y, x);
					waddch(screen, 'G');
				}
				else if (map[y][x] == 3) {
					wmove(screen, y, x);
					waddch(screen, 'O');
				}
				else if (map[y][x] == 4) {
					wmove(screen, y, x);
					waddch(screen, 'o');
				}
				else if (map[y][x] == 5) {
					wmove(screen, y, x);
					waddch(screen, '@');
				}

			}
		}

		wrefresh(screen);
	}
	else if (stage == 4) {
		wclear(screen);
		wrefresh(screen);
		screen = newwin(21, 36, 7, 7);
		for (int y = 0; y < 21; y++) {
			for (int x = 0; x < 36; x++) {
				if (map[y][x] == 2) {
					wmove(screen, y, x);
					waddch(screen, '+');
				}
				else if (map[y][x] == 1) {
					if (x == 0 || x == 35) {
						wmove(screen, y, x);
						waddch(screen, '|');
					}
					else if (y == 0 || y == 20) {
						wmove(screen, y, x);
						waddch(screen, '-');
					}

				}
				//gate 그리기
				else if (map[y][x] == 7 || map[y][x] == 8) {
					wmove(screen, y, x);
					waddch(screen, 'G');
				}
				else if (map[y][x] == 3) {
					wmove(screen, y, x);
					waddch(screen, 'O');
				}
				else if (map[y][x] == 4) {
					wmove(screen, y, x);
					waddch(screen, 'o');
				}
				else if (map[y][x] == 5) {
					wmove(screen, y, x);
					waddch(screen, '@');
				}
				else if (map[y][x] == 6) {
					wmove(screen, y, x);
					waddch(screen, '#');
				}
			}
		}
		wrefresh(screen);
	}
}

void gate(int stage) {
	int counter = 0;
	srand((unsigned)time(NULL));
	if (stage == 1) {
		for (int y = 0; y < 30; y++) {
			for (int x = 0; x < 50; x++) {
				if (y == 0 && (x == 0 || x == 49) || (y == 29 && (x == 0 || x == 49))) {
					map[y][x] = 2;

				}
				else if (x == 0 || x == 49) {
					if (map[y][x] != 7 || map[y][x] != 8) {
						map[y][x] = 1;
					}

				}
				else if (y == 0 || y == 29) {
					if (map[y][x] != 7 || map[y][x] != 8) {
						map[y][x] = 1;
					}
				}
			}
		}
		while (counter < 2) {
			int g1 = rand() % 30;
			int g2 = rand() % 50;
			if (map[g1][g2] == 1) {
				if (counter == 0) {
					map[g1][g2] = 7;
					gateOneY = g1;
					gateOneX = g2;
				}
				else {
					map[g1][g2] = 8;
					gateTwoY = g1;
					gateTwoX = g2;
				}
				counter++;
			}
		}
	}
	else if (stage == 2) {
		for (int y = 0; y < 30; y++) {
			for (int x = 0; x < 50; x++) {
				if (y == 0 && (x == 0 || x == 44) || (y == 25 && (x == 0 || x == 44))) {
					map[y][x] = 2;

				}
				else if (x == 0 || x == 44) {
					if (map[y][x] != 7 || map[y][x] != 8) {
						map[y][x] = 1;
					}

				}
				else if (y == 0 || y == 25) {
					if (map[y][x] != 7 || map[y][x] != 8) {
						map[y][x] = 1;
					}
				}
			}
		}
		while (counter < 2) {
			int g1 = rand() % 26;
			int g2 = rand() % 44;
			if (map[g1][g2] == 1) {
				if (counter == 0) {
					map[g1][g2] = 7;
					gateOneY = g1;
					gateOneX = g2;
				}
				else {
					map[g1][g2] = 8;
					gateTwoY = g1;
					gateTwoX = g2;
				}
				counter++;
			}
		}
	}
	else if (stage == 3) {
		for (int y = 0; y < 30; y++) {
			for (int x = 0; x < 50; x++) {
				if (y == 0 && (x == 0 || x == 39) || (y == 21 && (x == 0 || x == 39))) {
					map[y][x] = 2;

				}
				else if (x == 0 || x == 39) {
					if (map[y][x] != 7 || map[y][x] != 8) {
						map[y][x] = 1;
					}

				}
				else if (y == 0 || y == 21) {
					if (map[y][x] != 7 || map[y][x] != 8) {
						map[y][x] = 1;
					}
				}

			}
		}
		while (counter < 2) {
			int g1 = rand() % 22;
			int g2 = rand() % 40;
			if (map[g1][g2] == 1) {
				if (counter == 0) {
					map[g1][g2] = 7;
					gateOneY = g1;
					gateOneX = g2;
				}
				else {
					map[g1][g2] = 8;
					gateTwoY = g1;
					gateTwoX = g2;
				}
				counter++;
			}
		}
	}
	else if (stage == 4) {
		for (int y = 0; y < 30; y++) {
			for (int x = 0; x < 50; x++) {
				if (y == 0 && (x == 0 || x == 35) || (y == 20 && (x == 0 || x == 35))) {
					map[y][x] = 2;

				}
				else if (x == 0 || x == 35) {
					if (map[y][x] != 7 || map[y][x] != 8) {
						map[y][x] = 1;
					}

				}
				else if (y == 0 || y == 20) {
					if (map[y][x] != 7 || map[y][x] != 8) {
						map[y][x] = 1;
					}
				}

			}
		}
		while (counter < 2) {
			int g1 = rand() % 21;
			int g2 = rand() % 36;
			if (map[g1][g2] == 1) {
				if (counter == 0) {
					map[g1][g2] = 7;
					gateOneY = g1;
					gateOneX = g2;
				}
				else {
					map[g1][g2] = 8;
					gateTwoY = g1;
					gateTwoX = g2;
				}
				counter++;
			}
		}
	}
}

//mission창 score창 프린트
void mission_score() {
	if (stage == 1) {
		mission_B = 5;
		mission_F = 5;
		mission_G = 1;
		mission_P = 2;
	}
	else if (stage == 2) {
		mission_B = 8;
		mission_F = 10;
		mission_G = 2;
		mission_P = 5;
	}
	else if (stage == 3) {
		mission_B = 11;
		mission_F = 15;
		mission_G = 3;
		mission_P = 7;
	}
	else if (stage == 4) {
		mission_B = 14;
		mission_F = 20;
		mission_G = 5;
		mission_P = 7;
	}
	if (mission_B <=  (3 + countPrey - countPoison) % (3 + countPrey) && mission_B != 0) {
		checkB[0] = '(';
		checkB[1] = 'V';
		checkB[2] = ')';
		checkMission1 = true;
	}
	else {
		checkB[0] = '(';
		checkB[1] = ' ';
		checkB[2] = ')';
		checkMission1 = true;
	}
	if (mission_F <= countPrey && mission_F != 0) {
		checkF[0] = '(';
		checkF[1] = 'V';
		checkF[2] = ')';
		checkMission2 = true;
	}
	else {
		checkF[0] = '(';
		checkF[1] = ' ';
		checkF[2] = ')';
		
	}
	if (mission_G <= countGate && mission_G != 0) {
		checkG[0] = '(';
		checkG[1] = 'V';
		checkG[2] = ')';
		checkMission3 = true;
	}
	else {
		checkG[0] = '(';
		checkG[1] = ' ';
		checkG[2] = ')';
		
	}
	if (mission_P <= countPoison &&mission_P !=0) {
		checkP[0] = '(';
		checkP[1] = 'V';
		checkP[2] = ')';
		checkMission4 = true;
	}
	else {
		checkP[0] = '(';
		checkP[1] = ' ';
		checkP[2] = ')';
		
	}
	WINDOW* score;
	WINDOW* mission;
	mission = newwin(11, 25,18 , 60);
	wborder(mission, '|', '|', '-', '-', '+', '+', '+', '+');
	mvwprintw(mission, 1, 2, "Mission");
	mvwprintw(mission, 3, 2, "B: %d %s", mission_B,checkB);
	mvwprintw(mission, 5, 2, "+: %d %s", mission_F, checkF);
	mvwprintw(mission, 7, 2, "-: %d %s", mission_P, checkP);
	mvwprintw(mission, 9, 2, "G: %d %s", mission_G, checkG);
	wrefresh(mission);
	score = newwin(11, 25, 7, 60);
	wborder(score, '|', '|', '-', '-', '+', '+', '+', '+');
	mvwprintw(score, 1, 2, "Score Board");
	mvwprintw(score, 3, 2, "B: %d", (3 + countPrey - countPoison) % (3 + countPrey));
	mvwprintw(score, 5, 2, "+: %d", countPrey);
	mvwprintw(score, 7, 2, "-: %d" , countPoison);
	mvwprintw(score, 9, 2, "G: %d", countGate);
	wrefresh(score);
	if (checkMission1 && checkMission2 && checkMission3 && checkMission4) {
		stage++;
		wclear(screen);
		clear();
		countPrey = 0;
		countPoison = 0;
		countGate = 0;
		checkMission1 = false;
		checkMission2 = false;
		checkMission3 = false;
		checkMission4 = false;
		run = 1;
		currentHead = 2;
		for (int i = 0; i < 30; i++) {
			for (int j = 0; j < 50; j++) {
				map[i][j] = 0;
			}
			
		}
	}
}

// 초기 snake의 위치 지정
void initialize() {
	yPos.clear();
	xPos.clear();
	yPos.push_back (height/2 ); 
	yPos.push_back(height / 2 );
	yPos.push_back(height / 2 );
	xPos.push_back(width/2);
	xPos.push_back(width / 2 );
	xPos.push_back(width / 2 );
	map[height / 2 ][width / 2 ] = 3;
	map[height / 2 ][width / 2 ] = 4;
	map[height/2][width / 2] = 4;
}
//다음 단계 넘어갈때 문구
void nextStage(int stage) {
	if (stage == 2) {
		clear();
		mvprintw(20, 47, "------------------");
		mvprintw(21, 47, "|                |");
		mvprintw(22, 47, "|    Stage. 2    |");
		mvprintw(23, 47, "|                |");
		mvprintw(24, 47, "------------------");
		refresh();
		clear();
		sleep(5);
	}
	else if (stage == 3) {
		clear();
		mvprintw(20, 47, "------------------");
		mvprintw(21, 47, "|                |");
		mvprintw(22, 47, "|    Stage. 3    |");
		mvprintw(23, 47, "|                |");
		mvprintw(24, 47, "------------------");
		refresh();
		clear();
		sleep(5);
	}
	else if (stage == 4) {
		clear();
		mvprintw(20, 47, "------------------");
		mvprintw(21, 47, "|                |");
		mvprintw(22, 47, "|    Stage. 4    |");
		mvprintw(23, 47, "|                |");
		mvprintw(24, 47, "------------------");
		refresh();
		clear();
		sleep(5);
	}
	else if(stage == 5){
		mvprintw(20, 47, " ------------------");
		mvprintw(21, 47, "|                  |");
		mvprintw(22, 47, "|  Congratulation! |");
		mvprintw(23, 47, "|                  |");
		mvprintw(24, 47, " ------------------");
		refresh();
		sleep(5);
		clear();
	}
}
//시작화면
void title() {
	mvprintw(20, 47, "------------------");
	mvprintw(21, 47, "|                |");
	mvprintw(22, 47, "|   Snake Game   |");
	mvprintw(23, 47, "|                |");
	mvprintw(24, 47, "------------------");
	mvprintw(27, 45, "Press Any Key to Start!");
	refresh();
	start = getch();
	clear();

	mvprintw(20, 47, "------------------");
	mvprintw(21, 47, "|                |");
	mvprintw(22, 47, "|    Stage. 1    |");
	mvprintw(23, 47, "|                |");
	mvprintw(24, 47, "------------------");
	refresh();
	sleep(5);
}
void over() {
	clear();
	mvprintw(20, 47, "-----------------");
	mvprintw(21, 47, "|   Game Over   |");
	mvprintw(22, 47, "-----------------");
	refresh();	
	currentstage = 6;
	stage = 6;
	
}
int main() {
	setlocale(LC_ALL, "");
	initscr();
	resize_term(45, 110);
	keypad(stdscr, TRUE);  // 키보드 입력 설정
	curs_set(0);  // 커서 지우기
	noecho();  // 입력한 키 값을 화면에 보이지 않기
	title();
	clear();
	nodelay(stdscr, TRUE);
	initialize();
	thread tStart(startGame);
	thread tSet(setItem);
	tStart.join();
	tSet.join();

	
	nextStage(stage);
	endwin();
	return 0;
	
}
