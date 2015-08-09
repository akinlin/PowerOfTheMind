#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "JsonReader.h"

#define MAX_VALUES 2
#define MAX_TIMES 200

enum GAME_OBJECTS
{
	GO_ORB,
	GO_YOURGREENLABEL,
	GO_YOURREDLABEL,
	GO_LEVELLABEL,
	GO_YOURALLTIMELABEL,
	GO_BUTTONMENU
};

enum GAME_STATE
{
	GS_IDLE,
	GS_RUNNING,
	GS_NEXTLEVEL,
};

static const char* YOUR_ALL_TIME_G_KEY = "youralltimeg";
static const char* YOUR_ALL_TIME_R_KEY = "youralltimer";

static const float TINT_DURATION = .25f;

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);

	// change color based on a random value
	void changeColor(int value);

	// timer event for new random value
	void changeEvent(float dt);

	// update loop
	void update(float dt);
	void updateLables(float dt);

	// start the game
	void startNewGame(Ref* pSender);
	void changeTurn();
	void nextLevel();

	// game over
	void gameOver();

	// network test code
	void sendResults(int yourGreens, int yourReds);
	void getFlip(Ref* pSender);
	void onHttpRequestCompleted(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);

private:
	Sprite* orb;
	MenuItemImage* startButton;
	JsonReader* jsonRead;
	ValueVector randNums;

	Label* yourGreenCountLabel;
	Label* yourRedCountLabel;
	int yourGCount;
	int yourRCount;

	Label* levelLabel;
	int level;

	Label* yourAllTimeTotals;
	int yourAllTimeGreenCount;
	int yourAllTimeRedCount;

	int loopCount;
	int gameState;
	boolean yourTurn;

	// create the hud
	void createHud();
	// hud for count labels
	void createCountLabels();
	// hud for level labels
	void createLevelLabel();
	// hud for all time counts
	void createAllTimeLabels();
	// hud for close button
	void createCloseButton();

	// create the orb
	void createOrb();

	// check if level passed
	bool didPassLevel();
};

#endif // __PowerOfTheMindGame_SCENE_H__
