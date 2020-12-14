#pragma once
#include "GameRule.h"

class Node :public GameRule
{
public:
	double value;
	Point bestop;

	Node();
	Node(Node* fa, Point op);

	double Evaluate(Color my);
};

