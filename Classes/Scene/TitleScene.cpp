
#include "cocos2d.h"

#include "Platform.h"
#include "Base/Sprite3D.h"
#include "Scene/TitleScene.h"
#include "Scene/GameScene.h"
#include "Scene/CreditScene.h"
#include "System/ResourceLoader.h"
#include "System/Sound.h"

USING_NS_CC;
using namespace TapGun;
 
/**
*	タイトルシーンの作成
*
*	@author	minaka
*	@param	なし
*	@date	2/19 Ver 1.0
*/
Scene* TitleScene::createScene()
{
	auto scene = Scene::create();
	auto layer = TitleScene::create();
	scene -> addChild( layer);
	return scene;
}


/**
*	タイトルシーンの初期化
*
*	@author	minaka
*	@param	なし
*	@return 初期化成功時 true 失敗時 false
*	@date	2/19 Ver 1.0
*/
bool TitleScene::init()
{
	if (!Layer::init()) { return false; }
	
	// 描画シーンのフラグをチームロゴに設定
	menuFlag = TeamLogo;

	// 各種パラメータを初期化
	logoWaitCount = 0;
	teamLogoState = LogoIn;
	menuActionFlag = false;
	alphaCount = 0;
	logoAlphaFlag = false;
	logoAlphaCount = 0;
	logoAlphaWaitCount = 0;
	
	// リソースファイルの読み込みと初期化
	loadPicture();
	loadSound();
	setSprite();
	setMenu();

	// タッチ入力受け取りイベントを作成
	auto listener = EventListenerTouchOneByOne::create();
	listener -> setSwallowTouches( _swallowsTouches);	
	listener -> onTouchBegan = CC_CALLBACK_2( TitleScene::onTouchBegan, this);
	listener -> onTouchMoved = CC_CALLBACK_2( TitleScene::onTouchMoved, this);
	listener -> onTouchEnded = CC_CALLBACK_2( TitleScene::onTouchEnded, this);
	_eventDispatcher -> addEventListenerWithSceneGraphPriority( listener, this);
	
	// update関数が呼ばれるようにスケジュールをセット
	scheduleUpdate();

	return true;
}

/**
*	タイトルシーンの更新
*
*	@author	minaka
*	@param	cocos依存なので省略
*	@return なし
*	@date	2/19 Ver 1.0
*/
void TitleScene::update( float delta)
{
	// サウンドクラスのインスタンスを取得
	auto sound = Sound::getInstance();

	loadModels();

	// 現在の描画シーンフラグに従って描画処理
	switch( menuFlag)
	{
	case TeamLogo:
		teamLogoAction();
		break;

	case TitleLogoIn:
		alphaCount += 4;
		for( auto &p : sprite) { p -> setOpacity( alphaCount); }
		if( alphaCount > 180)
		{
			menuFlag = TitleLogoOK;
			sound -> playBGMLoop();
		}
		break;

	case TitleLogoOK:
		sprite[Menu] -> setVisible( true);
		if( logoAlphaFlag) 
		{ 
			sprite[Menu] -> setOpacity( logoAlphaCount);
			if( logoAlphaCount == 0) { logoAlphaWaitCount++; }
			else { logoAlphaCount -= 5; }
		}
		else
		{
			sprite[Menu] -> setOpacity( logoAlphaCount);
			if( logoAlphaCount == 250) { logoAlphaWaitCount++; }
			else { logoAlphaCount += 5; }
		}
		if( ( logoAlphaCount == 250 && logoAlphaWaitCount == 20) || ( logoAlphaCount == 0 && logoAlphaWaitCount == 5)) 
		{
			logoAlphaFlag = !logoAlphaFlag;
			logoAlphaWaitCount = 0;
		}
		break;

	case MenuIn:
		break;

	case MenuOK:
		break;

	case TitleEnd:
		break;

	default:
		log( "Title : FlagError");
		break;
	}
}

// 以下未使用入力受け取り関数
bool TitleScene::onTouchBegan( Touch *pTouch, Event *pEvent) { return true; }
void TitleScene::onTouchMoved( Touch *pTouch, Event *pEvent) {}

/**
*	タッチ入力の受け取り (離されたとき)
*
*	@author	minaka
*	@param	cocos依存なので省略
*	@return なし
*	@date	2/19 Ver 1.0
*/
void TitleScene::onTouchEnded( Touch *pTouch, Event *pEvent)
{
	auto sound = Sound::getInstance();

	if( menuFlag == TitleLogoOK)
	{ 
		menuFlag = MenuIn;
		sprite[Logo] -> runAction( MoveTo::create( 1, Point( 3000, sprite[Logo] -> getPositionY())));
		auto action = Blink::create( 0.2, 3);
		sound -> playSE( "MoveSE");
		auto func = CallFunc::create( [&](void) -> void 
		{ 
			sprite[Menu] -> setVisible( false); 
			menuAction();
		});
		sprite[Menu] -> runAction( Sequence::create( action, func, NULL));
	}
}

/**
*	チームロゴの描画制御
*
*	@author	minaka
*	@param	なし
*	@return なし
*	@date	2/19 Ver 1.0
*/
void TitleScene::teamLogoAction( void)
{
	if( teamLogoState == LogoIn)
	{
		alphaCount += AlphaValue;
		if( teamLogo -> getOpacity() == AlphaValue * 50) { teamLogoState = LogoOut; }
		teamLogo -> setOpacity( alphaCount);
	}
	else if( teamLogoState == LogoOut)
	{
		if( logoWaitCount > WaitTime) { alphaCount -= AlphaValue; }
		else { logoWaitCount++; }
		if( teamLogo -> getOpacity() == 0) 
		{
			logoWaitCount = 0;
			teamLogoState = Wait;
			teamLogo -> setVisible( false);
		}		
		teamLogo -> setOpacity( alphaCount);
	}
	else if( teamLogoState == Wait)
	{
		if( logoWaitCount > 50) 
		{ 
			alphaCount = 0;
			menuFlag = TitleLogoIn;
		}
		logoWaitCount++;
	}
}

/**
*	タイトルシーン用のスプライト作成
*
*	@author	minaka
*	@param	なし
*	@return なし
*	@date	2/19 Ver 1.0
*/
void TitleScene::setSprite( void)
{
	auto visibleSize = Director::getInstance() -> getVisibleSize();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	teamLogo = Sprite::create( "ty.png");
#else
	teamLogo = Sprite::create( "Graph/Pictures/ty.png");
#endif
	teamLogo -> setPosition( Vec2( visibleSize.width / 2, visibleSize.height / 2));
	teamLogo -> setOpacity( 0);
	addChild( teamLogo);

	sprite[BG] = Sprite::createWithSpriteFrameName( "title_haikei.png");
	sprite[Frame] = Sprite::createWithSpriteFrameName( "title_waku.png");
	sprite[Logo] = Sprite::createWithSpriteFrameName( "title_log.png");
	sprite[Menu] = Sprite::createWithSpriteFrameName( "title_touchscreen.png");
	
	for( auto &p : sprite)
	{
		p -> setPosition( Vec2( visibleSize.width / 2, visibleSize.height / 2));
		p -> setOpacity( 0);
		addChild( p);
	}
	sprite[Menu] -> setVisible( false);
}

/**
*	タイトルシーン用のメニューの作成
*
*	@author	minaka
*	@param	なし
*	@return なし
*	@date	2/19 Ver 1.0
*/
void TitleScene::setMenu( void)
{
	auto visibleSize = Director::getInstance() -> getVisibleSize();

	auto item = MenuItemSprite::create( Sprite::createWithSpriteFrameName( "title_start.png"), 
				Sprite::createWithSpriteFrameName( "title_start_push.png"), CC_CALLBACK_1( TitleScene::menuStartCallback, this));
	menu[Start] = Menu::create( item, NULL);
	menu[Start] -> setPosition( Vec2( -1200, visibleSize.height - 270));
	addChild( menu[Start]);

	item = MenuItemSprite::create( Sprite::createWithSpriteFrameName( "title_end.png"), 
				Sprite::createWithSpriteFrameName( "title_end_push.png"), CC_CALLBACK_1( TitleScene::menuEndCallback, this));
	menu[End] = Menu::create( item, NULL);
	menu[End] -> setPosition( Vec2( -1200, visibleSize.height / 2));
	addChild( menu[End]);
	
	item = MenuItemSprite::create( Sprite::createWithSpriteFrameName( "title_credit.png"), 
				Sprite::createWithSpriteFrameName( "title_credit_push.png"), CC_CALLBACK_1( TitleScene::menuCreditCallback, this));
	menu[Credit] = Menu::create( item, NULL);
	menu[Credit] -> setPosition( Vec2( -1200, visibleSize.height - 530));
	addChild( menu[Credit]);
}

/**
*	メニューの移動処理
*
*	@author	minaka
*	@param	なし
*	@return なし
*	@date	2/19 Ver 1.0
*/
void TitleScene::menuAction( void)
{
	auto visibleSize = Director::getInstance() -> getVisibleSize();

	for( auto &p : menu)
	{
		auto action = MoveTo::create( 0.8, Vec2(  visibleSize.width / 2, p -> getPositionY()));
		p -> runAction( action);
		auto func = CallFunc::create( [&](void) -> void { menuActionFlag = true; });
		p -> runAction( Sequence::create( action, func, NULL));
	}
}

/**
*	GameStartのコールバック関数
*
*	@author	minaka
*	@param	cocos依存なので省略
*	@return なし
*	@date	2/19 Ver 1.0
*/
void TitleScene::menuStartCallback( Ref* pSender)
{
	if( !menuActionFlag) { return; }
	auto sound = Sound::getInstance();
	sound -> stopBGM();
	sound -> releaseBGM();
	sound -> loadBGM( "Main01");
	sound -> playSE( "MoveSE");
	auto scene = GameScene::CreateScene();
	auto tran = TransitionCrossFade::create( 1, scene);
	Director::getInstance() -> replaceScene( tran);
}

/**
*	GameEndのコールバック関数
*
*	@author	minaka
*	@param	cocos依存なので省略
*	@return なし
*	@date	2/19 Ver 1.0
*/
void TitleScene::menuEndCallback( Ref* pSender)
{
	if( !menuActionFlag) { return; }
	auto sound = Sound::getInstance();
	sound -> stopBGM();
	sound -> playSE( "MoveSE");
	Director::getInstance() -> end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

/**
*	クレジットのコールバック関数
*
*	@author	minaka
*	@param	cocos依存なので省略
*	@return なし
*	@date	2/19 Ver 1.0
*/
void TitleScene::menuCreditCallback( Ref* pSender)
{
	if( !menuActionFlag) { return; }
	auto sound = Sound::getInstance();
	sound -> stopBGM();
	sound -> playSE( "MoveSE");
	auto scene = CreditScene::createScene();
	auto tran = TransitionCrossFade::create( 1, scene);
	Director::getInstance() -> replaceScene( tran);
}

/**
*	各種画像データの読み込み
*
*	@author	minaka
*	@param	cocos依存なので省略
*	@return なし
*	@date	2/19 Ver 1.0
*/
void TitleScene::loadPicture( void)
{
	static bool loadFlag = false;

	if( loadFlag) { return; }

	// テクスチャアトラスの読み込み
	auto cache = SpriteFrameCache::getInstance();
	// ファイルパス制御クラスのインスタンスを取得
	auto access = FileAccess::getInstance();

	// スプライトシートを一括読み込み
	cache -> addSpriteFramesWithFile( access -> getPicturePath( "SpriteSheet/Title.plist"));
	cache -> addSpriteFramesWithFile( access -> getPicturePath( "SpriteSheet/P_Hit.plist"));
	cache -> addSpriteFramesWithFile( access -> getPicturePath( "SpriteSheet/E_Hit.plist"));
	cache -> addSpriteFramesWithFile( access -> getPicturePath( "SpriteSheet/Logo.plist"));
	cache -> addSpriteFramesWithFile( access -> getPicturePath( "SpriteSheet/Number.plist"));
	cache -> addSpriteFramesWithFile( access -> getPicturePath( "SpriteSheet/HPGauge.plist"));
	cache -> addSpriteFramesWithFile( access -> getPicturePath( "SpriteSheet/continue.plist"));
	cache -> addSpriteFramesWithFile( access -> getPicturePath( "SpriteSheet/Reticle.plist"));
	cache -> addSpriteFramesWithFile( access -> getPicturePath( "SpriteSheet/Enemy_Attack.plist"));

	loadFlag = true;
}

/**
*	各種モデルデータの読み込み
*
*	@author	minaka
*	@param	cocos依存なので省略
*	@return なし
*	@date	2/19 Ver 1.0
*/
void TitleScene::loadModels( void)
{
	// リソースファイル読み込みフレームの制御カウンタ
	static unsigned int frame = 0;

	// 各種モデルデータの読み込み
	if( frame == ResourceLoader::Map)
	{
		ResourceLoader::getInstance() -> loadModel( "Stage/stage");
	}
	else if( frame >= ResourceLoader::EnemyStart && frame <= ResourceLoader::EnemyEnd)
	{
		ResourceLoader::getInstance() -> loadModel( "Enemy/enemy", "Enemy.anime");
	}
	else if( frame >= ResourceLoader::BulletStart && frame <= ResourceLoader::BulletEnd)
	{
		ResourceLoader::getInstance() -> loadModel( "Bullet/tama");
	}
	else if( frame == ResourceLoader::Player)
	{
		ResourceLoader::getInstance() -> loadModel( "Player/player", "Player.anime");
	}
	frame++;
}

/**
*	各種サウンドデータの読み込み
*
*	@author	minaka
*	@param	cocos依存なので省略
*	@return なし
*	@date	2/19 Ver 1.0
*/
void TitleScene::loadSound( void)
{
	auto sound = Sound::getInstance();

	sound -> loadBGM( "Title");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	sound -> loadSE( "MoveSE");
	sound -> loadSE( "Shot");
	sound -> loadSE( "Reload");
#else
	sound -> loadSE( "Title/MoveSE");
	sound -> loadSE( "Gun/Shot");
	sound -> loadSE( "Gun/Reload");
#endif
}

// 以下フラグ制御関数
template<class P> bool TitleScene::checkFlag( P* flag, const P number) { return ( ( *flag & number) != 0); }
template<class P> void TitleScene::setFlag( P* flag, const P number) { *flag |= number; }
template<class P> void TitleScene::resetFlag( P* flag, const P number) { *flag &= ~number; }

template bool TitleScene::checkFlag( unsigned char* a, const unsigned char b);
template void TitleScene::setFlag( unsigned char* a, const unsigned char b);
template void TitleScene::resetFlag( unsigned char* a, const unsigned char b);
