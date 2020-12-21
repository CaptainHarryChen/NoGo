#include "GameAI_MCTS.h"
#include "GameAI_Minmax.h"

GameAI_MCTS::GameAI_MCTS(Color col) : GameAI(col)
{
	start_time = 0;
	pMain = nullptr;
	need_move = false;
	ready_move = false;

	root = nullptr;
	board = nullptr;
	tboard = nullptr;
}

GameAI_MCTS::~GameAI_MCTS()
{
	this->End();
	if (pMain != nullptr)
		delete pMain;
}

void GameAI_MCTS::Move(Point mv)
{
	board->setPiece(mv.x, mv.y, board->moveColor());
	MCTS_Node* tmp = root->son[mv.x][mv.y];
	if (tmp == nullptr)
		tmp = new MCTS_Node();
	root->son[mv.x][mv.y] = nullptr;
	delete root;
	root = tmp;
}

double GameAI_MCTS::Rollout()
{
	GameRule tmp = *tboard;
	std::vector<Point> op[2];
	unsigned int cnt[2], c;

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			if (tmp.isLegal(i, j, Color::BLACK))
				op[0].push_back(Point(i, j));
			if (tmp.isLegal(i, j, Color::WHITE))
				op[1].push_back(Point(i, j));
		}
	cnt[0] = (unsigned int)op[0].size();
	cnt[1] = (unsigned int)op[1].size();

	if (cnt[0] && cnt[1])
	{
		for (int i = 0; i < RolloutStep; i++)
		{
			c = tmp.moveColor() == Color::WHITE;
			int id = rand() % op[c].size();
			while (cnt[c] && (op[c][id].x == -1 || !tmp.isLegal(op[c][id].x, op[c][id].y, tmp.moveColor())))
			{
				cnt[c]--;
				op[c][id].x = -1;
				id = rand() % op[c].size();
			}
			if (cnt[c] == 0)
				break;
			tmp.setPiece(op[c][id].x, op[c][id].y, tmp.moveColor());
			op[c][id].x = -1;
			cnt[c]--;
		}
	}
	c = tmp.moveColor() == Color::WHITE;
	if (cnt[c] == 0)
		return ai_color == tmp.moveColor() ? -1e100 : 1e100;
	if (cnt[c ^ 1] == 0)
		return ai_color == tmp.moveColor() ? 1e100 : -1e100;
	int ret = 0;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			if (tmp.isLegal(i, j, ai_color))
				ret++;
			if (tmp.isLegal(i, j, ai_color == Color::BLACK ? Color::WHITE : Color::BLACK))
				ret--;
		}
	return ret;
}

double GameAI_MCTS::Search(MCTS_Node* cur)
{
	if (cur->que.empty())
	{
		if (cur->n < LeastVisitTime)
		{
			double up = Rollout();
			cur->n++;
			if (tboard->moveColor() != ai_color)
				cur->value -= up;
			else
				cur->value += up;
			return up;
		}
		else
		{
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 9; j++)
					if (tboard->isLegal(i, j, tboard->moveColor()))
					{
						cur->son[i][j] = new MCTS_Node();
						cur->que.push(cmp(Point(i, j), 1e100));
					}
			if (cur->que.empty())
				return tboard->moveColor() == ai_color ? -1e100 : 1e100;
		}
	}
	cmp t = cur->que.top();
	cur->que.pop();
	tboard->setPiece(t.op.x, t.op.y, tboard->moveColor());
	double up = Search(cur->son[t.op.x][t.op.y]);
	t.ucb = cur->son[t.op.x][t.op.y]->UCB();
	cur->que.push(t);
	cur->n++;
	if (tboard->moveColor() != ai_color)
		cur->value -= up;
	else
		cur->value += up;
	return up;
}

void GameAI_MCTS::Run()
{
	bool bQuit = false;
	while (!bQuit)
	{
		//Process Message
		for (;;)
		{
			Message message;

			if (!this->qmsg.try_pop(message))
				break;

			if (message == Message::END)
			{
				bQuit = true;
				break;
			}
			else if (message == Message::MOVE)
			{
				Move(player_move);
			}
			else if (message == Message::CALC)
			{
				start_time = clock();
				std::lock_guard<std::mutex> lg(this->mv_lock);
				need_move = true;
				ready_move = false;
			}
		}

		if (bQuit)
			break;

		//¥¶¿Ì“∆∂Ø
		if (need_move)
		{
			while (clock() - start_time <= 950)
			{
				memcpy(tboard, board, sizeof(GameRule));
				Search(root);
			}
			ai_move = root->que.top().op;
			Move(ai_move);

			std::lock_guard <std::mutex> lg(this->mv_lock);
			need_move = false;
			ready_move = true;
		}
		else
			std::this_thread::yield();
	}
}

void GameAI_MCTS::SetBeginningState()
{
	if (root != nullptr)
		delete root;
	root = new MCTS_Node();
	if (board != nullptr)
		delete board;
	board = new GameRule();
	if (tboard != nullptr)
		delete tboard;
	tboard = new GameRule();
}
void GameAI_MCTS::SetBeginningState(const Color A[9][9])
{
	SetBeginningState();
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			board->A[i][j] = A[i][j];
	board->Restucture();
}

void GameAI_MCTS::Start()
{
	srand((unsigned int)time(0));
	pMain = new std::thread(&GameAI_MCTS::Run, this);
}

void GameAI_MCTS::End()
{
	this->SendGameMessage(Message::END);
	pMain->join();
}

void GameAI_MCTS::SendGameMessage(Message msg)
{
	qmsg.push(msg);
}

bool GameAI_MCTS::GetMove(Point& res)
{
	if (!ready_move)
		return false;
	std::lock_guard<std::mutex> lg(this->mv_lock);
	res = ai_move;
	ready_move = false;
	return true;
}

void GameAI_MCTS::PlayerMove(Point p)
{
	this->SendGameMessage(Message::MOVE);
	player_move = p;
}

