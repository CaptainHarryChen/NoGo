#include "GameRule.h"
#include <cstring>

GameRule::GameRule()
{
	memset(A, 0, sizeof A);
	step = 0;
}

bool GameRule::isLegal(int x, int y)
{
	if (step == 0 && x == 4 && y == 4)
		return false;
	int col = step & 1;
	return true;
}
