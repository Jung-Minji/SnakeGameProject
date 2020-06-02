#include <ncurses.h>
#include <iostream>
#include <locale.h>
#include <unistd.h>
#include <vector>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
using namespace std;

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

int headDirection[4][2] = {
  {-1, 0}, {1, 0}, {0, -1}, {0, 1}
};  // snake head가 움직이는 방향; 위, 아래, 왼, 오

class Snake{
public:
  vector <int> xPos;
  vector <int> yPos;

  void setX(int x){
    xPos.push_back(x);
  }

  void setY(int y){
    yPos.push_back(y);
  }

  int getX(){
    return xPos.back();
  }

  int getY(){
    return yPos.back();
  }

  int len(){
    return xPos.size();
  }
  //
  void move(int direction){  // 이전의 snake를 지움
    for(int i=0; i<len(); i++){
      mvprintw(xPos[i], yPos[i], " ");
    }

    for(int i=len(); i>0; i--){  // head를 제외한 body를 한 칸씩 옮김
      xPos[i] = xPos[i-1];
      yPos[i] = yPos[i-1];
    }
    xPos[0] = xPos[0] + headDirection[direction][0]; // 방향키에 따라 head의 좌표 재설정
    yPos[0] = yPos[0] + headDirection[direction][1];

    for(int i=0; i<len(); i++){  // 화면상에 출력
      if(i == 0) mvprintw(xPos[i], yPos[i], "\u2B1B");
      else mvprintw(xPos[i], yPos[i], "\u2B1C");
    }

  }
  // head의 방향 결정 -> 방향키의 입력을 매개변수로 받아 처리
  int setHeadDirection(int inputKey){
    int setDirect;
    switch (inputKey) {
      case KEY_UP:
        setDirect = 0;
        break;

      case KEY_DOWN:
        setDirect = 1;
        break;

      case KEY_LEFT:
        setDirect = 2;
        break;

      case KEY_RIGHT:
        setDirect = 3;
        break;

      default:
        break;
    }
    return setDirect;
  }

};

int main(){
  setlocale(LC_ALL, "");
  initscr();
  keypad(stdscr, TRUE);  // 키보드 입력 설정
  //start_color();
  curs_set(0);  // 커서 지우기
  noecho();  // 입력한 키 값을 화면에 보이지 않기
  //init_pair(1, COLOR_RED, COLOR_WHITE);
  //attron(COLOR_PAIR(1));
  //attroff(COLOR_PAIR(1));


  Snake snake;
  snake.setX(10);
  snake.setY(15); // 초기 snake의 위치 지정
  snake.setX(10);
  snake.setY(16);
  snake.setX(10);
  snake.setY(17);
  for(int i=0; i<snake.len(); i++){
    if (i == 0)
    mvprintw(snake.xPos[i], snake.yPos[i],"\u2B1B"); // 화면에 ■ □ 출력
    else mvprintw(snake.xPos[i], snake.yPos[i],"\u2B1C");
  }

  int inputKey;
  int currentHead = 0;

  while (true){
    inputKey = wgetch(stdscr); // 키보드 입력받음
    currentHead = snake.setHeadDirection(inputKey);
    snake.move(currentHead);
    }

  refresh();
  getch();
  endwin();
  return 0;
}
