#include "iGraphics.h"
#include <windows.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

void showbrick();
void showbrick2();
void showbrick3();
void restart();
void reset();
void bricks();
void bricks2();
void bricks3();
void collcheck1();
void collcheck2();
void collcheck3();
void perk();
void perkreset();
void perksup();
void showscore();
void highscore();
void showhighscore();
void updatehighscore();
void savehighscore();

int balltimer = 0;
int x1 = 1200, Y = 675;
int paddle_x = x1 / 2 - 75, paddle_y = 1, paddle_dx = 150, paddle_dy = 7;	  // paddle
int ball_x = paddle_x + 70, ball_y = paddle_y + 16, ball_dx = 5, ball_dy = 7; // ball
int s = 1;
int t = 1;
int count = 0;
int count2 = 0;
int count3 = 0;
int lyf = 3;
int level = 1;
int difficult = 1;
int opt = 0;
int resume = 1;
int fastball = 0;
int slowball = 0;
int fastcount = 0;
int slowcount = 0;
int gothrough = 0;
int gocount = 0;
int score = 0;
const int leaderboards = 5;
int flag=0;

bool start = false;
bool musicOn = true;

char paddle[3][2000] = {"bc\\007.bmp", "bc\\smallpaddle.bmp", "bc\\bigpaddle.bmp"};
char starting[2][100] = {"bc\\home2.bmp", "dxball\\2.bmp"};																			  // loading page
char background[20] = {"bg.bmp"};																									  // background
char home[6][2000] = {"bc\\home1.bmp", "menu\\Play.bmp", "menu\\New.bmp", "menu\\Resume.bmp", "menu\\Options.bmp", "menu\\Exit.bmp"}; // menupage
char dx[3][2000] = {"dxball\\4.bmp", "dxball\\5.bmp", "dxball\\6.bmp"};																  // 4==back, 5==pause, 10==options
char life[3][200] = {"dxball\\x.bmp", "dxball\\4x.bmp", "dxball\\3x.bmp"};
char difficulty[3][2000] = {"dxball\\easy.bmp", "dxball\\medium.bmp", "dxball\\hard.bmp"};
char cont[2][2000] = {"dxball\\continue.bmp", "dxball\\gameover.bmp"};
char go[200] = {"dxball\\pressR.bmp"};
char power[6][200] = {"perks\\smallpaddle.bmp", "perks\\bigpaddle.bmp", "perks\\fastball.bmp", "perks\\slowball.bmp", "perks\\addlife.bmp", "perks\\throughbrick.bmp"};

struct LeaderboardEntry
{
	char name[50];
	int score;
};

LeaderboardEntry leaderboard[leaderboards];
int leaderboardSize = 0;
char playerName[50] = "";

struct brick
{
	int x;
	int y;
	int dx = 99;
	int dy = 19;
	int hp = 1;
	int flag = 0;
	bool show = true;
};
struct brick b[60];
struct brick b2[80];
struct brick b3[100];

struct perks
{
	int x;
	int y;
	int type;
	bool show = false;
	bool active = false;
	bool used = false;
	bool move = false;
};
struct perks p[6];
/*
	function iDraw() is called again and again by the system.

	*/
void highscore()
{
	FILE *file = fopen("leaderboard.txt", "r");
	if (file)
	{
		leaderboardSize = 0;
		while (fscanf(file, "%s %d", leaderboard[leaderboardSize].name, &leaderboard[leaderboardSize].score) == 2)
		{
			++leaderboardSize;
			if (leaderboardSize >= leaderboards)
			{
				break;
			}
		}
		fclose(file);
	}

	// Sort the leaderboard
	for (int i = 0; i < leaderboardSize - 1; ++i)
	{
		for (int j = i + 1; j < leaderboardSize; ++j)
		{
			if (leaderboard[i].score < leaderboard[j].score)
			{
				LeaderboardEntry temp = leaderboard[i];
				leaderboard[i] = leaderboard[j];
				leaderboard[j] = temp;
			}
		}
	}
}

void showhighscore()
{
	iSetColor(255, 255, 255);
	iText(500, 600, "Leaderboard", GLUT_BITMAP_HELVETICA_18);
	for (int i = 0; i < leaderboardSize; ++i)
	{
		char entry[50];
		sprintf(entry, "%d. %s - %d", i + 1, leaderboard[i].name, leaderboard[i].score);
		iText(500, 570 - i * 20, entry, GLUT_BITMAP_HELVETICA_18);
	}
}
void updateLeaderboard(const char *playerName, int playerScore)
{
	// Check if the score is among the top five
	if (leaderboardSize < leaderboards || playerScore > leaderboard[leaderboardSize - 1].score)
	{
		if (leaderboardSize < leaderboards)
		{
			++leaderboardSize;
		}

		// Insert the new score in the correct position
		int pos = leaderboardSize - 1;
		for (int i = 0; i < leaderboardSize - 1; ++i)
		{
			if (playerScore > leaderboard[i].score)
			{
				pos = i;
				break;
			}
		}

		for (int i = leaderboardSize - 1; i > pos; --i)
		{
			leaderboard[i] = leaderboard[i - 1];
		}

		strncpy(leaderboard[pos].name, playerName, sizeof(leaderboard[pos].name) - 1);
		leaderboard[pos].name[sizeof(leaderboard[pos].name) - 1] = '\0';
		leaderboard[pos].score = playerScore;
	}
}
void saveLeaderboard()
{
	FILE *file = fopen("leaderboard.txt", "w");
	if (file)
	{
		for (int i = 0; i < leaderboardSize; ++i)
		{
			fprintf(file, "%s %d\n", leaderboard[i].name, leaderboard[i].score);
		}
		fclose(file);
	}
}

void perkreset()
{
	for (int i = 0; i < 6; i++)
	{
		p[i].active = false;
		p[i].used = false;
		p[i].move = false;
		p[i].show = false;
		fastball = 0;
		slowball = 0;
		fastcount = 0;
		slowcount = 0;
		gothrough = 0;
		gocount = 0;
	}
}

void showscore()
{
	char scoreStr[20];
	sprintf(scoreStr, "Score: %d", score);
	iSetColor(255, 255, 255);
	iText(550, 650, scoreStr, GLUT_BITMAP_HELVETICA_18);
}

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
	else if (s == 4 && t == 2)
	{
		iClear();
		iShowBMP(0, 0, dx[2]);
		iShowBMP(2, 648, dx[0]);
		if (difficult == 1)
		{
			iShowBMP(500, 189, difficulty[0]);
		}
		else if (difficult == 2)
		{
			iShowBMP(500, 189, difficulty[1]);
		}
		else if (difficult == 3)
		{
			iShowBMP(500, 189, difficulty[2]);
		}
	}

	else if (s == 3)
	{
		iClear();
		iShowBMP(0, 0, background);
		iSetColor(237, 145, 72);
		iFilledCircle(ball_x, ball_y, 7);
		if (paddle_dx == 120)
		{
			iShowBMP2(paddle_x, paddle_y, paddle[1], 0);
		}
		else if (paddle_dx == 150)
		{
			iShowBMP2(paddle_x, paddle_y, paddle[0], 0);
		}
		else if (paddle_dx == 180)
		{
			iShowBMP2(paddle_x, paddle_y, paddle[2], 0);
		}
		if (resume == 2)
		{
			iShowBMP(300, 110, go);
		}

		if (level == 1)
		{
			showbrick();
		}
		else if (level == 2)
		{
			showbrick2();
		}
		else if (level == 3)
		{
			showbrick3();
		}
		iShowBMP(2, 648, life[lyf - 1]);
		showscore();
	}
	else if (s == 5)
	{
		iShowBMP(0, 0, cont[0]);
	}
	else if (s == 6)
	{
		iShowBMP(0, 0, cont[1]);
		iSetColor(0, 0, 0);
		iFilledRectangle(530, 200, 195, 24);
		iSetColor(255, 255, 255);
		iText(532, 205, "Press F to Pay Respect", GLUT_BITMAP_HELVETICA_18);
	}
	else if (s == 7)
	{
		iClear();
		iSetColor(255, 255, 255);
		iText(500, 400, "Enter your name:", GLUT_BITMAP_HELVETICA_18);
		iText(500, 370, playerName, GLUT_BITMAP_HELVETICA_18);
	}
	else if (s == 8 && t == 10)
	{
		iClear();
		iShowBMP(2,620,dx[0]);
		showhighscore();
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
				score = count * 10;
				if (fastball == 1)
				{
					fastcount++;
					if (fastcount == 8)
					{
						fastball = 0;
						fastcount = 0;
					}
				}
				else if (slowball == 1)
				{
					slowcount++;
					if (slowcount == 8)
					{
						slowball = 0;
						slowcount = 0;
					}
				}
				else if (gothrough == 1)
				{
					gocount++;
					if (gocount == 9)
					{
						gothrough = 0;
						gocount = 0;
					}
				}
				if (count == 3)
				{
					s = 5;
				}
				printf("%d\n", count);

				bool topCollision = ball_y - 7 < b[i].y && ball_y + 7 > b[i].y;
				bool bottomCollision = ball_y + 7 > b[i].y + b[i].dy && ball_y - 7 < b[i].y + b[i].dy;
				bool leftCollision = ball_x - 7 < b[i].x && ball_x + 7 > b[i].x;
				bool rightCollision = ball_x + 7 > b[i].x + b[i].dx && ball_x - 7 < b[i].x + b[i].dx;

				if (topCollision && (ball_dy > 0))
				{
					if (gothrough == 1)
					{
					}
					else
						ball_dy = -ball_dy;
				}
				else if (bottomCollision && (ball_dy < 0))
				{
					if (gothrough == 1)
					{
					}
					else
						ball_dy = -ball_dy;
				}
				else if (leftCollision && (ball_dx > 0))
				{
					if (gothrough == 1)
					{
					}
					else
						ball_dx = -ball_dx;
				}
				else if (rightCollision && (ball_dx < 0))
				{
					if (gothrough == 1)
					{
					}
					else
						ball_dx = -ball_dx;
				}
			}
		}
		if (i == 9 && b[i].show == false && p[0].used == false)
		{
			p[0].active = true;
			p[0].type = 1;
			if (!p[0].move)
			{
				p[0].x = b[i].x + b[i].dx / 2 - 15;
				p[0].y = b[i].y - 30;
			}
			else
			{
				p[0].y -= 3;
				if (p[0].y <= 7 && p[0].y >= 0 && p[0].x + 30 >= paddle_x && p[0].x <= paddle_x + paddle_dx)
				{
					p[0].used = true;
					paddle_dx -= 30;
					p[0].y = -30;
					printf("%d\n", paddle_x);
				}
			}
		}
		if (i == 20 && b[i].show == false && p[1].used == false)
		{
			p[1].active = true;
			p[1].type = 2;
			if (!p[1].move)
			{
				p[1].x = b[i].x + b[i].dx / 2 - 15;
				p[1].y = b[i].y - 30;
			}
			else
			{
				p[1].y -= 3;
				if (p[1].y <= 7 && p[1].y >= 0 && p[1].x + 30 >= paddle_x && p[1].x <= paddle_x + paddle_dx)
				{
					p[1].used = true;
					paddle_dx += 30;
					if (paddle_dx > 180)
					{
						paddle_dx = 180;
					}
					p[1].y = -30;
					printf("%d\n", paddle_x);
				}
			}
		}
		if (i == 26 && b[i].show == false && p[2].used == false)
		{
			p[2].active = true;
			p[2].type = 3;
			if (!p[2].move)
			{
				p[2].x = b[i].x + b[i].dx / 2 - 15;
				p[2].y = b[i].y - 30;
			}
			else
			{
				p[2].y -= 3;
				if (p[2].y <= 7 && p[2].y >= 0 && p[2].x + 30 >= paddle_x && p[2].x <= paddle_x + paddle_dx)
				{
					p[2].used = true;
					fastball = 1;
					p[2].y = -30;
					printf("%d\n", paddle_x);
				}
			}
		}
		if (i == 37 && b[i].show == false && p[3].used == false)
		{
			p[3].active = true;
			p[3].type = 4;
			if (!p[3].move)
			{
				p[3].x = b[i].x + b[i].dx / 2 - 15;
				p[3].y = b[i].y - 30;
			}
			else
			{
				p[3].y -= 3;
				if (p[3].y <= 7 && p[3].y >= 0 && p[3].x + 30 >= paddle_x && p[3].x <= paddle_x + paddle_dx)
				{
					p[3].used = true;
					slowball = 1;
					p[3].y = -30;
					printf("%d\n", paddle_x);
				}
			}
		}
		if (i == 44 && b[i].show == false && p[4].used == false)
		{
			p[4].active = true;
			p[4].type = 5;
			if (!p[4].move)
			{
				p[4].x = b[i].x + b[i].dx / 2 - 15;
				p[4].y = b[i].y - 30;
			}
			else
			{
				p[4].y -= 5;
				if (p[4].y <= 7 && p[4].y >= 0 && p[4].x + 30 >= paddle_x && p[4].x <= paddle_x + paddle_dx)
				{
					p[4].used = true;
					if (lyf < 3)
					{
						lyf++;
					}
					p[4].y = -30;
					printf("%d\n", paddle_x);
				}
			}
		}
		if (i == 53 && b[i].show == false && p[5].used == false)
		{
			p[5].active = true;
			p[5].type = 6;
			if (!p[5].move)
			{
				p[5].x = b[i].x + b[i].dx / 2 - 15;
				p[5].y = b[i].y - 30;
			}
			else
			{
				p[5].y -= 3;
				if (p[5].y <= 7 && p[5].y >= 0 && p[5].x + 30 >= paddle_x && p[5].x <= paddle_x + paddle_dx)
				{
					p[5].used = true;
					if (gocount < 10)
					{
						gothrough = 1;
					}
					p[5].y = -30;
					printf("%d\n", paddle_x);
				}
			}
		}
	}
}

void collcheck2()
{
	for (int i = 0; i < 80; i++)
	{
		if (b2[i].show)
		{
			bool withinX = ball_x + 7 >= b2[i].x && ball_x - 7 <= b2[i].x + b2[i].dx;
			bool withinY = ball_y + 7 >= b2[i].y && ball_y - 6 <= b2[i].y + b2[i].dy;

			if (withinX && withinY)
			{
				if (b2[i].hp == 2)
				{
					b2[i].hp = 1;
					b2[i].flag = 1;
				}
				else if (b2[i].hp == 1)
				{
					b2[i].show = false;
					count2++;
					score = count2 * 12 + 600;
					if (fastball == 1)
					{
						fastcount++;
						if (fastcount == 8)
						{
							fastball = 0;
							fastcount = 0;
						}
					}
					else if (slowball == 1)
					{
						slowcount++;
						if (slowcount == 8)
						{
							slowball = 0;
							slowcount = 0;
						}
					}
					else if (gothrough == 1)
					{
						gocount++;
						if (gocount == 9)
						{
							gothrough = 0;
							gocount = 0;
						}
					}
					if (count2 == 100)
					{
						s = 5;
					}
				}

				bool topCollision = ball_y - 7 < b2[i].y && ball_y + 7 > b2[i].y;
				bool bottomCollision = ball_y + 7 > b2[i].y + b2[i].dy && ball_y - 7 < b2[i].y + b2[i].dy;
				bool leftCollision = ball_x - 7 < b2[i].x && ball_x + 7 > b2[i].x;
				bool rightCollision = ball_x + 7 > b2[i].x + b2[i].dx && ball_x - 7 < b2[i].x + b2[i].dx;

				if (topCollision && (ball_dy > 0))
				{
					if (gothrough == 1)
					{
					}
					else
						ball_dy = -ball_dy;
				}
				else if (bottomCollision && (ball_dy < 0))
				{
					if (gothrough == 1)
					{
					}
					else
						ball_dy = -ball_dy;
				}
				else if (leftCollision && (ball_dx > 0))
				{
					if (gothrough == 1)
					{
					}
					else
						ball_dx = -ball_dx;
				}
				else if (rightCollision && (ball_dx < 0))
				{
					if (gothrough == 1)
					{
					}
					else
						ball_dx = -ball_dx;
				}
			}
		}
		if (i == 19 && b2[i].show == false && p[0].used == false)
		{
			p[0].active = true;
			p[0].type = 1;
			if (!p[0].move)
			{
				p[0].x = b2[i].x + b2[i].dx / 2 - 15;
				p[0].y = b2[i].y - 30;
			}
			else
			{
				p[0].y -= 3;
				if (p[0].y <= 7 && p[0].y >= 0 && p[0].x + 30 >= paddle_x && p[0].x <= paddle_x + paddle_dx)
				{
					p[0].used = true;
					paddle_dx -= 30;
					p[0].y = -30;
					printf("%d\n", paddle_x);
				}
			}
		}
		if (i == 30 && b2[i].show == false && p[1].used == false)
		{
			p[1].active = true;
			p[1].type = 2;
			if (!p[1].move)
			{
				p[1].x = b2[i].x + b2[i].dx / 2 - 15;
				p[1].y = b2[i].y - 30;
			}
			else
			{
				p[1].y -= 3;
				if (p[1].y <= 7 && p[1].y >= 0 && p[1].x + 30 >= paddle_x && p[1].x <= paddle_x + paddle_dx)
				{
					p[1].used = true;
					paddle_dx += 30;
					if (paddle_dx > 180)
					{
						paddle_dx = 180;
					}
					p[1].y = -30;
					printf("%d\n", paddle_x);
				}
			}
		}
		if (i == 36 && b2[i].show == false && p[2].used == false)
		{
			p[2].active = true;
			p[2].type = 3;
			if (!p[2].move)
			{
				p[2].x = b2[i].x + b2[i].dx / 2 - 15;
				p[2].y = b2[i].y - 30;
			}
			else
			{
				p[2].y -= 3;
				if (p[2].y <= 7 && p[2].y >= 0 && p[2].x + 30 >= paddle_x && p[2].x <= paddle_x + paddle_dx)
				{
					p[2].used = true;
					fastball = 1;
					p[2].y = -30;
					printf("%d\n", paddle_x);
				}
			}
		}
		if (i == 47 && b2[i].show == false && p[3].used == false)
		{
			p[3].active = true;
			p[3].type = 4;
			if (!p[3].move)
			{
				p[3].x = b2[i].x + b2[i].dx / 2 - 15;
				p[3].y = b2[i].y - 30;
			}
			else
			{
				p[3].y -= 3;
				if (p[3].y <= 7 && p[3].y >= 0 && p[3].x + 30 >= paddle_x && p[3].x <= paddle_x + paddle_dx)
				{
					p[3].used = true;
					slowball = 1;
					p[3].y = -30;
					printf("%d\n", paddle_x);
				}
			}
		}
		if (i == 58 && b2[i].show == false && p[4].used == false)
		{
			p[4].active = true;
			p[4].type = 3;
			if (!p[4].move)
			{
				p[4].x = b2[i].x + b2[i].dx / 2 - 15;
				p[4].y = b2[i].y - 30;
			}
			else
			{
				p[4].y -= 5;
				if (p[4].y <= 7 && p[4].y >= 0 && p[4].x + 30 >= paddle_x && p[4].x <= paddle_x + paddle_dx)
				{
					p[4].used = true;
					if (lyf < 3)
					{
						lyf++;
					}
					p[4].y = -30;
					printf("%d\n", paddle_x);
				}
			}
		}
		if (i == 67 && b2[i].show == false && p[5].used == false)
		{
			p[5].active = true;
			p[5].type = 6;
			if (!p[5].move)
			{
				p[5].x = b2[i].x + b2[i].dx / 2 - 15;
				p[5].y = b2[i].y - 30;
			}
			else
			{
				p[5].y -= 3;
				if (p[5].y <= 7 && p[5].y >= 0 && p[5].x + 30 >= paddle_x && p[5].x <= paddle_x + paddle_dx)
				{
					p[5].used = true;
					if (gocount < 10)
					{
						gothrough = 1;
					}
					p[5].y = -30;
					printf("%d\n", paddle_x);
				}
			}
		}
	}
}

void collcheck3()
{
	count = 0;
	for (int i = 0; i < 100; i++)
	{
		if (b3[i].show)
		{
			bool withinX = ball_x + 7 >= b3[i].x && ball_x - 7 <= b3[i].x + b3[i].dx;
			bool withinY = ball_y + 7 >= b3[i].y && ball_y - 6 <= b3[i].y + b3[i].dy;

			if (withinX && withinY)
			{
				if (b3[i].hp == 3)
				{
					b3[i].hp = 2;
					b3[i].flag = 2;
				}
				else if (b3[i].hp == 2)
				{
					b3[i].hp = 1;
					b3[i].flag = 1;
				}
				else if (b3[i].hp == 1)
				{
					b3[i].show = false;
					count3++;
					score = count3 * 15 + 1800;
					if (fastball == 1)
					{
						fastcount++;
						if (fastcount == 8)
						{
							fastball = 0;
							fastcount = 0;
						}
					}
					else if (slowball == 1)
					{
						slowcount++;
						if (slowcount == 8)
						{
							slowball = 0;
							slowcount = 0;
						}
					}
					else if (gothrough == 1)
					{
						gocount++;
						if (gocount == 9)
						{
							gothrough = 0;
							gocount = 0;
						}
					}
					if (count3 == 276)
					{
						s = 5;
					}
				}

				bool topCollision = ball_y - 7 < b3[i].y && ball_y + 7 > b3[i].y;
				bool bottomCollision = ball_y + 7 > b3[i].y + b3[i].dy && ball_y - 7 < b3[i].y + b3[i].dy;
				bool leftCollision = ball_x - 7 < b3[i].x && ball_x + 7 > b3[i].x;
				bool rightCollision = ball_x + 7 > b3[i].x + b3[i].dx && ball_x - 7 < b3[i].x + b3[i].dx;

				if (topCollision && (ball_dy > 0))
				{
					if (gothrough == 1)
					{
					}
					else
						ball_dy = -ball_dy;
				}
				else if (bottomCollision && (ball_dy < 0))
				{
					if (gothrough == 1)
					{
					}
					else
						ball_dy = -ball_dy;
				}
				else if (leftCollision && (ball_dx > 0))
				{
					if (gothrough == 1)
					{
					}
					else
						ball_dx = -ball_dx;
				}
				else if (rightCollision && (ball_dx < 0))
				{
					if (gothrough == 1)
					{
					}
					else
						ball_dx = -ball_dx;
				}
			}
			if (i == 15 && b2[i].show == false && p[0].used == false)
			{
				p[0].active = true;
				p[0].type = 1;
				if (!p[0].move)
				{
					p[0].x = b2[i].x + b2[i].dx / 2 - 15;
					p[0].y = b2[i].y - 30;
				}
				else
				{
					p[0].y -= 3;
					if (p[0].y <= 7 && p[0].y >= 0 && p[0].x + 30 >= paddle_x && p[0].x <= paddle_x + paddle_dx)
					{
						p[0].used = true;
						paddle_dx -= 30;
						p[0].y = -30;
						printf("%d\n", paddle_x);
					}
				}
			}
			if (i == 33 && b2[i].show == false && p[1].used == false)
			{
				p[1].active = true;
				p[1].type = 2;
				if (!p[1].move)
				{
					p[1].x = b2[i].x + b2[i].dx / 2 - 15;
					p[1].y = b2[i].y - 30;
				}
				else
				{
					p[1].y -= 3;
					if (p[1].y <= 7 && p[1].y >= 0 && p[1].x + 30 >= paddle_x && p[1].x <= paddle_x + paddle_dx)
					{
						p[1].used = true;
						paddle_dx += 30;
						if (paddle_dx > 180)
						{
							paddle_dx = 180;
						}
						p[1].y = -30;
						printf("%d\n", paddle_x);
					}
				}
			}
			if (i == 45 && b2[i].show == false && p[2].used == false)
			{
				p[2].active = true;
				p[2].type = 3;
				if (!p[2].move)
				{
					p[2].x = b2[i].x + b2[i].dx / 2 - 15;
					p[2].y = b2[i].y - 30;
				}
				else
				{
					p[2].y -= 3;
					if (p[2].y <= 7 && p[2].y >= 0 && p[2].x + 30 >= paddle_x && p[2].x <= paddle_x + paddle_dx)
					{
						p[2].used = true;
						fastball = 1;
						p[2].y = -30;
						printf("%d\n", paddle_x);
					}
				}
			}
			if (i == 54 && b2[i].show == false && p[3].used == false)
			{
				p[3].active = true;
				p[3].type = 4;
				if (!p[3].move)
				{
					p[3].x = b2[i].x + b2[i].dx / 2 - 15;
					p[3].y = b2[i].y - 30;
				}
				else
				{
					p[3].y -= 3;
					if (p[3].y <= 7 && p[3].y >= 0 && p[3].x + 30 >= paddle_x && p[3].x <= paddle_x + paddle_dx)
					{
						p[3].used = true;
						slowball = 1;
						p[3].y = -30;
						printf("%d\n", paddle_x);
					}
				}
			}
			if (i == 69 && b2[i].show == false && p[4].used == false)
			{
				p[4].active = true;
				p[4].type = 3;
				if (!p[4].move)
				{
					p[4].x = b2[i].x + b2[i].dx / 2 - 15;
					p[4].y = b2[i].y - 30;
				}
				else
				{
					p[4].y -= 5;
					if (p[4].y <= 7 && p[4].y >= 0 && p[4].x + 30 >= paddle_x && p[4].x <= paddle_x + paddle_dx)
					{
						p[4].used = true;
						if (lyf < 3)
						{
							lyf++;
						}
						p[4].y = -30;
						printf("%d\n", paddle_x);
					}
				}
			}
			if (i == 82 && b2[i].show == false && p[5].used == false)
			{
				p[5].active = true;
				p[5].type = 6;
				if (!p[5].move)
				{
					p[5].x = b2[i].x + b2[i].dx / 2 - 15;
					p[5].y = b2[i].y - 30;
				}
				else
				{
					p[5].y -= 3;
					if (p[5].y <= 7 && p[5].y >= 0 && p[5].x + 30 >= paddle_x && p[5].x <= paddle_x + paddle_dx)
					{
						p[5].used = true;
						if (gocount < 10)
						{
							gothrough = 1;
						}
						p[5].y = -30;
						printf("%d\n", paddle_x);
					}
				}
			}
		}
	}
}
void perksup()
{
	for (int i = 0; i < 6; i++)
	{
		if (p[i].active)
		{
			if (p[i].type == 1)
			{
				iShowBMP(p[i].x, p[i].y, power[0]);
				p[i].move = true; // smallpaddle
			}
			else if (p[i].type == 2)
			{
				iShowBMP(p[i].x, p[i].y, power[1]); // bigpaddle
				p[i].move = true;
			}
			else if (p[i].type == 3)
			{
				iShowBMP(p[i].x, p[i].y, power[2]); // fastball
				p[i].move = true;
			}
			else if (p[i].type == 4)
			{
				iShowBMP(p[i].x, p[i].y, power[3]); // slowball
				p[i].move = true;
			}
			else if (p[i].type == 5)
			{
				iShowBMP(p[i].x, p[i].y, power[4]); // addlife
				p[i].move = true;
			}
			else if (p[i].type == 6)
			{
				iShowBMP(p[i].x, p[i].y, power[5]); // throughbrick
				p[i].move = true;
			}
		}
	}
}
void showbrick()
{
	perksup();
	bricks();
	iShowBMP(1170, 645, dx[1]);
	for (int i = 0; i < 60; i++)
	{
		if (b[i].show)
		{
			if (i % 2 == 0)
			{
				iSetColor(253, 157, 18);
				iFilledRectangle(b[i].x, b[i].y, b[i].dx, b[i].dy);
			}
			else
			{
				iSetColor(197, 73, 161);
				iFilledRectangle(b[i].x, b[i].y, b[i].dx, b[i].dy);
			}
		}
	}
}
void showbrick2()
{
	perksup();
	bricks2();
	iShowBMP(1170, 645, dx[1]);
	for (int i = 0; i < 80; i++)
	{
		if (b2[i].show)
		{
			if (i % 2 == 0)
			{
				iSetColor(189, 252, 201);
				iFilledRectangle(b2[i].x, b2[i].y, b2[i].dx, b2[i].dy);
			}
			else
			{
				iSetColor(255, 215, 0);
				iFilledRectangle(b2[i].x, b2[i].y, b2[i].dx, b2[i].dy);
			}
			if (i >= 0 && i < 10 || i >= 70 && i < 80)
			{
				if (b2[i].flag == 1 && b2[i].hp == 1)
				{
					iSetColor(0, 0, 130);
					iFilledRectangle(b2[i].x, b2[i].y, b2[i].dx, b2[i].dy);
				}
				else if (b2[i].flag == 0)
				{
					b2[i].hp = 2;
					iSetColor(100, 150, 100);
					iFilledRectangle(b2[i].x, b2[i].y, b2[i].dx, b2[i].dy);
				}
			}
		}
	}
}

void showbrick3()
{
	perksup();
	bricks3();
	iShowBMP(1170, 645, dx[1]);
	for (int i = 0; i < 100; i++)
	{
		if (b3[i].show)
		{
			if (i % 2 == 0)
			{
				iSetColor(189, 252, 201);
				iFilledRectangle(b3[i].x, b3[i].y, b3[i].dx, b3[i].dy);
			}
			else
			{
				iSetColor(255, 215, 0);
				iFilledRectangle(b3[i].x, b3[i].y, b3[i].dx, b3[i].dy);
			}
			if (i >= 0 && i < 10 || i >= 90 && i < 100 || i % 10 == 0 || i % 10 == 9)
			{
				if (b3[i].flag == 2 && b3[i].hp == 2)
				{
					iSetColor(100, 150, 100);
					iFilledRectangle(b3[i].x, b3[i].y, b3[i].dx, b3[i].dy);
				}
				else if (b3[i].flag == 1 && b3[i].hp == 1)
				{
					iSetColor(0, 0, 130);
					iFilledRectangle(b3[i].x, b3[i].y, b3[i].dx, b3[i].dy);
				}
				else if (b3[i].flag == 0)
				{
					b3[i].hp = 3;
					iSetColor(75, 0, 130);
					iFilledRectangle(b3[i].x, b3[i].y, b3[i].dx, b3[i].dy);
				}
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
void bricks3()
{
	int startx = 100;
	int starty = 350;
	for (int i = 0; i < 100; i++)
	{
		b3[i].x = startx;
		b3[i].y = starty;
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
		if (difficult == 1)
		{
			ball_x += ball_dx;
			ball_y += ball_dy;
		}
		else if (difficult == 2)
		{
			ball_x += (int)(1.2 * ball_dx);
			ball_y += (int)(1.2 * ball_dy);
		}
		else if (difficult == 3)
		{
			ball_x += (int)(1.4 * ball_dx);
			ball_y += (int)(1.4 * ball_dy);
		}
		if (fastball == 1)
		{
			ball_x += (int)(1.52 * ball_dx);
			ball_y += (int)(1.52 * ball_dy);
		}
		else if (slowball == 1)
		{
			ball_x += (int)(0.75 * ball_dx);
			ball_y += (int)(0.75 * ball_dy);
		}
		if (level == 1)
		{
			collcheck1();
		}
		else if (level == 2)
		{
			collcheck2();
		}
		else if (level == 3)
		{
			collcheck3();
		}
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
			if (ball_y < paddle_y)
			{
				restart();
				lyf--;
				if (lyf == 0)
				{
					s = 6;
					restart();
				}
			}
		}
	}
}

void reset()
{
	if (level == 1)
	{
		for (int i = 0; i < 60; i++)
		{
			b[i].show = true;
		}
	}
	else if (level == 2)
	{
		for (int i = 0; i < 80; i++)
		{
			b2[i].show = true;
		}
	}
	else if (level == 3)
	{
		for (int i = 0; i < 100; i++)
		{
			b3[i].show = true;
		}
	}
	lyf = 3;
	count = 0;
	count2 = 0;
	count3 = 0;
	level = 1;
	restart();
	perkreset();
	score=0;
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
		if (s == 5)
		{
			if (mx >= 344 && mx <= 585 && my >= 55 && my <= 120)
			{
				s = 3;
				if (level == 1)
				{
					level = 2;
					lyf = 3;
					restart();
					count = 0;
					perkreset();
				}
				else if (level == 2)
				{
					level = 3;
					lyf = 3;
					count2 = 0;
					restart();
					perkreset();
				}
			}
			if(mx>=608 && mx<=845  && my>=52 && my<=120){
				s=7;
			}
		}
		if (s == 2)
		{ 
			if (mx >= 945 && mx < 1095 && my >= 255 && my < 305) // start
			{
				s = 3;
				flag=1;
				if (t == 3)
				{
					reset();
					iResumeTimer(0);
					resume=1;
				}
			}
			if (mx >= 945 && mx < 1095 && my >= 190 && my < 240 ) // resume
			{
				if (t == 1)
				{
				}
				else if (t == 3)
				{
					s = 3;
					resume = 2;
				}
			}
			if (mx >= 945 && mx < 1095 && my >= 125 && my < 175) // option
			{
				s = 4;
				t = 2;
			}
			if (mx >= 945 && mx < 1095 && my >= 60 && my < 110) // exit
			{
				exit(0);
			}
		}
		if(s==8){
		if (mx >= 2 && mx <= 27 && my >= 620 && my <= 645) // option
			{
				s = 4;
				t = 2;
			}
		}
		if (s == 3)
		{
			if (mx >= 1170 && mx <= 1195 && my >= 645 && my <= 670)
			{
				iPauseTimer(0);
				s = 2;
				t = 3;
			}
		}
		if (t == 2 && s == 4)
		{
			if (mx >= 116 && mx <= 940 && my >= 35 && my <= 185)
			{
				s = 8;
				t = 10;
			}
			else if (mx >= 2 && mx <= 27 && my >= 648 && my <= 673) // option
			{
				s = 2;
				t = 3;
			}
			else if (mx >= 80 && mx <= 320 && my >= 545 && my <= 605)
			{
				difficult = 2;
			}
			else if (mx >= 880 && mx <= 1120 && my >= 545 && my <= 605)
			{
				difficult = 1;
			}
			else if (mx >= 480 && mx <= 720 && my >= 545 && my <= 605)
			{
				difficult = 3;
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
	if (s == 6 && key == 'f')
	{
		s = 7;
	}
	else if (s == 7)
	{
		if (key == '\r')
		{
			updateLeaderboard(playerName, score);
			saveLeaderboard();
			reset();
			s = 2;
		}
		else if (key == '\b')
		{
			int len = strlen(playerName);
			if (len > 0)
			{
				playerName[len - 1] = '\0';
			}
		}
		else
		{
			int len = strlen(playerName);
			if (len < sizeof(playerName) - 1)
			{
				playerName[len] = key;
				playerName[len + 1] = '\0';
			}
		}
	}
	if (key == 'r')
	{
		iResumeTimer(0);
		resume = 1;
	}
	if (s == 3)
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
	if (t == 2)
	{
		if (key == 'c')
		{
			opt = 1;
		}
	}
	if (key == 'c')
	{
		s = 2;
		t = 3;
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
	highscore();
	// place your own initialization codes here.
	iSetTimer(16, ballchange);
	iInitialize(x1, Y, "demo0000");
	return 0;
}
