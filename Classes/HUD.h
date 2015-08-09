#ifndef __HUD_H__
#define __HUD_H__

#include "cocos2d.h"
USING_NS_CC;

enum GAME_OBJECTS
{
	GO_GREENLABEL,
	GO_REDLABEL,
	GO_LEVELLABEL,
	GO_ALLTIMELABEL,
	GO_BUTTONMENU
};

typedef std::function<void(Ref*)> StartGameCallback;

class HUD : public cocos2d::Layer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

	// set the callback for the start button
	void setStartGameCallback(const StartGameCallback& callback)  { _startCallback = callback; };
	void setStartEnabled(bool isEnabled);
    
    // implement the "static create()" method manually
	CREATE_FUNC(HUD);

	// sets the hud display values
	void setHudValues(int greens, int reds, int totGreens, int totReds, int level);

private:
	Label* greenCountLabel;
	Label* redCountLabel;
	Label* levelLabel;
	Label* allTimeTotals;

	MenuItemImage* startButton;
	StartGameCallback _startCallback;
	void startSelected(Ref* pSender);

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
};

#endif
