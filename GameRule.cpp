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
	memset(B, -1, sizeof B);
	step = 0;
}


Color GameRule::moveColor()
{
	return step % 2 == 0 ? Color::BLACK : Color::WHITE;
}

bool GameRule::isLegal(int x, int y, Color col)
{
	int ff = col == Color::BLACK ? 0 : 1;
	if (B[ff][x][y] != -1)
		return B[ff][x][y];
	//std::cerr << "Check move (" << x << "," << y << "," << col << ")" << std::endl;
	B[ff][x][y] = 0;
	if (step == 0 && x == 4 && y == 4)
		return false;
	if (A[x][y] != Color::SPACE)
		return false;

	bool has_hp = false, dead = false;
	for (int d = 0; d < 4; d++)
	{
		Point v(x + dir[d][0], y + dir[d][1]);
		if (inBoard(v))
		{
			if (A[v.x][v.y] == Color::SPACE)
				has_hp = true;
			else
			{
				v = dsu.Root(v);
				dsu.hp[v.x][v.y]--;
			}
		}
	}
	int sum = 0;
	for (int d = 0; d < 4; d++)
	{
		Point v(x + dir[d][0], y + dir[d][1]);
		if (inBoard(v) && A[v.x][v.y] != Color::SPACE)
		{
			if (A[v.x][v.y] != col)
			{
				v = dsu.Root(v);
				if (dsu.hp[v.x][v.y] <= 0)
					dead = true;
			}
			if (A[v.x][v.y] == col)
			{
				v = dsu.Root(v);
				sum += dsu.hp[v.x][v.y];
			}
			dsu.hp[v.x][v.y]++;
		}
	}

	if (dead)
		return false;
	if (!has_hp && sum <= 0)
		return false;

	//std::cerr << "is legal move" << std::endl;
	B[ff][x][y] = 1;
	return true;
}

int GameRule::isOver()
{
	int cnt1 = 0, cnt2 = 0;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			int t = 0;
			if (isLegal(i, j, Color::BLACK))
				t |= 1, cnt1++;
			if (isLegal(i, j, Color::WHITE))
				t |= 2, cnt2++;
		}
	if (moveColor() == Color::BLACK && cnt1 == 0)
		return GAME_WHITE_WIN;
	if (moveColor() == Color::WHITE && cnt2 == 0)
		return GAME_BLACK_WIN;
	return 0;
}

void GameRule::setPiece(int x, int y, Color col)
{
	step++;
	A[x][y] = col;
	dsu.hp[x][y] = 0;
	Point u(x, y), r1, r2;
	int cnt = 0;
	for (int d = 0; d < 4; d++)
	{
		Point v(x + dir[d][0], y + dir[d][1]);
		if (inBoard(v))
		{
			if (A[v.x][v.y] != Color::SPACE)
			{
				r1 = dsu.Root(u);
				r2 = dsu.Root(v);
				dsu.hp[r2.x][r2.y]--;
				if (A[v.x][v.y] == col && !(r1 == r2))
				{
					dsu.fa[r1.x][r1.y] = r2;
					dsu.hp[r2.x][r2.y] += dsu.hp[r1.x][r1.y];
				}
			}
			else
				cnt++;
		}
	}
	r1 = dsu.Root(u);
	dsu.hp[r1.x][r1.y] += cnt;
	memset(B, -1, sizeof B);
	//std::cerr << "(" << x << "," << y << ")'s hp is " << dsu.hp[r1.x][r1.y] << std::endl;
}

void GameRule::Restucture()
{
	memset(B, -1, sizeof B);
	dsu.Reset();
	step = 0;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (A[i][j] != Color::SPACE)
			{
				step++;
				for (int d = 0; d < 4; d++)
				{
					int x = i + dir[d][0], y = j + dir[d][1];
					if (inBoard(Point(x, y)))
					{
						if (A[x][y] == Color::SPACE)
						{
							Point v = dsu.Root(Point(i, j));
							dsu.hp[v.x][v.y]++;
						}
						else if (A[x][y] == A[i][j])
						{
							dsu.Merge(Point(x, y), Point(i, j));
						}
					}
				}
			}
}
