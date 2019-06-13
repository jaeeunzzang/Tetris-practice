#include <stdio.h>
#include <Windows.h>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACEBAR 32


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






