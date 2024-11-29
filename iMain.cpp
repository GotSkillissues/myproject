# include "iGraphics.h"
#include<windows.h>
#include<iostream>
using namespace std;
#include<string.h>
#include<stdlib.h>
int balltimer=0;
void restart();
void bricks();
int x1=1200,Y=675;
int paddle_x=x1/2-75,paddle_y=1,paddle_dx=150,paddle_dy=7;
int ball_x=paddle_x+70,ball_y=paddle_y+16, ball_dx=5,ball_dy=7;
char paddle[]="bc\\007.bmp";
char starting[2][100]={"bc\\home2.bmp","dxball\\2.bmp"};
char background[20]={"bg.bmp"};
char home[6][2000]={"bc\\home1.bmp","menu\\Play.bmp","menu\\New.bmp","menu\\Resume.bmp","menu\\Options.bmp","menu\\Exit.bmp"};
int bcIndex = 0;
bool start=false;
bool musicOn=true;
int s=1;
struct brick{
	int x;
	int y;
	int dx=100;
	int dy=20;
	bool show=true;
};
struct brick b[60];

/*
	function iDraw() is called again and again by the system.

	*/

void iDraw() {
	if(s==1){
		iClear();
		iShowBMP(0,0,starting[0]);
		iShowBMP2(425,50,starting[1],0);
	
	}
	else if(s==2){
		iClear();
		iShowBMP(0,0,home[0]);
		iShowBMP(945,60,home[5]);
		iShowBMP(945,125,home[4]);
		iShowBMP(945,190,home[3]);
		iShowBMP(945,255,home[2]);
	}

	else{
	//place your drawing codes here
	iClear();
	//iSetColor(186, 208, 182);
	//iFilledRectangle(paddle_x, paddle_y, paddle_dx, paddle_dy);
	//iFilledRectangle(10, 30, 20, 20);
	iShowBMP(0,0,background);
	iSetColor(237,145,72);
	iFilledCircle(ball_x,ball_y,7);
	iShowBMP2(paddle_x, paddle_y ,paddle,0);
	for(int i=0;i<60;i++){
		if(i%2==0){
			iSetColor( 255,255,255);
			iFilledRectangle(b[i].x,b[i].y,b[i].dx-1,b[i].dy-1);
		}
		else{
			iSetColor( 0,255,255);
			iFilledRectangle(b[i].x,b[i].y,b[i].dx-1,b[i].dy-1);
		}
	}
	//iShowBMP(ball_x,ball_y,bc[bcIndex+1]);
	//iText(40, 4, "Hi, I am iGraphics");
	}
	}
	
void bricks(){
	int startx=100;
int starty=350;
	for(int i=0;i<60;i++){
		b[i].x=startx;
		b[i].y=starty;
		startx+=100;
		if(startx>1000){
			startx=100;
			starty+=20;
		}


	}
}
void ballchange(){
	if(start){
	ball_x+=ball_dx;
	ball_y+=ball_dy;
	if(ball_x<=0 || ball_x>=1200){
		ball_dx=-ball_dx;
	}
	if(ball_y<=0||ball_y>=650){
		ball_dy=-ball_dy;
	}
	if(ball_x>=paddle_x && ball_x<=paddle_dx+paddle_x && ball_y<=(paddle_y+paddle_dy+15) && ball_y>=paddle_y){
		ball_dy=-ball_dy;
	}
	else{
		if(ball_y<paddle_y)
		restart();
	}
}
}
void restart(){
paddle_x=x1/2-75,paddle_y=1,paddle_dx=150,paddle_dy=7;
ball_x=paddle_x+70,ball_y=paddle_y+16,ball_dx=5,ball_dy=7;
start=false;
}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my) {
	printf("x = %d, y= %d\n",mx,my);
	//place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		//place your codes here
		//	printf("x = %d, y= %d\n",mx,my);
	if(mx>=945 && mx<1095 && my>=255 &&my<305){
		s=3;
	}
	if(mx>=945 && mx<1095 && my>=60 &&my<110){
		exit(0);
	}

	if(ball_x > 1200 || ball_x < 0)ball_dx = -ball_dx;
	if(ball_y > 700 || ball_y < 0)ball_dy = -ball_dy;
		}
	
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		//place your codes here

	}
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key) {
	if(s!=1){
	if (key == 'a'){
		paddle_x-=8;
		if(!start)
		ball_x-=8;
		if(paddle_x<0){
		paddle_x=0;
		if(!start)
		ball_x=70;
		}
	}
	if (key== 'd'){
		paddle_x+=8;
		if(!start)
		ball_x+=8;
		if(paddle_x>1060||paddle_x>1770){
		paddle_x=1060;
		if(!start)
		ball_x=paddle_x+70;
		}
	}
	
	if(key==' '){
		if(!start)
		start=true;
	}
	}
	if(s==1){
		if(key==' ')
		s=2;
	}
	
	//place your codes for other keys here
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/
void iSpecialKeyboard(unsigned char key) {


	if (key == GLUT_KEY_LEFT) {
		paddle_x-=8;
		if(!start)
		ball_x-=8;
		if(paddle_x<0){
		paddle_x=0;
		if(!start)
		ball_x=70;
		}
	}
	if (key == GLUT_KEY_RIGHT) {
		paddle_x+=8;
		if(!start)
		ball_x+=8;
		if(paddle_x>1060||paddle_x>1770){
		paddle_x=1060;
		if(!start)
		ball_x=paddle_x+70;
		}
	}
	if(key==GLUT_KEY_END)
	exit(0);
	//place your codes for other keys here
}


int main() {
	//place your own initialization codes here.
	bricks();
	iSetTimer(16, ballchange);
	
	iInitialize(x1, Y, "demo0000");
}