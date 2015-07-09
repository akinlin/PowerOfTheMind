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

	// add the labels for the green and red counts
	createLabels();

	// create the orb image
	createOrb();
    
    return true;
}

void HelloWorld::createLabels()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	int halfScreenWidth = visibleSize.width / 2;
	char buffer[50];

	yourGreenCountLabel = Label::createWithTTF("Your Green:-", "fonts/Marker Felt.ttf", 24);
	yourGreenCountLabel->setPosition(Vec2(origin.x + halfScreenWidth - (halfScreenWidth / 2), origin.y + (yourGreenCountLabel->getContentSize().height * 2) + 10));
	this->addChild(yourGreenCountLabel, 2);

	yourRedCountLabel = Label::createWithTTF("Your Red:-", "fonts/Marker Felt.ttf", 24);
	yourRedCountLabel->setPosition(Vec2(origin.x + halfScreenWidth - (halfScreenWidth / 2), origin.y + yourRedCountLabel->getContentSize().height + 10));
	this->addChild(yourRedCountLabel, 3);

	compGreenCountLabel = Label::createWithTTF("Comp Green:-", "fonts/Marker Felt.ttf", 24);
	compGreenCountLabel->setPosition(Vec2(origin.x + halfScreenWidth + (halfScreenWidth / 2), origin.y + (compGreenCountLabel->getContentSize().height * 2) + 10));
	this->addChild(compGreenCountLabel, 5);

	compRedCountLabel = Label::createWithTTF("Comp Red:-", "fonts/Marker Felt.ttf", 24);
	compRedCountLabel->setPosition(Vec2(origin.x + halfScreenWidth + (halfScreenWidth / 2), origin.y + compRedCountLabel->getContentSize().height + 10));
	this->addChild(compRedCountLabel, 6);
}

void HelloWorld::createHud()
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
	this->addChild(menu, 1);
}

void HelloWorld::createOrb()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// add orb
	orb = Sprite::create("orb.png");
	orb->setColor(Color3B::GRAY);

	// position the sprite on the center of the screen
	orb->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(orb, 0);
}

void HelloWorld::startNewGame(Ref* pSender)
{
	loopCount = 0;
	yourTurn = true;
	yourGCount = 0;
	yourRCount = 0;
	compGCount = 0;
	compRCount = 0;

	// schedule a random update
	schedule(schedule_selector(HelloWorld::changeEvent), 1.0);

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
	if (value > MAX_VALUES / 2)
	{
		if (yourTurn)
		{
			++yourGCount;
			orb->setColor(Color3B::GREEN);
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
			orb->setColor(Color3B::RED);
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
	if (loopCount < MAX_TIMES)
	{
		changeColor(rand() % MAX_VALUES);
	}
	else if (yourTurn)
	{
		// switch to comp turn
		loopCount = 0;
		yourTurn = false;
		orb->setColor(Color3B::GRAY);
	}
	else
	{
		// turn done
		gameOver();
	}
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
}