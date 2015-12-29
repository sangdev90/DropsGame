#pragma once
#include "cocos2d.h"
#include "Drop.h"
USING_NS_CC;
using namespace cocos2d;

enum Dir
{
	Up,
	Down,
	Left,
	Right
};

class DropBullet : public Sprite
{
public:

	Dir getDir() { return m_Direction; } // return direction of current bullet
	Drop* getOwner() { return m_Owner; } // return the owner of current bullet
	bool  isAlive() { return m_Alive; }  // return life state of current bullet

	static DropBullet* creatSprite(Drop& owner, Dir direction, int speed);
	virtual bool init(Drop& owner, Dir direction, int speed);

private:
	Drop * m_Owner;
	Dir    m_Direction;
	int    m_Speed;
	bool   m_Alive;     // show bullet
};
