#include "LoseLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

bool LoseLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	return true;
}

Layer* LoseLayer::createScene()
{
	return LoseLayer::create(); // return layer only
}

