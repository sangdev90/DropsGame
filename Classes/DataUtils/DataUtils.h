#pragma once

#include "cocos2d.h"
#include <iostream>
USING_NS_CC;
using namespace cocos2d;

// data type for read or save
enum DataType
{
	UserName,
	TopLevel,
	TopLevelTime,
	CurrentGameLevel
};

// input file data
struct InputData
{
	int zero;
	int one;
	int two;
	int three;
	int four;
};

class DataUtils
{
public:
	static void save(DataType type, std::string data);
	static std::string read(DataType type);
	static InputData read(const char* filename);      // read data form file
};

