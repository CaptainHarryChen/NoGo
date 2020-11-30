#pragma once
#include <GL/glut.h>
#include "Graphic.h"
#include "GameRule.h"
#include "GameAI.h"

class CheckerBoard
{
	Rect pos;
	GLint img_board[10], img_black, img_white;
	GameRule* pRuler;
	GameAI* pAI;
	Color board[9][9];

public:
	CheckerBoard(Rect _pos, GameRule *p, GameAI *pai);
	void Init();
	void Draw();
	void OnMouseClick(Point a);

	bool in(Point a);
	void setPiece(int x, int y, Color col);
};
