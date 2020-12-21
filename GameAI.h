#pragma once
#include "GameRule.h"

class GameAI
{
public:
	enum class Message { END, MOVE, CALC };
protected:
	Color ai_color;
public:
	GameAI(Color color);
	// 注意：若析构函数不为虚函数，则子类的析构函数不会被调用！！
	virtual ~GameAI() = 0;
	// 向AI发送消息
	virtual void SendGameMessage(Message = Message::CALC) = 0;
	// 设置起始状态
	virtual void SetBeginningState() = 0;
	virtual void SetBeginningState(const Color A[9][9]) = 0;
	// 启动AI
	virtual void Start() = 0;
	// 停止AI
	virtual void End() = 0;
	// 获取AI的下一步移动（非阻塞）
	virtual bool GetMove(Point& res) = 0;
	// 获取AI的下一步移动（阻塞）
	virtual void WaitNextMove(Point& res);
	// 设置玩家的移动
	virtual void PlayerMove(Point p) = 0;
};

