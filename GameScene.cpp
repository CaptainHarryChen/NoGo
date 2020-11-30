#include "GameScene.h"
#include <GL/glut.h>

GameScene::GameScene(int width, int height) :scene_width(width), scene_height(height)
{
	pRuler = new GameRule;
	pAI = new GameAI(WHITE);
	pCheckerBoard = new CheckerBoard(Rect(0, 0, 900, 900), pRuler, pAI);
}

void GameScene::Init()
{
	pCheckerBoard->Init();
	pAI->Start();
}

void GameScene::Display()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, scene_width, scene_height, 0);

	pCheckerBoard->Draw();

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
		}
	}
	Display();
}

void GameScene::OnMouseMove(int x, int y)
{
}

void GameScene::OnMouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (pCheckerBoard->in(Point(x, y)))
			pCheckerBoard->OnMouseClick(Point(x, y));

		if (pRuler->moveColor() == WHITE)
			pAI->SendMoveMessage();
	}
}
