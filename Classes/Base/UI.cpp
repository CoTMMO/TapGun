
#include "UI.h"

#include "Define.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "GameMaster.h"
#include "GameUILayer.h"

#else

#include "Base/GameMaster.h"
#include "Scene/GameUILayer.h"

#endif


using namespace TapGun;
USING_NS_CC;


/**
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
//UI::UI(void)
//{
//	初期化内容は随時更新します
//
//	管理フラグ
//	valid = FALSE;
//	kind = UIKIND_NONE;//UI種別をNONEに
//
//	変数
//	speed = 0.0f;//移動速度
//	speedVec = Vec2(0.0f, 0.0f);//移動ベクトル
//	targetPos = Vec2(0.0f, 0.0f);//移動目標
//
//	frame = 0;//管理フレーム
//}


/**
*	UIクラスのメンバ変数を初期化
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
void UI::Init(void)
{
	//初期化内容は随時更新します

	//管理フラグ
	valid = FALSE;
	kind = UIKIND_NONE;//UI種別をNONEに

	//変数
	//speed = 0.0f;//移動速度
	//speedVec = Vec3(0, 0, 0);//移動ベクトル
	//targetPos = Vec3(0, 0, 0);//移動目標

	frame = 0;//管理フレーム
}







/**
*	UIクラスのメンバ変数を初期化
*
*	@author	sasebon
*	@param	UI配列の番号,UI種別
*	@return	正常終了:1 初期化エラー:-1
*	@date	1/8 Ver 1.0
*/
int UI::Init(int num, int utype)
{
	//初期化内容は随時更新します


	//num番のUI構造体が使用されているか初期化されていない、またはUI種別が不正、または配列外の場合はエラー
	if(FALSE != valid || 0 > utype || UIKIND_NUM <= utype || 0 > num || MAX_UI <= num)
	{
		return FALSE;
	}

	//フラグの初期化
	valid = TRUE;
	kind = utype;

	//変数
	speed = 0.0f;
	//speedVec = Vec3(0, 0, 0);
	//targetPos = Vec3(0, 0, 0);

	frame = 0;//管理フレーム

	//モデルの種別によって当たり判定の設定を行う
	//敵や弾の種類（副種別）によってさらに条件分けする
	switch(utype)
	{
	case UIKIND_ARROW:

		break;
	default:

		break;
	}
}




//UI* UI::Create( void)
//{
//	auto sprite = new UI();
//	return sprite;
//}





/**
*	当たり判定の初期化
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
void UI::SetCollision(void)
{

}



/**
*	速度をもとにしたUI移動
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
void UI::UpdatePos(void)
{

}



/**
*	引数座標へのUI移動
*
*	@author	sasebon
*	@param	座標
*	@return	なし
*	@date	1/8 Ver 1.0
*/
void UI::UpdatePos(Vec2 pos)
{
}


GameUI* GameUI::getInstance()
{
	static GameUI *p = nullptr;
	if( !p) { p = new GameUI; }
	return p;
}

void GameUI::init( Layer* layer)
{
	auto cache = SpriteFrameCache::getInstance();

	hpParent = Node::create();
	hp[0] = Sprite::createWithSpriteFrameName( "HPFrame.png");
	hp[1] = Sprite::createWithSpriteFrameName( "HPGaugeBG.png");
	for( int i = 2; i < HPGauge; i++)
	{
		char buf[64];
		sprintf( buf, "HPGauge_%02d.png", i - 1);
		hp[i] = Sprite::createWithSpriteFrameName( buf);
	}
	for( auto &p : hp)
	{
		p -> setPosition( Vec2( 640, 700));
		p -> setScale( 0.8);
		hpParent -> addChild( p,2);
	}
	layer -> addChild( hpParent);
	
	timeParent = Node::create();
	for( int i = 0; i < TimeNumber; i++) // NumberSprite Count
	{
		for( int k = 0; k < 10; k++)	// NumberSprite init Loop
		{
			char buf[64];
			sprintf( buf, "%d.png", k);
			timeNumber[i][k] = Sprite::createWithSpriteFrameName( buf);
			timeNumber[i][k] -> setScale( 0.16f);
			timeNumber[i][k] -> setVisible( false);
			timeParent -> addChild( timeNumber[i][k]);
		}
	}

	for( int i = 0; i < 10; i++)
	{
		timeNumber[0][i] -> setPosition( Vec2( 560, 700));
		timeNumber[1][i] -> setPosition( Vec2( 600, 700));
		timeNumber[2][i] -> setPosition( Vec2( 640, 700));
		timeNumber[3][i] -> setPosition( Vec2( 690, 695));
		timeNumber[4][i] -> setPosition( Vec2( 720, 695));
		timeNumber[3][i] -> setScale( 0.12f);
		timeNumber[4][i] -> setScale( 0.12f);
	}
	timeNumber[0][3] -> setVisible( true);
	timeNumber[1][0] -> setVisible( true);
	timeNumber[2][0] -> setVisible( true);
	timeNumber[3][0] -> setVisible( true);
	timeNumber[4][0] -> setVisible( true);
	
	layer -> addChild( timeParent);

	bulletParent = Node::create();
	
	bullet = Sprite::createWithSpriteFrameName( "BulletFrame.png");
	bullet -> setPosition( Vec2( 640, 60));
	bullet -> setScale( 0.15f);
	bulletParent -> addChild( bullet, 1);
	
	for( int i = 0; i < BulletNumber; i++) // NumberSprite Count
	{
		for( int k = 0; k < 10; k++)	// NumberSprite init Loop
		{
			char buf[64];
			sprintf( buf, "%d_.png", k);
			bulletNumber[i][k] = Sprite::createWithSpriteFrameName( buf);
			bulletNumber[i][k] -> setVisible( false);
			bulletNumber[i][k] -> setScale( 0.15f);
			bulletParent -> addChild( bulletNumber[i][k], 3);
		}
	}
	
	for( int i = 0; i < 10; i++)
	{
		bulletNumber[0][i] -> setPosition( Vec2( 580, 55));
		bulletNumber[1][i] -> setPosition( Vec2( 615, 55));
		bulletNumber[2][i] -> setPosition( Vec2( 670, 50));
		bulletNumber[3][i] -> setPosition( Vec2( 700, 50));
		bulletNumber[2][i] -> setScale( 0.12f);
		bulletNumber[3][i] -> setScale( 0.12f);
	}
	bulletNumber[0][3] -> setVisible( true);
	bulletNumber[1][0] -> setVisible( true);
	bulletNumber[2][3] -> setVisible( true);
	bulletNumber[3][0] -> setVisible( true);
	
	layer -> addChild( bulletParent);
	
	for( int i = 0; i < Escape; i++)
	{
		escape[i] = Sprite::createWithSpriteFrameName( "Escape.png");
		escape[i] -> setScale( 0.4f);
		layer -> addChild( escape[i]);
	}
	escape[0] -> setPosition( Vec2( 130, 130));
	escape[1] -> setPosition( Vec2( 1150, 130));
	auto flip = FlipX::create(true);
	escape[1] -> runAction(flip);
	
	pause = Sprite::create();
	
	for( int i = 0; i < EnemyAttack; i++)
	{
		for( int k = 0; k < 3; k++)
		{
			char buf[64];
			sprintf( buf, "kougeki_icon_%d.png", k + 1);
			enemyAttack[i][k] = Sprite::createWithSpriteFrameName( buf);
			enemyAttack[i][k] -> setVisible( false);
			layer -> addChild( enemyAttack[i][k]);
		}
	}
	
	for( int i = 0; i < Delete; i++)
	{
		deleteLogo[i] = Sprite::createWithSpriteFrameName( "Derete.png");
		deleteLogo[i] -> setVisible( false);
		layer -> addChild( deleteLogo[i]);
	}

	logo[LogoNumber::Action] = Sprite::createWithSpriteFrameName( "action.png");
	logo[LogoNumber::Wait] = Sprite::createWithSpriteFrameName( "wait.png");
	logo[LogoNumber::Reload] = Sprite::createWithSpriteFrameName( "rerode.png");

	for( auto &p : logo)
	{
		p -> setPosition( 1280 / 2, 250);
		p -> setScale( 0.3f);
		p -> setVisible( false);
		layer -> addChild( p);
	}
}

void GameUI::update( void)
{
	auto master = GameMaster::GetInstance();
	
	setLogo();
	setHP( master -> GetPlayerHP());
	setGameTime( master -> GetGameTime());
	setBulletCount( master -> GetPlayerBullets());
		
}

void GameUI::setLogo( void)
{
	auto master = GameMaster::GetInstance();
	static bool waitFlag = false;
	static int waitTime = 0;

	if( master -> GetGameState() == GSTATE_WAIT)
	{
		if( waitTime % 30 == 0 || waitTime == 0)
		{
			waitFlag = !waitFlag;
			logo[LogoNumber::Wait] -> setVisible( waitFlag);
		}
		escape[0] -> setOpacity( 25);
		escape[1] -> setOpacity( 25);
		waitTime++;
	}
	else if( master -> GetGameState() == GSTATE_PLAY_SET)
	{
		waitFlag = false;
		logo[LogoNumber::Wait] -> setVisible(false);
	}
	else if( master -> GetGameState() == GSTATE_PLAY_ACTION)
	{
		logo[LogoNumber::Action] -> setVisible(true);
		if( master -> playerSide == PSIDE_LEFT)
		{
			escape[0] -> setOpacity( 255);
		}
		else
		{
			escape[1] -> setOpacity( 255);
		}
	}
	else if( master -> GetGameState() == GSTATE_PLAY)
	{
		logo[LogoNumber::Action] -> setVisible(false);
	}
}

void GameUI::setHP( int count)
{
	switch( count)
	{
	case 0:
		hp[7] -> setVisible( false);
		break;
	case 1:
		hp[6] -> setVisible( false);
		break;
	case 2:
		hp[5] -> setVisible( false);
		break;
	case 3:
		hp[4] -> setVisible( false);
		break;
	case 4:
		hp[3] -> setVisible( false);
		break;
	case 5:
		hp[2] -> setVisible( false);
		break;
	default:
		break;
	}
}

void GameUI::setGameTime( float time)
{
	auto master = GameMaster::GetInstance();

	static int frame = 0;

	int timer = time * 0.001f;
	int count100 = timer / 100;
	int count10 = ((int)timer % 100);
	int count1 = count10 - ((count10 / 10) * 10);
	static int count01 = 10;
	static int count001 = 10;
		
	for( int i = 0; i < 10; i++)
	{
		if( timer / 100 == i) { continue; }
		timeNumber[0][i] -> setVisible( false);
	}
	timeNumber[0][(int)timer / 100] -> setVisible( true);
	
	for( int i = 0; i < 10; i++)
	{
		if( count10 / 10 == i) { continue; }
		timeNumber[1][i] -> setVisible( false);
	}
	timeNumber[1][count10 / 10] -> setVisible( true);
	
	for( int i = 0; i < 10; i++)
	{
		if( count1 / 10 == i) { continue; }
		timeNumber[2][i] -> setVisible( false);
	}
	timeNumber[2][count1] -> setVisible( true);
	
	if( master -> GetGameState() == GSTATE_PLAY)
	{
		if( frame % 6 == 0)
		{
			for( int i = 0; i < 10; i++)
			{
				if( count01 / 10 == i) { continue; }
				timeNumber[3][i] -> setVisible( false);
			}
			timeNumber[3][count01] -> setVisible( true);
			if( count01 == 0) { count01 = 10; }
			count01--;
		}
		for( int i = 0; i < 10; i++)
		{
			if( count001 / 10 == i) { continue; }
			timeNumber[4][i] -> setVisible( false);
		}
		timeNumber[4][count001] -> setVisible( true);
		if( count001 == 0) { count001 = 10; }
		count001--;
	
		frame++;
	}
}

void GameUI::setBulletCount( int count)
{
	int Bcount1 = count - ((count / 10) * 10);
		
	for( int i = 0; i < 10; i++)
	{
		if( count / 10 == i) { continue; }
		bulletNumber[0][i] -> setVisible( false);
	}
	bulletNumber[0][count / 10] -> setVisible( true);
		
	for( int i = 0; i < 10; i++)
	{
		if( Bcount1 == i) { continue; }
		bulletNumber[1][i] -> setVisible( false);
	}
	bulletNumber[1][Bcount1] -> setVisible( true);
}



//
//
//LifeUI* LifeUI::getInstance( void)
//{
//	static LifeUI* P;
//	if( !P) P = new LifeUI;
//	return P;
//}
//
//LifeUI::LifeUI()
//{
//}
//
//void LifeUI::init( Layer* layer)
//{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	SpriteFrameCache::getInstance() -> addSpriteFramesWithFile( "HPGauge.plist");
//	SpriteFrameCache::getInstance() -> addSpriteFramesWithFile( "Number.plist");
//	frame = Sprite::create( "lifekara.png");
//	bullet = Sprite::create( "Bullet.png");
//#else
//	SpriteFrameCache::getInstance() -> addSpriteFramesWithFile( "Graph/Pictures/SpriteSheet/HPGauge.plist");
//	SpriteFrameCache::getInstance() -> addSpriteFramesWithFile( "Graph/Pictures/SpriteSheet/Number.plist");
//#endif
//
//	frame = Sprite::createWithSpriteFrameName( "HPFrame.png");
//	bullet = Sprite::createWithSpriteFrameName( "BulletFrame.png");
//	
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	bullet -> setScale( 0.2f);
//	bullet -> setPosition3D( Vec3( 640, 100, 0));
//#else
//	bullet -> setScale( 0.1f);
//	bullet -> setPosition3D( Vec3( 640, 100, 0));
//#endif
//	
//	
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	frame -> setScale( 1.0f);
//	frame -> setPosition3D( Vec3( 640, 800 - 100, 0));
//#else
//	frame -> setScale( 0.05f);
//	frame -> setPosition3D( Vec3( 640 - 5, 800 - 100, 0));
//	
//	life[Up] = Sprite::createWithSpriteFrameName( "life4.png");
//	life[Up] -> setPosition( Vec2( 2130, 3280));
//	life[Up] -> setScale( 1.9f);
//	life[Up] -> setColor( Color3B::GREEN);
//	life[Down] = Sprite::createWithSpriteFrameName( "life2.png");
//	life[Down] -> setPosition( Vec2( 2160, 1160));
//	life[Down] -> setScale( 1.9f);
//	life[Down] -> setColor( Color3B::GREEN);
//	life[Left] = Sprite::createWithSpriteFrameName( "life1.png");
//	life[Left] -> setPosition( Vec2( 620, 1810));
//	life[Left] -> setScale( 1.9f);
//	life[Left] -> setColor( Color3B::GREEN);
//	life[Right] = Sprite::createWithSpriteFrameName( "life3.png");
//	life[Right] -> setPosition( Vec2( 3700, 2220));
//	life[Right] -> setScale( 1.9f);
//	life[Right] -> setColor( Color3B::GREEN);
//
//	for( auto &p : life) { frame -> addChild( p); }
//	
//	number[0] = Sprite::createWithSpriteFrameName( "Num1.png");
//	number[0] -> setPosition( Vec2( 1330, 2180));
//	number[0] -> setScale( 1.9f);
//	number[1] = Sprite::createWithSpriteFrameName( "Num0.png");
//	number[1] -> setPosition( Vec2( 1830, 2200));
//	number[1] -> setScale( 1.9f);
//	number[2] = Sprite::createWithSpriteFrameName( "Num0.png");
//	number[2] -> setPosition( Vec2( 2330, 2200));
//	number[2] -> setScale( 1.9f);
//	number[3] = Sprite::createWithSpriteFrameName( "Num0.png");
//	number[3] -> setPosition( Vec2( 2900, 1920));
//	number[3] -> setScale( 1.0f);
//	number[4] = Sprite::createWithSpriteFrameName( "Num0.png");
//	number[4] -> setPosition( Vec2( 3200, 1920));
//	number[4] -> setScale( 1.0f);
//	
//	for( auto &p : number) { frame -> addChild( p); }
//#endif
//	
//	// auto time = GameMaster::GetInstance() -> gameTime;
//	
//	/*
//	if( time > 100)
//	{
//		number[0] = Sprite::createWithSpriteFrameName( "Num1.png");
//		number[0] -> setVisible( true);
//	}
//	else if( time >= 90 && time < 100)
//	{
//		number[0] -> setVisible( false);
//		number[1] = Sprite::createWithSpriteFrameName( "Num9.png");
//	}*/
//	
//	layer -> addChild( frame);
//	layer -> addChild( bullet);
//}
//
//void LifeUI::update( void)
//{
//
//}


LogoUI* LogoUI::getInstance( void)
{
	static LogoUI* P;
	if( !P) P = new LogoUI;
	return P;
}

LogoUI::LogoUI()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	SpriteFrameCache::getInstance() -> addSpriteFramesWithFile( "Logo.plist");
#else
	SpriteFrameCache::getInstance() -> addSpriteFramesWithFile( "Graph/Pictures/Logo.plist");
#endif
	logo[Action] = Sprite::createWithSpriteFrameName( "action.png");
	logo[Wait] = Sprite::createWithSpriteFrameName( "wait.png");
	logo[Reload] = Sprite::createWithSpriteFrameName( "rerode.png");
	for( auto &p : logo)
	{
		p -> setScale( 0.3f);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		p -> setPosition( Vec2( 640 - 5, 800 / 3));
#else
		p -> setPosition( Vec2( 1280 / 2, 800 / 3));
#endif
		p -> setVisible( false);
	}
}

void LogoUI::init( Layer* layer)
{
	for( auto &p : logo) { layer -> addChild( p); }
}

void LogoUI::update( void)
{

}

void LogoUI::setLogo( LogoNumber num)
{
//	logo[num] -> setVisible(true);
//	logo[num] -> runAction(Sequence::create( DelayTime::create( 1.5f), Hide::create(), NULL));
}

void LogoUI::resetLogo( LogoNumber num)
{
//	logo[num] -> setVisible(false);
}
