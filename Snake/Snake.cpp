#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <ctime>

struct Tile {
	char tale = ' ';
	int lifeTime = 0;
};

const int HEIGHT = 20, WIDHT = 20;
int gameover, score = 0;
int headPositionX, headPositionY;
int fruitX, fruitY;
int direction, lengthBody = 0;
Tile area[WIDHT][HEIGHT];



void generateFruit() 
{
	srand((unsigned int)std::time(nullptr));
	fruitX = rand() % (WIDHT - 2) + 1;
	fruitY = rand() % (HEIGHT - 2) + 1;
	while (area[fruitX][fruitY].tale != ' ')
	{
		fruitX = rand() % (WIDHT - 2) + 1;
		fruitY = rand() % (HEIGHT - 2) + 1;
	}
	area[fruitX][fruitY].tale = '*';
}

void setup() 
{
	HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX consoleCurrentFontEx;

	consoleCurrentFontEx.cbSize = sizeof(CONSOLE_FONT_INFOEX);

	GetCurrentConsoleFontEx(hConsoleOutput, false, &consoleCurrentFontEx);
	wcscpy_s(consoleCurrentFontEx.FaceName, L"Terminal");
	consoleCurrentFontEx.dwFontSize = { 12 , 16 };
	printf("%d %d", consoleCurrentFontEx.dwFontSize.X, consoleCurrentFontEx.dwFontSize.Y);
	SetCurrentConsoleFontEx(hConsoleOutput, true, &consoleCurrentFontEx);
	gameover = 0;

	for (int i = 0; i < WIDHT; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			if (j == 0 || j == HEIGHT - 1 || i == 0 || i == WIDHT - 1)
			{
				area[i][j].tale = '#';
			}
		}
	}

	headPositionX = WIDHT / 2;
	headPositionY = HEIGHT / 2;
	area[headPositionX][headPositionY].tale = '@';

	generateFruit();
}

void draw() 
{
	system("cls");
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDHT; j++)
		{
			printf("%c", area[j][i].tale);
		}
		printf("\n");
	}
	printf("Score = %d", score);
}

void input() 
{

	if (_kbhit()) 
	{
		switch (_getch())
		{
		case 'w':
			if (direction != 3)
				direction = 1;
			break;
		case 'a':
			if (direction != 4)
				direction = 2;
			break;
		case 's':
			if (direction != 1)
				direction = 3;
			break;
		case 'd':
			if (direction != 2)
				direction = 4;
			break;
		}
	}
}

void logic()
{
	Sleep(50);

	area[headPositionX][headPositionY].tale = '0';
	area[headPositionX][headPositionY].lifeTime = lengthBody;
	
	switch (direction)
	{
	case 1:
		headPositionY--;
		break;
	case 2:
		headPositionX--;
		break;
	case 3:
		headPositionY++;
		break;
	case 4:
		headPositionX++;
		break;
	}

	if (headPositionX <= 0 || headPositionX >= WIDHT - 1 || headPositionY <= 0 ||
		headPositionY >= HEIGHT - 1 || area[headPositionX][headPositionY].tale == '0' && lengthBody != 0)
	{
		gameover++;
	}

	area[headPositionX][headPositionY].tale = '@';

	if (headPositionX == fruitX && headPositionY == fruitY)
	{
		score++;
		lengthBody++;

		generateFruit();
	}
	else
	{
		for (int i = 0; i < WIDHT; i++)
		{
			for (int j = 0; j < HEIGHT; j++)
			{

				if (area[i][j].lifeTime <= 0 && area[i][j].tale == '0')
				{
					area[i][j].tale = ' ';
				}
				if (area[i][j].lifeTime > 0 && area[i][j].tale == '0')
				{
					area[i][j].lifeTime--;
				}
			}
		}
	}

}

int main() 
{
	setup();

	while (!gameover)
	{
		draw();
		input();
		logic();
	}
}
