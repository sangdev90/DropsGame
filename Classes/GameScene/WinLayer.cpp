#include "WinLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

bool WinLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	return true;
}

Layer* WinLayer::createScene()
{
	return WinLayer::create();  // return layer only
}

