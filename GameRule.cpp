#include "GameRule.h"
#include <cstring>

GameRule::GameRule()
{
	memset(A, 0, sizeof A);
	step = 0;
}

bool GameRule::moveColor()
{
	return step & 1;
}

bool GameRule::isLegal(int x, int y, bool col)
{
	if (step == 0 && x == 4 && y == 4)
		return false;
	if (moveColor() != col)
		return false;
	if (A[x][y] != 0)
		return false;
	return true;
}

void GameRule::setPiece(int x, int y, bool col)
{
	step++;
	A[x][y] = col + 1;
}
