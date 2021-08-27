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

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

#define ENTITY_CAPACITY 5

HANDLE MY_HANDLE;

enum Eenviroment
{
	GRASS,
	ROCK,
	WATER
};

typedef struct Senviroment
{
	char symbol;
	int color;
	int type;
} Tenviroment;

typedef struct Sentity
{
	char symbol;
	int color;
	int x;
	int y;
} Tentity;

Tenviroment enviroment_map[MAP_HEIGHT][MAP_WIDTH];

Tentity entity_map[ENTITY_CAPACITY];

void BuildEnviromentMap()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (j + i / 2 + rand() % 2 < 15)
			{
				enviroment_map[i][j].symbol = rand() % 2 ? '~' : '=';
				enviroment_map[i][j].color = (i + j / 10) % 2 ? CYAN : BLUE;
				enviroment_map[i][j].type = WATER;
			}
			else if (rand() % 100 < 5)
			{
				enviroment_map[i][j].symbol = rand() % 2 ? 'o' : 'n';
				enviroment_map[i][j].color = rand() % 2 ? GRAY : LIGHT_GRAY;
				enviroment_map[i][j].type = ROCK;
			}
			else
			{
				enviroment_map[i][j].symbol = rand() % 2 ? '\"' : '^';
				enviroment_map[i][j].color = rand() % 2 ? GREEN : DARK_GREEN;
				enviroment_map[i][j].type = GRASS;
			}
		}
}

void PrintSymbol(char symbol, int color)
{
	SetConsoleTextAttribute(MY_HANDLE, color);
	printf("%c", symbol);
}

int IsEntityCell(int x, int y)
{
	for (int i = 0; i < ENTITY_CAPACITY; i++)
	{
		if (entity_map[i].x == x && entity_map[i].y == y)
			return (i);
	}
	return (-1);
}

void PrintEnviromentMap()
{
	int entity_id;
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			entity_id = IsEntityCell(j, i);
			if (entity_id >= 0)
				PrintSymbol(entity_map[entity_id].symbol, entity_map[entity_id].color);
			else
				PrintSymbol(enviroment_map[i][j].symbol, enviroment_map[i][j].color);
		}
		PrintSymbol('\n', WHITE);
	}
}

int InBounds(int x, int y)
{
	return (x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT);
}

void MoveEntity(Tentity *entity, int dx, int dy)
{
	int newx = entity->x + dx;
	int newy = entity->y + dy;

	if (InBounds(newx, newy))
	{
		if (enviroment_map[newy][newx].type == GRASS)
		{
			entity->x = newx;
			entity->y = newy;
		}
	}
}

void BuildEntityMap()
{
	entity_map[0].x = MAP_WIDTH / 2;
	entity_map[0].y = MAP_HEIGHT / 2;
	entity_map[0].symbol = '@';
	entity_map[0].color = YELLOW;
	for (int i = 1; i < ENTITY_CAPACITY; i++)
	{
		entity_map[i].x = MAP_WIDTH / 2 + rand() % 10 - 5;
		entity_map[i].y = MAP_HEIGHT / 4 + rand() % 10 - 5;
		entity_map[i].symbol = 'h';
		entity_map[i].color = DARK_YELLOW;
	}
}

void UpdateEntities()
{
	for (int i = 1; i < ENTITY_CAPACITY; i++)
	{
		if (rand() % 5)
		{
			MoveEntity(&entity_map[i], rand() % 3 - 1, rand() % 3 - 1);
		}
	}
}

void SetCursorPosition(int x, int y)
{
	COORD coord = {0};
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(MY_HANDLE, coord);
}

void SetWindowSize()
{
	COORD coord = {};
	coord.X = (SHORT)(CONSOLE_WIDTH) + 2;
	coord.Y = (SHORT)(CONSOLE_HEIGHT);
	SetConsoleScreenBufferSize(MY_HANDLE, coord);

	SMALL_RECT rect = {};
	rect.Bottom = coord.Y;
	rect.Right = coord.X;
	SetConsoleWindowInfo(MY_HANDLE, TRUE, &rect);
}

void PrintUI()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if ((i == 0 && (j == 0 || j == MAP_WIDTH - 1)) ||
				(i == 5 && (j == 0 || j == MAP_WIDTH - 1)))
				PrintSymbol('#', WHITE);
			else if (j == 0 || j == MAP_WIDTH - 1)
				PrintSymbol('|', WHITE);
			else if (i == 0 || i == 5)
				PrintSymbol('-', WHITE);
			else
				PrintSymbol(' ', WHITE);
		}
		PrintSymbol('\n', WHITE);
	}
}

int main()
{
	srand(0);

	MY_HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);

	SetWindowSize();

	BuildEnviromentMap();
	BuildEntityMap();

	while (GetKeyState(VK_ESCAPE) >= 0)
	{
		SetCursorPosition(0, 0);
		PrintUI();
		PrintEnviromentMap();

		if (GetAsyncKeyState(VK_W)) MoveEntity(&entity_map[0], 0, -1);
		if (GetAsyncKeyState(VK_S)) MoveEntity(&entity_map[0], 0, 1);
		if (GetAsyncKeyState(VK_A)) MoveEntity(&entity_map[0], -1, 0);
		if (GetAsyncKeyState(VK_D)) MoveEntity(&entity_map[0], 1, 0);

		UpdateEntities();
	}

	return 0;
}