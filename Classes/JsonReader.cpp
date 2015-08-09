#include "JsonReader.h"

USING_NS_CC;
using namespace cocostudio;

//------------------------------------------------------------------
//
// JsonReader
//
//------------------------------------------------------------------
JsonReader::JsonReader(const char* json)
{
	setUp(json);
}

JsonReader::JsonReader(std::string pFileName)
{
	auto content = FileUtils::getInstance()->getStringFromFile(pFileName);
	setUp(content.c_str());
}

void JsonReader::setUp(const char* json)
{
	jsonDoc.Parse<0>(json);
	if (jsonDoc.HasParseError())
	{
		log("JsonReader::JsonReader doc.HasParseError: %c", jsonDoc.GetParseError());
	}

	auto val = parseValueFromJsonValue(jsonDoc);
	jsonMap = val.asValueMap();
}

cocos2d::ValueMap JsonReader::getMap(const char* mapName)
{
	ValueMap map;
	if (jsonDoc.IsNull())
	{
		return map;
	}
	const rapidjson::Value& mapRef = jsonDoc[mapName];

	if (!mapRef.IsObject())
	{
		return map;
	}

	auto val = parseValueFromJsonValue(mapRef);
	return val.asValueMap();
}

Value JsonReader::parseValueFromJsonValue(const rapidjson::Value& value)
{
	// parse by type
	auto t = value.GetType();

	//    if(t == rapidjson::Type::kNullType)
	//    {
	//        // don't add anything
	//        return Value();
	//    }

	if (t == rapidjson::Type::kFalseType) {
		return Value(false);
	}

	if (t == rapidjson::Type::kTrueType) {
		return Value(true);
	}

	if (t == rapidjson::Type::kStringType) {
		return Value(value.GetString());
	}

	if (t == rapidjson::Type::kNumberType) {
		if (value.IsDouble()) {
			return Value(value.GetDouble());
		}
		else if (value.IsUint()) {
			int temp = value.GetUint();
			return Value(temp);
		}
		else if (value.IsInt()) {
			return Value(value.GetInt());
		}
	}

	if (t == rapidjson::Type::kObjectType) {
		ValueMap dict;
		for (rapidjson::Value::ConstMemberIterator itr = value.MemberonBegin(); itr != value.MemberonEnd(); ++itr)
		{
			auto jsonKey = itr->name.GetString();
			auto el = parseValueFromJsonValue(itr->value);
			dict[jsonKey] = el;
		}
		return Value(dict);
	}
	if (t == rapidjson::Type::kArrayType) {
		ValueVector arr;
		
		for (rapidjson::SizeType i = 0; i < value.Size(); i++)
		{
			const rapidjson::Value& arrValue = value[i];
			auto el = parseValueFromJsonValue(arrValue);
			arr.push_back(el);
		}
		return Value(arr);
	}

	// none
	return Value();
}

ValueMap JsonReader::getValueMapFromJsonFile(const std::string& filename)
{
	auto content = FileUtils::getInstance()->getStringFromFile(filename);

	rapidjson::Document doc;
	doc.Parse<0>(content.c_str());
	if (!doc.HasParseError())
	{
		// check that root is object not array
		auto val = parseValueFromJsonValue(doc);
		// should check here or have caller check if actually map
		return val.asValueMap();
	}

	CCLOG("GetParseError: %s\n", doc.GetParseError());
	return ValueMapNull;
}

void JsonReader::print()
{
	if (!jsonDoc.IsNull())
	{
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		jsonDoc.Accept(writer);
		log("JsonReader::print");
		log("%s: ", buffer.GetString());
		log("");
	}
	else
	{
		log("JsonReader::print Document is empty");
		log("");
	}
}