/*
 * two game mode : classical and extream
 * change game mode by input mode param
 */
#pragma once
#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d;

class Drop;

// two game mode 
enum GameMode
{
	Classical,
	Extream
};

class GameScene : public Layer
{
public:
	virtual bool init(GameMode mode, int level);

	// return game scene by game mode 
	static Scene* createScene(GameMode mode, int level);
	static GameScene* create(GameMode mode, int level);

private:
	int m_Time;  // game time
	Vec2 m_ClassicalPos[6][6];
	Vec2 m_ExtreamPosL[5][3];
	Vec2 m_ExtreamPosR[5][3];
	Vec2 m_ExtreamCenter;

	Drop * test;
};

