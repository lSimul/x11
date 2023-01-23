#include "common.h"
#include "file.h"
#include "image.h"

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
	BULD,
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
	board.height = board.width = 7;
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

	for (int i = 0; i < board.size; i++)
	{
		printf("X: %d\nY: %d\nT: %d\n\n", board.tiles[i].x, board.tiles[i].y, board.tiles[i].type);
	}

	free(board.tiles);
}
