#include <stdio.h>
#include <windows.h>

#define CONSOLE_WIDTH 60
#define CONSOLE_HEIGHT 30

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

void Print(const char *message, int color)
{
	SetConsoleTextAttribute(MY_HANDLE, color);
	printf("%s", message);
}

int main()
{
	MY_HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);

	SetWindowSize();

	Print("Hello ", WHITE);
	Print("W", RED);
	Print("O", DARK_YELLOW);
	Print("R", YELLOW);
	Print("L", GREEN);
	Print("D", CYAN);
	Print("!", BLUE);

	getchar();
	return 0;
}