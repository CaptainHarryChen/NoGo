#include "MinMaxNode.h"
#include <cmath>
#include <cstring>
#include <cstdlib>

MinMaxNode::MinMaxNode()
{
	value = 0.0;
}

MinMaxNode::MinMaxNode(MinMaxNode* fa, Point op)
{
	value = 0.0;
	memcpy(A, fa->A, sizeof A);
	memcpy(&dsu, &fa->dsu, sizeof dsu);
	step = fa->step;
	setPiece(op.x, op.y, moveColor());
}

double MinMaxNode::Evaluate(Color my)
{
	double ret = 0;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (A[i][j] == Color::SPACE)
			{
				bool f1 = isLegal(i, j, my), f2 = isLegal(i, j, my == Color::WHITE ? Color::BLACK : Color::WHITE);
				if (f1 && !f2)
					ret++;
				if (!f1 && f2)
					ret-=10;
			}
	return ret;
}

void MinMaxNode::debug()
{
	std::cerr << "Debug:" << this << std::endl;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			std::cerr << (int)A[i][j];
		std::cerr << std::endl;
	}
}
