#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define HEIGHT 50
#define WIDTH 100
#define BACKGROUND '-'
#define CELL '0'
#define SPEED 100

typedef enum{
	DEAD,
	ALIVE,
}State;

typedef struct{
	State symbol;
}Cell;


Cell map[HEIGHT][WIDTH]={0};


void initMap(){
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
			map[i][j].symbol=DEAD;
		}
	}
}

int getNeighbour(int i,int j){
	int alive = 0;
	for(int p=-1;p<=1;p++){
		for(int q=-1;q<=1;q++){
			if(p==0 && q ==0) continue;
			int row = (i + p + HEIGHT)%HEIGHT;
			int col = (j + q + WIDTH)%WIDTH;
			if(map[row][col].symbol==ALIVE) alive++;
		}
	}
	return alive;
}

void nextState(){
	Cell nextMap[HEIGHT][WIDTH]={0};
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
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
	memcpy(map,nextMap,sizeof(Cell)*HEIGHT*WIDTH);
}

void drawGlider(int offsetRow, int offsetCol){
	map[offsetRow][offsetCol+1].symbol=ALIVE;
	map[offsetRow+1][offsetCol+2].symbol=ALIVE;
	map[offsetRow+2][offsetCol+0].symbol=ALIVE;
	map[offsetRow+2][offsetCol+1].symbol=ALIVE;
	map[offsetRow+2][offsetCol+2].symbol=ALIVE;
}

int printMap(){
	int aliveCells = 0;
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
			if(map[i][j].symbol==DEAD){
				printf("%c",BACKGROUND);
			}
			else{
				printf("%c",CELL);
				aliveCells++;
			}
		}
		printf("\n");
	}
	return aliveCells;
}


int main(){
	initMap();
	drawGlider(20%HEIGHT,40%WIDTH);
	while(printMap()){
		nextState();
		usleep(SPEED*1000);
		system("clear");
	}
	return 0;
}


