#include "Setting.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

bool Setting::init()
{
	if (!Layer::init())
	{
		return false;
	}
	// init setting scene here

	return true;
}

Scene* Setting::createScene()
{
	auto scene = Scene::create();
	auto layer = Setting::create();
	scene->addChild(layer);
	return scene;
}

