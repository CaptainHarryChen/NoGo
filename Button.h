#pragma once
#include <GL/glut.h>
#include "Graphic.h"

class Button
{
	Rect pos;
	GLint texture[3];
	int state;

public:
	Button(Rect _pos, const char* path0, const char* path1, const char* path2);

	void Draw();
	void OnMouse(Point u);
	bool OnClick(Point u,int mouse_state);
};

