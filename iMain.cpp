#include "iGraphics.h"
#include <windows.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

void showbrick();
void showbrick2();
void restart();
void bricks();
void bricks2();
void collcheck1();
void collcheck2();

int balltimer = 0;
int x1 = 1200, Y = 675;
int paddle_x = x1 / 2 - 75, paddle_y = 1, paddle_dx = 150, paddle_dy = 7;	  // paddle
int ball_x = paddle_x + 70, ball_y = paddle_y + 16, ball_dx = 5, ball_dy = 7; // ball
int s = 1;
int t=1;
int count = 0;
int lyf=3;

bool start = false;
bool musicOn = true;

char paddle[2000] = {"bc\\007.bmp"};
char starting[2][100] = {"bc\\home2.bmp", "dxball\\2.bmp"};																			  // loading page
char background[20] = {"bg.bmp"};																									  // background
char home[6][2000] = {"bc\\home1.bmp", "menu\\Play.bmp", "menu\\New.bmp", "menu\\Resume.bmp", "menu\\Options.bmp", "menu\\Exit.bmp"}; // menupage
char dx[3][2000]={"dxball\\4.bmp","dxball\\5.bmp","dxball\\6.bmp"};  //4==back, 5==pause, 10==options
char life[3][200]={"dxball\\x.bmp","dxball\\4x.bmp","dxball\\3x.bmp"};

struct brick
{
	int x;
	int y;
	int dx = 99;
	int dy = 19;
	bool show = true;
};
struct brick b[60];
struct brick b2[80];
struct brick b3[100];

/*
	function iDraw() is called again and again by the system.

	*/

void iDraw()
{
	if (s == 1) // startpage
	{
		iClear();
		iShowBMP(0, 0, starting[0]);
		iShowBMP2(450, 50, starting[1], 0);
	}
	else if (s == 2) // menu page
	{
		iClear();
		iShowBMP(0, 0, home[0]);
		iShowBMP(945, 60, home[5]);	 // exit
		iShowBMP(945, 125, home[4]); // options
		iShowBMP(945, 190, home[3]); // resume
		iShowBMP(945, 255, home[2]); // newgame
	}
	else if(s==4 && t==2){
		iShowBMP(0,0, dx[2]);
		iShowBMP(2,648,dx[0]);
	}

	else if(s==3)
	{
		// place your drawing codes here
		iClear();
		// iSetColor(186, 208, 182);
		// iFilledRectangle(paddle_x, paddle_y, paddle_dx, paddle_dy);
		// iFilledRectangle(10, 30, 20, 20);
		iShowBMP(0, 0, background);
		iSetColor(237, 145, 72);
		iFilledCircle(ball_x, ball_y, 7);
		iShowBMP2(paddle_x, paddle_y, paddle, 0);
		showbrick();
		iShowBMP(2,648,life[lyf-1]);
		if (count == 60)
		{
			iSetColor(119, 119, 255);
			iText(545, 150, "you won");
			restart();
		}
		// iShowBMP(ball_x,ball_y,bc[bcIndex+1]);
		// iText(40, 4, "Hi, I am iGraphics");
	}
}

void collcheck1()
{
    for (int i = 0; i < 60; i++)
    {
        if (b[i].show)
        {
            bool withinX = ball_x + 7 >= b[i].x && ball_x - 7 <= b[i].x + b[i].dx;
            bool withinY = ball_y + 7 >= b[i].y && ball_y - 7 <= b[i].y + b[i].dy;

            if (withinX && withinY)
            {
                b[i].show = false;
                count++;
                
                bool topCollision = ball_y - 7 < b[i].y && ball_y + 7 > b[i].y;
                bool bottomCollision = ball_y + 7 > b[i].y + b[i].dy && ball_y - 7 < b[i].y + b[i].dy;
                bool leftCollision = ball_x - 7 < b[i].x && ball_x + 7 > b[i].x;
                bool rightCollision = ball_x + 7 > b[i].x + b[i].dx && ball_x - 7 < b[i].x + b[i].dx;

                if (topCollision && (ball_dy > 0))
                {
                    ball_dy = -ball_dy;
                }
                else if (bottomCollision && (ball_dy < 0))
                {
                    ball_dy = -ball_dy;
                }
                else if (leftCollision && (ball_dx > 0))
                {
                    ball_dx = -ball_dx;
                }
                else if (rightCollision && (ball_dx < 0))
                {
                    ball_dx = -ball_dx;
                }
            }
        }
    }
}

void collcheck2()
{
    for (int i = 0; i < 60; i++)
    {
        if (b2[i].show)
        {
            bool withinX = ball_x + 7 >= b2[i].x && ball_x - 7 <= b2[i].x + b2[i].dx;
            bool withinY = ball_y + 7 >= b2[i].y && ball_y - 7 <= b2[i].y + b2[i].dy;

            if (withinX && withinY)
            {
                b[i].show = false;
                count++;
                
                bool topCollision = ball_y - 7 < b2[i].y && ball_y + 7 > b2[i].y;
                bool bottomCollision = ball_y + 7 > b2[i].y + b2[i].dy && ball_y - 7 < b2[i].y + b2[i].dy;
                bool leftCollision = ball_x - 7 < b2[i].x && ball_x + 7 > b2[i].x;
                bool rightCollision = ball_x + 7 > b2[i].x + b2[i].dx && ball_x - 7 < b2[i].x + b2[i].dx;

                if (topCollision && (ball_dy > 0))
                {
                    ball_dy = -ball_dy;
                }
                else if (bottomCollision && (ball_dy < 0))
                {
                    ball_dy = -ball_dy;
                }
                else if (leftCollision && (ball_dx > 0))
                {
                    ball_dx = -ball_dx;
                }
                else if (rightCollision && (ball_dx < 0))
                {
                    ball_dx = -ball_dx;
                }
            }
        }
    }
}

void showbrick()
{
	bricks();
	iShowBMP(1170,645,dx[1]);
	for (int i = 0; i < 60; i++)
	{
		if (b[i].show)
		{
			if (i % 2 == 0)
			{
				iSetColor(253,157,18);
				iFilledRectangle(b[i].x, b[i].y, b[i].dx, b[i].dy);
			}
			else
			{
				iSetColor(197,73,161);
				iFilledRectangle(b[i].x, b[i].y, b[i].dx, b[i].dy);
			}
		}
	}
}
void showbrick2()
{
	bricks2();
	iShowBMP(1170,645,dx[1]);
	for (int i = 0; i < 80; i++)
	{
		if (b2[i].show)
		{
			if (i % 2 == 0)
			{
				iSetColor(255, 255, 255);
				iFilledRectangle(b2[i].x, b2[i].y, b2[i].dx, b2[i].dy);
			}
			else
			{
				iSetColor(0, 255, 255);
				iFilledRectangle(b2[i].x, b2[i].y, b2[i].dx, b2[i].dy);
			}
			if (i >= 0 && i < 10 || i >= 70 && i < 80)
			{
				iSetColor(100, 150, 100);
				iFilledRectangle(b2[i].x, b2[i].y, b2[i].dx, b2[i].dy);
			}
		}
	}
}
void bricks()
{
	int startx = 100;
	int starty = 350;
	for (int i = 0; i < 60; i++)
	{
		b[i].x = startx;
		b[i].y = starty;
		startx += 100;
		if (startx > 1000)
		{
			startx = 100;
			starty += 20;
		}
	}
}
void bricks2()
{
	int startx = 100;
	int starty = 350;
	for (int i = 0; i < 80; i++)
	{
		b2[i].x = startx;
		b2[i].y = starty;
		startx += 100;
		if (startx > 1000)
		{
			startx = 100;
			starty += 20;
		}
	}
}
void ballchange()
{
	if (start)
	{
		ball_x += ball_dx;
		ball_y += ball_dy;
		collcheck1();
		//collcheck2();
		if (ball_x <= 0 || ball_x >= 1200)
		{
			ball_dx = -ball_dx;
		}
		if (ball_y <= 0 || ball_y >= 650)
		{
			ball_dy = -ball_dy;
		}
		if (ball_x >= paddle_x && ball_x <= paddle_dx + paddle_x && ball_y <= (paddle_y + paddle_dy + 15) && ball_y >= paddle_y)
		{
			ball_dy = -ball_dy;
		}
		else
		{
			if (ball_y < paddle_y){
				restart();
				lyf--;
			} 
		}
	}
}
void restart()
{
	paddle_x = x1 / 2 - 75, paddle_y = 1, paddle_dx = 150, paddle_dy = 7;
	ball_x = paddle_x + 70, ball_y = paddle_y + 16, ball_dx = 5, ball_dy = 7;
	start = false;
}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my)
{
	printf("x = %d, y= %d\n", mx, my);
	// place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// place your codes here
		//	printf("x = %d, y= %d\n",mx,my);
		if (mx >= 945 && mx < 1095 && my >= 255 && my < 305)		//start
		{
			s = 3;
		}
		if (mx >= 945 && mx < 1095 && my >= 190 && my < 240)		//resume
		{
			s=3;
			iResumeTimer(0);
		}
		if (mx >= 945 && mx < 1095 && my >= 125 && my < 175)		//option
		{
			s=4;
			t=2;
		}if(s==3){
		if(mx>=1170 && mx<=1195 && my>=645 && my<=670){
			iPauseTimer(0);
			s=2;
			t=3;
		}
		}
		if (mx >= 945 && mx < 1095 && my >= 60 && my < 110)			//exit
		{
			exit(0);
		}
		if(t==2){
		if (mx >= 2 && mx <= 27 && my >= 648 && my <= 673)		//option
		{
			s=2;
			t=1;
		}
		}

		if (ball_x > 1200 || ball_x < 0)
			ball_dx = -ball_dx;
		if (ball_y > 700 || ball_y < 0)
			ball_dy = -ball_dy;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		// place your codes here
	}
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key)
{
	if (s != 1)
	{
		if (key == 'a')
		{
			paddle_x -= 8;
			if (!start)
				ball_x -= 8;
			if (paddle_x < 0)
			{
				paddle_x = 0;
				if (!start)
					ball_x = 70;
			}
		}
		if (key == 'd')
		{
			paddle_x += 8;
			if (!start)
				ball_x += 8;
			if (paddle_x > 1060 || paddle_x > 1770)
			{
				paddle_x = 1060;
				if (!start)
					ball_x = paddle_x + 70;
			}
		}

		if (key == ' ')
		{
			if (!start)
				start = true;
		}
	}
	if (s == 1)
	{
		if (key == ' ')
			s = 2;
	}

	// place your codes for other keys here
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

void iSpecialKeyboard(unsigned char key)
{

	if (key == GLUT_KEY_LEFT)
	{
		paddle_x -= 15;
		if (!start)
			ball_x -= 15;
		if (paddle_x < 0)
		{
			paddle_x = 0;
			if (!start)
				ball_x = 70;
		}
	}
	if (key == GLUT_KEY_RIGHT)
	{
		paddle_x += 15;
		if (!start)
			ball_x += 15;
		if (paddle_x > 1060 || paddle_x > 1770)
		{
			paddle_x = 1060;
			if (!start)
				ball_x = paddle_x + 70;
		}
	}
	if (key == GLUT_KEY_END)
		exit(0);
	// place your codes for other keys here
}

int main()
{
	// place your own initialization codes here.
	iSetTimer(16, ballchange);
	iInitialize(x1, Y, "demo0000");
	return 0;
}
