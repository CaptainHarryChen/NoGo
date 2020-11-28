#pragma once
#include <GL/glut.h>
#include "Graphic.h"
#include "GameRule.h"

class CheckerBoard
{
	Rect pos;
	GLint img_board[10], img_black, img_white;
	GameRule* pRuler;

public:
	CheckerBoard(Rect _pos, GameRule *p);
	void Init();

	void Draw();
};
