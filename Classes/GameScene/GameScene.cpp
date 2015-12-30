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
	auto rootNode = CSLoader::createNode("game-classical.csb");
	addChild(rootNode);

	// load action
	auto action = CSLoader::createTimeline("game-classical.csb");
	rootNode->runAction(action);

	// run action by frame
	action->gotoFrameAndPlay(0, 6, true);

	// get button continue
	Button* btn_continue = static_cast<Button*>(rootNode->getChildByName("btn-set"));

	return true;
}

Scene* GameScene::createScene(GameMode mode)
{
    auto scene = Scene::create();
    auto layer = GameScene::create(mode);
    scene->addChild(layer);
    return scene;
}

GameScene* GameScene::create(GameMode mode)
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


