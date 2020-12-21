#include "GameScene.h"
#include <GL/glut.h>

#include "GameAI_Minmax.h"

void GameScene::StartGame(Color c)
{
	col_human = c;
	col_ai = c == Color::BLACK ? Color::WHITE : Color::BLACK;

	if (pRuler != nullptr)
		delete pRuler;
	pRuler = new GameRule;

	if (pAI != nullptr)
		delete pAI;
	pAI = new GameAI_Minmax(col_ai);

	pCheckerBoard->Init(c, pRuler, pAI);
	pAI->SetBeginningState();
	pAI->Start();
	game_state = IN_GAME;
}

GameScene::GameScene(int width, int height) :scene_width(width), scene_height(height)
{
	pAI = nullptr;
	pRuler = nullptr;

	pCheckerBoard = new CheckerBoard(Rect(0, 0, 900, 900));
	pMenuBoard = new MenuBoard(Rect(900, 0, 1200, 900));
	pStartBlack = new Button(Rect(950, 425, 1150, 475), "img//button//blackstart1.bmp", "img//button//blackstart2.bmp", "img//button/blackstart3.bmp");

	game_state = MAIN_MENU;
	col_human = Color::BLACK;
	col_ai = Color::WHITE;
}

GameScene::~GameScene()
{
	if (this->pAI != nullptr)
		delete this->pAI;
	if (this->pRuler != nullptr)
		delete this->pRuler;

	delete this->pCheckerBoard;
	delete this->pMenuBoard;
	delete this->pStartBlack;
}

void GameScene::Init()
{
	//pCheckerBoard->Init();
	pMenuBoard->Init();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
}

void GameScene::Display()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, scene_width, scene_height, 0);

	pCheckerBoard->Draw();
	pMenuBoard->Draw();
	pStartBlack->Draw();

	glutSwapBuffers();
}

void GameScene::Idle()
{
	if (game_state == IN_GAME)
	{
		if (pRuler->moveColor() == col_ai)
		{
			Point a;
			if (pAI->GetMove(a) == true)
			{
				pRuler->setPiece(a.x, a.y, col_ai);
				pCheckerBoard->setPiece(a.x, a.y, col_ai);
				int tmp = pRuler->isOver();
				if (tmp)
					game_state = tmp;
			}
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
	{
		pCheckerBoard->SetMousePos(Point(-1, -1));
		pStartBlack->OnMouse(Point(x, y));
	}
}

void GameScene::OnMouseClick(int button, int state, int x, int y)
{
	if(game_state==IN_GAME)
	{
		if (button == GLUT_LEFT_BUTTON)
			if(state == GLUT_UP)
			{
				if (pCheckerBoard->in(Point(x, y)))
					pCheckerBoard->OnMouseClick(Point(x, y));

				if (pRuler->moveColor() == col_ai)
					pAI->SendGameMessage();
			
				int tmp = pRuler->isOver();
				if (tmp)
					game_state = tmp;
			}
	}
	else
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
				pStartBlack->OnClick(Point(x, y), state);
			else if (state == GLUT_UP)
			{
				if (pStartBlack->OnClick(Point(x, y), state))
					StartGame(Color::BLACK);
			}
		}
	}
}
