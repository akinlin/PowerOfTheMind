#include "HelloWorldScene.h"
#include "../external/json/document.h"
#include "../external/json/writer.h"
#include "../external/json/stringbuffer.h"

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool HelloWorld::init()
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
	createOrb();
    
    return true;
}

void HelloWorld::createHud()
{
	// add the close button
	createCloseButton();

	// add the labels for the green and red counts
	createCountLabels();

	// add the level label
	createLevelLabel();

	// add the all time labels
	createAllTimeLabels();
}

void HelloWorld::createCloseButton()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// add a "new game" button to start a new game
	startButton = MenuItemImage::create(
		"button.png",
		"button.png",
		CC_CALLBACK_1(HelloWorld::getFlip, this));

	startButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 50));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, startButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, GO_BUTTONMENU);
}

void HelloWorld::createCountLabels()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	int halfScreenWidth = visibleSize.width / 2;
	char buffer[50];

	yourGreenCountLabel = Label::createWithTTF("Your Green:-", "fonts/Marker Felt.ttf", 24);
	yourGreenCountLabel->setPosition(Vec2(origin.x + halfScreenWidth, origin.y + (yourGreenCountLabel->getContentSize().height * 2) + 10));
	this->addChild(yourGreenCountLabel, GO_YOURGREENLABEL);

	yourRedCountLabel = Label::createWithTTF("Your Red:-", "fonts/Marker Felt.ttf", 24);
	yourRedCountLabel->setPosition(Vec2(origin.x + halfScreenWidth, origin.y + yourRedCountLabel->getContentSize().height + 10));
	this->addChild(yourRedCountLabel, GO_YOURREDLABEL);
}

void HelloWorld::createLevelLabel()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	int halfScreenWidth = visibleSize.width / 2;
	char buffer[50];

	levelLabel = Label::createWithTTF("Level:-", "fonts/Marker Felt.ttf", 24);
	levelLabel->setPosition(Vec2(origin.x + halfScreenWidth, origin.y + visibleSize.height - (levelLabel->getContentSize().height * 2) - 10));
	this->addChild(levelLabel, GO_LEVELLABEL);
}

void HelloWorld::createAllTimeLabels()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// get the all time count values from the user default store
	yourAllTimeGreenCount = cocos2d::UserDefault::getInstance()->getIntegerForKey(YOUR_ALL_TIME_G_KEY);
	yourAllTimeRedCount = cocos2d::UserDefault::getInstance()->getIntegerForKey(YOUR_ALL_TIME_R_KEY);

	int halfScreenWidth = visibleSize.width / 2;
	char buffer[50];

	sprintf(buffer, "Your All Time: \nGreens:%d Reds:%d", yourAllTimeGreenCount, yourAllTimeRedCount);
	std::string yourLabel = buffer;
	yourAllTimeTotals = Label::createWithTTF(yourLabel, "fonts/Marker Felt.ttf", 24);
	yourAllTimeTotals->setPosition(Vec2(origin.x + 10, origin.y + visibleSize.height - (yourAllTimeTotals->getContentSize().height) - 10));
	yourAllTimeTotals->setAnchorPoint(Point::ZERO);
	this->addChild(yourAllTimeTotals, GO_YOURALLTIMELABEL);
}

void HelloWorld::createOrb()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// add orb
	orb = Sprite::create("orb.png");
	auto action = TintTo::create(TINT_DURATION, 166, 166, 166);
	orb->runAction(action);

	// position the sprite on the center of the screen
	orb->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(orb, GO_ORB);
}

void HelloWorld::startNewGame(Ref* pSender)
{
	loopCount = 0;
	yourGCount = 0;
	yourRCount = 0;
	level = 1;

	// schedule a random update
	schedule(schedule_selector(HelloWorld::changeEvent), .15);

	// schedule update
	scheduleUpdate();

	startButton->setVisible(false);

	// set the game state
	gameState = GS_RUNNING;
}

void HelloWorld::nextLevel()
{
	gameState = GS_RUNNING;
	++level;
	loopCount = 0;
	//yourTurn = true;
}

void HelloWorld::gameOver()
{
	// schedule a random update
	unscheduleAllSelectors();

	// unschedule update
	unscheduleUpdate();

	startButton->setVisible(true);

	// add the totals to the all time totals in the user default store
	yourAllTimeGreenCount += yourGCount;
	cocos2d::UserDefault::getInstance()->setIntegerForKey(YOUR_ALL_TIME_G_KEY, yourAllTimeGreenCount);
	yourAllTimeRedCount += yourRCount;
	cocos2d::UserDefault::getInstance()->setIntegerForKey(YOUR_ALL_TIME_R_KEY, yourAllTimeRedCount);

	// refresh the all time labels
	char buffer[50];

	sprintf(buffer, "Your All Time: \nGreens:%d Reds:%d", yourAllTimeGreenCount, yourAllTimeRedCount);
	std::string yourLabel = buffer;
	yourAllTimeTotals->setString(yourLabel);

	sendResults(yourGCount, yourRCount);

	// set the game state
	gameState = GS_IDLE;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
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

void HelloWorld::changeColor(int value)
{
	if (value)
	{
		++yourGCount;
		auto action = TintTo::create(TINT_DURATION, 0, 255, 0);
		orb->runAction(action);
	}
	else
	{
		++yourRCount;
		auto action = TintTo::create(TINT_DURATION, 255, 0, 0);
		orb->runAction(action);
	}
}

void HelloWorld::changeEvent(float dt)
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

bool HelloWorld::didPassLevel()
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

void HelloWorld::update(float dt)
{
	updateLables(dt);
}

void HelloWorld::updateLables(float dt)
{
	char buffer[50];

	sprintf(buffer, "Your Green:%d", yourGCount);
	std::string greenCountString = buffer;
	yourGreenCountLabel->setString(greenCountString);

	sprintf(buffer, "Your Red:%d", yourRCount);
	std::string redCountString = buffer;
	yourRedCountLabel->setString(redCountString);

	sprintf(buffer, "Level:%d", level);
	std::string levelString = buffer;
	levelLabel->setString(levelString);
}

void HelloWorld::sendResults(int yourGreens, int yourReds)
{
	cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
	request->setUrl("http://ec2-52-2-116-170.compute-1.amazonaws.com:8080/ajax/AddBasicLog");
	request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(HelloWorld::onHttpRequestCompleted, this));

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
	request->setResponseCallback(CC_CALLBACK_2(HelloWorld::onHttpRequestCompleted, this));
	request->setTag("GET test1");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
	*/
}

void HelloWorld::getFlip(Ref* pSender)
{
	cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
	request->setUrl("https://api.random.org/json-rpc/1/invoke");
	request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(HelloWorld::onHttpRequestCompleted, this));

	// write the post data
	std::string rpc = "{ \"jsonrpc\": \"2.0\",\"method\" : \"generateIntegers\",\"params\" : {\"apiKey\": \"eed1f674-aace-4135-aa0d-c39c9185dc68\",\"n\" : 200,\"min\" : 0,\"max\" : 1,\"replacement\" : true,\"base\" : 10},\"id\": 32595}";
	request->setRequestData(rpc.c_str(), strlen(rpc.c_str()));

	request->setTag("POST test2");
	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void HelloWorld::onHttpRequestCompleted(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response)
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