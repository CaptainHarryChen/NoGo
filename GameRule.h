#pragma once

enum Color
{
	SPACE, BLACK, WHITE
};

class GameRule
{
	Color A[9][9];
	int step;

public:
	GameRule();
	Color moveColor();
	bool isLegal(int x, int y, Color col);
	void setPiece(int x, int y, Color col);
};

