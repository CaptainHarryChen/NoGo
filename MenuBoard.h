#pragma once
#include "Graphic.h"

class MenuBoard
{
	Rect pos;
	GLint img_board;

	int game_state;

public:
	MenuBoard(Rect _pos);
	void Init();
	void Draw();
	void OnMouseClick(Point a);
	void SetGameState(int id);

	bool in(Point a);
};

