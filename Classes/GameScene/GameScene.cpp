#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

bool GameScene::init(GameMode mode)
{
	if (!Layer::init())
	{
		return false;
	}
	// init game scene here

	return true;
}

GameScene* GameScene::createScene(GameMode mode)
{
	GameScene *pRet = new(std::nothrow) GameScene();
	if (pRet && pRet->init(mode))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

