#pragma once
#include "GameRule.h"

class Node :public GameRule
{
public:
	double value;

	Node();
	Node(Node* fa, Point op);

	void debug();

	double Evaluate(Color my);
};

