
#include <sstream>
#include <iostream>
#include "TestScene.h"
#include "SimpleAudioEngine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "Sprite3D.h"
#include "Errorfunc.h"
#include "Sound.h"
#include "UI.h"
#include "Effect.h"

#else

#include "Base/UI.h"
#include "Base/Sprite3D.h"
#include "Object/Effect.h"
#include "System/Directory.h"
#include "System/Errorfunc.h"
#include "System/Sound.h"
#include "Object/Effect.h"
#include "System/ResourceLoader.h"
#include "Scene/ContinueLayer.h"

#endif

USING_NS_CC;
//using namespace ADX2;
using namespace std;
using namespace TapGun;
using namespace CocosDenshion;

class TestTT
{
public:
	_Sprite3D* sprite3d;
};

TestTT *ts;

Sprite *hp[8];
bool flag[8];

Scene* Test::createScene()
{
	auto scene = Scene::create();
	auto layer = Test::create();
	scene -> addChild( layer);
	return scene;
}

bool Test::init()
{
	if ( !Layer::init()) { return false; }

	auto cache = SpriteFrameCache::getInstance();

	cache -> addSpriteFramesWithFile( "Graph/Pictures/SpriteSheet/continue.plist");

	auto con = ContinueLayer::create();
//	addChild( con);

	auto sprit = _Sprite3D::create( "Enemy/enemy");
	sprit -> setCullFace( GL_FRONT);
	sprit -> setPosition( 640, 0);
	sprit -> setScale( 300.0f);
	addChild( sprit);
	

	//現在はタッチイベントのリスナーをここに用意しています
	auto dispatcher = Director::getInstance()->getEventDispatcher();

	listener = EventListenerTouchOneByOne::create();
	listener -> onTouchBegan = CC_CALLBACK_2( Test::onTouchBegan, this);
	listener -> onTouchMoved = CC_CALLBACK_2( Test::onTouchMoved, this);
	listener -> onTouchEnded = CC_CALLBACK_2( Test::onTouchEnded, this);

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	scheduleUpdate();
	schedule( schedule_selector(Test::moveTime), 0.016f);

	return true;
}

void Test::update( float delta)
{

}

void Test::moveTime( float delta)
{

}

bool Test::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{

	return true;
}


void Test::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
}

void Test::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
}
