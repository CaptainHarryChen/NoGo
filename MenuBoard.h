#pragma once
#include "Graphic.h"
#include "GameRule.h"

class MenuBoard
{
	Rect pos;
	GLint img_board;

	int game_state;
	Color move_color;

public:
	MenuBoard(Rect _pos);
	void Init();
	void Draw();
	void OnMouseClick(Point a);
	void SetGameState(int id);
	void SetMoveColor(Color id);

	bool in(Point a);
};

