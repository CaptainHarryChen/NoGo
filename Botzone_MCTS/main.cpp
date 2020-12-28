#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <iostream>
#include <string>
#include "jsoncpp/json.h"

//************************************************************Graphic.h

#define MAIN_MENU 1
#define IN_GAME 2
#define GAME_WHITE_WIN 3
#define GAME_BLACK_WIN 4

const int CircleSegment = 50;

struct Point
{
	int x, y;
	Point() { x = y = 0; }
	Point(int a, int b) :x(a), y(b) {}
	bool operator == (const Point& t)const
	{
		return x == t.x && y == t.y;
	}
	Point operator + (const Point& t)const
	{
		return Point(x + t.x, y + t.y);
	}
};

struct Rect
{
	int lx, ly, rx, ry;
	Rect() { lx = ly = rx = ry = 0; }
	Rect(int a, int b, int c, int d) :lx(a), ly(b), rx(c), ry(d) {}
	bool inRect(Point a)
	{
		if (lx <= a.x && a.x <= rx && ly <= a.y && a.y <= ry)
			return true;
		return false;
	}
};

//************************************************************GameRule.h

enum class Color
{
	SPACE, BLACK, WHITE
};

struct DSU
{
	Point fa[9][9];
	int hp[9][9];
	DSU() { memset(fa, -1, sizeof fa); }
	void Reset()
	{
		memset(fa, -1, sizeof fa);
		memset(hp, 0, sizeof hp);
	}
	Point Root(Point u)
	{
		return fa[u.x][u.y] == Point(-1, -1) ? u : (fa[u.x][u.y] = Root(fa[u.x][u.y]));
	}
	void Merge(Point u, Point v)
	{
		Point r1 = Root(u), r2 = Root(v);
		if (r1 == r2)
			return;
		fa[r1.x][r1.y] = r2;
		hp[r2.x][r2.y] += hp[r1.x][r1.y];
	}
};

class GameRule
{
	const int dir[4][2] = { {-1,0},{0,1},{1,0},{0,-1} };

public:
	Color A[9][9];
	int B[2][9][9];
	int step;

	DSU dsu;

	bool inBoard(Point u);

public:
	GameRule();
	Color moveColor();
	bool isLegal(int x, int y, Color col);
	int isOver();
	void setPiece(int x, int y, Color col);
	void Restucture();
	int Evaluate(Color col);
};

bool GameRule::inBoard(Point u)
{
	return Rect(0, 0, 8, 8).inRect(u);
}

GameRule::GameRule()
{
	memset(A, 0, sizeof A);
	memset(B, -1, sizeof B);
	step = 0;
}


Color GameRule::moveColor()
{
	return step % 2 == 0 ? Color::BLACK : Color::WHITE;
}

bool GameRule::isLegal(int x, int y, Color col)
{
	int ff = col == Color::BLACK ? 0 : 1;
	if (B[ff][x][y] != -1)
		return B[ff][x][y];
	//std::cerr << "Check move (" << x << "," << y << "," << col << ")" << std::endl;
	B[ff][x][y] = 0;
	if (step == 0 && x == 4 && y == 4)
		return false;
	if (A[x][y] != Color::SPACE)
		return false;

	bool has_hp = false, dead = false;
	for (int d = 0; d < 4; d++)
	{
		Point v(x + dir[d][0], y + dir[d][1]);
		if (inBoard(v))
		{
			if (A[v.x][v.y] == Color::SPACE)
				has_hp = true;
			else
			{
				v = dsu.Root(v);
				dsu.hp[v.x][v.y]--;
			}
		}
	}
	int sum = 0;
	for (int d = 0; d < 4; d++)
	{
		Point v(x + dir[d][0], y + dir[d][1]);
		if (inBoard(v) && A[v.x][v.y] != Color::SPACE)
		{
			if (A[v.x][v.y] != col)
			{
				v = dsu.Root(v);
				if (dsu.hp[v.x][v.y] <= 0)
					dead = true;
			}
			if (A[v.x][v.y] == col)
			{
				v = dsu.Root(v);
				sum += dsu.hp[v.x][v.y];
			}
		}
	}
	for (int d = 0; d < 4; d++)
	{
		Point v(x + dir[d][0], y + dir[d][1]);
		if (inBoard(v) && A[v.x][v.y] != Color::SPACE)
		{
			v = dsu.Root(v);
			dsu.hp[v.x][v.y]++;
		}
	}
	if (dead)
		return false;
	if (!has_hp && sum <= 0)
		return false;

	//std::cerr << "is legal move" << std::endl;
	B[ff][x][y] = 1;
	return true;
}

int GameRule::isOver()
{
	int cnt1 = 0, cnt2 = 0;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			int t = 0;
			if (isLegal(i, j, Color::BLACK))
				t |= 1, cnt1++;
			if (isLegal(i, j, Color::WHITE))
				t |= 2, cnt2++;
		}
	if (moveColor() == Color::BLACK && cnt1 == 0)
		return GAME_WHITE_WIN;
	if (moveColor() == Color::WHITE && cnt2 == 0)
		return GAME_BLACK_WIN;
	return 0;
}

void GameRule::setPiece(int x, int y, Color col)
{
	step++;
	A[x][y] = col;
	dsu.hp[x][y] = 0;
	Point u(x, y), r1, r2;
	int cnt = 0;
	for (int d = 0; d < 4; d++)
	{
		Point v(x + dir[d][0], y + dir[d][1]);
		if (inBoard(v))
		{
			if (A[v.x][v.y] != Color::SPACE)
			{
				r1 = dsu.Root(u);
				r2 = dsu.Root(v);
				dsu.hp[r2.x][r2.y]--;
				if (A[v.x][v.y] == col && !(r1 == r2))
				{
					dsu.fa[r1.x][r1.y] = r2;
					dsu.hp[r2.x][r2.y] += dsu.hp[r1.x][r1.y];
				}
			}
			else
				cnt++;
		}
	}
	r1 = dsu.Root(u);
	dsu.hp[r1.x][r1.y] += cnt;
	memset(B, -1, sizeof B);
	//std::cerr << "(" << x << "," << y << ")'s hp is " << dsu.hp[r1.x][r1.y] << std::endl;
}

void GameRule::Restucture()
{
	memset(B, -1, sizeof B);
	dsu.Reset();
	step = 0;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (A[i][j] != Color::SPACE)
			{
				step++;
				for (int d = 0; d < 4; d++)
				{
					int x = i + dir[d][0], y = j + dir[d][1];
					if (inBoard(Point(x, y)))
					{
						if (A[x][y] == Color::SPACE)
						{
							Point v = dsu.Root(Point(i, j));
							dsu.hp[v.x][v.y]++;
						}
						else if (A[x][y] == A[i][j])
						{
							dsu.Merge(Point(x, y), Point(i, j));
						}
					}
				}
			}
}

int GameRule::Evaluate(Color col)
{
	int ret = 0;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
		{
			if (isLegal(i, j, col))
				ret++;
			if (isLegal(i, j, col == Color::BLACK ? Color::WHITE : Color::BLACK))
				ret--;
		}
	return ret;
}

//************************************************************MCTS


const double Confidence = 1.414;
const int LeastVisitTime = 5;
const int RolloutStep = 5;

struct MCTS_Node
{
	double value;
	int n;
	//std::priority_queue<cmp> que;
	std::vector<Point> que;
	MCTS_Node* son[9][9];

	MCTS_Node()
	{
		value = 0;
		n = 0;
		memset(son, 0, sizeof son);
	}

	~MCTS_Node()
	{
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				if (son[i][j] != nullptr)
					delete son[i][j];
	}

	double UCB(int N)const
	{
		if (n == 0)
			return 1e100;
		return value / n + Confidence * sqrt(log(N) / n);
	}

	Point FindMaxUCB()
	{
		Point ret(-1, -1);
		double mx = -2e100;
		for (auto u : que)
			if (son[u.x][u.y]->UCB(n) > mx)
				mx = son[u.x][u.y]->UCB(n), ret = u;
		return ret;
	}

	Point FindMaxRating()
	{
		Point ret(-1, -1);
		double mx = -2e100;
		for (auto u : que)
			if (son[u.x][u.y]->value / son[u.x][u.y]->n > mx)
				mx = son[u.x][u.y]->value / son[u.x][u.y]->n, ret = u;
		return ret;
	}
};

class GameAI_MCTS
{
	Point player_move, ai_move;
	clock_t start_time;
	Color ai_color;

	int beginning_value;
	GameRule* board, * tboard;
	MCTS_Node* root;

public:
	double Rollout();
	double Search(MCTS_Node* cur);
	Point Run();

	GameAI_MCTS(Color col);

	void SetBeginningState();
	void SetBeginningState(const Color A[9][9]);
};

GameAI_MCTS::GameAI_MCTS(Color col)
{
	ai_color = col;
	start_time = 0;

	root = nullptr;
	board = nullptr;
	tboard = nullptr;
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
	return ret;
}

double GameAI_MCTS::Search(MCTS_Node* cur)
{
	if (cur->que.empty())
	{
		if (cur->n < LeastVisitTime)
		{
			double up = Rollout();
			if (tboard->moveColor() == ai_color)
				up = -up;
			cur->n++;
			if (up > 0)
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
	double up = -Search(cur->son[t.x][t.y]);
	cur->n++;

	if (up > 0)
		cur->value += up;

	//cur->value += up;
	return up;
}

Point GameAI_MCTS::Run()
{
	start_time = clock();
	int search_times = 0;
	beginning_value = tboard->Evaluate(ai_color);
	while (1.0 * (clock() - start_time) / CLOCKS_PER_SEC <= 0.80)
	{
		memcpy(tboard, board, sizeof(GameRule));
		Search(root);
		search_times++;
	}
	/*std::cerr << "Search times:" << search_times << std::endl;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (root->son[i][j])
				std::cerr << "Operation(" << i << "," << j << ")'s ucb is " << root->son[i][j]->UCB(root->n)
				<< "    value is " << root->son[i][j]->value
				<< "    n is " << root->son[i][j]->n
				<< "    The rating is " << root->son[i][j]->value / root->son[i][j]->n
				<< std::endl;*/
	ai_move = root->FindMaxRating();
	return ai_move;
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

Color color, A[9][9];
int main()
{
	using namespace std;
	srand((unsigned)time(0));
	string str;
	int x, y;
	// 读入JSON
	getline(cin, str);
	//getline(cin, str);
	Json::Reader reader;
	Json::Value input;
	reader.parse(str, input);
	// 分析自己收到的输入和自己过往的输出，并恢复状态
	int turnID = input["responses"].size();
	x = input["requests"][0]["x"].asInt();
	
	if (x == -1)
		color = Color::BLACK;
	else
		color = Color::WHITE;
	Color ncol = color == Color::WHITE ? Color::BLACK : Color::WHITE;
	for (int i = 0; i < turnID; i++)
	{
		x = input["requests"][i]["x"].asInt(), y = input["requests"][i]["y"].asInt();
		if (x != -1) A[x][y] = ncol;
		x = input["responses"][i]["x"].asInt(), y = input["responses"][i]["y"].asInt();
		if (x != -1) A[x][y] = color;
	}
	x = input["requests"][turnID]["x"].asInt(), y = input["requests"][turnID]["y"].asInt();
	if (x != -1) A[x][y] = ncol;
	// 输出决策JSON
	Json::Value ret;
	Json::Value action;

	GameAI_MCTS ai(color);
	ai.SetBeginningState(A);
	Point ai_move = ai.Run();

	action["x"] = ai_move.x; action["y"] = ai_move.y;
	ret["response"] = action;
	Json::FastWriter writer;

	cout << writer.write(ret) << endl;
	return 0;
}
