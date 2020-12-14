#include "GameRule.h"
#include <cstring>

#include <iostream>

bool GameRule::inBoard(Point u)
{
	return Rect(0, 0, 8, 8).inRect(u);
}

GameRule::GameRule()
{
	memset(A, 0, sizeof A);
	step = 0;
}

bool GameRule::dfs_air(int fx, int fy)
{
	dfs_air_visit[fx][fy] = true;
	bool flag = false;
	for (int d = 0; d < 4; d++)
	{
		int dx = fx + dir[d][0], dy = fy + dir[d][1];
		if (inBoard(Point(dx, dy)))
		{
			if (A[dx][dy] == SPACE)
				flag = true;
			if (A[dx][dy] == A[fx][fy] && !dfs_air_visit[dx][dy])
				if (dfs_air(dx, dy))
					flag = true;
		}
	}
	return flag;
}

Color GameRule::moveColor()
{
	return step % 2 == 0 ? Color::BLACK : Color::WHITE;
}

//true: available
bool GameRule::isLegal(int x, int y, Color col)
{
	//std::cerr << "Check move (" << x << "," << y << "," << col << ")" << std::endl;
	if (step == 0 && x == 4 && y == 4)
		return false;
	if (moveColor() != col)
		return false;
	if (A[x][y] != SPACE)
		return false;
	A[x][y] = col;
	memset(dfs_air_visit, 0, sizeof(dfs_air_visit));
	if (!dfs_air(x, y))
	{
		A[x][y] = SPACE;
		return false;
	}
	for (int d = 0; d < 4; d++)
	{
		int dx = x + dir[d][0], dy = y + dir[d][1];
		if (inBoard(Point(dx, dy)))
		{
			if (A[dx][dy] != SPACE && !dfs_air_visit[dx][dy])
				if (!dfs_air(dx, dy))
				{
					A[x][y] = SPACE;
					return false;
				}
		}
	}
	A[x][y] = SPACE;
	return true;
}

int GameRule::isOver()
{
	int cnt1 = 0, cnt2 = 0;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			int t = 0;
			if (isLegal(i, j, BLACK))
				t |= 1, cnt1++;
			if (isLegal(i, j, WHITE))
				t |= 2, cnt2++;
		}
	if (moveColor() == BLACK && cnt1 == 0)
		return GAME_WHITE_WIN;
	if (moveColor() == WHITE && cnt2 == 0)
		return GAME_BLACK_WIN;
	return 0;
}

void GameRule::setPiece(int x, int y, Color col)
{
	step++;
	A[x][y] = col;
}
