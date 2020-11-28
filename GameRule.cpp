#include "GameRule.h"
#include <cstring>

GameRule::GameRule()
{
	memset(A, 0, sizeof A);
	step = 0;
}

bool GameRule::isLegal(int x, int y, bool col)
{
	if (step == 0 && x == 4 && y == 4)
		return false;
	if ((step & 1) != col)
		return false;
	return true;
}

void GameRule::setPiece(int x, int y, bool col)
{
	step++;
	A[x][y] = col + 1;
}

void GameRule::Register(CheckerBoard* p)
{
	pBoard = p;
}
