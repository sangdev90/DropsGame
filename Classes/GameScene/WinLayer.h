#pragma once
#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d;

class WinLayer : public Layer
{
public:
	CREATE_FUNC(WinLayer);
	virtual bool init();
	static Layer* createScene();
};

