#include "Button.h"
//#include <iostream>

Button::Button(Rect _pos, const Text& _text, Point _text_pos) :text(_text)
{
	pos = _pos;
	texture[0] = LoadTexture("img//button//button1.bmp");
	texture[1] = LoadTexture("img//button//button2.bmp");
	texture[2] = LoadTexture("img//button//button3.bmp");
	text_pos = Point(pos.lx, pos.ly) + _text_pos;
	state = 0;
}

void Button::SetText(const Text& _text)
{
	text = _text;
}

void Button::Draw()
{
	DrawTexture(pos, texture[state]);
	DrawString(text, text_pos + (state == 2 ? Point(5, 5) : Point()));
}

void Button::OnMouse(Point u)
{
	//std::cerr << "(" << u.x << "," << u.y << ")" << std::endl;
	if (state != 2)
	{
		if (pos.inRect(u))
			state = 1;
		else
			state = 0;
	}
}

bool Button::OnClick(Point u, int mouse_state)
{
	if (pos.inRect(u) && mouse_state == GLUT_DOWN)
		state = 2;
	if (mouse_state == GLUT_UP)
		state = 0;
	return pos.inRect(u);
}

