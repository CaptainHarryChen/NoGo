#pragma once
#include "GameRule.h"

class MinMaxNode :public GameRule
{
public:
	double value;

	MinMaxNode();
	MinMaxNode(MinMaxNode* fa, Point op);

	void debug();

	double Evaluate(Color my);
};

