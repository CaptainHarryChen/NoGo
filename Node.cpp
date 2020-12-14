#include "Node.h"
#include <cmath>
#include <cstring>
#include <cstdlib>

Node::Node()
{
	value = -1;
}

Node::Node(Node* fa, Point op)
{
	value = 0.0;
	memcpy(A, fa->A, sizeof A);
	step = fa->step;
	setPiece(op.x, op.y, moveColor());
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

void Node::debug()
{
	std::cerr << "Debug:" << this << std::endl;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			std::cerr << (int)A[i][j];
		std::cerr << std::endl;
	}
}
