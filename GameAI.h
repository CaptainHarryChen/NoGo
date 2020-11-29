#pragma once
#include <thread>
#include <mutex>
#include <ctime>
#include "Graphic.h"
#include "GameRule.h"

class GameAI
{
	std::thread* pMain;
	std::mutex msg_lock,mv_lock;

	enum { NONE, END, MOVE, CALC } message;
	Point player_move, ai_move;
	GameRule* pRuler;
	int color;
	clock_t start_time;
	bool need_move;

	int A[9][9];

	bool ProcessMessage();
	void Run();
	
public:
	GameAI(GameRule* p, int col);

	void Start();
	void End();
	void SendMoveMessage();
	bool GetMove(Point &res);
	void PlayerMove(Point p);
};

