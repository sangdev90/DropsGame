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
	TopLevelTime
};

class DataUtils
{
public:
	static void save(DataType type,void* data);
	static void read(DataType type);
	static void read(const char* filename);      // read data form file
};

