#pragma once
#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d;

class Loading : public Layer
{
public:
	CREATE_FUNC(Loading);
	virtual bool init();
	static Scene* createScene();
};
