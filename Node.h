#pragma once
#include "GameRule.h"

class Node :public GameRule
{
	const double Confidence = 2.0;

public:
	int n;
	double value;
	Node* son[9][9], * father;

	Node();

	bool isLeaf();
	double UCB(int N);
	double CalcValue(Color col);
	Point FindMax();
	void Expand();
	double Rollout(Color my);
};

