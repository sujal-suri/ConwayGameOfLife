#include<stdio.h>
#include<stdlib.h>
#include<string.h> //Used for memCpy
#include<stdbool.h>
#include "raylib.h"

// -----------------------------------------Constants Declaration---------------------------------------------//

#define SCREEN_HEIGHT 500
#define SCREEN_WIDTH 1000
#define CELL_PER_ROW 50
#define CELL_PER_COLUMN 25
#define DEAD_COLOR LIGHTGRAY
#define ALIVE_COLOR RAYWHITE
#define BACKGROUND_COLOR WHITE
#define MARGIN 1

// ----------------------------------------------------------------------------------------------------------//

// -----------------------------------------Struct  Declaration---------------------------------------------//
typedef enum{
	DEAD,
	ALIVE,
}State;

typedef struct{
	int x;
	int y;
}IntVector2;


typedef struct{
	IntVector2 position;
	State symbol;
}Cell;

// ----------------------------------------------------------------------------------------------------------//


// -----------------------------------------Global Variables-------------------------------------------------//

static int CELL_WIDTH = (SCREEN_WIDTH/CELL_PER_ROW) - MARGIN;
static int CELL_HEIGHT = (SCREEN_HEIGHT/CELL_PER_COLUMN) - MARGIN;
Cell map[CELL_PER_COLUMN][CELL_PER_ROW]={0};
bool pause = false;

// ---------------------------------------------------------------------------------------------------------//


// -----------------------------------------Fuction Declaration---------------------------------------------//

void nextState();
void DrawGlider(int offsetRow, int offsetCol);
void InitGameOfLife(Cell* frame);
void DrawGameOfLife();
int PrintGameOfLife();
void UpdateGameOfLife();
void AlterCells();

// ---------------------------------------------------------------------------------------------------------//



int main(){
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Conway Game Of Live");
	SetTargetFPS(12);
	InitGameOfLife(&map);
	DrawGlider(10,20);
	while(!WindowShouldClose()){
		DrawGameOfLife();
		if(IsKeyPressed(KEY_SPACE)){
			pause = !pause;
		}
	}
	CloseWindow();
	return 0;
}

void DrawGameOfLife(void){
	BeginDrawing();
	ClearBackground(BACKGROUND_COLOR);
	PrintGameOfLife();
	AlterCells();
	if(!pause){
		UpdateGameOfLife();
	}
	EndDrawing();
}

void AlterCells(){
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		int mouseX = GetMouseX();
		int mouseY = GetMouseY();
		int row = mouseY/(CELL_HEIGHT+MARGIN);
		int col = mouseX/(CELL_WIDTH+MARGIN);
		map[row][col].symbol = !map[row][col].symbol;
	}

}

int PrintGameOfLife(){
	int alive = 0;
	for(int i=0; i<CELL_PER_COLUMN ; i++){
		for(int j=0 ;j<CELL_PER_ROW ; j++){
			if(map[i][j].symbol==DEAD){
				DrawRectangle(map[i][j].position.x,map[i][j].position.y,CELL_WIDTH, CELL_HEIGHT, DEAD_COLOR);
			}
			else{
				DrawRectangle(map[i][j].position.x,map[i][j].position.y,CELL_WIDTH, CELL_HEIGHT, ALIVE_COLOR);
				alive++;
			}
		}
	}
	return alive;
}

void UpdateGameOfLife(void){
	nextState();
}


void InitGameOfLife(Cell *frame){
	int curX = 0;
	int curY = 0;
	for(int i=0;i<CELL_PER_COLUMN;i++){
		for(int j=0;j<CELL_PER_ROW;j++){
			int index = i*CELL_PER_ROW + j;
			frame[index].symbol = DEAD;
			frame[index].position.x = curX;
			frame[index].position.y = curY;
			curX += CELL_WIDTH+MARGIN;
		}
		curX = 0;
		curY += CELL_HEIGHT+MARGIN;
	}
}



int getNeighbour(int i,int j){
	int alive = 0;
	for(int p=-1;p<=1;p++){
		for(int q=-1;q<=1;q++){
			if(p==0 && q ==0) continue;
			int row = (i + p + CELL_PER_COLUMN)%CELL_PER_COLUMN;
			int col = (j + q + CELL_PER_ROW)%CELL_PER_ROW;
			if(map[row][col].symbol==ALIVE) alive++;
		}
	}
	return alive;
}

void nextState(){
	Cell nextMap[CELL_PER_COLUMN][CELL_PER_ROW]={0};
	InitGameOfLife(&nextMap);
	for(int i=0;i<CELL_PER_COLUMN;i++){
		for(int j=0;j<CELL_PER_ROW;j++){
			int alive = getNeighbour(i,j);
			switch(alive){
				case 0:
				case 1:
					nextMap[i][j].symbol = DEAD;
					break;
				case 2:
					if(map[i][j].symbol==DEAD){
						nextMap[i][j].symbol=DEAD;
						break;
					}
				case 3:
					nextMap[i][j].symbol = ALIVE;
					break;
				case 4:
				case 5:
				case 6:
				default:
					nextMap[i][j].symbol = DEAD;
					break;
			}
		}
	}
	memcpy(map,nextMap,sizeof(Cell)*(CELL_PER_ROW*CELL_PER_COLUMN));
}

void DrawGlider(int offsetRow, int offsetCol){
	map[offsetRow][offsetCol+1].symbol=ALIVE;
	map[offsetRow+1][offsetCol+2].symbol=ALIVE;
	map[offsetRow+2][offsetCol+0].symbol=ALIVE;
	map[offsetRow+2][offsetCol+1].symbol=ALIVE;
	map[offsetRow+2][offsetCol+2].symbol=ALIVE;
}

