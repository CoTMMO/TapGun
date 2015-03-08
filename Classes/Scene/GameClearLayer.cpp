
#include "cocos2d.h"

#include "Scene/GameClearLayer.h"
#include "Scene/TitleScene.h"
#include "System/Sound.h"

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
