
#include <sstream>
#include <iostream>
#include "TestScene.h"
#include "SimpleAudioEngine.h"
#include "Platform.h"


#include "Base/UI.h"
#include "Base/Sprite3D.h"
#include "Object/Effect.h"
#include "System/Sound.h"
#include "Object/Effect.h"
#include "System/ResourceLoader.h"
#include "System/EnemySettingFile.h"
#include "Scene/ContinueLayer.h"


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

_Sprite3D* pl;

Scene* Test::createScene()
{
	auto scene = Scene::create();
	auto layer = Test::create();
	scene -> addChild( layer);
	return scene;
}

bool Test::init()
{
	if (!Layer::init()) { return false; }

	//	auto cache = SpriteFrameCache::getInstance();
	//
	//	cache -> addSpriteFramesWithFile( "Graph/Pictures/SpriteSheet/continue.plist");
	//
	//	auto con = ContinueLayer::create();
	////	addChild( con);
	//
	//	auto sprit = _Sprite3D::create( "Enemy/enemy");
	//	sprit -> setCullFace( GL_FRONT);
	//	sprit -> setPosition( 640, 0);
	//	sprit -> setScale( 300.0f);
	//	addChild( sprit);
	//
	//
		auto cache = SpriteFrameCache::getInstance();
	// ファイルパス制御クラスのインスタンスを取得
	auto access = FileAccess::getInstance();

	// スプライトシートを一括読み込み
	cache -> addSpriteFramesWithFile( access -> getPicturePath( "SpriteSheet/Title.plist"));
	cache -> addSpriteFramesWithFile( access -> getPicturePath( "SpriteSheet/P_Hit.plist"));
	cache -> addSpriteFramesWithFile( access -> getPicturePath( "SpriteSheet/E_Hit.plist"));
		//現在はタッチイベントのリスナーをここに用意しています
		auto dispatcher = Director::getInstance()->getEventDispatcher();
	
		listener = EventListenerTouchOneByOne::create();
		listener -> onTouchBegan = CC_CALLBACK_2( Test::onTouchBegan, this);
		listener -> onTouchMoved = CC_CALLBACK_2( Test::onTouchMoved, this);
		listener -> onTouchEnded = CC_CALLBACK_2( Test::onTouchEnded, this);
	
		dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	scheduleUpdate();
	//	schedule( schedule_selector(Test::moveTime), 0.016f);

	EnemySettingFile::create("sample.csv");

	pl = _Sprite3D::create("Enemy/enemy", "Enemy.anime");

	pl->setPosition3D(Vec3(640, 50, 3));
	pl->setRotation3D(Vec3(0, 0, 0));
	pl->setScale(300);
	addChild(pl);
	return true;
}

void Test::update( float delta)
{
	Vec3 rot = pl->getRotation3D();
	rot.y += delta * 100.0f;
//	pl->setRotation3D(rot);
	Effect::getInstance() -> muzzleUpdate();
}

void Test::moveTime( float delta)
{

}

bool Test::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	Effect::getInstance() -> setEnemyMuzzle( pl, "Po_1", "Po_2");

	return true;
}


void Test::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
}

void Test::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
}
