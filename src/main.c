#include <stdio.h>
#include <windows.h>

#define CONSOLE_WIDTH 60
#define CONSOLE_HEIGHT 30

HANDLE MYHANDLE;

void SetWindowSize()
{
	COORD coord = {};
	coord.X = (SHORT)(CONSOLE_WIDTH);
	coord.Y = (SHORT)(CONSOLE_HEIGHT);
	SetConsoleScreenBufferSize(MYHANDLE, coord);

	SMALL_RECT rect = {};
	rect.Bottom = coord.Y - 1;
	rect.Right = coord.X - 1;
	SetConsoleWindowInfo(MYHANDLE, TRUE, &rect);
}

int main()
{
	MYHANDLE = GetStdHandle(STD_OUTPUT_HANDLE);

	SetWindowSize();

	printf("Hello world!\n");

	getchar();
	return 0;
}