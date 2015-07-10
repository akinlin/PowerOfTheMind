#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#define MAX_VALUES 2
#define MAX_TIMES 200

enum GAME_OBJECTS
{
	GO_ORB,
	GO_BUTTONMENU,
	GO_YOURGREENLABEL,
	GO_YOURREDLABEL,
	GO_COMPGREENLABEL,
	GO_COMPREDLABEL,
	GO_LEVELLABEL,
	GO_YOURALLTIMELABEL,
	GO_COMPALLTIMELABEL,
};

static const char* YOUR_ALL_TIME_G_KEY = "youralltimeg";
static const char* YOUR_ALL_TIME_R_KEY = "youralltimer";

static const char* COMP_ALL_TIME_G_KEY = "compalltimeg";
static const char* COMP_ALL_TIME_R_KEY = "compalltimer";

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

	// start the game
	void startNewGame(Ref* pSender);

	// game over
	void gameOver();

private:
	Sprite* orb;
	MenuItemImage* startButton;

	Label* yourGreenCountLabel;
	Label* yourRedCountLabel;
	int yourGCount;
	int yourRCount;

	Label* compGreenCountLabel;
	Label* compRedCountLabel;
	int compGCount;
	int compRCount;

	Label* levelLabel;
	int level;

	Label* yourAllTimeTotals;
	int yourAllTimeGreenCount;
	int yourAllTimeRedCount;

	Label* compAllTimeTotals;
	int compAllTimeGreenCount;
	int compAllTimeRedCount;

	int loopCount;
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

#endif // __HELLOWORLD_SCENE_H__
