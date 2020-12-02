#include "MenuBoard.h"

MenuBoard::MenuBoard(Rect _pos)
{
	pos = _pos;
	img_board = 0;
}

void MenuBoard::Init()
{
	img_board = LoadTexture("img//menu//menuboard.bmp");
}

void MenuBoard::Draw()
{
	DrawTexture(pos, img_board);
}

void MenuBoard::OnMouseClick(Point a)
{
}

bool MenuBoard::in(Point a)
{
	return pos.inRect(a);
}
