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

int new_block_on=0; //새로운 블럭이 필요함을 알리는 플래그
int crush_on=0; //현재 이동중인 블록이 충돌상태인지 알려주는 플래그
int level_up_on=0; //다음레벨로 진행을 알리는 플래그
int space_key_on=0; //하드드롭상태임을 알려주는 플래그

/* 함수 원형 정의 */
void title(void); //게임 시작화면
void reset(void); //게임판 초기화
void reset_main(void); //메인 게임판 main_org[][]를 초기화
void draw_map(void); //게임 전체 인터페이스 표시
void draw_main(void); //게임판을 그림
void new_block(void); //새로운 블록을 만듬
void check_key(void); //키보드로 키를 입력받음
void drop_block(void); //블록을 아래로 떨어트림
void move_block(int dir); //dir방향으로 블록을 움직임
void check_line(void); //줄이 가득 찼는지 판단하고 지움
void check_levelup(void);// 레벨목표가 달성되었는지 판단하고 레벨업
void check_game_over(void);//게임오버인지 판단하고 진행
void pause(void);//게임 일시정지

int check_crush(int bx,int by, int rotation); //bx,by위치에 로테이션 회전값을 갖는경우 충돌판단

void gotoxy(int x,int y) {
  COORD pos={2*x,y};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

typedef enum{
  NOCURSOR,SOLIDCURSOR,NORMALCURSOR
}CURSOR_TYPE;

void setcursortype(CURSOR_TYPE c){
  CONSOLE_CURSOR_INFO CurInfo;
  
  switch(c){
    case NOCURSOR:
      CurInfo.dwSize=1;
      CurInfo.bVisible=FALSE;
      break;
      
    case SOLIDCURSOR:
      CurInfo.dwSize=100;
      CurInfo.bVisivle=TRUE;
      break;
      
    case NORMALCURSOR:
      CurInfo.dwSize=20;
      CurInfo.bVisible=TRUE;
      break;
  }
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE)&CurInfo);
}

  
  
   
  
  

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






