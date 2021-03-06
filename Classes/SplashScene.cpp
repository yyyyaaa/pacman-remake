#include "SplashScene.h"

USING_NS_CC;

Scene* SplashScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SplashScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //add loading screen
	auto loading = Sprite::create("loading.jpg");
	loading->setScaleX(visibleSize.width / loading->getContentSize().width );
	loading->setScaleY(visibleSize.height / loading->getContentSize().height );
	loading->setPosition(visibleSize.width/2, visibleSize.height/2);
	this->addChild(loading);
    
	this->scheduleOnce(schedule_selector(SplashScene::goToGameScene), TRANSITION_TIME);
    return true;
}


void SplashScene::goToGameScene(float dt)
{
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(dt, scene));
}
