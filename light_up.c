#include "common.h"
#include "file.h"
#include "image.h"
#include "movement.h"

#include <unistd.h>

#define BOARD_SIZE 7

#define OPTIONS 7

#define TILE_SIZE 124

#define X_START 526
#define Y_START 148

typedef enum tile_type
{
	VALUE_0,
	VALUE_1,
	VALUE_2,
	VALUE_3,
	VALUE_4,

	BLOCK,
	EMPTY,

	LIGHT,
	BULB,
	DISABLED,
} TILE_TYPE;

typedef struct tile
{
	int x;
	int y;

	TILE_TYPE type;
} TILE;

typedef struct board
{
	int width;
	int height;
	int size;

	TILE *tiles;
} BOARD;

void lightUp(BOARD *board, int x, int y);

int coordsToIndex(int refX, int refY);

int orderTiles(const void *l, const void *r)
{
	const TILE *tl = l;
	const TILE *tr = r;

	if (tl->y == tr->y)
	{
		return tl->x - tr->x;
	}
	return tl->y - tr->y;
}

int main()
{
	// Order is matching with TILE_TYPE.
	const char *tiles[OPTIONS] = {
		"tiles/0.bmp",
		"tiles/1.bmp",
		"tiles/2.bmp",
		"tiles/3.bmp",
		"tiles/4.bmp",

		"tiles/block.bmp",
		"tiles/empty.bmp",
	};

	X_INSTANCE instance = {};
	instance.display = XOpenDisplay(NULL);
	if (instance.display == NULL)
	{
		printf("No display found.\n");
		return 1;
	}
	instance.window = XDefaultRootWindow(instance.display);
	XGetWindowAttributes(instance.display, instance.window, &instance.attrs);

	BOARD board = {};
	// I will start with 7x7 puzzle only.
	board.height = board.width = BOARD_SIZE;
	board.size = board.height * board.width;
	board.tiles = malloc(board.size * sizeof(*board.tiles));
	int sum = 0;
	for (int i = 0; i < OPTIONS; i++)
	{
		BITMAP bitmap = {};
		if (readBitmap(&bitmap, tiles[i]))
		{
			printf("Fail while reading '%s'.\n", tiles[i]);
			return 1;
		}

		int size = 0;
		COORDS *coords = findAllImages(&size, &bitmap, &instance);
		for (int t = 0; t < size; t++)
		{
			int x = coords[t].x - X_START;
			int y = coords[t].y - Y_START;
			if (x % TILE_SIZE || y % TILE_SIZE)
			{
				continue;
			}

			board.tiles[sum].x = coords[t].x;
			board.tiles[sum].y = coords[t].y;
			board.tiles[sum].type = i;

			sum++;
		}
		free(bitmap.data);
		free(coords);
	}
	qsort(board.tiles, board.size, sizeof(*board.tiles), orderTiles);

	// Very naive approach.
	for (int i = 0; i < board.size; i++)
	{
		int x = i % BOARD_SIZE;
		int y = (i - i % BOARD_SIZE) / BOARD_SIZE;

		int coordX = board.tiles[i].x;
		int coordY = board.tiles[i].y;

		coordX += TILE_SIZE / 2;
		coordY += TILE_SIZE / 2;

		if (board.tiles[i].type == VALUE_0)
		{
			if (x + 1 < BOARD_SIZE)
			{
				moveAndRightClick(instance.display, &instance.window, coordX + TILE_SIZE, coordY);
				sleep(1);
			}
			if (x - 1 >= 0)
			{
				moveAndRightClick(instance.display, &instance.window, coordX - TILE_SIZE, coordY);
				sleep(1);
			}
			if (y + 1 < BOARD_SIZE)
			{
				moveAndRightClick(instance.display, &instance.window, coordX, coordY + TILE_SIZE);
				sleep(1);
			}
			if (y - 1 >= 0)
			{
				moveAndRightClick(instance.display, &instance.window, coordX, coordY - TILE_SIZE);
				sleep(1);
			}
		}
		else if (board.tiles[i].type == VALUE_4)
		{
			moveAndClick(instance.display, &instance.window, coordX + TILE_SIZE, coordY);
			sleep(1);
			lightUp(&board, x, y);

			moveAndClick(instance.display, &instance.window, coordX - TILE_SIZE, coordY);
			sleep(1);
			lightUp(&board, x, y);

			moveAndClick(instance.display, &instance.window, coordX, coordY + TILE_SIZE);
			sleep(1);
			lightUp(&board, x, y);

			moveAndClick(instance.display, &instance.window, coordX, coordY - TILE_SIZE);
			sleep(1);
			lightUp(&board, x, y);
		}
	}

	char end = 0;
	while (1)
	{
		end = 0;
		for (int i = 0; i < board.size; i++)
		{
			if (board.tiles[i].type == EMPTY)
			{
				end++;
				break;
			}
		}
		if (!end)
		{
			break;
		}

		for (int i = 0; i < board.size; i++)
		{

			int x = i % BOARD_SIZE;
			int y = (i - i % BOARD_SIZE) / BOARD_SIZE;

			int coordX = board.tiles[i].x;
			int coordY = board.tiles[i].y;

			coordX += TILE_SIZE / 2;
			coordY += TILE_SIZE / 2;
		}
		printf("Everything is not resolved.");
		break;
	}

	free(board.tiles);
}

void lightUp(BOARD *board, int refX, int refY)
{
	int x = refX;
	int y = refY;
	while (1)
	{
		if (x >= BOARD_SIZE || x < 0 || y >= BOARD_SIZE || y < 0)
		{
			break;
		}
		int i = coordsToIndex(x, y);
		TILE t = board->tiles[i];
		if (t.type == EMPTY || t.type == LIGHT)
		{
			t.type = LIGHT;
			x--;
		}
		else
		{

			break;
		}
	}

	x = refX;
	y = refY;
	while (1)
	{
		if (x >= BOARD_SIZE || x < 0 || y >= BOARD_SIZE || y < 0)
		{
			break;
		}
		int i = coordsToIndex(x, y);
		TILE t = board->tiles[i];
		if (t.type == EMPTY || t.type == LIGHT)
		{
			t.type = LIGHT;
			y--;
		}
		else
		{

			break;
		}
	}

	x = refX;
	y = refY;
	while (1)
	{
		if (x >= BOARD_SIZE || x < 0 || y >= BOARD_SIZE || y < 0)
		{
			break;
		}
		int i = coordsToIndex(x, y);
		TILE t = board->tiles[i];
		if (t.type == EMPTY || t.type == LIGHT)
		{
			t.type = LIGHT;
			x++;
		}
		else
		{

			break;
		}
	}

	x = refX;
	y = refY;
	while (1)
	{
		if (x >= BOARD_SIZE || x < 0 || y >= BOARD_SIZE || y < 0)
		{
			break;
		}
		int i = coordsToIndex(x, y);
		TILE t = board->tiles[i];
		if (t.type == EMPTY || t.type == LIGHT)
		{
			t.type = LIGHT;
			y++;
		}
		else
		{

			break;
		}
	}
}

int coordsToIndex(int x, int y)
{
	return x + y * BOARD_SIZE;
}
