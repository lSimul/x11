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

typedef struct empty_bulb
{
	char empty;
	char bulb;
} EMPTY_BULB;

void lightUp(BOARD *board, int x, int y);

int coordsToIndex(int refX, int refY);

EMPTY_BULB evaluateTileSurroundings(const BOARD *board, int x, int y);

void lightUpSurroundingTilesIfAble(X_INSTANCE *instance, BOARD *board, int x, int y, int coordX, int coordY);

void disableTilesAboveLimit(X_INSTANCE *instance, BOARD *board, int x, int y, int coordX, int coordY);

int findEmptyTiles(const BOARD *board, COORDS *firstOccurence, int refX, int refY);

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
			disableTilesAboveLimit(&instance, &board, x, y, coordX, coordY);
		}
		else if (board.tiles[i].type == VALUE_4)
		{
			lightUpSurroundingTilesIfAble(&instance, &board, x, y, coordX, coordY);
		}
	}

	char end = 0;
	char tileSolved = 0;
	while (1)
	{
		tileSolved = 0;

		for (int i = 0; i < board.size; i++)
		{
			int x = i % BOARD_SIZE;
			int y = (i - i % BOARD_SIZE) / BOARD_SIZE;

			int coordX = board.tiles[i].x;
			int coordY = board.tiles[i].y;

			coordX += TILE_SIZE / 2;
			coordY += TILE_SIZE / 2;

			EMPTY_BULB stats = evaluateTileSurroundings(&board, x, y);

			TILE_TYPE t = board.tiles[i].type;
			if (t == VALUE_1 || t == VALUE_2 || t == VALUE_3)
			{
				if (stats.bulb == t && stats.empty > 0)
				{
					disableTilesAboveLimit(&instance, &board, x, y, coordX, coordY);
					tileSolved++;
				}
				else if (stats.bulb + stats.empty == t && stats.bulb < t)
				{
					lightUpSurroundingTilesIfAble(&instance, &board, x, y, coordX, coordY);
					tileSolved++;
				}
			}
			// These two options are similar, almost symmetrical.
			else if (t == EMPTY)
			{
				// Goal is to check if this is the only empty tile around. If yes, this tile can be lighten up only by this tile.
				COORDS c = {};
				int result = findEmptyTiles(&board, &c, x, y);

				if (result == 0)
				{
					moveAndClick(instance.display, &instance.window, coordX, coordY);
					sleep(1);
					lightUp(&board, x, y);
					tileSolved++;
				}
			}
			else if (t == DISABLED)
			{
				// Looks around the tiles. If only one empty tile is found, light has to be turned on „there“.
				// Otherwise it does not have a solution.
				COORDS c = {};
				int result = findEmptyTiles(&board, &c, x, y);

				if (result == 1)
				{
					TILE t = board.tiles[coordsToIndex(c.x, c.y)];

					int coordX = t.x;
					int coordY = t.y;
					coordX += TILE_SIZE / 2;
					coordY += TILE_SIZE / 2;

					moveAndClick(instance.display, &instance.window, coordX, coordY);
					sleep(1);
					lightUp(&board, c.x, c.y);
					tileSolved++;
				}
			}
		}
		if (tileSolved)
		{
			continue;
		}

		end = 0;
		for (int i = 0; i < board.size; i++)
		{
			if (board.tiles[i].type == EMPTY)
			{
				end++;
			}
		}
		break;
	}
	printf("Just %d tiles has to be lighten up.\n", end);

	free(board.tiles);
}

void lightUp(BOARD *board, int refX, int refY)
{
	int x = refX;
	int y = refY;

	board->tiles[coordsToIndex(x, y)].type = BULB;

	TILE *t = NULL;

	while (1)
	{
		if (x < 0)
		{
			break;
		}
		t = &board->tiles[coordsToIndex(x, y)];
		if (t->type == BULB || t->type == LIGHT)
		{
			x--;
		}
		else if (t->type == EMPTY || t->type == DISABLED)
		{
			t->type = LIGHT;
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
		if (y < 0)
		{
			break;
		}
		t = &board->tiles[coordsToIndex(x, y)];
		if (t->type == BULB || t->type == LIGHT)
		{
			y--;
		}
		else if (t->type == EMPTY || t->type == DISABLED)
		{
			t->type = LIGHT;
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
		if (x >= BOARD_SIZE)
		{
			break;
		}
		t = &board->tiles[coordsToIndex(x, y)];
		if (t->type == BULB || t->type == LIGHT)
		{
			x++;
		}
		else if (t->type == EMPTY || t->type == DISABLED)
		{
			t->type = LIGHT;
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
		if (y >= BOARD_SIZE)
		{
			break;
		}
		t = &board->tiles[coordsToIndex(x, y)];
		if (t->type == BULB || t->type == LIGHT)
		{
			y++;
		}
		else if (t->type == EMPTY || t->type == DISABLED)
		{
			t->type = LIGHT;
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

EMPTY_BULB evaluateTileSurroundings(const BOARD *board, int x, int y)
{
	EMPTY_BULB result = {};

	result.empty = 0;
	result.bulb = 0;
	if (x + 1 < BOARD_SIZE)
	{
		TILE_TYPE t = board->tiles[coordsToIndex(x + 1, y)].type;
		if (t == EMPTY)
		{
			result.empty++;
		}
		else if (t == BULB)
		{
			result.bulb++;
		}
	}
	if (x - 1 >= 0)
	{
		TILE_TYPE t = board->tiles[coordsToIndex(x - 1, y)].type;
		if (t == EMPTY)
		{
			result.empty++;
		}
		else if (t == BULB)
		{
			result.bulb++;
		}
	}
	if (y + 1 < BOARD_SIZE)
	{
		TILE_TYPE t = board->tiles[coordsToIndex(x, y + 1)].type;
		if (t == EMPTY)
		{
			result.empty++;
		}
		else if (t == BULB)
		{
			result.bulb++;
		}
	}
	if (y - 1 >= 0)
	{
		TILE_TYPE t = board->tiles[coordsToIndex(x, y - 1)].type;
		if (t == EMPTY)
		{
			result.empty++;
		}
		else if (t == BULB)
		{
			result.bulb++;
		}
	}
	return result;
}

void lightUpSurroundingTilesIfAble(X_INSTANCE *instance, BOARD *board, int x, int y, int coordX, int coordY)
{
	if (x + 1 < BOARD_SIZE)
	{
		if (board->tiles[coordsToIndex(x + 1, y)].type == EMPTY)
		{
			moveAndClick(instance->display, &instance->window, coordX + TILE_SIZE, coordY);
			sleep(1);
			lightUp(board, x + 1, y);
		}
	}
	if (x - 1 >= 0)
	{
		if (board->tiles[coordsToIndex(x - 1, y)].type == EMPTY)
		{
			moveAndClick(instance->display, &instance->window, coordX - TILE_SIZE, coordY);
			sleep(1);
			lightUp(board, x - 1, y);
		}
	}
	if (y + 1 < BOARD_SIZE)
	{
		if (board->tiles[coordsToIndex(x, y + 1)].type == EMPTY)
		{
			moveAndClick(instance->display, &instance->window, coordX, coordY + TILE_SIZE);
			sleep(1);
			lightUp(board, x, y + 1);
		}
	}
	if (y - 1 >= 0)
	{
		if (board->tiles[coordsToIndex(x, y - 1)].type == EMPTY)
		{
			moveAndClick(instance->display, &instance->window, coordX, coordY - TILE_SIZE);
			sleep(1);
			lightUp(board, x, y - 1);
		}
	}
}

void disableTilesAboveLimit(X_INSTANCE *instance, BOARD *board, int x, int y, int coordX, int coordY)
{
	TILE *t = NULL;
	if (x + 1 < BOARD_SIZE)
	{
		t = &board->tiles[coordsToIndex(x + 1, y)];
		if (t->type == EMPTY)
		{
			moveAndRightClick(instance->display, &instance->window, coordX + TILE_SIZE, coordY);
			sleep(1);
			t->type = DISABLED;
		}
	}
	if (x - 1 >= 0)
	{
		t = &board->tiles[coordsToIndex(x - 1, y)];
		if (t->type == EMPTY)
		{
			moveAndRightClick(instance->display, &instance->window, coordX - TILE_SIZE, coordY);
			sleep(1);
			t->type = DISABLED;
		}
	}
	if (y + 1 < BOARD_SIZE)
	{
		t = &board->tiles[coordsToIndex(x, y + 1)];
		if (t->type == EMPTY)
		{
			moveAndRightClick(instance->display, &instance->window, coordX, coordY + TILE_SIZE);
			sleep(1);
			t->type = DISABLED;
		}
	}
	if (y - 1 >= 0)
	{
		t = &board->tiles[coordsToIndex(x, y - 1)];
		if (t->type == EMPTY)
		{
			moveAndRightClick(instance->display, &instance->window, coordX, coordY - TILE_SIZE);
			sleep(1);
			t->type = DISABLED;
		}
	}
}

int findEmptyTiles(const BOARD *board, COORDS *firstOccurence, int refX, int refY)
{
	int result = 0;

	TILE *t = NULL;

	int x = refX - 1;
	int y = refY;

	while (1)
	{
		if (x < 0)
		{
			break;
		}
		t = &board->tiles[coordsToIndex(x, y)];
		if (t->type == BULB || t->type == LIGHT || t->type == DISABLED)
		{
			x--;
		}
		else if (t->type == EMPTY)
		{
			firstOccurence->x = x;
			firstOccurence->y = y;
			result++;

			x--;
		}
		else
		{
			break;
		}
	}

	x = refX;
	y = refY - 1;
	while (1)
	{
		if (y < 0)
		{
			break;
		}
		t = &board->tiles[coordsToIndex(x, y)];
		if (t->type == BULB || t->type == LIGHT || t->type == DISABLED)
		{
			y--;
		}
		else if (t->type == EMPTY)
		{
			firstOccurence->x = x;
			firstOccurence->y = y;
			result++;

			y--;
		}
		else
		{
			break;
		}
	}

	x = refX + 1;
	y = refY;
	while (1)
	{
		if (x >= BOARD_SIZE)
		{
			break;
		}
		t = &board->tiles[coordsToIndex(x, y)];
		if (t->type == BULB || t->type == LIGHT || t->type == DISABLED)
		{
			x++;
		}
		else if (t->type == EMPTY)
		{
			firstOccurence->x = x;
			firstOccurence->y = y;
			result++;

			x++;
		}
		else
		{
			break;
		}
	}

	x = refX;
	y = refY + 1;
	while (1)
	{
		if (y >= BOARD_SIZE)
		{
			break;
		}
		t = &board->tiles[coordsToIndex(x, y)];
		if (t->type == BULB || t->type == LIGHT || t->type == DISABLED)
		{
			y++;
		}
		else if (t->type == EMPTY)
		{
			firstOccurence->x = x;
			firstOccurence->y = y;
			result++;

			y++;
		}
		else
		{

			break;
		}
	}

	return result;
}
