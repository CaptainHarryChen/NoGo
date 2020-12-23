#pragma once
#include "CheckerBoard.h"
#include "MenuBoard.h"
#include "Button.h"
#include "GameRule.h"
#include "GameAI.h"
#include "Graphic.h"

class GameScene
{
	const int scene_width, scene_height;

	int game_state;
	Color col_human, col_ai;
	bool msg_send;

	GameRule* pRuler;
	GameAI* pAI;
	CheckerBoard* pCheckerBoard;
	MenuBoard* pMenuBoard;
	Button* pStartBlack, * pStartWhite;

	void StartGame(Color c);
public:
	GameScene(int width, int height);
	~GameScene();

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
