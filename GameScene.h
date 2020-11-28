#pragma once
#include "CheckerBoard.h"

class GameScene
{
	const int scene_width = 1200, scene_height = 900;
	
	CheckerBoard checkerBoard;

public:
	GameScene();

	void Init();
	void Display();
	//void Idle();
	//void Reshape(int new_w, int new_h);
	//void Timer(int id);
	//void OnKeyBoard(unsigned char key, int x, int y);
	//void OnMouse(int button, int state, int x, int y);
	//void OnSpecialKey(int key, int x, int y);
};
