#include <stdio.h>
#include <Windows.h>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACEBAR 32


typedef struct _tetris{
  int level;
  int gameover;
  int score;
  int rotation;
  int curblock;
  int nextblock;
  int absX,absY;
  int curX,curY;
  }Tetris;
  
  char key;
  key=getch();
  
  




