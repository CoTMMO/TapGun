
#include "cocos2d.h"

#include "Scene/ContinueLayer.h"
#include "Scene/GameScene.h"
#include "Scene/TitleScene.h"
#include "System/Sound.h"

USING_NS_CC;
using namespace TapGun;

bool ContinueLayer::init( void)
{
	if (!Layer::init()) { return false; }

	auto visibleSize = Director::getInstance() -> getVisibleSize();

	auto item = MenuItemSprite::create( Sprite::createWithSpriteFrameName( "continue_yes.png"), 
				Sprite::createWithSpriteFrameName( "continue_yes_push.png"), CC_CALLBACK_1( ContinueLayer::menuStartCallback, this));
	menu[Restart] = cocos2d::Menu::create( item, NULL);
	menu[Restart] -> setPosition( Vec2( visibleSize.width / 2, visibleSize.height / 2));
	addChild( menu[Restart], 2);

	item = MenuItemSprite::create( Sprite::createWithSpriteFrameName( "continue_no.png"), 
				Sprite::createWithSpriteFrameName( "continue_no_push.png"), CC_CALLBACK_1( ContinueLayer::menuStartCallback, this));
	menu[End] = cocos2d::Menu::create( item, NULL);
	menu[End] -> setPosition( Vec2( visibleSize.width / 2, visibleSize.height / 2));
	menu[End] -> setContentSize( Size( 460, 150));
	addChild( menu[End], 2);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	sprite[BG] = Sprite::create( "gameoverBG.png");
#else
	sprite[BG] = Sprite::create( "Graph/Pictures/gameoverBG.png");
#endif
	sprite[BG] -> setPosition( Vec2( visibleSize.width / 2, visibleSize.height / 2));
	addChild( sprite[BG], 1);

	sprite[Logo] = Sprite::createWithSpriteFrameName( "continue_logo.png");
	sprite[Logo] -> setPosition( Vec2( visibleSize.width / 2, visibleSize.height / 2));
	addChild( sprite[Logo], 2);

	return true;
}

void ContinueLayer::update( float delta)
{

}

void ContinueLayer::moveTime( float delta)
{

}

void ContinueLayer::menuStartCallback( Ref* pSender)
{
	
}

void ContinueLayer::menuEndCallback( Ref* pSender)
{

}
