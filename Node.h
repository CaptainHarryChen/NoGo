#pragma once
#include "GameRule.h"

class Node :public GameRule
{
	const double Confidence = 2.0;

public:
	int n;
	double value;
	Node* son[9][9], * father;
	bool isLeaf;

	Node();
	Node(Node* fa, Point op);
	~Node();

	double UCB(int N);
	Point FindMax();
	void Expand();
	double Rollout(Color my);
};

