#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Definitions.h"
#include "Level.h"
#include "Player.h"
#include <vector>
#include <string>
#include <algorithm>


//----------------
//--Pacman sprite will use global coords
//--Player saves Tile position (node coords)
class GameScene : public cocos2d::Layer
{
private:
	Level* _level;
	Player* _player;
	Sprite* _pacman;
	std::vector<EventKeyboard::KeyCode> heldKeys;

	Animate* moveRight;
	Animate* moveLeft;
	Animate* moveUp;
	Animate* moveDown;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

public:
	//handling keyboard events
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	//update game state
	void updatePlayer(float interval);
	void updatePlayerSprite(float interval);
	void updateScene(float interval);
	int signum(float x);

	//get direction keycode from heldkeys
	EventKeyboard::KeyCode whichDirection();
	//clamp the velocity X axis
	void clampVelocityX();
	//clamp the velocity Y axis
	void clampVelocityY();

	void setupAnimations();
};

#endif // __SPLASH_SCENE_H__
