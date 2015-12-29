#include "Help.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

bool Help::init()
{
	if (!Layer::init())
	{
		return false;
	}
	// init help scene here
    // laod scene csd file
    auto rootNode = CSLoader::createNode("game-help.csb");
    addChild(rootNode);

	return true;
}

Scene* Help::createScene()
{
	auto scene = Scene::create();
	auto layer = Help::create();
	scene->addChild(layer);
	return scene;  // return scene with layer
}

