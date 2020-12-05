#include "GameAI.h"
#include <cstdlib>
#include <ctime>

GameAI::GameAI(Color col)
{
	color = col;
	start_time = 0;
	pMain = NULL;
	need_move = false;
	ready_move = false;

	root = NULL;
}

bool GameAI::ProcessMessage(Node *&cur)
{
	for (;;)
	{
		msg_lock.lock();
		if (qmsg.empty())
		{
			msg_lock.unlock();
			break;
		}
		Message message = qmsg.front();
		qmsg.pop();
		msg_lock.unlock();
		if (message == END)
			return false;
		else if (message == MOVE)
		{
			cur = root->son[player_move.x][player_move.y];
			if (cur == NULL)
				cur = new Node(root, player_move);
			root->son[player_move.x][player_move.y] = NULL;
			delete root;
			root = cur;
		}
		else if (message == CALC)
		{
			start_time = clock();
			mv_lock.lock();
			need_move = true;
			ready_move = false;
			mv_lock.unlock();
		}
	}
	return true;
}

void GameAI::Run()
{
	Node* cur = root;
	for (;;)
	{
		if (ProcessMessage(cur) == false)
			break;
		//search
		if (cur->isLeaf)
		{
			if (cur->n == 0)
				cur->Expand();
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
			cur = root->son[ai_move.x][ai_move.y];
			if (cur == NULL)
				cur = new Node(root, ai_move);
			root->son[ai_move.x][ai_move.y] = NULL;
			delete root;
			root = cur;
			need_move = false;
			ready_move = true;
		}
		mv_lock.unlock();
	}
}

void GameAI::SetBeginningState()
{
	root = new Node();
}
void GameAI::SetBeginningState(const Color A[9][9])
{
	root = new Node();
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (A[i][j] != SPACE)
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
	qmsg.push(END);
	msg_lock.unlock();
	pMain->join();
}

void GameAI::SendMoveMessage()
{
	msg_lock.lock();
	qmsg.push(CALC);
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
	qmsg.push(MOVE);
	player_move = p;
	msg_lock.unlock();
}

