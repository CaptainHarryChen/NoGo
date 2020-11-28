#pragma once
#include "CheckerBoard.h"

class GameRule
{
	unsigned char A[9][9];
	int step;
	CheckerBoard* pBoard;

public:
	GameRule();
	bool isLegal(int x, int y, bool col);
	void setPiece(int x, int y, bool col);
	void Register(CheckerBoard* p);
};

