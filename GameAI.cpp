#include "GameAI.h"

GameAI::GameAI(GameRule* p, int col)
{
	pRuler = p;
	color = col;
	memset(A, 0, sizeof A);
	message = NONE;
	pMain = NULL;
	need_move = false;
}

bool GameAI::ProcessMessage()
{
	msg_lock.lock();
	if (message == END)
	{
		msg_lock.unlock();
		return false;
	}
	else if (message == MOVE)
	{
		message = NONE;
		msg_lock.unlock();
		A[player_move.x][player_move.y] = (color == 1 ? 2 : 1);
	}
	else if (message == CALC)
	{
		message = NONE;
		msg_lock.unlock();
		start_time = clock();
		need_move = true;
		mv_lock.lock();
	}
	else
		msg_lock.unlock();
	return true;
}

void GameAI::Run()
{
	for (;;)
	{
		if (ProcessMessage() == false)
			break;
		//search
		if (need_move == true)
		{
			bool flag = false;
			for (int i = 0; i < 9 && !flag; i++)
				for (int j = 0; j < 9 && !flag; j++)
					if (pRuler->isLegal(i, j, color))
						ai_move = Point(i, j), flag = true;
			need_move = false;
			mv_lock.unlock();
		}
	}
}

void GameAI::Start()
{
	pMain = new std::thread(&GameAI::Run, this);
}

void GameAI::End()
{
	msg_lock.lock();
	message = END;
	msg_lock.unlock();
	pMain->join();
}

void GameAI::SendMoveMessage()
{
	msg_lock.lock();
	message = CALC;
	msg_lock.unlock();
}

bool GameAI::GetMove(Point &res)
{
	if (!mv_lock.try_lock())
		return false;
	res = ai_move;
	mv_lock.unlock();
	return true;
}

void GameAI::PlayerMove(Point p)
{
	msg_lock.lock();
	message = MOVE;
	msg_lock.unlock();
	player_move = p;
}

