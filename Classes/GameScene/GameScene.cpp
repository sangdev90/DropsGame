#include "GameScene.h"
#include "../Entity/Drop.h"
#include "../Entity/DropBulet.h"

extern Vector<DropBullet*> m_DropBulletList;

bool GameScene::init(GameMode mode, int level)
{
	if (!Layer::init())
	{
		return false;
	}
	// init game scene here

	m_SetDown = false;
	m_SoundDown = false;
	m_Gamemode = mode;

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
	case Extreme:
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
	btn_set = static_cast<Button*>(rootNode->getChildByName("btn-set"));
	btn_restart = static_cast<Button*>(rootNode->getChildByName("btn-restart"));
	btn_about = static_cast<Button*>(rootNode->getChildByName("btn-about"));
	btn_sound = static_cast<Button*>(rootNode->getChildByName("btn-sound"));

	// set button event
	btn_set->addTouchEventListener(CC_CALLBACK_2(GameScene::btn_set_callback, this));
	btn_about->addTouchEventListener(CC_CALLBACK_2(GameScene::btn_about_callback, this));
	btn_restart->addTouchEventListener(CC_CALLBACK_2(GameScene::btn_restart_callback, this));
	btn_sound->addTouchEventListener(CC_CALLBACK_2(GameScene::btn_sound_callback, this));

	btn_bg = static_cast<Sprite*>(rootNode->getChildByName("btn-bg"));
	btn_nosound = static_cast<Sprite*>(rootNode->getChildByName("no_sound"));
	btn_bg->setVisible(false);     // default visible : false
	btn_about->setVisible(false);
	btn_restart->setVisible(false);
	btn_sound->setVisible(false);
	btn_nosound->setVisible(false);

	TextBMFont* game_time = static_cast<TextBMFont*>(rootNode->getChildByName("game-time")); // game time 
	TextBMFont* top_round = static_cast<TextBMFont*>(rootNode->getChildByName("top-round")); // game record 

	// game layer
	Node* game_layer = rootNode->getChildByName("game-layer");
	TextBMFont* drops = static_cast<TextBMFont*>(game_layer->getChildByName("drops"));
	TextBMFont* round = static_cast<TextBMFont*>(game_layer->getChildByName("round"));
	TextBMFont* score = static_cast<TextBMFont*>(game_layer->getChildByName("score"));

	// Rect classical
	if (mode == GameMode::Classical)
	{
		Node* bound_left = rootNode->getChildByName("bound_left");
		Node* bound_right = rootNode->getChildByName("bound_right");
		m_GridBoundClassical = Rect(bound_left->getPositionX(), bound_left->getPositionY(),
			bound_right->getPositionX() - bound_left->getPositionX(),
			bound_right->getPositionY() - bound_left->getPositionY());
	}

	//////////////////////////////////////////////////////////////////////////

	// add objects
	switch (mode)
	{
	case Classical:  // classical game
		{
			auto data = DataUtils::read("data/level1.lev");
			DropsType type = DropsType::Drops_one;
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					int tmp = rand_0_1() * 100 / 1;
					if (tmp < data.zero)
					{
						type = DropsType::Drops_zero;
					}else if(tmp < data.one) {
						type = DropsType::Drops_one;
					} else if (tmp < data.two)
					{
						type = DropsType::Drops_two;
					} else if (tmp < data.three)
					{
						type = DropsType::Drops_three;
					} else {
						type = DropsType::Drops_four;
					}

					if (type == DropsType::Drops_zero) {
						continue;
					}
					auto drop = Drop::createSprite(m_ClassicalPos[i][j], type);
					m_DropList.pushBack(drop);
				}
			}
		}
		break;
	case Extreme:  // extream game
		{
			auto test = Drop::createSprite(m_ExtreamPosL[2][2], DropsType::Drops_one);
			m_DropList.pushBack(test);
		}
		break;
	}
	
	// show all objects
	for (int i = 0;i < m_DropList.size();i++)
	{
		this->addChild(m_DropList.at(i), 2);
	}

	// add touch event
	auto event = EventListenerTouchOneByOne::create();
	event->onTouchBegan = [&](Touch * tou, Event * eve) {
		return true;
	};
	event->onTouchMoved = [&](Touch * tou, Event * eve) {

	};
	event->onTouchEnded = [&](Touch * tou, Event * eve) {
		auto location = tou->getLocation();

		// update all drops
		for (int i = 0;i < m_DropList.size();i++)
		{
			auto rect = m_DropList.at(i)->getRect();
			if (location.x > rect.getMinX() && location.x < rect.getMaxX() && location.y > rect.getMinY() && location.y < rect.getMaxY())
			{
				if (m_DropList.at(i)->getState() != Drops_four)
				{
					m_DropList.at(i)->setState((enum DropsType)(m_DropList.at(i)->getState() + 1));  // upgrade drops
				}
				else
				{
					m_DropDeleteList.pushBack(m_DropList.at(i));  // add to delete list
				}
			}
		}
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event, this);

	this->scheduleUpdate();  // update game

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

void GameScene::update(float delta)
{
	//////////////////////////////Classocal mode////////////////////////////////////////////
	switch (m_Gamemode)
	{
	case GameMode::Classical:
		{
			// add bullet to bulletDeleteList
			for (int i = 0;i < m_DropBulletList.size();i++)
			{
				auto bullet = (DropBullet*)m_DropBulletList.at(i);
				if ((bullet->getPositionX() <= m_GridBoundClassical.getMinX()) |
					(bullet->getPositionX() >= m_GridBoundClassical.getMaxX()) |
					(bullet->getPositionY() <= m_GridBoundClassical.getMinY()) |
					(bullet->getPositionY() >= m_GridBoundClassical.getMaxY()))
				{
					m_BulletDeleteList.pushBack(bullet);
				}
				for (int j = 0;j < m_DropList.size();j++)
				{
					auto drop = (Drop*)m_DropList.at(j);
					if ((drop->isAlive()) && (!drop->m_IsAtacking) && (drop->getPosition() - bullet->getPosition()).length() <= (drop->getR() + bullet->getR()))
					{
						// bullet attack drop
						drop->m_IsAtacking = true;  // avoide upgrade to explode
						if (drop->getState() != Drops_four)
						{
							drop->setState((enum DropsType)(drop->getState() + 1));  // upgrade drops
							m_BulletCombineList.pushBack(bullet); // add bullet to combine list 
						}
						else
						{
							m_DropDeleteList.pushBack(drop);      // add drop to delete list
							m_BulletCombineList.pushBack(bullet); // add bullet to combine list 
						}
					}
				}
			}
		}
		break;
	case GameMode::Extreme:
		{
		}
		break;
	}
	// update DropList and DropDeleteList
	for (int i = 0;i < m_DropDeleteList.size(); i++)
	{
		auto obj = (Drop*)m_DropDeleteList.at(i);
		obj->blast();
		m_DropDeleteList.eraseObject(obj);
		m_DropList.eraseObject(obj);
	}

	// update bullet delete list
	for (int i = 0;i < m_BulletDeleteList.size();i++)
	{
		auto bullet = (DropBullet*)m_BulletDeleteList.at(i);
		bullet->blast();
		m_BulletDeleteList.eraseObject(bullet);
		m_DropBulletList.eraseObject(bullet);
	}

	// update combine list
	for (int i = 0;i < m_BulletCombineList.size();i++)
	{
		auto bullet = (DropBullet*)m_BulletCombineList.at(i);
		bullet->combine();
		m_BulletCombineList.eraseObject(bullet);
		m_DropBulletList.eraseObject(bullet);
	}

	//////////////////////////////////Extream mode////////////////////////////////////////
}

void GameScene::btn_set_callback(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		m_SetDown = !m_SetDown;
		if (m_SetDown)
		{
			btn_bg->setVisible(true);     // default visible : false
			btn_about->setVisible(true);
			btn_restart->setVisible(true);
			btn_sound->setVisible(true);
		}
		else
		{
			btn_bg->setVisible(false);     // default visible : false
			btn_about->setVisible(false);
			btn_restart->setVisible(false);
			btn_sound->setVisible(false);
		}
	}
}

void GameScene::btn_restart_callback(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		// restart game scene
		if (m_Gamemode == GameMode::Classical)
		{
			Director::getInstance()->replaceScene(TransitionProgressOutIn::create(0.5, GameScene::createScene(GameMode::Classical, 1)));
		}
		else {
			Director::getInstance()->replaceScene(TransitionProgressOutIn::create(0.5, GameScene::createScene(GameMode::Extreme, 1)));
		}
	}
}

// sound btn callback
void GameScene::btn_sound_callback(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		m_SoundDown = !m_SoundDown;
		if (m_SoundDown)
		{
			btn_nosound->setVisible(true);
			// stop sound
		}
		else {
			btn_nosound->setVisible(false);
			// resume sound
		}
	}
}

void GameScene::btn_about_callback(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{

	}
}

