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
	else
	{
		c = tmp.moveColor() == Color::WHITE;
		if (cnt[c] == 0)
			return ai_color == tmp.moveColor() ? -1e100 : 1e100;
		if (cnt[c ^ 1] == 0)
			return ai_color == tmp.moveColor() ? 1e100 : -1e100;
	}
	int ret = tmp.Evaluate(ai_color) - beginning_value;
	//if (ret < 0)
	//	return -ret * ret;
	//return ret * ret;
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
			if (up < 0 && tboard->moveColor() == ai_color) //说明父亲结点是对方下棋
				cur->value -= up;
			else if (up > 0 && tboard->moveColor() != ai_color) //说明父亲节点是ai自己下棋
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
						cur->que.push_back(Point(i, j));
					}
			if (cur->que.empty())
				return tboard->moveColor() == ai_color ? -1e100 : 1e100;
		}
	}
	Point t = cur->FindMaxUCB();
	tboard->setPiece(t.x, t.y, tboard->moveColor());
	double up = Search(cur->son[t.x][t.y]);
	cur->n++;
	if (up < 0 && tboard->moveColor() == ai_color) //说明父亲节点是对方下棋
		cur->value -= up;
	else if (up > 0 && tboard->moveColor() != ai_color) //说明父亲节点是ai自己下棋
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
				delete root;
				root = new MCTS_Node();
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

		//处理移动
		if (need_move)
		{
			int search_times = 0;
			beginning_value = tboard->Evaluate(ai_color);
			while /*(search_times<=1600)*/(clock() - start_time <= 50)
			{
				memcpy(tboard, board, sizeof(GameRule));
				Search(root);
				search_times++;
			}
			std::cerr << "Search times:" << search_times << std::endl;
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 9; j++)
					if (root->son[i][j])
						std::cerr << "Operation(" << i << "," << j << ")'s ucb is " << root->son[i][j]->UCB(root->n)
						<< "    value is " << root->son[i][j]->value
						<< "    n is " << root->son[i][j]->n
						<< "    The rating is " << root->son[i][j]->value / root->son[i][j]->n
						<< std::endl;
			ai_move = root->FindMaxRating();
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

