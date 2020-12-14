#pragma once
#include <cstring>
#include "Graphic.h"

#define GAME_WHITE_WIN 3
#define GAME_BLACK_WIN 4

enum Color
{
	SPACE, BLACK, WHITE
};

struct DSU
{
	Point fa[9][9];
	int hp[9][9];
	DSU() { memset(fa, -1, sizeof fa); }
	void Reset()
	{
		memset(fa, -1, sizeof fa);
		memset(hp, 0, sizeof hp);
	}
	Point Root(Point u)
	{
		return fa[u.x][u.y] == Point(-1, -1) ? u : (fa[u.x][u.y] = Root(fa[u.x][u.y]));
	}
	void Merge(Point u, Point v)
	{
		Point r1 = Root(u), r2 = Root(v);
		if (r1 == r2)
			return;
		fa[r1.x][r1.y] = r2;
		hp[r2.x][r2.y] += hp[r1.x][r1.y];
	}
};

class GameRule
{
	const int dir[4][2] = { {-1,0},{0,1},{1,0},{0,-1} };

public:
	Color A[9][9];
	int B[2][9][9];
	int step;

	DSU dsu;

	bool inBoard(Point u);

public:
	GameRule();
	Color moveColor();
	bool isLegal(int x, int y, Color col);
	int isOver();
	void setPiece(int x, int y, Color col);
};
