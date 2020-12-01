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

Color GameRule::moveColor()
{
	return step % 2 == 0 ? Color::BLACK : Color::WHITE;
}

bool GameRule::isLegal(int x, int y, Color col)
{
	//std::cerr << "Check move (" << x << "," << y << "," << col << ")" << std::endl;
	if (step == 0 && x == 4 && y == 4)
		return false;
	if (moveColor() != col)
		return false;
	if (A[x][y] != SPACE)
		return false;

	bool has_hp = false;
	for (int d = 0; d < 4; d++)
	{
		Point v(x + dir[d][0], y + dir[d][1]);
		if (inBoard(v))
		{
			if (A[v.x][v.y] == SPACE)
				has_hp = true;
			else if (A[v.x][v.y] == col)
			{
				v = dsu.Root(v);
				if (dsu.hp[v.x][v.y] > 1)
					has_hp = true;
			}
			else if (A[v.x][v.y] != col)
			{
				v = dsu.Root(v);
				if (dsu.hp[v.x][v.y] <= 1)
					return false;
			}
		}
	}

	if (!has_hp)
		return false;

	//std::cerr << "is legal move" << std::endl;
	return true;
}

void GameRule::setPiece(int x, int y, Color col)
{
	step++;
	A[x][y] = col;
	dsu.hp[x][y] = 0;
	Point u(x, y), r1, r2;
	Point vis[4];
	int it = 0;
	for (int d = 0; d < 4; d++)
	{
		Point v(x + dir[d][0], y + dir[d][1]);
		if (inBoard(v) && A[v.x][v.y] != SPACE)
		{
			r1 = dsu.Root(u);
			r2 = dsu.Root(v);
			if (!(r1 == r2))
			{
				bool flag = false;
				for (int i = 0; i < it && !flag; i++)
					flag = (vis[i] == r2);
				if (!flag)
				{
					dsu.hp[r2.x][r2.y]--;
					vis[it++] = r2;
				}
				if (A[v.x][v.y] == col)
					dsu.fa[r1.x][r1.y] = r2;
			}
		}
	}
	r1 = dsu.Root(u);
	for (int d = 0; d < 4; d++)
	{
		Point v(x + dir[d][0], y + dir[d][1]);
		if (inBoard(v) && A[v.x][v.y] == SPACE)
		{
			bool has_calc = false;
			for (int i = 0; i < 4 && !has_calc; i++)
			{
				Point w(v.x + dir[i][0], v.y + dir[i][1]);
				if (inBoard(w) && !(w == u) && dsu.Root(w) == r1)
					has_calc = true;
			}
			if (!has_calc)
				dsu.hp[r1.x][r1.y]++;
		}
	}
	//std::cerr << "(" << x << "," << y << ")'s hp is " << dsu.hp[r1.x][r1.y] << std::endl;
}
