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

int check_crush(int bx,int by, int b_rotation); //bx,by위치에 로테이션 회전값을 갖는경우 충돌판단

void gotoxy(int x,int y) {
  COORD pos={2*x,y};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

typedef enum{
  NOCURSOR,SOLIDCURSOR,NORMALCURSOR
}CURSOR_TYPE; //커서 숨기는 함수에 사용되는 열거형 

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
} //커서 숨기는 함수


/* 메인함수 */

int main()
{
  int i;
  
  srand((unsigned)time(NULL)); //랜덤함수 난수생성
  setcursortype(NOCURSOR); //커서없앰
  title();
  reset();
  
  while(1)
  {
    for(i=0;i<5;i++) //블록이 한칸 떨어지는동안 다섯번 키 입력 가능.
    {
      check_key(); //키 입력 확인
      draw_main(); //화면을 그림
      Sleep(speed); //게임 속도 조절
      if(crush_on&&check_crush(bx,by+1,b_rotation)==false)
        Sleep(100);
      //블록이 충돌중일경우 추가로 이동 및 회전할 시간을 갖음
      
      if(space_key_on==1) //스페이스바를 누른 경우 추가로 이동 및 회전할수 없음.
      {
        space_key_on=0;
        break;
      }
    }
    drop_block(); //블록 한칸 내림
    check_level_up(); //레벨업 체크
    check_game_over(); //게임오버 체크
    if(new_block_on==1) //뉴블럭 플래그가 있으면 새로운블럭 생성
      new_block();
  }
}
    
  /* 타이틀 함수 작성 */

void title(void)
{
  int x=5;
  int y=4;
  int cnt;
  
  goto(x,y+0);  printf("┌───────────────────┐"); Sleep(100);
  goto(x,y+1);  printf("│                   │"); Sleep(100);
  goto(x,y+2);  printf("│                   │"); Sleep(100);
  goto(x,y+3);  printf("│                   │"); Sleep(100);
  goto(x,y+4);  printf("└───────────────────┘"); Sleep(100);
  goto(x,y+5);  printf("Press Any Key to Start");
  
  for(cnt=0;;cnt++)
  {
    if(kbhit())
      break;
    if(cnt%200==0)
    {
      goto(x+1,y+2);
      printf("☆ T E T R I S ☆");
    }
    if(cnt%200-100==0)
    {
      goto(x+1,y+2);
      printf("                 ");
    }
    
    while(kbhit())
      getch();
  }
  
  void reset(void)
  {
    FILE *file=fopen("score.dat","rt");
    if(file==0)
      best_score=0;
    else
    {
      fscanf(file,"%d",&best_score);
      fclose(file);
    }
    
    level=1;
    score=0;
    level_goal=1000;
    key=0;
    crush_on=0;
    cnt=0;
    speed=100;
    
    system("cls");
    reset_main();
    draw_map();
    draw_main();
    
    b_type_next=rand()%7;
    new_block();
    
  }
  
  void reset_main(void)  //게임판 초기화
  {
    int i,j;
    
    for(i=0;i<MAIN_Y;i++) //게임판 0으로 초기화
    {
      for(j=0;j<MAIN_X;j++)
      {
        main_org[i][j]=0;
        main_cpy[i][j]=0;
      }
    }
    for(j=1;j<MAIN_X;j++) //y값이 3인 위치에 천장을 만듬.
    {
      main_org[3][j]=CEILLING;
    }
    for(i=1;i<MAIN_Y-1;i++) //좌우 벽 생성
    {
      main_org[i][0]=WALL;
      main_org[i][MAIN_X-1]=WALL;
    }
      for(j=0;j<MAIN_X;j++) //바닥 벽 생성
      {
        main_org[MAIN_Y-1][j]=WALL;
      }
  }
  
  void reset_main_cpy(void) //main_cpy 초기화
  {
    int i,j;
    
    for(i=0;i<MAIN_Y;i++) //게임판에 게임에 사용되지 않는 숫자를 넣음
    {
      for(j=0;j<MAIN_X;j++) //main_org와 같은 숫자가 없게 하기 위해서.
      {
        main_cpy[i][j]=100;
      }
    }
  }
  
 void draw_map(void) //게임상태표시 함수
 {
   int y=3;
   
   gotoxy(STATUS_X_ADJ,STATUS_Y_LEVEL=y);
   printf("LEVEL : %5d",level);
   gotoxy(STATUS_X_ADJ,STATUS_Y_GOAL=y+1);
   printf("GOAL : %5d",10-cnt);
   gotoxy(STATUS_X_ADJ,y+2); printf("+- NEXT -+");
   gotoxy(STATUS_X_ADJ,y+3); printf("|        |");
   gotoxy(STATUS_X_ADJ,y+4); printf("|        |");
   gotoxy(STATUS_X_ADJ,y+5); printf("|        |");
   gotoxy(STATUS_X_ADJ,y+6); printf("|        |");
   gotoxy(STATUS_X_ADJ,y+7); printf("+--------+");
   gotoxy(STATUS_X_ADJ,y+8); printf("YOUR SCORE :");
   gotoxy(STATUS_X_ADJ,STATUS_Y_SCORE=y+9); printf("    %6d",score);
   gotoxy(STATUS_X_ADJ,y+10); printf("LAST SCORE :");
   gotoxy(STATUS_X_ADJ,y+11); printf("    %6d",last_score);
   gotoxy(STATUS_X_ADJ,y+12); printf("BEST SCORE :");
   gotoxy(STATUS_X_ADJ,y+13); printf("    %6d",best_score);
   gotoxy(STATUS_X_ADJ,y+14); printf(""); /*여기서부터 조작법설명*/
 }
  
  void draw_main(void) //게임판을 그려주는 함수
  {
    int i,j;
    
    for(j=1;j<MAIN_X-1;j++)
    {
      if(main_org[3][j]==EMPTY) //계속 새로운 블럭이 지나가서 지워지면 새로 그림
        main_org[3][j]=CEILLING;
    }
    
    for(i=0;i<MAIN_Y;i++)
    {
      for(j=0;j<MAIN_X;j++)
      {
        if(main_cpy[i][j]!=main_org[i][j])
          /*cpy랑 비교해서 값이 달라진 부분만 새로 그려줌
          없으면 게임판 전체를 계속 새로그려서 느려지고 반짝거림.*/
        {
          gotoxy(MAIN_X_ADJ+j,MAIN_Y_ADJ+i);
          switch(main_org[i][j])
          {
            case EMPTY:
              printf(" "); //빈칸
              break;
            case CEILLING:
              printf(". "); //천장
              break;
            case WALL:
              printf("▩"); //벽
              break;
            case INACTIVE_BLOCK:
              printf("□"); //굳은 블록
              break;
            case ACTIVE_BLOCK:
              printf("■"); //움직이는 블록
              break;
          }
        }
      }
    }
    for(i=0;i<MAIN_Y;i++) //게임판을 그린 후 main_cpy에 복사.
    {
      for(j=0;j<MAIN_X;j++)
      {
        main_cpy[i][j]=main_org[i][j];
      }
    }
  }
  
  void new_block(void)
  {
    int i,j;
    bx=(MAIN_X/2)-1; //블록 생성위치 X 좌표
    by=0; //블록 생성위치 Y 좌표
    b_type=b_type_next; //다음 블럭값을 가져옴
    b_type_next=rand()%7; //다음 블럭 생성
    b_rotation=0; //회전0번으로 생성
    new_block_on=0;
    
    for(i=0;i<4;i++)
    {
      for(j=0;j<4;j++)
      {
        if(blocks[b_type][b_rotation][i][j]==1)
          main_org[by+i][bx+j]=ACTIVE_BLOCK;
      }
    }
    for(i=1;i<3;i++)
    {
      for(j=0;j<4;j++)
      {
        if(blocks[b_type_next][0][i][j]==1)
           {
             gotoxy(STATUS_X_ADJ+2+j,i+6);
             printf("■");
           }
           else
           {
             gotoxy(STATUS_X_ADJ+2+j,i+6);
             printf(" ");
           }
      }
    }
  }
  
  void check_key(void)
  {
    key=0;
    
    if(kbhit())
    {
      key=getch();
      if(key==224)
      {
        do
        {
          key=getch();
        } while(key==224);
        /*방향키*/
        switch(key)
        {
          case LEFT:
            if(check_crush(bx-1,by,b_rotation)==true)
              move_block(LEFT);
            break;
          case RIGHT:
            if(check_crush(bx+1,by,b_rotation)==true)
              move_block(RIGHT);
            break;
          case DOWN:
            if(check_crush(bx,by+1,b_rotation)==true)
              move_block(DOWN);
            break;
          case UP:
            if(check_crush(bx,by,(b_rotation+1)%4)==true)
              move_block(UP); //회전체크후 가능하면 회전
            else if(crush_on==1&&check_crush(bx,by-1(b_rotation+1)%4)==true)
              move_block(100); //바닥에 닿은경우 위쪽으로 한칸 띄워서 회전이 가능하면 회전.
            /*break 여긴안하나?*/
        }  
      }
       /*특수동작*/
      else
      {
        switch(key)
        {
          case SPACE: //스페이스바 누르면
            space_key_on=1;
            while(crush_on==0) //바닥에 닿을때까지 블록 이동
            {
              drop_block();
              score+=level; //추가점수 보너스
              gotoxy(STATUS_X_ADJ,STATUS_Y_SCORE);
              printf("    %6d",score);
            }break;
          case p:
          case P:
            pause();
            break; //일시정지
          case ESC:
            system("cls");
            exit(0); //게임종료
        }
      }
    }
   while(kbhit())
     getch();
  } //키버퍼 비우기
  
  void drop_block(void)
  {
    int i,j;
    
    if(crush_on&&check_crush(bx,by+1,b_rotation)==true)
      crush_on=0; //밑이 비어있으면 크러시 플래그 끔
    if(crush_on&&check_crush(bx,by+1,b_rotation)==false)
    {
      for(i=0;i<MAIN_Y;i++)
      {
        for(j=0;j<MAIN_X;j++) //밑이 비어있지 않고 크러시 플래그가 켜져있으면
        {
          if(main_org[i][j]==ACTIVE_BLOCK)
            main_org[i][j]=INACTIVE_BLOCLK; //현재 조작중인 블럭을 굳힘.
        }
        crush_on=0; //플래그 끔
        check_line(); //라인체크
        new_block_on=1; //새로운 블럭생성 플래그 켬
        return; //함수 종료
      }
      if(check_crush(bx,by+1,b_rotation)==true)
        move_block(DOWN); //밑이 비어있으면 밑으로 한칸이동
      if(check_crush(bx,by+1,b_rotation)==false)
        crush_on++; //밑으로 이동이 안되면 크러시플래그 켬
    }
    
   int check_crush(int bx,int by,int b_rotation) //지정된 좌표와 회전값으로 충돌이 있는지 검사.
   {
     int i,j;
     
     for(i=0;i<4;i++)
     {
       for(j=0;j<4;j++) //지정된 위치의 게임판과 블록모양을 비교해서 겹치면 false를 리턴.
       {
         if(blocks[b_type][b_rotation][i][j]==1&&main_org[by+i][bx+j]>0)
           return false;
       }
     }
     return true; //겹치는게 없으면 true리턴
   }; //세미콜론은 왜??
    
  void move_block(int dir) //블록 이동 함수
  {
    int i,j;
    
    switch(dir)
    {
      case LEFT:
        for(i=0;i<4;i++) //현재 좌표의 블럭을 지움
        {
          for(j=0;j<4;j++)
          {
            if(blocks[b_type][b_rotation][i][j]==1)
              main_org[bu+i][bx+j-1]=ACTIVE_BLOCK; //왼쪽으로 한칸 가서 active block지움
          }
        }
        bx--; //좌표값 이동.
        break;
        
      case RIGHT:
        for(i=0;i<4;i++)
        {
          for(j=0;j<4;j++)
          {
            if(blocks[b_type][b_rotation][i][j]==1)
              main_org[by+i][bx+j+1]=ACTIVE_BLOCK;
          }
        }
        bx++;
        break;
        
      case DOWN:
        for(i=0;i<4;i++)
        {
          for(j=0;j<4;j++)
          {
            if(blocks[b_type][b_rotation][i][j]==1)
              main_org[by+i+1][bx+j]=ACTIVE_BLOCK;
          }
        }
        by++;
        break;
        
      case UP: //회전
        for(i=0;i<4;i++) //현재좌표 블럭 지움
        {
          for(j=0;j<4;j++)
          {
            if(blocks[b_tupe][b_rotation][i][j]==1)
              main_org[by+i][bx+j]=EMPTY;
          }
        }
        b_rotation=(b_rotation+1)%4; //회전값 1 증가시킴. 4번째 회전시 0번 상태로.
        
        for(i=0;i<4;i++) //회전된 블록 나타냄.
        {
          for(j=0;j<4;j++)
          {
            if(blocks[b_type][b_rotation][i][j]==1)
              main_org[by+i][bx+j]=ACTIVE_BLOCK;
          }
        }
        break;
        
      case 100: //블럭이 바닥 혹은 다른 블록과 닿은 상태에서 한칸위로 올려 회전이 가능한 경우의 특수동작
        for(i=0;i<4;i++)
        {
          for(j=0;j<4;j++)
          {
            if(blocks[b_type][b_rotation][i][j]==1)
              main_org[by+i][bx+j]=EMPTY;
          }
        }
        b_rotation=(b_rotation+1)%4;
        for(i=0;i<4;i++)
        {
          for(j=0;j<4;j++)
          {
            if(blocks[b_type][b_rotation][i][j]==1)
              main_org[by+i-1][bx+j]=ACTIVE_BLOCK;
          }
          by--;
          break;
        }
    }
        
    void check_line(void)
    {
      int i,j,k,l;
      int block_amount; //한 줄의 블록개수 저장 변수
      int combo=0; //콤보갯수 저장변수. 지정및 초기화
      
      for(i=MAIN_Y-2;i>3;) //i=MAIN_Y-2 :밑쪽 벽의 윗칸부터 -> i>3 : 천장(3)아래까지 검사
      {
        block_amount=0; //블록 갯수 저장변수 초기화
        for(j=1;j<MAIN_X-1;j++) //벽과 벽 사이의 블럭 갯수를 셈
        {
          if(main_org[i][j]>0)
            block_amount++;
        }
        if(block_amount==MAIN_X-2) //블럭 한줄이 가득 찬 경우
        {
          if(level_up_on==0) //레벨업상태가 아닌경우
          {
            score+=100*level; //점수 추가
            cnt++; //지운 줄갯수 카운트 증가
            combo++; //콤보수 증가
          }
          for(k=i;k>1;k--) //윗줄이 천장이 아닌경우에만 윗줄을 한칸씩 내림.
          {
            for(l=1;l<MAIN_X-1;l++)
            {
              if(main_org[k-l][l]!=CELLING)
                main_org[k][l]=main_org[k-1][l];
              if(main_org[k-l][l]==CEILLING)
                main_org[k][l]=EMPTY; //윗줄이 천장인 경우에는 빈칸을 채움.
            }
          }
        }
        else i--;
      }
      if(combo) //줄 삭제가 있는경우 점수와 레벨목표를 새로 표시
      {
        if(combo>1) //2콤보 이상인 경우 보너스 및 메세지를 게임판에 띄웠다가 지움
        {
          gotoxy(MAIN_X_ADJ+(MAIN_X/2)-1,MAIN_Y_ADJ+by-2);
          printf("%d COMBO!",combo);
          Sleep(500);
          score+=(combo*level*100);
          reset_main_cpy(); //텍스트를 지우기 위해 메인cpy초기화.
        }
        gotoxy(STATUS_X_ADJ,STATUS_Y_GOAL);
        printf("GOAL : %5d",(cnt<=10)?10-cnt:0);
        gotoxy(STATUS_X_ADJ,STATUS_Y_SCORE);
        printf("   %6d",score);
      }
    }
   
    void check_level_up(void)
    {
      int i,j;
      
      if(cnt>=10)
      {
        draw_main();
        level_up_on=1;
        level+=1;
        cnt=0;
        
        for(i=0;i<4;i++)
        {
          gotoxy(MAIN_X_ADJ+(MAIN_X/2)-3,MAIN_Y_ADJ+4);
          printf("     ");
          gotoxy(MAIN_X_ADJ+(MAIN_X/2)-2,MAIN_Y_ADJ+6);
          printf("    ");
          Sleep(200);
          
          gotoxy(MAIN_X_ADJ+(MAIN_X/2)-3,MAIN_Y_ADJ+4);
          printf("LEVEL UP!");
          gotoxy(MAIN_X_ADJ+(MAIN_X/2)-2,MAIN_Y_ADJ+6);
          printf("SPEED UP!");
          Sleep(200);
        }
        reset_main_cpy();
        /* 텍스트를 지우기 위해 main_cpy를 초기화
        main_cpy와 main_org가 전부 다르므로 다음번 draw()호출시 게임판 전체를 새로 그리게 된다.*/
       
        for(i=MAIN_Y-2;I>MAIN_Y-2-(level-1);i--) //레벨업 보상으로 각 레벨 -1의 수만큼 아래쪽 줄을 지움.
        {
          for(j=1;j<MAIN_X-1;j++)
          {
            main_org[i][j]=INACTIVE_BLOCK; 
            gotoxy(MAIN_X_ADJ+j;MAIN_Y_ADJ+i);
            printf("★"); //별로 아랫줄 채우고 
            Sleep(20);
          }
        }
        Sleep(100);
        check_line(); //채운것 지우기
        switch(level) //레벨별 속도조절
        {
          case 2:
            speed=50;
            break;
          case 3:
            speed=25;
            break;
          case 4:
            speed=10;
            break;
          case 5:
            speed=5;
            break;
          case 6:
            speed=4;
            break;
          case 7:
            speed=3;
            break;
          case 8:
            speed=2;
            break;
          case 9:
            speed=1;
            break;
          case 10:
            speed=0;
            break;
        }
        level_up_on=0;
        gotoxy(STATUS_X_ADJ,STATUS_Y_LEVEL);
        printf("LEVEL:%5d",level);
        gotoxy(STATUS_X_ADJ,STATUS_Y_GOAL);
        printf("GOAL:%5d",10-cnt);
      }
    }
 
   void check_game_over(void) //게임오버 화면
   {
     int i;
     int x=5;
     int y=5;
     
     for(i=1;i<MAIN_X-2;i++)
     {
       if(main_org[3][i]>0) //천장(위에서 세번째 줄) 에 inactive block 이 생성되면 게임오버.
       {
         gotoxy(x,y+0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
         gotoxy(x,y+1); printf("▤                           ▤");
         gotoxy(x,y+2); printf("▤ +-----------------------+ ▤");
         gotoxy(x,y+3); printf("▤ |  G A M E  O V E R . . | ▤");
         gotoxy(x,y+4); printf("▤ +-----------------------+ ▤");
         gotoxy(x,y+5); printf("▤ Y O U R  S C O R E : %6d  ▤",score);
         gotoxy(x,y+6); printf("▤                           ▤");
         gotoxy(x,y+7); printf("▤  Press Any Key To Restart ▤");
         gotoxy(x,y+8); printf("▤                           ▤");
         gotoxy(x,y+9); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
         last_score=score; //게임 점수를 옮김.
         
         if(score>best_score) //최고기록 갱신시
         {
           FILE* file=fopen("score.dat","wt");
          gotoxy(x,y+6); printf("▤ ☆☆☆ BEST  SCORE! ☆☆☆ ▤");
           
           if(file==0) //파일에러 메시지
           {
             gotoxy(0,0);
             printf("FILE ERROR: SYSTEM CANNOT WRITE BEST SCORE ON\"SCORE.DAT\"");
           }
          else
          {
            fprintf(file,"%d",score);
            fclose(file);
          }
         }
         Sleep(1000);
         while (kbhit())
           getch();
         key=getch();
         reset();
       }
     }
   }
 
    void pause(void)
    {
      int i,j;
      int x=5;
      int y=5;
      
      for(i=1;i<MAIN_X-2;i++)
      {
         gotoxy(x,y+0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
         gotoxy(x,y+1); printf("▤                           ▤");
         gotoxy(x,y+2); printf("▤ +-----------------------+ ▤");
         gotoxy(x,y+3); printf("▤ |      P A U S E . .    | ▤");
         gotoxy(x,y+4); printf("▤ +-----------------------+ ▤");
         gotoxy(x,y+5); printf("▤ Press Any Key To Resume.. ▤");
         gotoxy(x,y+6); printf("▤                           ▤");
         gotoxy(x,y+7); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
      }
      getch();
      
      system("cls"); //화면 지우고 새로 그림
      reset_main_cpy();
      draw_main();
      draw_map();
      
      for(i=1;i<3;i++)
      {
        for(j=0;j<4;j++)
        {
          if(blocks[b_type_next][0][i][j]==1)
          {
            gotoxy(MAIN_X+MAIN_X_ADJ+3+j,i+6);
            printf("■");
          }
          else
          {
            gotoxy(MAIN_X+MAIN_X_ADJ+3+j,i+6);
            printf(" ");
          }
        }
      }
    }
      
