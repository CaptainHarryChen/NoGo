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
	isLeaf = true;
}

Node::Node(Node* fa, Point op)
{
	n = 0;
	value = 0.0;
	memset(son, 0, sizeof son);
	father = fa;
	isLeaf = true;
	memcpy(A, fa->A, sizeof A);
	memcpy(&dsu, &fa->dsu, sizeof dsu);
	step = fa->step;
	setPiece(op.x, op.y, moveColor());
}

Node::~Node()
{
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (son[i][j])
				delete son[i][j];
}

double Node::UCB(int N)
{
	if (n == 0)
		return 1e100;
	return value / n + Confidence * sqrt(log(N + 1) / n);
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
			{
				son[i][j] = new Node(this, Point(i, j));
				isLeaf = false;
			}
}

double Node::Rollout(Color my)
{
	GameRule u = *this;
	int flag = u.isOver();
	while (flag == 0)
	{
		Color c = u.moveColor();
		int cnt = 0;
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				if (u.isLegal(i, j, c))
					cnt++;
		int x = rand() % cnt + 1;
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
	return flag == my;
}
