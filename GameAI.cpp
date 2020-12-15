#include "GameAI.h"
#include <thread>

GameAI::GameAI(Color color)
{
	this->color = color;
}

void GameAI::WaitNextMove(Point& res)
{
	while (!GetMove(res))
		std::this_thread::yield();
}
