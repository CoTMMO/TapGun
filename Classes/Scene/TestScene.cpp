
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

	cache -> addSpriteFramesWithFile( "HPGauge.plist");
	cache -> addSpriteFramesWithFile( "Number.plist");

	billboard = BillBoard::create();

	//現在はタッチイベントのリスナーをここに用意しています
	auto dispatcher = Director::getInstance()->getEventDispatcher();

	listener = EventListenerTouchOneByOne::create();
	listener -> onTouchBegan = CC_CALLBACK_2( Test::onTouchBegan, this);
	listener -> onTouchMoved = CC_CALLBACK_2( Test::onTouchMoved, this);
	listener -> onTouchEnded = CC_CALLBACK_2( Test::onTouchEnded, this);

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	scheduleUpdate();
	schedule( schedule_selector(Test::moveTime), 0.016f);

	ResourceLoader::getInstance() -> loadModel( "Player/player", "", "Player.anime");

	ts = new TestTT;

	return true;
}

void Test::update( float delta)
{
//	Manager::getInstance() -> update();
	static int count = 0;
//	sprite3d -> setRotation3D(( Vec3( 0.0f, count, 0.0f)));
	Effect::getInstance() -> muzzleUpdate();

	if( count == 100)
	{
		ts -> sprite3d = ResourceLoader::getInstance() -> getSprite3D( ResourceLoader::Player);
		ts -> sprite3d -> setPosition3D( Vec3( 640, 200, -100));
		ts -> sprite3d -> setRotation3D( Vec3( 0.0f, 0.0f, 0.0f));
		ts -> sprite3d -> setScale( 200.0f);
		addChild( ts -> sprite3d);
	}

	count++;
}

void Test::moveTime( float delta)
{

}

bool Test::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
//	setEnemyHitEffect( (Sprite3D*)sprite3d);
//	Effect::getInstance() -> setEnemyHitEffect( sprite3d);
	
	Effect::getInstance() -> setPlayerMuzzle( ts -> sprite3d, "po_");
	ts -> sprite3d->stopAllActions();
	ts -> sprite3d -> startAnimation( "shot");
	return true;
}


void Test::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
}

void Test::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
}

void Test::setEnemyHitEffect( Sprite3D* sprite3d)
{
	auto cache = SpriteFrameCache::getInstance();

	auto animation = Animation::create();

	for( int i = 0; i < 8; i++)
	{
		char buf[256];
		sprintf( buf, "E_Hit%02d.png", i + 1);
		animation -> addSpriteFrame( cache -> getSpriteFrameByName( buf));
	}

	animation -> setDelayPerUnit( 0.05f);
	animation -> setRestoreOriginalFrame( true);

	auto action = Animate::create( animation);

	auto callfunc = CallFunc::create( [&](void)->void
	{
		billboard -> removeFromParent();
	});

	auto sequence = Sequence::create( action, callfunc, NULL);

	billboard -> runAction( sequence);

	billboard -> setPosition3D( Vec3( 1, 0.5, 1));

	billboard -> setScale( 0.003f);
	sprite3d -> addChild( billboard);
}
