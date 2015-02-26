
#include <sstream>
#include <iostream>
#include "TestScene.h"
#include "SimpleAudioEngine.h"
#include "Define.h"
//#include "Main.h"

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
#endif

USING_NS_CC;
//using namespace ADX2;
using namespace std;
using namespace TapGun;
using namespace CocosDenshion;

_Sprite3D* sprite3d;

Sprite *hp[8];
bool flag[8];

Scene* Test::createScene()
{
	SystemValue::windowSize = Director::getInstance() -> getVisibleSize();
	SystemValue::origin = Director::getInstance() -> getVisibleOrigin();
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


	for( auto &p : flag) p = false;

	auto Z = Node::create();

	hp[0] = Sprite::createWithSpriteFrameName( "HPFrame.png");
	hp[1] = Sprite::createWithSpriteFrameName( "HPGaugeBG.png");
	Z -> addChild( hp[0],1);
	for( int i = 2; i < 8; i++)
	{
		char buf[64];
		sprintf( buf, "HPGauge_%02d.png", i - 1);
		hp[i] = Sprite::createWithSpriteFrameName( buf);
		hp[i] -> setPosition( Vec2( 640, 700));
		hp[i] -> setScale( 0.8);

		Z -> addChild( hp[i],2);
	}

	Z -> addChild( hp[1],1);
	hp[0] -> setPosition( Vec2( 640, 700));
	addChild( Z,1);
	hp[1] -> setPosition( Vec2( 640, 700));


	hp[0] -> setScale( 0.8);
	hp[1] -> setScale( 0.8);

	scheduleUpdate();
	schedule( schedule_selector(Test::moveTime), 0.016f);



	sprite3d = _Sprite3D::create( "Enemy/enemy.c3t", "Enemy.anime");
	sprite3d -> setPosition3D( Vec3( 640, 200, -100));
	sprite3d -> setRotation3D( Vec3( 0.0f, 0.0f, 0.0f));
	sprite3d -> setScale( 200.0f);
//	addChild( sprite3d);

	return true;
}

void Test::update( float delta)
{
//	Manager::getInstance() -> update();
	static int count = 0;
//	sprite3d -> setRotation3D(( Vec3( 0.0f, count, 0.0f)));
//	Effect::getInstance() -> muzzleUpdate();

	static float sca = 1.0;
	static int pos = 640;
	static float rot = 0;


	for( int i = 0; i < 8; i++)
	{
		if(flag[i])
		{
			switch( i)
			{
			case 2:
					if( count == 0)
					{
						sca = 1.0;
						pos = 640;
					}
					if( count == 4)
					{
						flag[i] = false;
						count = 0;
						break;
					}
					sca -= 0.25;
					hp[i] -> setScaleX( sca);
					pos += 22;
					hp[i] -> setPositionX( pos);
					count++;

					break;
			case 3:
					if( count == 0)
					{
//						hp[i] -> setAnchorPoint( Vec2( 1.0, 0.0));
//						hp[i] -> setPosition( 804, 536);
						pos = 0;
					}
					if( count == 4)
					{
//						flag[i] = false;
//						hp[i] -> setAnchorPoint( Vec2( 0.5, 0.5));
//						count = 0;
						break;
					}
					pos += 10;

					hp[i] -> setRotation( pos * 2 + 10);
					hp[i] -> setPositionX( (hp[i] -> getPositionX() - (pos / 2)));
					hp[i] -> setPositionY( (hp[i] -> getPositionY() + (pos / 2 + 5)));
					count++;
					break;

			case 4:
					break;

			case 5:
					break;

			case 6:
					break;

			case 7:
					break;

				default:
					break;
			}
		}
	}
}

void Test::moveTime( float delta)
{

}

bool Test::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
//	setEnemyHitEffect( (Sprite3D*)sprite3d);
//	Effect::getInstance() -> setEnemyHitEffect( sprite3d);
//	sprite3d -> startAnimation( "shot");
//	Effect::getInstance() -> setEnemyMuzzle( sprite3d, "Po_1", "Po_2");
	static int i = 2;

	flag[i] = true;
	i++;
	if( i == 8) i = 2;
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
