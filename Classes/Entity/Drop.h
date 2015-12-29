#pragma once
#include "cocos2d.h"
USING_NS_CC;
using namespace cocos2d;
enum DropsType
{
	Drops_one,   // one drop
	Drops_two,   // two drops
	Drops_three, // three drops
	Drops_four,  // four drops
	Drops_five   // explode
};

class Drop : public Sprite
{
public:
	virtual bool init(int x, int y, DropsType type);
	static Drop* createSprite(int x, int y, DropsType type);

	void update(float delta); // uodate drop state
	void setState(DropsType state);

	int getX() { return m_X; }
	int getY() { return m_Y; }
	DropsType getType() { return m_Type; }
	bool isAlive() { return m_Alive; }
private:
	int m_X;
	int m_Y;
	DropsType m_Type;
	bool m_Alive;
};

