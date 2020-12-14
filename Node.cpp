#include "Node.h"
#include <cmath>
#include <cstring>
#include <cstdlib>

Node::Node()
{
	n = 0;
	value = -1;
	last_op = Point(-1, 9);
	memset(son, 0, sizeof son);
	father = NULL;
}

Node::Node(Node* fa, Point op)
{
	n = 0;
	value = -1;
	last_op = Point(-1, 9);
	memset(son, 0, sizeof son);
	father = fa;
	memcpy(A, fa->A, sizeof A);
	memcpy(&dsu, &fa->dsu, sizeof dsu);
	step = fa->step;
	setPiece(op.x, op.y, moveColor());
}

void Node::Release()
{
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (son[i][j])
			{
				son[i][j]->Release();
				delete son[i][j];
			}
}

void Node::NextOp()
{
	do
	{
		last_op.y++;
		if (last_op.y > 8)
			last_op.x++, last_op.y = 0;
		if (last_op.x < 9 && isLegal(last_op.x, last_op.y, moveColor()))
			return;
	} while (last_op.x < 9);
}

bool Node::Finish()
{
	return last_op.x > 8;
}

Point Node::FindMaxValue()
{
	double mx = -1e100;
	Point res;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (son[i][j] && son[i][j]->value > mx)
				mx = son[i][j]->value, res = Point(i, j);
	return res;
}

void Node::UpdateValue(double val, bool mode)
{
	if (value == -1)
		value = val;
	else
		value = mode ? std::max(value, val) : std::min(value, val);
}

double Node::Evaluate(Color my)
{
	double ret = 0;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (A[i][j] == SPACE)
			{
				bool f1 = isLegal(i, j, my), f2 = isLegal(i, j, my == WHITE ? BLACK : WHITE);
				if (f1 && !f2)
					ret++;
				if (!f1 && f2)
					ret--;
			}
	return ret;
}
