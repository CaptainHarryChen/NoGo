#include "Button.h"
//#include <iostream>

Button::Button(Rect _pos, const char* path0, const char* path1, const char* path2)
{
	pos = _pos;
	texture[0] = LoadTexture(path0);
	texture[1] = LoadTexture(path1);
	texture[2] = LoadTexture(path2);
	state = 0;
}

void Button::Draw()
{
	DrawTexture(pos, texture[state]);
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

bool Button::OnClick(Point u,int mouse_state)
{
	if (pos.inRect(u) && mouse_state == GLUT_DOWN)
		state = 2;
	if (mouse_state == GLUT_UP)
		state = 0;
	return pos.inRect(u);
}

