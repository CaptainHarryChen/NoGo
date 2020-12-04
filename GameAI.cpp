#include "GameAI.h"
#include <cstdlib>
#include <ctime>

GameAI::GameAI(Color col)
{
	color = col;
	message = NONE;
	start_time = 0;
	pMain = NULL;
	need_move = false;
	ready_move = false;

	root = NULL;
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
	}
	else if (message == CALC)
	{
		message = NONE;
		msg_lock.unlock();
		start_time = clock();
		mv_lock.lock();
		need_move = true;
		ready_move = false;
		mv_lock.unlock();
	}
	else
		msg_lock.unlock();
	return true;
}

void GameAI::Run()
{
	Node* cur = root;
	for (;;)
	{
		if (ProcessMessage() == false)
			break;
		//search
		if (cur->isLeaf())
		{
			double tmp = cur->Rollout(color);
			cur->n++;
			cur->value += tmp;
			while (cur != root)
			{
				cur = cur->father;
				cur->n++;
				cur->value += tmp;
			}
		}
		else
		{
			Point t = cur->FindMax();
			cur = cur->son[t.x][t.y];
		}

		mv_lock.lock();
		if (need_move == true && clock() - start_time >= 900)
		{
			ai_move = root->FindMax();
			need_move = false;
			ready_move = true;
		}
		mv_lock.unlock();
	}
}

void GameAI::SetBeginningState(const Color A[9][9])
{
	root = new Node();
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			root->setPiece(i, j, A[i][j]);
}

void GameAI::Start()
{
	srand((unsigned int)time(0));
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

bool GameAI::GetMove(Point& res)
{
	mv_lock.lock();
	if (!ready_move)
	{
		mv_lock.unlock();
		return false;
	}
	res = ai_move;
	ready_move = false;
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

