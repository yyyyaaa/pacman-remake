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
	_pacman->setPosition(_level->tileCoordinateToPosition(_pacman->getContentSize(), Point(1, 1)));
	this->addChild(_pacman, 1);

	//add player
	_player = new Player();
	_player->retain();
	this->setupAnimations();
	this->schedule(schedule_selector(GameScene::updateScene));

    return true;
}

/////////////////////////
//ANIMATION HANDLING
/////////////////////////
cocos2d::Vector<SpriteFrame*> GameScene::getAnimation(const char* format, int count)
{
	auto cache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> animFrames(count);
	for (int i = 1; i <= count; i++)
	{
		std::string fn = StringUtils::format(format, i);
		animFrames.pushBack(cache->getSpriteFrameByName(fn));
	}
	return animFrames;
}
void GameScene::setupMoveUpAnim()
{
	Vector<SpriteFrame*> frames = getAnimation("pac_up%d.png", 2);
	Animation* animation = Animation::createWithSpriteFrames(frames);
	Animate* animate = Animate::create(animation);
	animate->getAnimation()->setRestoreOriginalFrame(true);
	animate->setDuration(0.50f);
	animate->setTarget(this->_pacman);
	
	this->moveUp = animate;
	this->moveUp->retain();
}
void GameScene::setupMoveDownAnim()
{
	Vector<SpriteFrame*> frames = getAnimation("pac_down%d.png", 2);
	Animation* animation = Animation::createWithSpriteFrames(frames);
	Animate* animate = Animate::create(animation);
	animate->getAnimation()->setRestoreOriginalFrame(true);
	animate->setDuration(0.50f);
	animate->setTarget(this->_pacman);

	this->moveDown = animate;
	this->moveDown->retain();
}
void GameScene::setupMoveRightAnim()
{
	Vector<SpriteFrame*> frames = getAnimation("pac_left%d.png", 2);
	Animation* animation = Animation::createWithSpriteFrames(frames);
	Animate* animate = Animate::create(animation);
	animate->getAnimation()->setRestoreOriginalFrame(true);
	animate->setDuration(0.50f);
	animate->setTarget(this->_pacman);

	this->moveRight = animate;
	this->moveRight->retain();
}
void GameScene::setupMoveLeftAnim()
{
	Vector<SpriteFrame*> frames = getAnimation("pac_right%d.png", 2);
	Animation* animation = Animation::createWithSpriteFrames(frames);
	Animate* animate = Animate::create(animation);
	animate->getAnimation()->setRestoreOriginalFrame(true);
	animate->setDuration(0.50f);
	animate->setTarget(this->_pacman);

	this->moveLeft = animate;
	this->moveLeft->retain();
}
void GameScene::setupAnimations() 
{
	setupMoveDownAnim();
	setupMoveUpAnim();
	setupMoveRightAnim();
	setupMoveLeftAnim();
	moveRight->startWithTarget(_pacman);
}

/////////////////////////
//KEYBOARD HANDLING	/////
/////////////////////////
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

/////////////////////////
//VELOCITY HANDLING
/////////////////////////
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

/////////////////////////
//GAME STATE UPDATE
/////////////////////////
void GameScene::updatePlayerSprite(float delta, EventKeyboard::KeyCode direction)
{
	if (_player->state == Player::State::MOVING)
	{
		switch (direction)
		{
		case UP_ARROW:
			if (moveUp->isDone())
				moveUp->startWithTarget(_pacman);
			moveUp->step(delta);
			break;
		case DOWN_ARROW:
			if (moveDown->isDone())
				moveDown->startWithTarget(_pacman);
			moveDown->step(delta);
			break;
		case RIGHT_ARROW:
			if (moveRight->isDone())
				moveRight->startWithTarget(_pacman);
			moveRight->step(delta);
			break;
		case LEFT_ARROW:
			if (moveLeft->isDone())
				moveLeft->startWithTarget(_pacman);
			moveLeft->step(delta);
			break;
		}
	}
	else if (_player->state == Player::State::IDLE)
	{
		switch (direction)
		{
		case UP_ARROW:
			_pacman->setSpriteFrame(Sprite::createWithSpriteFrameName("pac_up1.png")->getSpriteFrame());
			break;
		case DOWN_ARROW:
			_pacman->setSpriteFrame(Sprite::createWithSpriteFrameName("pac_down1.png")->getSpriteFrame());
			break;
		case RIGHT_ARROW:
			_pacman->setSpriteFrame(Sprite::createWithSpriteFrameName("pac_right1.png")->getSpriteFrame());
			break;
		case LEFT_ARROW:
			_pacman->setSpriteFrame(Sprite::createWithSpriteFrameName("pac_left1.png")->getSpriteFrame());
			break;
		}
	}
}
void GameScene::updateScene(float delta) 
{
	updatePlayer(delta);
}
void GameScene::updatePlayer(float delta)
{
	//a key is pressed
	if (!heldKeys.empty())
	{
		if (_player->grounded) {
			_player->state = Player::State::MOVING;
		}

		auto direction = this->whichDirection();

		//handle velocity: on pressing keys will set max velocity to player
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
		
		//if next position is not path tile, set the velocity to ZERO
		
		// unscale the velocity by the inverse delta time and set
		// the latest position
		_player->velocity = _player->velocity * delta;
		_player->position = _player->position + _player->velocity;
		_player->velocity = _player->velocity * 1 / delta;

		if (direction == RIGHT_ARROW || direction == LEFT_ARROW)
		{
			_player->velocity.x *= DAMPING;
			Point newPosition = _level->positionToTileCoordinate(_pacman->getContentSize(),
				_pacman->getPosition() + _player->velocity);
			if (!_level->isValidPath(newPosition))
				_player->velocity.x = 0;
			_pacman->setPositionX(_pacman->getPositionX() + _player->velocity.x);
		}
		if (direction == UP_ARROW || direction == DOWN_ARROW)
		{
			_player->velocity.y *= DAMPING;
			Point newPosition = _level->positionToTileCoordinate(_pacman->getContentSize(),
				_pacman->getPosition() + _player->velocity);
			if (!_level->isValidPath(newPosition))
				_player->velocity.y = 0;
			_pacman->setPositionY(_pacman->getPositionY() + _player->velocity.y);
		}
		//update animation
		updatePlayerSprite(delta, direction);
	}
	else
	{
		_player->state = Player::State::IDLE;
	}
}