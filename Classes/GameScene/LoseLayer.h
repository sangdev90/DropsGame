#pragma once
#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d;

class LoseLayer : public Layer
{
public:
	CREATE_FUNC(LoseLayer);
	virtual bool init();
	static Layer* createScene();
};

