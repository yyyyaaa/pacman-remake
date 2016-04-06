#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	this->setKeyboardEnabled(true);

	//init level
	_level = new Level();
	_level->retain();
	_level->loadMap(MAP_NAME);
	_level->initOptions();
	this->addChild(_level->getMap(), 0);

	//add pacman sprite
	_pacman = Sprite::createWithSpriteFrameName("pac_right1.png");
	_pacman->setScale(PACMAN_SCALE);
	_pacman->setPosition(_level->positionForTileCoordinate(_pacman->getContentSize(), Point(1, 1)));
	this->addChild(_pacman, 1);

	//add player
	_player = new Player();
	_player->retain();
	this->setupAnimations();
	this->schedule(schedule_selector(GameScene::updateScene));

    return true;
}
void GameScene::setupAnimations() 
{

}
EventKeyboard::KeyCode GameScene::whichDirection()
{
	if (std::find(heldKeys.begin(), heldKeys.end(), RIGHT_ARROW) != heldKeys.end())
	{
		CCLOG("RIGHT ARROW");
		return RIGHT_ARROW;
	}
	if (std::find(heldKeys.begin(), heldKeys.end(), LEFT_ARROW) != heldKeys.end())
	{
		CCLOG("LEFT ARROW");
		return LEFT_ARROW;
	}
	if (std::find(heldKeys.begin(), heldKeys.end(), UP_ARROW) != heldKeys.end())
	{
		CCLOG("UP ARROW");
		return UP_ARROW;
	}
	if (std::find(heldKeys.begin(), heldKeys.end(), DOWN_ARROW) != heldKeys.end())
	{
		CCLOG("DOWN ARROW");
		return DOWN_ARROW;
	}
}
int GameScene::signum(float x)
{
	if (x > 0.0L)
		return 1.0L;
	else if (x < 0.0L)
		return -1.0L;
	else
		return 0.0L;
}
void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (std::find(heldKeys.begin(), heldKeys.end(), keyCode) == heldKeys.end()) {
		heldKeys.push_back(keyCode);
	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	heldKeys.erase(std::remove(heldKeys.begin(), heldKeys.end(), keyCode), heldKeys.end());
}
void GameScene::clampVelocityX()
{
	// clamp the velocity to the maximum, x-axis only
	if (std::abs(_player->velocity.x) > PLAYER_MAX_VELOCITY) {
		_player->velocity.x = signum(_player->velocity.x) * PLAYER_MAX_VELOCITY;
	}
	// clamp the velocity to 0 if it's < 1, and set the state to idle
	if (std::abs(_player->velocity.x) < 1) {
		_player->velocity.x = 0;
		if (_player->grounded) {
			_player->state = Player::State::IDLE;
		}
	}
}
void GameScene::clampVelocityY()
{
	// clamp the velocity to the maximum, y-axis only
	if (std::abs(_player->velocity.y) > PLAYER_MAX_VELOCITY) {
		_player->velocity.y = signum(_player->velocity.y) * PLAYER_MAX_VELOCITY;
	}
	// clamp the velocity to 0 if it's < 1, and set the state to idle
	if (std::abs(_player->velocity.y) < 1) {
		_player->velocity.y = 0;
		if (_player->grounded) {
			_player->state = Player::State::IDLE;
		}
	}
}
void GameScene::updateScene(float delta) 
{
	//key pressed
	if (!heldKeys.empty())
	{
		if (_player->grounded) {
			_player->state = Player::State::MOVING;
		}

		auto direction = this->whichDirection();
		//handle velocity: on pressing keys will set max velocity to player
		//MOVE RIGHT
		switch (direction) 
		{
		case RIGHT_ARROW:
			_player->velocity.x = PLAYER_MAX_VELOCITY;
			_player->facingRight = true;
			clampVelocityX();
			break;
		case LEFT_ARROW:
			_player->velocity.x = -PLAYER_MAX_VELOCITY;
			_player->facingRight = false;
			clampVelocityX();
			break;
		case UP_ARROW:
			_player->velocity.y = PLAYER_MAX_VELOCITY;
			_player->facingUp = true;
			clampVelocityY();
			break;
		case DOWN_ARROW:
			_player->velocity.y = -PLAYER_MAX_VELOCITY;
			_player->facingUp = false;
			clampVelocityY();
			break;
		}

		// unscale the velocity by the inverse delta time and set
		// the latest position
		_player->velocity = _player->velocity * delta;
		_player->position = _player->position + _player->velocity;
		_player->velocity = _player->velocity * 1 / delta;

		if (direction == RIGHT_ARROW || direction == LEFT_ARROW)
		{
			_player->velocity.x *= DAMPING;
			_pacman->setPositionX(_pacman->getPositionX() + _player->velocity.x);
		}
		if (direction == UP_ARROW || direction == DOWN_ARROW)
		{
			_player->velocity.y *= DAMPING;
			_pacman->setPositionY(_pacman->getPositionY() + _player->velocity.y);
		}
	}
	else
	{
		_player->state = Player::State::IDLE;
	}
}