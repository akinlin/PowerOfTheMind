#include "HelloWorldScene.h"

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
		CC_CALLBACK_1(HelloWorld::startNewGame, this));

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
	yourGreenCountLabel->setPosition(Vec2(origin.x + halfScreenWidth - (halfScreenWidth / 2), origin.y + (yourGreenCountLabel->getContentSize().height * 2) + 10));
	this->addChild(yourGreenCountLabel, GO_YOURGREENLABEL);

	yourRedCountLabel = Label::createWithTTF("Your Red:-", "fonts/Marker Felt.ttf", 24);
	yourRedCountLabel->setPosition(Vec2(origin.x + halfScreenWidth - (halfScreenWidth / 2), origin.y + yourRedCountLabel->getContentSize().height + 10));
	this->addChild(yourRedCountLabel, GO_YOURREDLABEL);

	compGreenCountLabel = Label::createWithTTF("Comp Green:-", "fonts/Marker Felt.ttf", 24);
	compGreenCountLabel->setPosition(Vec2(origin.x + halfScreenWidth + (halfScreenWidth / 2), origin.y + (compGreenCountLabel->getContentSize().height * 2) + 10));
	this->addChild(compGreenCountLabel, GO_COMPGREENLABEL);

	compRedCountLabel = Label::createWithTTF("Comp Red:-", "fonts/Marker Felt.ttf", 24);
	compRedCountLabel->setPosition(Vec2(origin.x + halfScreenWidth + (halfScreenWidth / 2), origin.y + compRedCountLabel->getContentSize().height + 10));
	this->addChild(compRedCountLabel, GO_COMPREDLABEL);
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

	compAllTimeGreenCount = cocos2d::UserDefault::getInstance()->getIntegerForKey(COMP_ALL_TIME_G_KEY);
	compAllTimeRedCount = cocos2d::UserDefault::getInstance()->getIntegerForKey(COMP_ALL_TIME_R_KEY);

	int halfScreenWidth = visibleSize.width / 2;
	char buffer[50];

	sprintf(buffer, "Your All Time: \nGreens:%d Reds:%d", yourAllTimeGreenCount, yourAllTimeRedCount);
	std::string yourLabel = buffer;
	yourAllTimeTotals = Label::createWithTTF(yourLabel, "fonts/Marker Felt.ttf", 24);
	yourAllTimeTotals->setPosition(Vec2(origin.x + 10, origin.y + visibleSize.height - (yourAllTimeTotals->getContentSize().height) - 10));
	yourAllTimeTotals->setAnchorPoint(Point::ZERO);
	this->addChild(yourAllTimeTotals, GO_YOURALLTIMELABEL);

	sprintf(buffer, "Comp All Time: \nGreens:%d Reds:%d", compAllTimeGreenCount, compAllTimeRedCount);
	std::string compLabel = buffer;
	compAllTimeTotals = Label::createWithTTF(compLabel, "fonts/Marker Felt.ttf", 24);
	compAllTimeTotals->setPosition(Vec2(origin.x + 10, origin.y + visibleSize.height - (compAllTimeTotals->getContentSize().height * 2) - 20));
	compAllTimeTotals->setAnchorPoint(Point::ZERO);
	this->addChild(compAllTimeTotals, GO_COMPALLTIMELABEL);
}

void HelloWorld::createOrb()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// add orb
	orb = Sprite::create("orb.png");
	//orb->setColor(Color3B::GRAY);
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
	yourTurn = true;
	yourGCount = 0;
	yourRCount = 0;
	compGCount = 0;
	compRCount = 0;
	level = 1;

	// schedule a random update
	schedule(schedule_selector(HelloWorld::changeEvent), .15);

	// schedule update
	scheduleUpdate();

	startButton->setVisible(false);
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

	compAllTimeGreenCount += compGCount;
	cocos2d::UserDefault::getInstance()->setIntegerForKey(COMP_ALL_TIME_G_KEY, compAllTimeGreenCount);
	compAllTimeRedCount += compRCount;
	cocos2d::UserDefault::getInstance()->setIntegerForKey(COMP_ALL_TIME_R_KEY, compAllTimeRedCount);

	// refresh the all time labels
	char buffer[50];

	sprintf(buffer, "Your All Time: \nGreens:%d Reds:%d", yourAllTimeGreenCount, yourAllTimeRedCount);
	std::string yourLabel = buffer;
	yourAllTimeTotals->setString(yourLabel);

	sprintf(buffer, "Comp All Time: \nGreens:%d Reds:%d", compAllTimeGreenCount, compAllTimeRedCount);
	std::string compLabel = buffer;
	compAllTimeTotals->setString(compLabel);
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
		if (yourTurn)
		{
			++yourGCount;
			//orb->setColor(Color3B::GREEN);
			auto action = TintTo::create(TINT_DURATION, 0, 255, 0);
			orb->runAction(action);
		}
		else
		{
			++compGCount;
		}
	}
	else
	{
		if (yourTurn)
		{
			++yourRCount;
			//orb->setColor(Color3B::RED);
			auto action = TintTo::create(TINT_DURATION, 255, 0, 0);
			orb->runAction(action);
		}
		else
		{
			++compRCount;
		}
	}
}

void HelloWorld::changeEvent(float dt)
{
	++loopCount;
	if (loopCount <= MAX_TIMES)
	{
		changeColor(rand() % MAX_VALUES);
	}
	else if (yourTurn)
	{
		// switch to comp turn
		loopCount = 0;
		yourTurn = false;
		//orb->setColor(Color3B::GRAY);
		auto action = TintTo::create(TINT_DURATION, 166, 166, 166);
		orb->runAction(action);
	}
	else
	{
		if (didPassLevel())
		{
			++level;
			loopCount = 0;
			yourTurn = true;
		}
		else
		{
			// didnt pass the current level
			gameOver();
		}
	}
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
	char buffer[50];

	sprintf(buffer, "Your Green:%d", yourGCount);
	std::string greenCountString = buffer;
	yourGreenCountLabel->setString(greenCountString);

	sprintf(buffer, "Your Red:%d", yourRCount);
	std::string redCountString = buffer;
	yourRedCountLabel->setString(redCountString);

	sprintf(buffer, "Comp Green:%d", compGCount);
	std::string compGreenCountString = buffer;
	compGreenCountLabel->setString(compGreenCountString);

	sprintf(buffer, "Comp Red:%d", compRCount);
	std::string compRedCountString = buffer;
	compRedCountLabel->setString(compRedCountString);

	sprintf(buffer, "Level:%d", level);
	std::string levelString = buffer;
	levelLabel->setString(levelString);
}