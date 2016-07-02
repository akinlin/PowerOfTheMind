#include "PowerOfTheMindGame.h"
#include "../external/json/document.h"
#include "../external/json/writer.h"
#include "../external/json/stringbuffer.h"

Scene* PowerOfTheMindGame::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = PowerOfTheMindGame::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool PowerOfTheMindGame::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	// seed random with the current time
	srand(time(NULL));

	// set the game state
	gameState = GS_IDLE;
	
	// add the hud menu
	createHud();

	// create the orb image
	//createOrb();

	createCharacters();

	createShuffle();
    
    return true;
}

void PowerOfTheMindGame::createShuffle()
{
	_shuffleLayer = ShuffleExample::create();
	addChild(_shuffleLayer);
}

void PowerOfTheMindGame::createHud()
{
	_HUD = HUD::create();
	_HUD->setStartGameCallback(CC_CALLBACK_1(PowerOfTheMindGame::getFlip, this));
	this->addChild(_HUD, GO_HUD);

	// get the all time count values from the user default store
	yourAllTimeGreenCount = cocos2d::UserDefault::getInstance()->getIntegerForKey(YOUR_ALL_TIME_G_KEY);
	yourAllTimeRedCount = cocos2d::UserDefault::getInstance()->getIntegerForKey(YOUR_ALL_TIME_R_KEY);
}

//void PowerOfTheMindGame::createOrb()
//{
//	Size visibleSize = Director::getInstance()->getVisibleSize();
//	Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//	// add orb
//	orb = Sprite::create("orb.png");
//	auto action = TintTo::create(TINT_DURATION, 166, 166, 166);
//	orb->runAction(action);
//
//	// position the sprite on the center of the screen
//	orb->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
//
//	// add the sprite as a child to this layer
//	this->addChild(orb, GO_ORB);
//}

void PowerOfTheMindGame::createCharacters()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// add goodguy
	_goodGuy = Character::create();
	auto action = TintTo::create(TINT_DURATION, 166, 166, 166);
	_goodGuy->runAction(action);

	// position the sprite on the center of the screen
	_goodGuy->setPosition(Vec2(visibleSize.width / 2 + origin.x - 50, visibleSize.height / 2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(_goodGuy, GO_CHARACTERS);

	// add badguy
	_badGuy = BadGuy::create();
	action = TintTo::create(TINT_DURATION, 166, 166, 166);
	_badGuy->runAction(action);

	// position the sprite on the center of the screen
	_badGuy->setPosition(Vec2(visibleSize.width / 2 + origin.x + 50, visibleSize.height / 2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(_badGuy, GO_CHARACTERS);
}

void PowerOfTheMindGame::startNewGame(Ref* pSender)
{
	loopCount = 0;
	yourGCount = 0;
	yourRCount = 0;
	level = 1;

	// schedule a random update
	schedule(schedule_selector(PowerOfTheMindGame::changeEvent), .15);

	// schedule update
	scheduleUpdate();

	_HUD->setStartEnabled(false);

	// set the game state
	gameState = GS_RUNNING;
}

void PowerOfTheMindGame::nextLevel()
{
	gameState = GS_RUNNING;
	++level;
	loopCount = 0;
}

void PowerOfTheMindGame::gameOver()
{
	// schedule a random update
	unscheduleAllSelectors();

	// unschedule update
	unscheduleUpdate();

	_HUD->setStartEnabled(true);

	// add the totals to the all time totals in the user default store
	yourAllTimeGreenCount += yourGCount;
	cocos2d::UserDefault::getInstance()->setIntegerForKey(YOUR_ALL_TIME_G_KEY, yourAllTimeGreenCount);
	yourAllTimeRedCount += yourRCount;
	cocos2d::UserDefault::getInstance()->setIntegerForKey(YOUR_ALL_TIME_R_KEY, yourAllTimeRedCount);

	_HUD->setHudValues(yourGCount, yourRCount, yourAllTimeGreenCount, yourAllTimeRedCount, level);

	sendResults(yourGCount, yourRCount);

	_goodGuy->die();
	_badGuy->attack();

	// set the game state
	gameState = GS_IDLE;
}

void PowerOfTheMindGame::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void PowerOfTheMindGame::changeColor(int value)
{
	if (value)
	{
		++yourGCount;
		auto action = TintTo::create(TINT_DURATION, 0, 255, 0);
		_goodGuy->runAction(action);

		if (yourGCount % 10)
		{
			_goodGuy->attack();
			_badGuy->takeDamage();
		}
	}
	else
	{
		++yourRCount;
		auto action = TintTo::create(TINT_DURATION, 255, 0, 0);
		_goodGuy->runAction(action);

		if (yourRCount % 10)
		{
			_goodGuy->takeDamage();
			_badGuy->attack();
		}
	}
}

void PowerOfTheMindGame::changeEvent(float dt)
{
	if (loopCount < randNums.size())//<= MAX_TIMES)
	{
		changeColor(randNums.at(loopCount).asInt());//rand() % MAX_VALUES);
	}
	else
	{
		if (didPassLevel())
		{
			gameState = GS_NEXTLEVEL;
			getFlip(NULL);
		}
		else
		{
			// didnt pass the current level
			gameOver();
		}
	}
	++loopCount;
}

bool PowerOfTheMindGame::didPassLevel()
{
	switch (level)
	{
	case 1:
		if (yourGCount >= 100)
		{
			return true;
		}
		break;
	case 2:
		if (yourGCount >= 250)
		{
			return true;
		}
		break;
	case 3:
		if (yourGCount >= 425)
		{
			return true;
		}
		break;
	case 4:
		if (yourGCount >= 625)
		{
			return true;
		}
		break;
	case 5:
		// do nothing for now but future check for 850 and show win state
	default:
		break;
	}

	return false;
}

void PowerOfTheMindGame::update(float dt)
{
	updateLables(dt);
}

void PowerOfTheMindGame::updateLables(float dt)
{
	_HUD->setHudValues(yourGCount, yourRCount, yourAllTimeGreenCount, yourAllTimeRedCount, level);
}

void PowerOfTheMindGame::sendResults(int yourGreens, int yourReds)
{
	cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
	request->setUrl("http://ec2-52-2-116-170.compute-1.amazonaws.com:8080/ajax/AddBasicLog");
	request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(PowerOfTheMindGame::onHttpRequestCompleted, this));

	// write the post data
	char buffer[100];
	sprintf(buffer, "green=%d&red=%d", yourGreens, yourReds);
	const char* postData = buffer;
	request->setRequestData(postData, strlen(postData));

	request->setTag("POST test1");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();

	/*
	GET REQUEST
	char buffer[100];
	//sprintf(buffer, "http://ec2-52-2-116-170.compute-1.amazonaws.com:8080/ajax/sendTurn.jsp?ygreen=%d&yred=%d&cgreen=%d&cred=%d", yourGreens, yourReds, compGreens, compReds);
	sprintf(buffer, "http://localhost:8080/ajax/sendTurn.jsp?ygreen=%d&yred=%d&cgreen=%d&cred=%d", yourGreens, yourReds, compGreens, compReds);
	const char* url = buffer;

	cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
	request->setUrl(url);
	request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(PowerOfTheMindGame::onHttpRequestCompleted, this));
	request->setTag("GET test1");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
	*/
}

void PowerOfTheMindGame::getFlip(Ref* pSender)
{
	cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
	request->setUrl("https://api.random.org/json-rpc/1/invoke");
	request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(PowerOfTheMindGame::onHttpRequestCompleted, this));

	// write the post data
	std::string rpc = "{ \"jsonrpc\": \"2.0\",\"method\" : \"generateIntegers\",\"params\" : {\"apiKey\": \"eed1f674-aace-4135-aa0d-c39c9185dc68\",\"n\" : 200,\"min\" : 0,\"max\" : 1,\"replacement\" : true,\"base\" : 10},\"id\": 32595}";
	request->setRequestData(rpc.c_str(), strlen(rpc.c_str()));

	request->setTag("POST test2");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void PowerOfTheMindGame::onHttpRequestCompleted(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response)
{
	if (response && response->getResponseCode() == 200 && response->getResponseData()) 
	{
		std::vector<char> *data = response->getResponseData();
		std::string ret(&(data->front()), data->size());

		jsonRead = new JsonReader(ret.c_str());
		jsonRead->print();
		ValueMap map = jsonRead->getMap("result");
		if (!map.empty())
		{
			ValueMap random = map.at("random").asValueMap();
			randNums.clear();
			randNums = random.at("data").asValueVector();
			for (std::size_t i = 0; i < randNums.size(); i++)
			{
				log("index:%d Num:%d", i, randNums.at(i).asInt());
			}

			if (gameState == GS_IDLE)
			{
				startNewGame(NULL);
			}
			else if (gameState == GS_NEXTLEVEL)
			{
				nextLevel();
			}
			
		}
	}
	else 
	{
		log("%s", ("Error " + std::to_string(response->getResponseCode()) + " in request").c_str());
	}
}