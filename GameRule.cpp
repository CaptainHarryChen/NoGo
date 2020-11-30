#include "GameRule.h"
#include <cstring>

GameRule::GameRule()
{
	memset(A, 0, sizeof A);
	step = 0;
}

Color GameRule::moveColor()
{
	return step % 2 == 0 ? BLACK : WHITE;
}

bool GameRule::isLegal(int x, int y, Color col)
{
	if (step == 0 && x == 4 && y == 4)
		return false;
	if (moveColor() != col)
		return false;
	if (A[x][y] != SPACE)
		return false;
	return true;
}

void GameRule::setPiece(int x, int y, Color col)
{
	step++;
	A[x][y] = col;
}
