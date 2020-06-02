#include <ncurses.h>
#include <iostream>
#include <locale.h>
#include <vector>
#include <stdio.h>
#include <unistd.h>
using namespace std;

int headDirection[4][2] = {
  {-1, 0}, {1, 0}, {0, -1}, {0, 1}
};  // snake head가 움직이는 방향; 위, 아래, 왼, 오


vector <int> xPos;  // snake의 x좌표가 있는 벡터
vector <int> yPos;  // snake의 y좌표가 있는 벡터

// snake 좌표 이동
void move(int direction){
  for(int i=0; i<xPos.size(); i++){  // 이전의 snake를 지움
    mvprintw(xPos[i], yPos[i], " ");
  }

  for(int i=xPos.size(); i>0; i--){  // head를 제외한 body를 한 칸씩 옮김
    xPos[i] = xPos[i-1];
    yPos[i] = yPos[i-1];
  }
  xPos[0] = xPos[0] + headDirection[direction][0]; // 방향키에 따라 head의 좌표 재설정
  yPos[0] = yPos[0] + headDirection[direction][1];

  for(int i=0; i<xPos.size(); i++){  // 화면상에 출력
    if(i == 0) mvprintw(xPos[i], yPos[i], "\u2B1B");
    else mvprintw(xPos[i], yPos[i], "\u2B1C");
  }

}

int main(){
  setlocale(LC_ALL, "");
  initscr();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);  // 키보드 입력 설정
  curs_set(0);  // 커서 지우기
  noecho();  // 입력한 키 값을 화면에 보이지 않기

  xPos.push_back(10);  // 초기 snake의 위치 지정
  xPos.push_back(10);
  xPos.push_back(10);
  yPos.push_back(45);
  yPos.push_back(46);
  yPos.push_back(47);

  // 초기화면 snake 출력
  for(int i=0; i<xPos.size(); i++){
    if (i == 0) mvprintw(xPos[i], yPos[i],"\u2B1B");
    else mvprintw(xPos[i], yPos[i],"\u2B1C");
  }

  int inputKey;
  int currentHead = 2;  // 초기 snake의 방향 : 왼쪽

  while (true){
    inputKey = wgetch(stdscr);
    if(inputKey == KEY_UP){
        currentHead = 0;
    }
    else if(inputKey == KEY_DOWN){
        currentHead = 1;
    }
    else if(inputKey == KEY_LEFT){
        currentHead = 2;
    }
    else if(inputKey == KEY_RIGHT){
        currentHead = 3;
    }
    move(currentHead);
    usleep(500000);  // 0.5초마다
    }

  refresh();
  getch();
  endwin();
  return 0;
}
