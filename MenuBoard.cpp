#include "MenuBoard.h"

MenuBoard::MenuBoard(Rect _pos)
{
	pos = _pos;
	img_board = 0;
	img_board = LoadTexture("img//menu//menuboard.bmp");
	game_state = MAIN_MENU;
	move_color = Color::BLACK;
}

void MenuBoard::Init()
{
	
}

void MenuBoard::Draw()
{
	DrawTexture(pos, img_board);
	Point tmp = Point(pos.lx, pos.ly) + Point(65, 100);
	DrawString("²»Î§Æå", "¿¬Ìå", 48, tmp.x, tmp.y, 142.0 / 256, 70.0 / 256, 49.0 / 256);

	tmp = Point(pos.lx, pos.ly) + Point(20, 370);
	if (game_state == GAME_BLACK_WIN)
		DrawString("ºÚÆå»ñÊ¤£¡", "¿¬Ìå", 48, tmp.x, tmp.y, 0, 0, 0);
	if (game_state == GAME_WHITE_WIN)
		DrawString("°×Æå»ñÊ¤£¡", "¿¬Ìå", 48, tmp.x, tmp.y, 1, 1, 1);

	tmp = Point(pos.lx, pos.ly) + Point(35, 310);
	if (game_state == IN_GAME)
	{
		if (move_color == Color::BLACK)
			DrawString("µÈ´ýºÚÆå", "¿¬Ìå", 48, tmp.x, tmp.y, 0, 0, 0);
		else
			DrawString("µÈ´ý°×Æå", "¿¬Ìå", 48, tmp.x, tmp.y, 1, 1, 1);
	}
}

void MenuBoard::OnMouseClick(Point a)
{
}

void MenuBoard::SetGameState(int id)
{
	game_state = id;
}

void MenuBoard::SetMoveColor(Color id)
{
	move_color = id;
}

bool MenuBoard::in(Point a)
{
	return pos.inRect(a);
}
