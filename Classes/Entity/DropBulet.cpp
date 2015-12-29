#include "DropBulet.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

DropBullet* DropBullet::creatSprite(Drop& owner, Dir direction, int speed)
{
	DropBullet *pRet = new(std::nothrow) DropBullet();
	if (pRet && pRet->init(owner, direction, speed))
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

bool DropBullet::init(Drop& owner, Dir direction, int speed)
{
	if (!Sprite::init())
	{
		return false;
	}
	// init dropbullet here

	return true;
}

