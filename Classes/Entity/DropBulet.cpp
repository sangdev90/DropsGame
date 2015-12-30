#include "DropBulet.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

DropBullet* DropBullet::createSprite(Vec2 postion, Dir direction, int speed)
{
	DropBullet *pRet = new(std::nothrow) DropBullet();
	if (pRet && pRet->init(postion, direction, speed))
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

bool DropBullet::init(Vec2 postion, Dir direction, int speed)
{
	if (!Sprite::init())
	{
		return false;
	}
	// init dropbullet here
	switch (direction)
	{
	case Up:
		m_Sprite = Sprite::create("drops/north.png");  // todo : change to sprite frame
		break;
	case Down:
		m_Sprite = Sprite::create("drops/south.png");
		break;
	case Left:
		m_Sprite = Sprite::create("drops/west.png");
		break;
	case Right:
		m_Sprite = Sprite::create("drops/east.png");
		break;
	}
	if (m_Sprite == NULL)
	{
		log("load bullet sprite error!");
		return false;
	}
	m_Sprite->setPosition(0, 0);
	m_Sprite->setScale(4.0f);
	this->setPosition(postion);
	this->addChild(m_Sprite);  // show sprite

	auto width = m_Sprite->getContentSize().width;
	auto height = m_Sprite->getContentSize().height;
	m_Rect = Rect(postion.x - 2 * width, postion.y - 2 * height, 4 * width, 4 * height);

	// save direction and speed
	m_Direction = direction;
	m_Speed = speed;

	this->scheduleUpdate();  // update position of bullet
	
	return true;
}

// update frame
void DropBullet::update(float delta)
{
	switch (m_Direction)
	{
	case Up:
		this->setPositionY(this->getPositionY() + m_Speed);
		break;
	case Down:
		this->setPositionY(this->getPositionY() - m_Speed);
		break;
	case Left:
		this->setPositionX(this->getPositionX() - m_Speed);
		break;
	case Right:
		this->setPositionX(this->getPositionX() + m_Speed);
		break;
	default:
		break;
	}
}

