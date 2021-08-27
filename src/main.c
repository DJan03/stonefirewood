#include <stdio.h>
#include <windows.h>

#define CONSOLE_WIDTH 60
#define CONSOLE_HEIGHT 30

#define MAP_WIDTH 60
#define MAP_HEIGHT 20

#define BLACK 0
#define DARK_BLUE 1
#define DARK_GREEN 2
#define DARK_CYAN 3
#define DARK_RED 4
#define DARK_PURPLE 5
#define DARK_YELLOW 6
#define LIGHT_GRAY 7
#define GRAY 8
#define BLUE 9
#define GREEN 10
#define CYAN 11
#define RED 12
#define PURPLE 13
#define YELLOW 14
#define WHITE 15

HANDLE MY_HANDLE;

typedef struct Senviroment
{
	char symbol;
	int color;
} Tenviroment;

Tenviroment enviroment_map[MAP_HEIGHT][MAP_WIDTH];

void BuildEnviromentMap()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			enviroment_map[i][j].symbol = rand() % 2 ? '\"' : '^';
			enviroment_map[i][j].color = rand() % 2 ? GREEN : DARK_GREEN;
		}
}

void PrintSybmol(char symbol, int color)
{
	SetConsoleTextAttribute(MY_HANDLE, color);
	printf("%c", symbol);
}

void PrintEnviromentMap()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
			PrintSybmol(enviroment_map[i][j].symbol, enviroment_map[i][j].color);
}

void SetWindowSize()
{
	COORD coord = {};
	coord.X = (SHORT)(CONSOLE_WIDTH);
	coord.Y = (SHORT)(CONSOLE_HEIGHT);
	SetConsoleScreenBufferSize(MY_HANDLE, coord);

	SMALL_RECT rect = {};
	rect.Bottom = coord.Y - 1;
	rect.Right = coord.X - 1;
	SetConsoleWindowInfo(MY_HANDLE, TRUE, &rect);
}

int main()
{
	MY_HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);

	SetWindowSize();

	BuildEnviromentMap();

	PrintEnviromentMap();

	getchar();
	return 0;
}