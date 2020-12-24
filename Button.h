#pragma once
#include <GL/glut.h>
#include "Graphic.h"

class Button
{
	Rect pos;
	GLint texture[3];
	Text text;
	Point text_pos;
	int state;

public:
	Button(Rect _pos, const Text &_text);
	
	void SetText(const Text& _text);
	void Draw();
	void OnMouse(Point u);
	bool OnClick(Point u,int mouse_state);
};

