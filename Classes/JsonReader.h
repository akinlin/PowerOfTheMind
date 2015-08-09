///
///  \file      JsonReader.h
///  \brief     JsonReader header file.
///  \author    Alexander Kinlin
///  \date      1/26/15
///  \copyright Legacy Code Games 2015. All rights reserved.
///

#ifndef _JSONREADER_H_
#define _JSONREADER_H_

#include "cocos2d.h"
#include <cocostudio/DictionaryHelper.h>
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

class JsonReader
{
public:
	JsonReader(const char* json);
	JsonReader(std::string pFileName);
	~JsonReader();

	cocos2d::ValueMap getMap(const char* mapName);
	static cocos2d::ValueMap getValueMapFromJsonFile(const std::string& filename);

	void print();

private:
	rapidjson::Document jsonDoc;
	cocos2d::ValueMap jsonMap;
	void setUp(const char* json);

	static cocos2d::Value parseValueFromJsonValue(const rapidjson::Value& value);
};

#endif