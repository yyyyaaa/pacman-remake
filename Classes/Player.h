#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Definitions.h"

//Save Pacman's current state, need this class for expanding to multiplayer
class Player : public cocos2d::Object
{
public:
	enum State {
		MOVING, IDLE
	};

	State state;
	bool facingUp;
	bool facingRight;
	bool grounded;
	float stateTime;

	//node position
	cocos2d::Point position;
	cocos2d::Point velocity;

	//ctor, dtor
	Player();
	~Player();
   
};

#endif // __PLAYER_SCENE_H__
