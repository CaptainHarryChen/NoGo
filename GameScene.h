#pragma once
#include "CheckerBoard.h"
#include "GameRule.h"
#include "GameAI.h"

class GameScene
{
	const int scene_width, scene_height;
	
	int game_state;
	GameRule *pRuler;
	GameAI* pAI;
	CheckerBoard *pCheckerBoard;

public:
	GameScene(int width, int height);

	void Init();
	void Display();
	void Idle();
	//void Reshape(int new_w, int new_h);
	//void Timer(int id);
	//void OnKeyBoard(unsigned char key, int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseClick(int button, int state, int x, int y);
	//void OnSpecialKey(int key, int x, int y);
};
