#include "HUD.h"

bool HUD::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	
	// add the hud menu
	createHud();

    return true;
}

void HUD::createHud()
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

void HUD::createCloseButton()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HUD::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// add a "new game" button to start a new game
	startButton = MenuItemImage::create(
		"button.png",
		"button.png",
		CC_CALLBACK_1(HUD::startSelected, this));

	startButton->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 50));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, startButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, GO_BUTTONMENU);
}

void HUD::createCountLabels()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	int halfScreenWidth = visibleSize.width / 2;
	char buffer[50];

	greenCountLabel = Label::createWithTTF("Greens:-", "fonts/Marker Felt.ttf", 24);
	greenCountLabel->setPosition(Vec2(origin.x + halfScreenWidth, origin.y + (greenCountLabel->getContentSize().height * 2) + 10));
	this->addChild(greenCountLabel, GO_GREENLABEL);

	redCountLabel = Label::createWithTTF("Reds:-", "fonts/Marker Felt.ttf", 24);
	redCountLabel->setPosition(Vec2(origin.x + halfScreenWidth, origin.y + redCountLabel->getContentSize().height + 10));
	this->addChild(redCountLabel, GO_REDLABEL);
}

void HUD::createLevelLabel()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	int halfScreenWidth = visibleSize.width / 2;

	levelLabel = Label::createWithTTF("Level:-", "fonts/Marker Felt.ttf", 24);
	levelLabel->setPosition(Vec2(origin.x + halfScreenWidth, origin.y + visibleSize.height - (levelLabel->getContentSize().height * 2) - 10));
	this->addChild(levelLabel, GO_LEVELLABEL);
}

void HUD::createAllTimeLabels()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	int halfScreenWidth = visibleSize.width / 2;

	allTimeTotals = Label::createWithTTF("All Time: \nGreens:-\nReds:-", "fonts/Marker Felt.ttf", 24);
	allTimeTotals->setPosition(Vec2(origin.x + 10, origin.y + visibleSize.height - (allTimeTotals->getContentSize().height) - 10));
	allTimeTotals->setAnchorPoint(Point::ZERO);
	this->addChild(allTimeTotals, GO_ALLTIMELABEL);
}

void HUD::menuCloseCallback(Ref* pSender)
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

void HUD::startSelected(Ref* pSender)
{
	_startCallback(startButton);
}

void HUD::setStartEnabled(bool isEnabled)
{
	startButton->setVisible(isEnabled);
}

void HUD::setHudValues(int greens, int reds, int totGreens, int totReds, int level)
{
	char buffer[50];

	sprintf(buffer, "Greens:%d", greens);
	std::string greenCountString = buffer;
	greenCountLabel->setString(greenCountString);

	sprintf(buffer, "Reds:%d", reds);
	std::string redCountString = buffer;
	redCountLabel->setString(redCountString);

	sprintf(buffer, "Level:%d", level);
	std::string levelString = buffer;
	levelLabel->setString(levelString);

	sprintf(buffer, "All Time: \nGreens:%d\nReds:%d", totGreens, totReds);
	std::string yourLabel = buffer;
	allTimeTotals->setString(yourLabel);
}