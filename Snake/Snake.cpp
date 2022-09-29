#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <ctime>
#include <string>
#include <queue>
#include <stack>

struct Tile
{
	char tile;
	int lifeTime = 0;
};

struct Cell
{
	int x;
	int y;
};

const int HEIGHT = 30, WIDTH = 30;
int gameover, score;
int headPositionX, headPositionY;
int fruitX, fruitY;
int direction, lengthBody;
int restart = 0, bestScore;
Tile area[WIDTH][HEIGHT];
HANDLE hConsoleOutput;
std::stack<Cell> path;


void bfs()
{
	bool visited[WIDTH][HEIGHT];
	std::queue<Cell> queue;
	Cell s;
	Cell prev[WIDTH][HEIGHT];

	wchar_t str[32];
	DWORD d;

	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			visited[i][j] = false;
		}
	}

	visited[headPositionX][headPositionY] = true;
	queue.push({ headPositionX, headPositionY });

	while (!queue.empty())
	{
		s = queue.front();
		queue.pop();

		int	dX[] = { 1 , 0 , -1 , 0 };
		int	dY[] = { 0 , 1 , 0 , -1 };

		if (s.x == fruitX && s.y == fruitY)
		{
			break;
		}

		for (int i = 0; i < 4; i++)
		{
			int x = s.x + dX[i];
			int y = s.y + dY[i];

			if (!visited[x][y] && (area[x][y].tile == ' ' || area[x][y].tile == '*'))
			{
				visited[x][y] = true;
				queue.push({ x , y });
				prev[x][y].x = s.x;
				prev[x][y].y = s.y;
			}
		}

	}

	int x = fruitX, y = fruitY;

	path.push({ x, y });

	while (path.top().x != headPositionX || path.top().y != headPositionY)
	{
		path.push({ prev[x][y].x, prev[x][y].y});
		x = path.top().x;
		y = path.top().y;
	}
}

void generateFruit()
{
	srand((unsigned int)std::time(nullptr));
	fruitX = rand() % (WIDTH - 2) + 1;
	fruitY = rand() % (HEIGHT - 2) + 1;

	while (area[fruitX][fruitY].tile != ' ')
	{
		fruitX = rand() % (WIDTH - 2) + 1;
		fruitY = rand() % (HEIGHT - 2) + 1;
	}

	area[fruitX][fruitY].tile = '*';

	bfs();
}

void setupConsole()
{
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX consoleCurrentFontEx;

	consoleCurrentFontEx.cbSize = sizeof(CONSOLE_FONT_INFOEX);

	GetCurrentConsoleFontEx(hConsoleOutput, false, &consoleCurrentFontEx);
	wcscpy_s(consoleCurrentFontEx.FaceName, L"Terminal");
	consoleCurrentFontEx.FontFamily = 48;
	consoleCurrentFontEx.dwFontSize = { 16 , 16 };
	SetCurrentConsoleFontEx(hConsoleOutput, true, &consoleCurrentFontEx);

	SMALL_RECT consoleWindowInfo = { 0, 0, 0, 0 };
	SetConsoleWindowInfo(hConsoleOutput, true, &consoleWindowInfo);

	COORD consoleDisplayMode = { WIDTH, HEIGHT + 3 };
	SetConsoleScreenBufferSize(hConsoleOutput, consoleDisplayMode);

	consoleWindowInfo = { 0, 0, WIDTH - 1, HEIGHT + 2 };
	SetConsoleWindowInfo(hConsoleOutput, true, &consoleWindowInfo);

	SetConsoleTitle(L"Snake");

	HWND hWnd = GetConsoleWindow();

	LONG_PTR WindowLongPtr = GetWindowLongPtr(hWnd, GWL_STYLE);

	WindowLongPtr &= ~(WS_THICKFRAME | WS_CAPTION);

	SetWindowLongPtr(hWnd, GWL_STYLE, WindowLongPtr);
}

void setup()
{
	gameover = 0;
	lengthBody = 0;
	score = 0;
	direction = 0;

	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			if (j == 0 || j == HEIGHT - 1 || i == 0 || i == WIDTH - 1)
				area[i][j].tile = '#';
			else
				area[i][j].tile = ' ';
		}
	}

	headPositionX = WIDTH / 2;
	headPositionY = HEIGHT / 2;
	area[headPositionX][headPositionY].tile = '@';

	generateFruit();
}

void draw()
{
	static char oldTiles[WIDTH][HEIGHT];

	for (short i = 0; i < HEIGHT; i++)
	{
		for (short j = 0; j < WIDTH; j++)
		{
			if (oldTiles[j][i] != area[j][i].tile)
			{
				CHAR_INFO ch;
				SMALL_RECT coords = { j, i, j, i };

				ch.Char.UnicodeChar = area[j][i].tile;
				ch.Attributes = FOREGROUND_BLUE;

				switch (ch.Char.UnicodeChar)
				{
				case '@':
					ch.Attributes = FOREGROUND_RED;
					break;
				case '0':
					ch.Attributes = FOREGROUND_GREEN;
					break;
				case '*':
					ch.Attributes = FOREGROUND_GREEN | FOREGROUND_RED;
					break;
				}

				WriteConsoleOutput(hConsoleOutput, &ch, { 1 , 1 }, { 0 , 0 }, &coords);
				oldTiles[j][i] = area[j][i].tile;
			}
		}
	}

	wchar_t str[16];
	DWORD d;

	size_t lenStr = swprintf(str, 16, L"Score = %d", score);

	WriteConsoleOutputCharacter(hConsoleOutput, str, lenStr, { 0 , HEIGHT + 1 }, &d);
}

void logic()
{
	//Sleep(50);

	area[headPositionX][headPositionY].tile = '0';
	area[headPositionX][headPositionY].lifeTime = lengthBody;

	headPositionX = path.top().x;
	headPositionY = path.top().y;
	path.pop();

	if (area[headPositionX][headPositionY].tile == '#' ||
		area[headPositionX][headPositionY].tile == '0' && lengthBody != 0)
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
		for (int i = 0; i < WIDTH; i++)
		{
			for (int j = 0; j < HEIGHT; j++)
			{
				if (area[i][j].tile != '0')
					continue;

				if (area[i][j].lifeTime <= 0)
					area[i][j].tile = ' ';
				else
					area[i][j].lifeTime--;
			}
		}
	}
}

void restartGame()
{
	wchar_t str[32];
	DWORD d;

	bestScore = max(bestScore, score);

	size_t lenStr = swprintf(str, 32, L"Best score = %d", bestScore);

	WriteConsoleOutputCharacter(hConsoleOutput, str, lenStr, { 0 , HEIGHT }, &d);

	lenStr = swprintf(str, 32, L"Press r to restart");

	WriteConsoleOutputCharacter(hConsoleOutput, str, lenStr, { 0 , HEIGHT + 1 }, &d);

	if (_getch() != 'r')
	{
		restart++;
		return;
	}

	WriteConsoleOutputCharacter(hConsoleOutput, std::wstring(lenStr, L' ').c_str(), lenStr, { 0 , HEIGHT + 1 }, &d);
}

int main()
{
	setupConsole();

	while (!restart)
	{
		setup();

		while (!gameover)
		{
			draw();
			//input();
			logic();
		}

		restartGame();
	}
}