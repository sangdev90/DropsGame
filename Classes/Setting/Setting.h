#pragma once
#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d;

class Setting : public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(Setting);
	static Scene* createScene();
};