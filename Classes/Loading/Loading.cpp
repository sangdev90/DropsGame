#include "Loading.h"

bool Loading::init()
{
	if (!Layer::init())
	{
		return false;
	}
	// init loading scene here
	// show a loading bar and load resources of the game

	return true;
}

Scene* Loading::createScene()
{
	auto scene = Scene::create();
	auto layer = Loading::create();
	scene->addChild(layer);
	return scene;
}

