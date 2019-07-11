#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define UP 72 //회전
#define DOWN 80 //천천히 내려가기
#define LEFT 75 //좌로 이동
#define RIGHT 77 //우로 이동
#define SPACEBAR 32 //한번에 떨어지기
#define ESC 27 //게임종료
#define p 112 //일시정지
#define P 80 //일시정지

#define false 0
#define true 1

#define ACTIVE_BLOCK -2//게임판 배열에 저장될 블록 상태
#define WALL 1 //벽
#define EMPTY 0//블록이 이동할수 없는 공간은 양수표현
#define CEILLING -1 //블록이 이동할수 있는공간은 0, 음수 표현
#define INACTIVE_BLOCK 2//이동 완료된 블록값

#define MAIN_X 11 //게임판 가로크기
#define MAIN_Y 23 //게임판 세로크기
#define MAIN_X_ADJ 3 //위치조정
#define MAIN_Y_ADJ 1 //위치조정

#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1 //게임정보표시 위치조정

/* 전역변수 선언 */

int STATUS_Y_GOAL; //골 정보 위치 y좌표
int STATUS_Y_LEVEL; //레벨정보위치 y좌표
int STATUS_Y_SCORE; //점수정보 위치 y좌표

int blocks[7][4][4][4]={
  {{0,0,0,0,
    0,1,1,0,
    0,1,1,0,
    0,0,0,0},
   {0,0,0,0,
    0,1,1,0,
    0,1,1,0,
    0,0,0,0},
   {0,0,0,0,
    0,1,1,0,
    0,1,1,0,
    0,0,0,0},
   {0,0,0,0,
    0,1,1,0,
    0,1,1,0,
    0,0,0,0}}, //정사각형
  
  {{0,0,0,0,
    0,1,1,0,
    1,1,0,0,
    0,0,0,0},
   {0,1,0,0,
    0,1,1,0,
    0,0,1,0,
    0,0,0,0},
   {0,0,0,0,
    0,1,1,0,
    1,1,0,0,
    0,0,0,0},
   {0,1,0,0,
    0,1,1,0,
    0,0,1,0,
    0,0,0,0}}, //s모양
  
  {{0,0,0,0,
    0,1,0,0,
    0,1,1,0,
    0,0,0,0},
   {0,0,0,0,
    0,1,1,0,
    0,1,0,0,
    0,0,0,0},
   {0,0,0,0,
    0,1,1,0,
    0,0,1,0,
    0,0,0,0},
   {0,0,0,0,
    0,0,1,0,
    0,1,1,0,
    0,0,0,0}},//ㄴ모양
  
  {{0,0,0,0,
    0,1,1,0,
    0,0,1,1,
    0,0,0,0},
   {0,0,1,0,
    0,1,1,0,
    0,1,0,0,
    0,0,0,0},
   {0,0,0,0,
    0,1,1,0,
    0,0,1,1,
    0,0,0,0},
   {0,0,1,0,
    0,1,1,0,
    0,1,0,0,
    0,0,0,0}},//z모양
  
  {{0,0,0,0,
    0,1,0,0,
    0,1,1,1,
    0,0,0,0},
   {0,1,1,0,
    0,1,0,0,
    0,1,0,0,
    0,0,0,0},
   {0,0,0,0,
    0,1,1,1,
    0,0,0,1,
    0,0,0,0},
   {0,0,1,0,
    0,0,1,0,
    0,1,1,0,
    0,0,0,0}},//ㄱ모양
  
  {{0,1,0,0,
    0,1,0,0,
    0,1,0,0,
    0,1,0,0},
   {0,0,0,0,
    0,0,0,0,
    1,1,1,1,
    0,0,0,0},
   {0,1,0,0,
    0,1,0,0,
    0,1,0,0,
    0,1,0,0},
   {0,0,0,0,
    0,0,0,0,
    1,1,1,1,
    0,0,0,0}},//I모양
  
  {{0,0,0,0,
    0,0,1,0,
    1,1,1,0,
    0,0,0,0},
   {0,0,0,0,
    0,1,0,0,
    0,1,0,0,
    0,1,1,0},
   {0,0,0,0,
    0,1,1,1,
    0,1,0,0,
    0,0,0,0},
   {0,1,1,0,
    0,0,1,0,
    0,0,1,0,
    0,0,0,0}}//L모양
}; //blocks[b_type][b_rotation][i][j]

int b_type; //블록 종류 저장
int b_rotation; //블록의 회전값 저장
int b_type_next; //다음 블록값 저장

int main_org[MAIN_Y][MAIN_X];//게임판 정보저장배열 모니터표시 -> main_cpy로 복사
int main_cpy[MAIN_Y][MAIN_X];//게임판이 모니터에 표시되기 전의 정보를 가지고있음.
/*main의 전체를 계속 모니터에 표시하지 않고 cpy와 배열을 비교해 값이 달라진 곳만 모니터에 고침.*/

int bx,by; //이동중인 블록의 게임 판 상의 x,y좌표 저장
int key; //키보드로 입력받은 키값저장.
int speed;
int level;
int level_goal; //다음레벨까지 목표점수
int cnt; //현재 레벨에서 제거한 줄수저장
int score;
int last_score=0;
int best_score=0;



   
  
  

typedef struct _COORD{
  short x;
  short y;
}COORD;


typedef struct _tetris{
  int level;
  int gameover;
  int score;
  int rotation+=rotation%4;
  int curblock;
  int nextblock;
  int absX,absY;
  int curX,curY;
 }Tetris;
  
  char key;
  key=getch();
  
while(kbhit()) {
     key=getch();
     switch(key) {
      
          case UP;
          break;
      
          case DOWN;
          break;
      
         case LEFT;
         break;
       
         case RIGHT;
         break;
          
               }
  
  if(key==SPACEBAR) {
    /* later
    */
    
                 }
         }
  
void gotoxy(int x,int y){
  COORD pos={x,y};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
      }
  
void removeCur(){
  CONSOLE_CURSOR_INFO curinfo;
  curinfo.bVisible=0;
  GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&curinfo);
  }

void removeCurBlock(Tetris t)
{
  for(int i=0;i<4;i++)
    for(int j=0;j<4;j++)
      if(blocks[t.curblock][t.rotation][j][i]==1)
      {
        setCursor(i+t.curX)*2+t.absX, j+t.curY+t.absY);
        printf(" ");
      }
}






