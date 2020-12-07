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
	//double val[9][9];
	Point mv;
	int flag = u.isOver(), step = MAX_ROLLOUT_STEP;
	while (flag == 0 && step)
	{
		step--;
		Color c = u.moveColor();
		//double tot = 0;
		double mx = 0;
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
			{
				//val[i][j] = 0;
				if (u.isLegal(i, j, c))
				{
					GameRule v = u;
					v.setPiece(i, j, c);
					double t = v.Evaluate(c);
					if (t > mx)
						mx = t, mv = Point(i, j);
					//val[i][j] = v.Evaluate(c);
					//tot += val[i][j];
				}
			}
		u.setPiece(mv.x, mv.y, c);
		/*double x = (1LL * rand() * rand() % 1000000) / 1000000.0;
		for (int i = 0; i < 9 && x >= 1e-10; i++)
			for (int j = 0; j < 9 && x >= 1e-10; j++)
			{
				x -= val[i][j] / tot;
				if (x < 1e10)
					u.setPiece(i, j, c);
			}*/
		flag = u.isOver();
	}
	return u.Evaluate(my);
}
