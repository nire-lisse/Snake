#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <ctime>

struct Tile {
	TCHAR tile = ' ';
	int lifeTime = 0;
};

const int HEIGHT = 50, WIDHT = 50;
int gameover, score = 0;
int headPositionX, headPositionY;
int fruitX, fruitY;
int direction, lengthBody = 0;
Tile area[WIDHT][HEIGHT];
HANDLE hConsoleOutput;

void generateFruit() 
{
	srand((unsigned int)std::time(nullptr));
	fruitX = rand() % (WIDHT - 2) + 1;
	fruitY = rand() % (HEIGHT - 2) + 1;
	while (area[fruitX][fruitY].tile != ' ')
	{
		fruitX = rand() % (WIDHT - 2) + 1;
		fruitY = rand() % (HEIGHT - 2) + 1;
	}
	area[fruitX][fruitY].tile = '*';
}

void setup() 
{
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
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
				area[i][j].tile = '#';
			}
		}
	}

	headPositionX = WIDHT / 2;
	headPositionY = HEIGHT / 2;
	area[headPositionX][headPositionY].tile = '@';

	generateFruit();
}

void draw() 
{
	wchar_t str[16];
	COORD coord;
	DWORD d;


	for (short i = 0; i < HEIGHT; i++)
	{
		for (short j = 0; j < WIDHT; j++)
		{
			coord = { j , i };
			WriteConsoleOutputCharacter(hConsoleOutput, &area[j][i].tile, 1, coord, &d);
		}
	}


	size_t lenStr = swprintf(str, 16, L"Score = %d", score);

	coord = { 0 , HEIGHT + 1 };
	WriteConsoleOutputCharacter(hConsoleOutput, str, lenStr, coord, &d);

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
	Sleep(70);

	area[headPositionX][headPositionY].tile = '0';
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
		headPositionY >= HEIGHT - 1 || area[headPositionX][headPositionY].tile == '0' && lengthBody != 0)
	{
		gameover++;
	}

	area[headPositionX][headPositionY].tile = '@';

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

				if (area[i][j].lifeTime <= 0 && area[i][j].tile == '0')
				{
					area[i][j].tile = ' ';
				}
				if (area[i][j].lifeTime > 0 && area[i][j].tile == '0')
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
