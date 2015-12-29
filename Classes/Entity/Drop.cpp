#include "Drop.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

bool Drop::init(int x, int y, DropsType type)
{
	if (!Sprite::init())
	{
		return false;
	}
	// init drop here


	return true;
}

Drop* Drop::createSprite(int x, int y, DropsType type)
{ 
	Drop *pRet = new(std::nothrow) Drop(); 
	if (pRet && pRet->init(x, y, type))
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

// update outlook and state of current drop
void Drop::update(float delta)
{

}

void Drop::setState(DropsType state)
{
	m_Type = state; // change type of current drop
}
