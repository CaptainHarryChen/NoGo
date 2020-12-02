#include "GameScene.h"
#include <GL/glut.h>

GameScene::GameScene(int width, int height) :scene_width(width), scene_height(height)
{
	pRuler = new GameRule;
	pAI = new GameAI(WHITE);
	pCheckerBoard = new CheckerBoard(Rect(0, 0, 900, 900), pRuler, pAI);
	pMenuBoard = new MenuBoard(Rect(900, 0, 1200, 900));
	game_state = MAIN_MENU;
}

void GameScene::Init()
{
	pCheckerBoard->Init();
	pMenuBoard->Init();
	pAI->Start();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  //定义源因子与目的因子
}

void GameScene::Display()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, scene_width, scene_height, 0);

	pCheckerBoard->Draw();
	pMenuBoard->Draw();

	glutSwapBuffers();
}

void GameScene::Idle()
{
	if (pRuler->moveColor() == WHITE)
	{
		Point a;
		if (pAI->GetMove(a) == true)
		{
			pRuler->setPiece(a.x, a.y, WHITE);
			pCheckerBoard->setPiece(a.x, a.y, WHITE);
			game_state = pRuler->isOver();
		}
	}
	Display();
}

void GameScene::OnMouseMove(int x, int y)
{
	if (game_state == IN_GAME)
	{
		if (pCheckerBoard->in(Point(x, y)))
			pCheckerBoard->SetMousePos(Point(x, y));
		else
			pCheckerBoard->SetMousePos(Point(-1, -1));
	}
	else
		pCheckerBoard->SetMousePos(Point(-1, -1));
}

void GameScene::OnMouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (game_state == IN_GAME)
		{
			if (pCheckerBoard->in(Point(x, y)))
				pCheckerBoard->OnMouseClick(Point(x, y));

			if (pRuler->moveColor() == WHITE)
				pAI->SendMoveMessage();
			
			game_state = pRuler->isOver();
		}
	}
}
