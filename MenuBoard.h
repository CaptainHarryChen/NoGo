#pragma once
#include "Graphic.h"

class MenuBoard
{
	Rect pos;
	GLint img_board;

public:
	MenuBoard(Rect _pos);
	void Init();
	void Draw();
	void OnMouseClick(Point a);

	bool in(Point a);
};

