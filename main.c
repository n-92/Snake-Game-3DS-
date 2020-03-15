/*
	Snake Game /f/ 3DS Console
	Dev: Aung Naing Oo
*/

#include <3ds.h>
#include <stdio.h>
#include <math.h>

const int widthOfScreen = 40;
const int heightOfScreen = 25;
enum Direction {STOP, LEFT, TOP, RIGHT, BOTTOM};
bool gameover = false;
const int SNAKETAILLENGTHARRSIZE = 150;
int score = 0;

int snakeTailLenth = 0;

typedef struct{
    int x;
    int y;
} Snake;

typedef struct{
    int x;
    int y;
} Food;

typedef struct{
    int x;
    int y;
} SnakeTail;


enum Direction dir;
Snake snake;
Food food;


int main(int argc, char **argv)
{
	gfxInitDefault();

	//Initialize console on top screen. Using NULL as the second argument tells the console library to use the internal console structure as current one
	consoleInit(GFX_TOP, NULL);
	
	//declare the snake and the food
	
	
	food.x = rand()%widthOfScreen+1;    //return values between 1 to widthOfScreen-1
    food.y = rand()%heightOfScreen+1;   //return values between 1 to heightOfscreen -1
    snake.x = widthOfScreen/2;
    snake.y = widthOfScreen/2;
	
	
	SnakeTail snakeTail[SNAKETAILLENGTHARRSIZE];
    
	bool isTailBeingPrinted = false;
    int prevTailX=0, prevTailY=0;
    int chainx=0, chainy=0;
	
	//The top screen has 30 rows and 50 columns
	//The bottom screen has 30 rows and 40 columns
	int horizontal = 0;
	int vertical = 0;

	while (aptMainLoop() && !gameover)
	{	
		//previous snake position
        prevTailX = snake.x;
        prevTailY = snake.y;
/////////////////////////////////////Input///////////////////////////////
        
		hidScanInput(); //Scan all the inputs. This should be done once for each frame
		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) gameover= true; // break in order to return to hbmenu
		else if(kDown & KEY_DLEFT)  dir = LEFT;
		else if(kDown & KEY_DRIGHT)  dir = RIGHT;
		else if(kDown & KEY_DUP)  dir = TOP;
		else if(kDown & KEY_DDOWN)  dir = BOTTOM;

/////////////////////////////////////Input///////////////////////////////

        if(dir == RIGHT) snake.x++;
        else if(dir == LEFT) snake.x--;
        else if(dir == TOP) snake.y--;
        else if(dir == BOTTOM) snake.y++;
		
		//for each tailComponent, update the tail coordinates
        for(int i=0; i<snakeTailLenth; i++){
            chainx = snakeTail[i].x;    //store the value of the first tail X position
            chainy = snakeTail[i].y;    //store the value of the first tail Y position
            snakeTail[i].x = prevTailX; //pass the value of the snake head X to the tail
            snakeTail[i].y = prevTailY; //pass the value of the snake head Y to the tail
            prevTailX = chainx;         //reload the value of the last known tail to previous Tail X
            prevTailY = chainy;         //reload the value of the last known tail to previous Tail X
        }
		
		for(int vertical=1; vertical<heightOfScreen; vertical++){
            for(int horizontal=1; horizontal<widthOfScreen;horizontal++){

                //printf("\x1b[%d;%dH*",vertical, horizontal);
				
                if(vertical == 1 || vertical== heightOfScreen -1){
					printf("\x1b[%d;%dH*",vertical, horizontal);
                }
                else if(horizontal ==1 || horizontal == widthOfScreen -1){
					printf("\x1b[%d;%dH*",vertical, horizontal);
                }
                else if(horizontal== food.x && vertical == food.y){
					printf("\x1b[%d;%dH@",vertical, horizontal);
                }
                else if (horizontal == snake.x && vertical == snake.y){
					printf("\x1b[%d;%dHO",vertical, horizontal);
                }
                else{
                    bool isTailBeingPrinted = false;
                    for(int i=0; i<snakeTailLenth;i++){
                        if (snakeTail[i].x == horizontal && snakeTail[i].y == vertical){
                            printf("\x1b[%d;%dHo",vertical, horizontal);
                            isTailBeingPrinted = true;
                            break;
                        }
                        if(snake.x ==snakeTail[i].x && snake.y == snakeTail[i].y){
                            gameover= true;
                        }
                    }
                    if(!isTailBeingPrinted)
						printf("\x1b[%d;%dH ",vertical, horizontal);
                }
            }
			printf("\x1b[%d;%dH\n",vertical, horizontal);
		}
		
		if (snake.x == food.x && snake.y == food.y){
			score++;
			snakeTailLenth++;
			
			//store the position where the snake was last when it got its food
			food.x = (rand()%widthOfScreen)+1;    //return values between 1 to widthOfScreen-1
			food.y = (rand()%heightOfScreen)+1;   //return values between 1 to heightOfscreen -1
			consoleClear();
        }
		printf("\x1b[2;40H Sco:%d",score);
		printf("\x1b[3;40H S_x: %d",snake.x);
		printf("\x1b[4;40H S_y: %d",snake.y);
		printf("\x1b[6;40H Dev:");
		printf("\x1b[7;40H Naing92");
		
		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();
	}

	gfxExit();
	return 0;
}
