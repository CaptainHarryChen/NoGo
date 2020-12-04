#include "Node.h"
#include <cmath>
#include <cstring>
#include <cstdlib>

Node::Node()
{
	n = 0;
	value = 0.0;
	memset(son, 0, sizeof son);
	father = NULL;
}

bool Node::isLeaf()
{
	return n == 0;
}

double Node::UCB(int N)
{
	return value / n + Confidence * sqrt(log(N + 1) / n);
}

double Node::CalcValue(Color col)
{
	if (isOver() == (int)col)
		return 1;
	return 0;
}

Point Node::FindMax()
{
	double mx = -1e100;
		Point res;
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				if (son[i][j] && son[i][j]->UCB(n) > mx)
					mx = son[i][j]->UCB(n), res = Point(i, j);
	return res;
}

void Node::Expand()
{
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (isLegal(i, j, moveColor()))
				son[i][j] = new Node;
}

double Node::Rollout(Color my)
{
	Node u = *this;
	int flag = u.isOver();
	while (flag == 0)
	{
		Color c = u.moveColor();
		int cnt = 0;
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				if (u.isLegal(i, j, c))
					cnt++;
		int x = rand() % cnt;
		for (int i = 0; i < 9 && x; i++)
			for (int j = 0; j < 9 && x; j++)
				if (u.isLegal(i, j, c))
				{
					x--;
					if (x == 0)
						u.setPiece(i, j, c);
				}
		flag = u.isOver();
	}
	return u.CalcValue(my);
}
