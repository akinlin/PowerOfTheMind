#include "Character.h"

bool Character::init()
{
	if (!Layer::init())
    {
        return false;
    }
	
	// add the hud menu
	createCharacter();

    return true;
}

void Character::createCharacter()
{
	_characterSprite = Sprite::create("good_idle.png");
	_characterSprite->setScale(2.0f);
	addChild(_characterSprite);
}

void Character::attack()
{
	_characterSprite->setTexture("good_attack.png");
}

void Character::takeDamage()
{
	_characterSprite->setTexture("good_hurt.png");
}

void Character::die()
{
	_characterSprite->setTexture("good_dead.png");
}

///////////////////
// BadGuy
///////////////////
bool BadGuy::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// add the hud menu
	createBadguy();

	return true;
}

void BadGuy::createBadguy()
{
	_badGuySprite = Sprite::create("bad_idle.png");
	_badGuySprite->setScale(2.0f);
	addChild(_badGuySprite);
}

void BadGuy::attack()
{
	_badGuySprite->setTexture("bad_attack.png");
}

void BadGuy::takeDamage()
{
	_badGuySprite->setTexture("bad_hurt.png");
}

void BadGuy::die()
{
	_badGuySprite->setTexture("bad_dead.png");
}