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

	enum Message { NONE, END, MOVE, CALC } message;
	Point player_move, ai_move;
	GameRule* pRuler;
	Color color;
	clock_t start_time;
	bool need_move, ready_move;

	Color A[9][9];

	bool ProcessMessage();
	void Run();
	
public:
	GameAI(Color col);

	void Start();
	void End();
	void SendMoveMessage();
	bool GetMove(Point &res);
	void PlayerMove(Point p);
};

