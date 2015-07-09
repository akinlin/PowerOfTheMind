#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#define MAX_VALUES 10
#define MAX_TIMES 10

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

	int loopCount;
	boolean yourTurn;

	// create the labels
	void createLabels();
	// create the hud
	void createHud();
	// create the orb
	void createOrb();
};

#endif // __HELLOWORLD_SCENE_H__
