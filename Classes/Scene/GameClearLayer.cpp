
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "GameClearLayer.h"
#include "TitleScene.h"
#include "Sound.h"

#else

#include "Scene/GameClearLayer.h"
#include "Scene/TitleScene.h"
#include "System/Sound.h"

#endif

USING_NS_CC;
using namespace TapGun;

bool GameClearLayer::init( void)
{
	if (!Layer::init()) { return false; }

	auto visibleSize = Director::getInstance() -> getVisibleSize();

	return true;
}

void GameClearLayer::menuEndCallback( Ref* pSender)
{
	auto scene = TitleScene::createScene();
	auto tran = TransitionCrossFade::create( 1, scene);
	Director::getInstance() -> replaceScene( tran);
}
