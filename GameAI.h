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
	// ע�⣺������������Ϊ�麯����������������������ᱻ���ã���
	virtual ~GameAI() = 0;
	// ��AI������Ϣ
	virtual void SendGameMessage(Message = Message::CALC) = 0;
	// ������ʼ״̬
	virtual void SetBeginningState() = 0;
	virtual void SetBeginningState(const Color A[9][9]) = 0;
	// ����AI
	virtual void Start() = 0;
	// ֹͣAI
	virtual void End() = 0;
	// ��ȡAI����һ���ƶ�����������
	virtual bool GetMove(Point& res) = 0;
	// ��ȡAI����һ���ƶ���������
	virtual void WaitNextMove(Point& res);
	// ������ҵ��ƶ�
	virtual void PlayerMove(Point p) = 0;
};

