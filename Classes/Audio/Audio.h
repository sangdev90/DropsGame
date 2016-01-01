#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace cocos2d;
using namespace CocosDenshion;

class Audio
{
public:
	static void set_volume(float volume);
	static float get_volume();
	static void play_bgmusic(const char* filename);
	static void play_effect(const char* filename);

	static void pause();
	static void resume();

private:
	static void set_bg_volume(float  volume);
	static void set_ef_volume(float volume);
};
