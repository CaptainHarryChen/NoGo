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
	Point mouse_pos;
	bool mouse_legal;
	Color col_human, col_ai;

public:
	CheckerBoard(Rect _pos);
	void Init(Color c_human, GameRule* p, GameAI* pai);
	void Draw();
	void SetMousePos(Point u);
	void OnMouseClick(Point a);

	bool in(Point a);
	void setPiece(int x, int y, Color col);
};
