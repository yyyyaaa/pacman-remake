#include "Player.h"

USING_NS_CC;

Player::Player()
{
	state = Player::State::IDLE;
	facingUp = true;	//default moving up
	facingRight = false;
	position = Point();
	velocity = Point(0, 0);
	stateTime = 0;
}
Player::~Player()
{
}