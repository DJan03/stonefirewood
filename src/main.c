#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>

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


typedef struct Sobject
{
	char symbol;
	int color;
} Tobject;

typedef enum Eenvironment
{
	GRASS,
	ROCK,
	WATER
} TenvType;

typedef struct Senvironment
{
	Tobject object;
	TenvType type;
} Tenvironment;

typedef struct Sentity
{
	Tobject object;
	int x;
	int y;
} Tentity;


Tobject MY_BUFFER[CONSOLE_HEIGHT][CONSOLE_WIDTH];
bool NEED_TO_UPDATE[CONSOLE_HEIGHT][CONSOLE_WIDTH];

HANDLE MY_HANDLE;

int inventory_id = 0;

Tenvironment environment_map[MAP_HEIGHT][MAP_WIDTH];

Tentity entity_map[ENTITY_CAPACITY];


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

void SetCursorPosition(int x, int y)
{
	COORD coord = {0};
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(MY_HANDLE, coord);
}


void PutObjectToBuffer(int x, int y, Tobject object)
{
	if (MY_BUFFER[y][x].symbol == object.symbol && MY_BUFFER[y][x].color == object.color)
		return ;
	NEED_TO_UPDATE[y][x] = TRUE;
	MY_BUFFER[y][x] = object;
}

void PutObjectToConsole(int x, int y, Tobject object)
{
	SetCursorPosition(x, y);
	SetConsoleTextAttribute(MY_HANDLE, object.color);
	printf("%c", object.symbol);
	NEED_TO_UPDATE[y][x] = FALSE;
}


int InBounds(int x, int y)
{
	return (x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT);
}


void BuildEnvironmentMap()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (j + i / 2 + rand() % 2 < 15)
			{
				environment_map[i][j].object.symbol = rand() % 2 ? '~' : '=';
				environment_map[i][j].object.color = (i + j / 10) % 2 ? CYAN : BLUE;
				environment_map[i][j].type = WATER;
			}
			else if (rand() % 100 < 5)
			{
				environment_map[i][j].object.symbol = rand() % 2 ? 'o' : 'n';
				environment_map[i][j].object.color = rand() % 2 ? GRAY : LIGHT_GRAY;
				environment_map[i][j].type = ROCK;
			}
			else
			{
				environment_map[i][j].object.symbol = rand() % 2 ? '\"' : '^';
				environment_map[i][j].object.color = rand() % 2 ? GREEN : DARK_GREEN;
				environment_map[i][j].type = GRASS;
			}
		}
}

void BuildEntityMap()
{
	entity_map[0].x = MAP_WIDTH / 2;
	entity_map[0].y = MAP_HEIGHT / 2;
	entity_map[0].object.symbol = '@';
	entity_map[0].object.color = YELLOW;
	for (int i = 1; i < ENTITY_CAPACITY; i++)
	{
		entity_map[i].x = MAP_WIDTH / 2 + rand() % 10 - 5;
		entity_map[i].y = MAP_HEIGHT / 4 + rand() % 10 - 5;
		entity_map[i].object.symbol = 'h';
		entity_map[i].object.color = DARK_YELLOW;
	}
}


void MoveEntity(Tentity *entity, int dx, int dy)
{
	int newx = entity->x + dx;
	int newy = entity->y + dy;

	if (InBounds(newx, newy))
	{
		if (environment_map[newy][newx].type == GRASS)
		{
			entity->x = newx;
			entity->y = newy;
		}
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


void PrintEnvironmentMap()
{
	for (int i = 0; i < MAP_HEIGHT; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
			PutObjectToBuffer(j, i + 6, environment_map[i][j].object);
}

void PrintEntities()
{
	for (int i = 0; i < ENTITY_CAPACITY; i++)
		PutObjectToBuffer(entity_map[i].x, entity_map[i].y + 6, entity_map[i].object);
}

void PrintUI()
{
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if ((i == 0 && (j == 0 || j == MAP_WIDTH - 1)) ||
				(i == 5 && (j == 0 || j == MAP_WIDTH - 1)))
				PutObjectToBuffer(j, i, (Tobject){'#', WHITE});
			else if (j == 0 || j == MAP_WIDTH - 1)
				PutObjectToBuffer(j, i, (Tobject){'|', WHITE});
			else if (i == 0 || i == 5)
				PutObjectToBuffer(j, i, (Tobject){'-', WHITE});
			else
				PutObjectToBuffer(j, i, (Tobject){' ', WHITE});
		}

	for (int i = 0; i < 8; i++)
		PutObjectToBuffer(3 + i % 4 * 2, 2 + i / 4, (Tobject){'.', WHITE});

	PutObjectToBuffer(2 + inventory_id % 4 * 2, 2 + inventory_id / 4, (Tobject){'[', WHITE});
	PutObjectToBuffer(2 + inventory_id % 4 * 2 + 2, 2 + inventory_id / 4, (Tobject){']', WHITE});
}


int main()
{
	srand(0);

	MY_HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);

	SetWindowSize();

	BuildEnvironmentMap();
	BuildEntityMap();

	const int FPS = 12;
	const double frameDelay = 1000 / FPS;

	double frameStart;
	double frameTime;

	while (GetKeyState(VK_ESCAPE) >= 0)
	{
		frameStart = clock();

		PrintUI();
		PrintEnvironmentMap();
		PrintEntities();

		SetCursorPosition(0, 0);
		for (int i = 0; i < CONSOLE_HEIGHT; i++)
		{
			for (int j = 0; j < CONSOLE_WIDTH; j++)
				if (NEED_TO_UPDATE[i][j] == TRUE)
					PutObjectToConsole(j, i, MY_BUFFER[i][j]);
		}

		if (GetAsyncKeyState(VK_W)) MoveEntity(&entity_map[0], 0, -1);
		if (GetAsyncKeyState(VK_S)) MoveEntity(&entity_map[0], 0, 1);
		if (GetAsyncKeyState(VK_A)) MoveEntity(&entity_map[0], -1, 0);
		if (GetAsyncKeyState(VK_D)) MoveEntity(&entity_map[0], 1, 0);

		if (GetAsyncKeyState(VK_TAB)) inventory_id = (inventory_id + 1) % 8;

		UpdateEntities();

		frameTime = clock() - frameStart;

		SetCursorPosition(0, 0);
		SetConsoleTextAttribute(MY_HANDLE, GREEN);
		if (frameDelay > frameTime)
		{
			Sleep(frameDelay - frameTime);
			printf("%3d", FPS);
		}
		else
			printf("%3d", (int)(1000 / frameTime));
	}

	return 0;
}