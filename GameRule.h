#pragma once
#include <cstring>
#include "Graphic.h"

#define GAME_WHITE_WIN 3
#define GAME_BLACK_WIN 4

enum Color
{
	SPACE, BLACK, WHITE
};

class GameRule
{
	const int dir[4][2] = { {-1,0},{0,1},{1,0},{0,-1} };

	bool dfs_air_visit[9][9];

protected:
	Color A[9][9];
	int step;

	bool inBoard(Point u);
	bool dfs_air(int fx, int fy);

public:
	GameRule();
	Color moveColor();
	bool isLegal(int x, int y, Color col);
	int isOver();
	void setPiece(int x, int y, Color col);
};

