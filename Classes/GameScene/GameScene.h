/*
 * two game mode : classical and extream
 * change game mode by input mode param
 */
#pragma once
#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d;

// two game mode 
enum GameMode
{
	Classical,
	Extream
};

class GameScene : public Layer
{
public:
	virtual bool init(GameMode mode);

	// return game scene by game mode 
	static GameScene* createScene(GameMode mode);

};

