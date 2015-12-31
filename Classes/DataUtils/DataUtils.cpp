#include "DataUtils.h"
#include <stdlib.h>
#include <stdio.h>

#define UDefault (UserDefault::getInstance())

// user prefrence data save
void DataUtils::save(DataType type, std::string data)
{
	switch (type)
	{
	case UserName:
		{
			UDefault->setStringForKey("UserName", data);
		}
		break;
	case TopLevel:
		{
			UDefault->setStringForKey("TopLevel", data);
		}
		break;
	case TopLevelTime:
		{
			UDefault->setStringForKey("TopLevelTime", data);
		}
		break;
	case CurrentGameLevel:
		{
			UDefault->setStringForKey("CurrentGameLevel", data);
		}
		break;
	}
}

// user prefrence data read
std::string DataUtils::read(DataType type)
{
	switch (type)
	{
	case UserName:
		return UDefault->getStringForKey("UserName");
		break;
	case TopLevel:
		return UDefault->getStringForKey("TopLevel");
		break;
	case TopLevelTime:
		return UDefault->getStringForKey("TopLevelTime");
		break;
	case CurrentGameLevel:
		return UDefault->getStringForKey("CurrentGameLevel");
		break;
	}
}

InputData DataUtils::read(const char* filename)
{
	FILE * file = fopen(filename, "r+");
	int zero, one, two, three, four;

	// zero to four
	if (fscanf(file, "zero:%d one:%d two:%d three:%d four:%d", &zero, &one, &two, &three, &four))
	{
		log("The integer read was:%d\n", zero);
	} else {
		fprintf(file, "Error reading an integer from file.\n");
		exit(1);
	}

	InputData result;
	result.zero = zero;
	result.one = one;
	result.two = two;
	result.three = three;
	result, four = four;

	return result;
}

