#include "WinLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

bool WinLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	// init win layer here
	auto rootNode = CSLoader::createNode("game-layer-win.csb");
	addChild(rootNode);

	// load action
	auto action = CSLoader::createTimeline("game-layer-lose.csb");
	rootNode->runAction(action);

	// run action by frame
	action->gotoFrameAndPlay(0, 6, true);

	// get button
	Button* btn_exit = static_cast<Button*>(rootNode->getChildByName("btn-exit"));
	Button* btn_next = static_cast<Button*>(rootNode->getChildByName("btn-next"));

	// button callback
	btn_exit->addTouchEventListener(CC_CALLBACK_1(WinLayer::btn_exit_callback, this));
	btn_next->addTouchEventListener(CC_CALLBACK_1(WinLayer::btn_next_callback, this));

	return true;
}

Layer* WinLayer::createScene()
{
	return WinLayer::create();  // return layer only
}

void WinLayer::btn_exit_callback(Ref* pSender)
{
	// exit app
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void WinLayer::btn_next_callback(Ref* pSender)
{
	// load game scene next round
}
