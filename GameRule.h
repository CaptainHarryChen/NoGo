#pragma once

class GameRule
{
	unsigned char A[9][9];
	int step;

public:
	GameRule();
	bool isLegal(int x, int y);
};

