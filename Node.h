#pragma once
#include "GameRule.h"

class Node :public GameRule
{
public:
	int n;
	double value;
	Node* son[9][9], * father;
	Point last_op;

	Node();
	Node(Node* fa, Point op);

	void Release();

	void NextOp();
	bool Finish();
	Point FindMaxValue();
	void UpdateValue(double val, bool mode);
	double Evaluate(Color my);
};

