
#include "UI.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "GameMaster.h"
#include "Sound.h"

#else

#include "Base/GameMaster.h"
#include "System/Sound.h"

#endif

using namespace TapGun;
USING_NS_CC;

/**
*	UIクラスの初期化	
*
*	@author	minaka
*	@param	なし
*	@date	2/27 Ver 1.0
*/
GameUI* GameUI::getInstance()
{
	static GameUI *p = nullptr;
	if( !p) { p = new GameUI; }
	return p;
}

/**
*	UIの初期化
*
*	@author	minaka
*	@param	表示先のレイヤー
*	@return	なし
*	@date	2/26 Ver 1.0
*/
void GameUI::init( Layer* layer)
{
	auto cache = SpriteFrameCache::getInstance();
	auto visibleSize = Director::getInstance() -> getVisibleSize();

	hpParent = Node::create();
	hp[0] = Sprite::createWithSpriteFrameName( "HPFrame.png");
	hp[1] = Sprite::createWithSpriteFrameName( "HPGaugeBG.png");
	for( int i = 2, k = HPGauge; i < HPGauge; i++, k--)
	{
		char buf[64];
		sprintf( buf, "HPGauge_%02d.png", i - 1);
		hp[k - 1] = Sprite::createWithSpriteFrameName( buf);
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
		escape[i] = Sprite::createWithSpriteFrameName( "escape.png");
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
			sprintf( buf, "enemy_attack_%02d.png", k + 1);
			enemyAttack[i][k] = Sprite::createWithSpriteFrameName( buf);
			enemyAttack[i][k] -> setVisible( false);
			layer -> addChild( enemyAttack[i][k]);
		}
	}
	
	for( int i = 0; i < Delete; i++)
	{
		deleteLogo[i] = Sprite::createWithSpriteFrameName( "derete.png");
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

	firstShotFlag = false;
	returnEscapeFlag = false;
	returnEscapeNoShotFlag = false;
	reticle[0] = Sprite::createWithSpriteFrameName( "reticle_idle.png");
	reticle[1] = Sprite::createWithSpriteFrameName( "reticle_shot.png");

	reticle[0] -> setOpacity( 50);

	for( auto &p : reticle)
	{
		p -> setPosition( Vec2( visibleSize.width / 2, visibleSize.height / 2));
		p -> setVisible( false);
		p -> setScale( 0.5f);
		layer -> addChild( p);
	}
}

/**
*	UIの更新
*
*	@author	minaka
*	@param	なし
*	@return	なし
*	@date	2/26 Ver 1.0
*/
void GameUI::update( void)
{
	auto master = GameMaster::GetInstance();
	
	setLogo();
	setHP( master -> GetPlayerHP());
	setGameTime( master -> GetGameTime());
	setBulletCount( master -> GetPlayerBullets());
	setReticlePoint();
}

/**
*	各ロゴの表示処理
*
*	@author	minaka
*	@param	なし
*	@return	なし
*	@date	2/26 Ver 1.0
*/
void GameUI::setLogo( void)
{
	auto master = GameMaster::GetInstance();
	static bool waitFlag = false;
	static int waitTime = 0;

	auto sound = Sound::getInstance();
	static bool bgmFlag = false;

	if( master -> GetGameState() == GSTATE_WAIT)
	{
		logo[LogoNumber::Reload] -> setVisible( false);
		if( bgmFlag == false && waitTime == 70)
		{
			// 適当に待ってからBGMを再生
			sound -> playBGMLoop();
			bgmFlag = true;
		}
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
		logo[LogoNumber::Wait] -> setVisible( false);
	}
	else if( master -> GetGameState() == GSTATE_PLAY_ACTION)
	{
		logo[LogoNumber::Action] -> setVisible( true);
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
		logo[LogoNumber::Action] -> setVisible( false);
	}
	else if(master->GetGameState() == GSTATE_CLEAR)
	{
		logo[LogoNumber::Wait]->setVisible(false);
	}

	//追加：ウェーブ中かつ残弾切れの時のみリロードUIを表示
	if(master->GetGameState() == GSTATE_PLAY && master->GetPlayerBullets() == 0)
	{
		logo[LogoNumber::Reload] -> setVisible( true);
	}
	else
	{
		logo[LogoNumber::Reload] -> setVisible( false);
	}
}

/**
*	HPゲージの更新処理
*
*	@author	minaka
*	@param	なし
*	@return	なし
*	@date	2/26 Ver 1.0
*/
void GameUI::setHP( int count)
{
	for( int i = 0; i < 6; i++)
	{
		hp[i + 2] -> setVisible( false);
	}
	for( int i = count; i > 0; i--)
	{
		hp[i + 1] -> setVisible( true);
	}
}

/**
*	HPゲージ内のタイマー更新処理
*
*	@author	minaka
*	@param	なし
*	@return	なし
*	@date	2/26 Ver 1.0
*/
void GameUI::setGameTime( float time)
{
	auto master = GameMaster::GetInstance();

	// 1/100秒の制御フラグ
	static bool timeFlag = false;
	// 1/10秒以下の制御カウンタ
	static int frame = 0;

	int timer = time * 0.001f;
	int count100 = timer / 100;
	int count10 = ((int)timer % 100);
	int count1 = count10 - ((count10 / 10) * 10);
	static int count01 = 0;
	static int count001 = 0;
		
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
		if( timeFlag)
		{
			for( int i = 0; i < 10; i++)
			{
				if( count001 / 10 == i) { continue; }
				timeNumber[4][i] -> setVisible( false);
			}
			timeNumber[4][count001] -> setVisible( true);
			if( count001 == 1) { count001 = 10; timeFlag = !timeFlag; }
			count001 -= 2;
		}
		else
		{
			for( int i = 0; i < 10; i++)
			{
				if( count001 / 10 == i) { continue; }
				timeNumber[4][i] -> setVisible( false);
			}
			timeNumber[4][count001] -> setVisible( true);
			if( count001 == 0) { count001 = 11; timeFlag = !timeFlag; }
			count001 -= 2;
		}
		frame++;
	}
	else if(master->GetGameState() == GSTATE_TIMEOVER)//追加
	{
		//タイムオーバー時はコンマ以下の数字を0にする
		for(int i = 0; i < 10; i++)
		{
			if(count01 / 10 == i) { continue; }
			timeNumber[3][i]->setVisible(false);
		}
		timeNumber[3][0]->setVisible(true);

		for(int i = 0; i < 10; i++)
		{
			if(count001 / 10 == i) { continue; }
			timeNumber[4][i]->setVisible(false);
		}
		timeNumber[4][0]->setVisible(true);
	}
}

/**
*	残弾数値の更新処理
*
*	@author	minaka
*	@param	なし
*	@return	なし
*	@date	2/26 Ver 1.0
*/
void GameUI::setBulletCount( int count)
{
	int Bcount = count - ((count / 10) * 10);
		
	for( int i = 0; i < 10; i++)
	{
		if( count / 10 == i) { continue; }
		bulletNumber[0][i] -> setVisible( false);
	}
	bulletNumber[0][count / 10] -> setVisible( true);
		
	for( int i = 0; i < 10; i++)
	{
		if( Bcount == i) { continue; }
		bulletNumber[1][i] -> setVisible( false);
	}
	bulletNumber[1][Bcount] -> setVisible( true);
}

void GameUI::setReticlePoint( void)
{
	static Vec2 pos;

	auto visibleSize = Director::getInstance() -> getVisibleSize();
	auto master = GameMaster::GetInstance();

	//追加：ウェーブ中以外はレティクル非表示
	if(master->GetGameState() != GSTATE_PLAY && master->GetGameState() != GSTATE_PLAY_ACTION)
	{
		reticle[1]->setVisible(false);
		reticle[0]->setVisible(false);
		return;
	}

	if( master -> GetTouchState() == TSTATE_ON || master -> GetTouchState() == TSTATE_MOVE)
	{
		firstShotFlag = true;
		pos = master -> GetTouchPos();
	}

	if( ( master -> GetPlayerState() == PSTATE_SHOT || master -> GetPlayerState() == PSTATE_IDLE) && firstShotFlag && !returnEscapeFlag)
	{
		if( returnEscapeNoShotFlag)
		{
			reticle[1] -> setVisible( false);
			reticle[0] -> setVisible( true);
			if( master -> GetTouchState() == TSTATE_ON || master -> GetTouchState() == TSTATE_MOVE)
			{
				returnEscapeNoShotFlag = false;
			}
		}
		else
		{
			reticle[0] -> setVisible( false);
			reticle[1] -> setVisible( true);
		}

		reticle[1] -> setPosition( pos.x, pos.y + visibleSize.height * master -> reticleAjust);
		
		if( reticle[1] -> getPositionY() > visibleSize.height) 
		{
			reticle[1] -> setPositionY( reticle[1] -> getPositionY() - ( reticle[1] -> getPositionY() - visibleSize.height));
		}
	}
	else
	{
		reticle[1] -> setVisible( false);
		reticle[0] -> setVisible( true);
		if( master -> GetTouchState() == TSTATE_OFF && returnEscapeFlag)
		{
			returnEscapeFlag = false;
			firstShotFlag = true;
			returnEscapeNoShotFlag = true;
			pos = Vec2( visibleSize.width / 2, visibleSize.height / 2 - visibleSize.height * master -> reticleAjust);
		}
		else
		{
			returnEscapeFlag = true;
		}
	}
}


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

