#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__
#include "cocos2d.h"
USING_NS_CC;

#define TRANSITION_TIME 1
#define PACMAN_SPEED 20
#define SCALE_FACTOR 1.5
#define PACMAN_SCALE 1.2

const std::string MAP_NAME = "pactilemap.tmx";

const EventKeyboard::KeyCode ENTER = EventKeyboard::KeyCode::KEY_KP_ENTER;
const EventKeyboard::KeyCode UP_ARROW = EventKeyboard::KeyCode::KEY_UP_ARROW;
const EventKeyboard::KeyCode DOWN_ARROW = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
const EventKeyboard::KeyCode LEFT_ARROW = EventKeyboard::KeyCode::KEY_LEFT_ARROW;
const EventKeyboard::KeyCode RIGHT_ARROW = EventKeyboard::KeyCode::KEY_RIGHT_ARROW;

const float PLAYER_MAX_VELOCITY = 4.0f;
const float DAMPING = 0.87f;

#endif // __SPLASH_SCENE_H__
