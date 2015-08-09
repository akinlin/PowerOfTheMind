#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "cocos2d.h"
USING_NS_CC;

class Character : public cocos2d::Layer
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(Character);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

	virtual void attack();
	virtual void takeDamage();
	virtual void die();

private:
	Sprite* _characterSprite;

	// create the character
	void createCharacter();
};

class BadGuy : public Character
{
public:
	// implement the "static create()" method manually
	CREATE_FUNC(BadGuy);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	void attack() override;
	void takeDamage() override;
	void die() override;

private:
	Sprite* _badGuySprite;

	// create the character
	void createBadguy();
};

#endif
