#pragma once
#include <GL/glut.h>
#include "functions.h"

class CheckerBoard
{
	Rect pos;
	GLint img_board[10], img_black, img_white;

public:
	CheckerBoard(Rect _pos);
	void Init();

	void Draw();
};
