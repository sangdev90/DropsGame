#include "GameScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "../Entity/Drop.h"

using namespace cocos2d::ui;

bool GameScene::init(GameMode mode, int level)
{
	if (!Layer::init())
	{
		return false;
	}
	// init game scene here

	Node* rootNode = NULL;
	switch (mode)
	{
	case Classical:
		rootNode = CSLoader::createNode("game-classical.csb");
		for (int i = 0;i < 6;i++)
		{
			for (int j = 0;j < 6;j++)
			{
				auto path = StringUtils::format("pos_%d_%d", i + 1,j + 1);
				m_ClassicalPos[i][j] = rootNode->getChildByName(path)->getPosition();
			}
		}
		break;
	case Extream:
		rootNode = CSLoader::createNode("game-extream.csb");
		for (int i = 0;i < 5;i++)
		{
			for (int j = 0;j < 3;j++)
			{
				// grid position L
				auto path = StringUtils::format("gridl_%d_%d", i + 1, j + 1);
				m_ExtreamPosL[i][j] = rootNode->getChildByName(path)->getPosition();
			}
		}
		for (int i = 0;i < 5;i++)
		{
			for (int j = 0;j < 3;j++)
			{
				// grid position R
				auto path = StringUtils::format("gridr_%d_%d", i + 1, j + 1);
				m_ExtreamPosR[i][j] = rootNode->getChildByName("game-layer")->getChildByName(path)->getPosition();
			}
		}
		m_ExtreamCenter = rootNode->getChildByName("game-layer")->getChildByName("grid_center")->getPosition();
		break;
	}
	if (rootNode == NULL)
	{
		log("game scene file GameScene.csb load error!");
		return false;
	}
	addChild(rootNode);

	// load action
	auto action = CSLoader::createTimeline("game-classical.csb");
	rootNode->runAction(action);

	// run action by frame
	action->gotoFrameAndPlay(0, 6, true);

	// get button 
	Button* btn_set = static_cast<Button*>(rootNode->getChildByName("btn-set"));
	Button* btn_restart = static_cast<Button*>(rootNode->getChildByName("btn-restart"));
	Button* btn_about = static_cast<Button*>(rootNode->getChildByName("btn-about"));
	Button* btn_sound = static_cast<Button*>(rootNode->getChildByName("btn-sound"));

	Sprite* btn_bg = static_cast<Sprite*>(rootNode->getChildByName("btn-bg"));
	btn_bg->setVisible(false);     // default visible : false
	btn_about->setVisible(false);
	btn_restart->setVisible(false);
	btn_sound->setVisible(false);

	TextBMFont* game_time = static_cast<TextBMFont*>(rootNode->getChildByName("game-time")); // game time 
	TextBMFont* top_round = static_cast<TextBMFont*>(rootNode->getChildByName("top-round")); // game record 

	// game layer
	Node* game_layer = rootNode->getChildByName("game-layer");
	TextBMFont* drops = static_cast<TextBMFont*>(game_layer->getChildByName("drops"));
	TextBMFont* round = static_cast<TextBMFont*>(game_layer->getChildByName("round"));
	TextBMFont* score = static_cast<TextBMFont*>(game_layer->getChildByName("score"));

	// add objects
	test = Drop::createSprite(m_ClassicalPos[2][2], DropsType::Drops_one);
	this->addChild(test, 2);

	// touch event
	auto event = EventListenerTouchOneByOne::create();
	event->onTouchBegan = [&](Touch * tou, Event * eve) {
		return true;
	};
	event->onTouchMoved = [&](Touch * tou, Event * eve) {

	};
	event->onTouchEnded = [&](Touch * tou, Event * eve) {
		auto location = tou->getLocation();
		auto rect = test->getRect();
		if (location.x > rect.getMinX() && location.x < rect.getMaxX() && location.y > rect.getMinY() && location.y < rect.getMaxY())
		{
			if (test->getState() != Drops_four)
			{
				test->setState((enum DropsType)(test->getState() + 1));  // upgrade drops
			}
			else
			{
				test->blast();  // drop explodes
			}
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event, this);

	return true;
}

Scene* GameScene::createScene(GameMode mode, int level)
{
	auto scene = Scene::create();
	auto layer = GameScene::create(mode, level);
	scene->addChild(layer);
	return scene;
}

// override create func
GameScene* GameScene::create(GameMode mode, int level)
{
	GameScene *pRet = new(std::nothrow) GameScene();
	if (pRet && pRet->init(mode, level))
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

