
#include "cocos2d.h"

#include "Platform.h"
#include "Base/Sprite3D.h"
#include "Scene/TitleScene.h"
#include "Scene/GameScene.h"
#include "Scene/CreditScene.h"
#include "System/EnemySettingFile.h"
#include "System/ResourceLoader.h"
#include "System/Sound.h"

USING_NS_CC;
using namespace TapGun;

Scene* TitleScene::createScene( void)
{
	auto scene = Scene::create();
	auto layer = TitleScene::create();
	scene -> addChild( layer);
	return scene;
}

bool TitleScene::init( void)
{
	if( !Layer::init()) { return false; }

	// �`��V�[���̃t���O���`�[�����S�ɐݒ�
	menuFlag = TeamLogo;

	// �e��p�����[�^��������
	logoWaitCount = 0;
	teamLogoState = LogoIn;
	menuActionFlag = false;
	alphaCount = 0;
	logoAlphaFlag = false;
	logoAlphaCount = 0;
	logoAlphaWaitCount = 0;

	// ���\�[�X�t�@�C���̓ǂݍ��݂Ə�����
	loadPicture();
	loadSound();
	setSprite();
	setMenu();

	// �^�b�`���͎󂯎��C�x���g���쐬
	auto listener = EventListenerTouchOneByOne::create();
	listener -> setSwallowTouches( _swallowsTouches);
	listener -> onTouchBegan = CC_CALLBACK_2( TitleScene::onTouchBegan, this);
	listener -> onTouchMoved = CC_CALLBACK_2( TitleScene::onTouchMoved, this);
	listener -> onTouchEnded = CC_CALLBACK_2( TitleScene::onTouchEnded, this);
	_eventDispatcher -> addEventListenerWithSceneGraphPriority( listener, this);

	// update�֐����Ă΂��悤�ɃX�P�W���[�����Z�b�g
	scheduleUpdate();

	return true;
}

void TitleScene::update( float delta)
{
	// �T�E���h�N���X�̃C���X�^���X���擾
	auto sound = Sound::getInstance();

	loadModels();

	// ���݂̕`��V�[���t���O�ɏ]���ĕ`�揈��
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

bool TitleScene::onTouchBegan( Touch *pTouch, Event *pEvent) { return true; }
void TitleScene::onTouchMoved( Touch *pTouch, Event *pEvent) {}

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

void TitleScene::setSprite( void)
{
	auto visibleSize = Director::getInstance() -> getVisibleSize();
	// �t�@�C���p�X����N���X�̃C���X�^���X���擾
	auto access = FileAccess::getInstance();

	teamLogo = Sprite::create( access -> getPicturePath( "ty.png"));
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

void TitleScene::menuStartCallback( Ref* pSender)
{
	if( !menuActionFlag) { return; }
	auto sound = Sound::getInstance();
	sound -> stopBGM();
	sound -> loadBGM( "Main01");
	sound -> playSE( "MoveSE");
	auto scene = GameScene::CreateScene();
	auto tran = TransitionCrossFade::create( 1, scene);
	Director::getInstance() -> replaceScene( tran);
}

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

void TitleScene::loadPicture( void)
{
	static bool loadFlag = false;

	if( loadFlag) { return; }

	// �e�N�X�`���A�g���X�̓ǂݍ���
	auto cache = SpriteFrameCache::getInstance();
	// �t�@�C���p�X����N���X�̃C���X�^���X���擾
	auto access = FileAccess::getInstance();

	// �X�v���C�g�V�[�g���ꊇ�ǂݍ���
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

void TitleScene::loadModels( void)
{
	// ���\�[�X�t�@�C���ǂݍ��݃t���[���̐���J�E���^
	static unsigned int frame = 0;

	// �t�@�C���p�X����N���X�̃C���X�^���X���擾
	auto access = FileAccess::getInstance();

	// �e�탂�f���f�[�^�̓ǂݍ���
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

void TitleScene::loadSound( void)
{
	auto sound = Sound::getInstance();
	// �t�@�C���p�X����N���X�̃C���X�^���X���擾
	auto access = FileAccess::getInstance();

	sound -> loadBGM( "Title");
	sound -> loadSE( "Title/MoveSE");
	sound -> loadSE( "Gun/Shot");
	sound -> loadSE( "Gun/Reload");
}

template<class P> bool TitleScene::checkFlag( P* flag, const P number) { return ( ( *flag & number) != 0); }
template<class P> void TitleScene::setFlag( P* flag, const P number) { *flag |= number; }
template<class P> void TitleScene::resetFlag( P* flag, const P number) { *flag &= ~number; }

template bool TitleScene::checkFlag( unsigned char* a, const unsigned char b);
template void TitleScene::setFlag( unsigned char* a, const unsigned char b);
template void TitleScene::resetFlag( unsigned char* a, const unsigned char b);
