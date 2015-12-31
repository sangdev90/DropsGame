#include "GameScene.h"
#include "../Entity/Drop.h"
#include "../Entity/DropBulet.h"

#include "LoseLayer.h"
#include "WinLayer.h"
#include "DarkLayer.h"

extern Vector<DropBullet*> m_DropBulletList;

int minInt(int a, int b)
{
	return a > b ? b : a;
}

void GameScene::set_tank_fore_height(int number)
{
	if (number == 0)
	{
		tank_fore_top->setVisible(false);
	}
	else {
		tank_fore_top->setVisible(true);
	}
	number = minInt(number, 20); // 限制高度 水滴数量不限制
	tank_fore->setScaleY(number / 20.0 * 0.3);
	tank_fore_top->setPositionY(tank_fore->getPositionY() + tank_fore->getContentSize().height * number / 20.0 * 0.3);
}

// 增加分数
void GameScene::addPoints(int point)
{
	int p = atoi(DataUtils::read(GameScore).c_str());
	DataUtils::save(GameScore, StringUtils::format("%d", p + point));
	score->setText(DataUtils::read(GameScore));
}

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
	m_Click = false;
	m_DropCount = 0;

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
	drops = static_cast<TextBMFont*>(game_layer->getChildByName("drops"));
	round = static_cast<TextBMFont*>(game_layer->getChildByName("round"));
	score = static_cast<TextBMFont*>(game_layer->getChildByName("score"));
	game_type = static_cast<TextBMFont*>(game_layer->getChildByName("game-type"));

	// 更新top round
	auto top = atoi(DataUtils::read(TopLevel).c_str());
	if (level > top)
	{
		DataUtils::save(TopLevel, StringUtils::format("%d", level));
	}
	top_round->setText(StringUtils::format("%d", atoi(DataUtils::read(TopLevel).c_str())));

	// 更新当前level
	DataUtils::save(GameLevel, StringUtils::format("%d", level));

	// 更新round
	round->setText(StringUtils::format("%d", level));

	// 更新score
	if (level == 1)
	{
		score->setText("0");
		DataUtils::save(GameScore, "0"); // 更新每局初始分数
	}
	else {
		score->setText(DataUtils::read(GameScore)); // 显示前面关卡已积累分数
	}

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
	auto game_info = DataUtils::game_info("data/game.config");
	
	// add objects
	switch (mode)
	{
	case Classical:  // classical game
		{
			if (level > game_info.max_classical_level)
			{
				return false;
			}
			// 加载当前等级的场景数据
			auto data = DataUtils::read(StringUtils::format("data/level%d.lev", level).c_str()); // load game level : level
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

					if (type == DropsType::Drops_zero){
						continue;
					}
					auto drop = Drop::createSprite(m_ClassicalPos[i][j], type);
					m_DropList.pushBack(drop);
				}
			}

			// 加载初始水箱储蓄量
			tank_fore = static_cast<Sprite*>(game_layer->getChildByName("tank-fore"));
			tank_fore_top = static_cast<Sprite*>(game_layer->getChildByName("tank-fore-top"));
			set_tank_fore_height(game_info.init_tank_drops);
			drops->setText(StringUtils::format("%02d", game_info.init_tank_drops)); // 更新水箱显示数值

			if (level == 1) // 第一关初始化水滴数量
			{
				DataUtils::save(GameDrops, StringUtils::format("%d", game_info.init_tank_drops));
			}
			else {
				drops->setText(DataUtils::read(GameDrops));
			}

			// 更改游戏模式label
			game_type->setText("Classical");
		}
		break;
	case Extreme:  // extream game
		{
			auto test = Drop::createSprite(m_ExtreamPosL[2][2], DropsType::Drops_one);
			m_DropList.pushBack(test);

			// 更改游戏模式label
			game_type->setText("Extreme");
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
		if (!m_Click)
		{
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
						m_Click = true;		  // 连击判断开始
						m_CoundBegin = false; // 判断计数开始点
						m_DropDeleteList.pushBack(m_DropList.at(i));  // add to delete list
					}
					// 减少水箱水滴
					auto num = atoi(DataUtils::read(GameDrops).c_str());
					DataUtils::save(GameDrops, StringUtils::format("%d", num - 1)); // 减少
					if (atoi(DataUtils::read(GameDrops).c_str()) > -1)
					{
						drops->setText(DataUtils::read(GameDrops));
					}
					// 更新水箱高度
					set_tank_fore_height(atoi(DataUtils::read(GameDrops).c_str()));
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
	/////////////////////////////////all mode/////////////////////////////////////////
	// update DropList and DropDeleteList
	for (int i = 0;i < m_DropDeleteList.size(); i++)
	{
		auto obj = (Drop*)m_DropDeleteList.at(i);
		addPoints(2);  // 水滴爆炸+2分
		obj->blast();
		m_DropCount++; // 计数器增加
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
		addPoints(1); // 水滴结合+1分
		bullet->combine();
		m_BulletCombineList.eraseObject(bullet);
		m_DropBulletList.eraseObject(bullet);
	}

	// 判断输赢
	if (atoi(DataUtils::read(GameDrops).c_str()) == 0 && m_BulletDeleteList.size() == 0 && m_BulletCombineList.size() == 0)
	{
		// 输
		gameLose();
		unscheduleUpdate();
	}

	if (m_DropList.size() == 0 && m_DropBulletList.size() == 0 && m_BulletDeleteList.size() == 0)
	{
		if (atoi(DataUtils::read(GameDrops).c_str()) >= 0)
		{
			// 赢
			gameWin();
			unscheduleUpdate();
		}
	}

	if ((m_CoundBegin == false) && m_Click && m_DropBulletList.size() > 0)
	{
		m_CoundBegin = true; // 开始计数
		m_DropCount = 0;	 // 计数器清零
		m_upInt = 1;		 // 计数器跟踪器，避免多次执行水滴递增操作
	}

	if (m_CoundBegin)
	{
		if (m_DropCount == m_upInt)
		{
			log("-----------%d----------", m_DropCount);
			if (m_DropCount % 2 == 0)
			{
				// 增加水箱水滴
				int drop = atoi(DataUtils::read(GameDrops).c_str());
				DataUtils::save(GameDrops, StringUtils::format("%d", drop + 1));
				set_tank_fore_height(drop + 1);						 // 水滴图片增高
				drops->setText(StringUtils::format("%d", drop + 1)); // 水滴数字增加 1
				log("=====%d=====", drop + 1);
			}
			m_upInt++;
		}
	}

	if (m_CoundBegin && m_DropBulletList.size() == 0 && m_BulletCombineList.size() == 0 && m_BulletDeleteList.size() == 0)
	{
		// 结束计数
		m_CoundBegin = false;
		m_Click = false;
	}
}

void GameScene::gameLose()
{
	auto size = Director::getInstance()->getVisibleSize();
	auto dark = DarkLayer::create();
	auto lose = LoseLayer::createLayer();
	//lose->setPosition(size.width, size.height);
	//dark->addChild(lose);
	//dark->setPosition(0, 0);
	//this->addChild(dark, 2);
	lose->setPosition(size.width / 2, size.height / 2);
	this->addChild(lose);
}

void GameScene::gameWin()
{
	auto size = Director::getInstance()->getVisibleSize();
	auto dark = DarkLayer::create();
	auto win = WinLayer::createLayer();
	//lose->setPosition(size.width, size.height);
	//dark->addChild(lose);
	//dark->setPosition(0, 0);
	//this->addChild(dark, 2);
	win->setPosition(size.width / 2, size.height / 2);
	this->addChild(win);
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