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

	// continue button callback
	void btn_continue_callback(Ref* pSender);
};