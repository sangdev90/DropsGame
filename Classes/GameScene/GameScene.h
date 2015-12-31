/*
 * two game mode : classical and extream
 * change game mode by input mode param
 */
#pragma once
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d;
using namespace cocos2d::ui;

class Drop;
class DropBullet;

// two game mode 
enum GameMode
{
	Classical,
	Extream
};

class GameScene : public Layer
{
public:
	virtual bool init(GameMode mode, int level);

	// return game scene by game mode 
	static Scene* createScene(GameMode mode, int level);
	static GameScene* create(GameMode mode, int level);

	// game update : game logic
	void update(float delta);

	// button callback
	// void btn_set_callback(Ref* pSender);
	void btn_set_callback(Ref* pSender, Widget::TouchEventType type);

private:
	int m_Time;  // game time
	bool m_SetDown; // button set is clicked
	Vec2 m_ClassicalPos[6][6];
	Vec2 m_ExtreamPosL[5][3];
	Vec2 m_ExtreamPosR[5][3];
	Vec2 m_ExtreamCenter;

	DropBullet * test;

	Rect m_GridBoundClassical; // rect for bullet ContactListen
	Rect m_GridBoundExtreamL;
	Rect m_GridBoundExtreamR;

	// btn
	Button* btn_set;
	Button* btn_restart;
	Button* btn_about;
	Button* btn_sound;

	// btn -bg
	Sprite* btn_bg;

	GameMode m_Gamemode;

public:
	Vector<Drop*> m_DropList;

	Vector<Drop*> m_DropDeleteList;
	Vector<DropBullet*> m_BulletDeleteList;

	Vector<DropBullet*> m_BulletCombineList;
};

