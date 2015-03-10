
#include "GameUILayer.h"

#include "Base/GameMaster.h"
#include "System/Sound.h"

USING_NS_CC;
using namespace TapGun;

/**
 *	ゲーム本編のUIレイヤーを初期化
 *
 *	@author	minaka
 *	@param	なし
 *	@return	初期化成功時 true 失敗時 false
 */
bool GameUILayer::init()
{
	if( !Layer::init()) { return false; }
	return true;
}

/**
 *	各種UIを初期化
 *
 *	@author	minaka
 *	@param	なし
 *	@return	なし
 */
void GameUILayer::InitLayer( void)
{
	initHPGauge();
	initBulletCounter();
	initOtherUI();
}

/**
 *	UIの更新
 *
 *	@author	minaka
 *	@param	なし
 *	@return	なし
 */
void GameUILayer::UpdateLayer( void)
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
void GameUILayer::setLogo( void)
{
	auto master = GameMaster::GetInstance();
	static bool waitFlag = false;
	static int waitTime = 0;

	auto sound = Sound::getInstance();
	static bool bgmFlag = false;

	if( master -> GetGameState() == GSTATE_WAIT)
	{
		logo[ReloadLogo] -> setVisible( false);
		if( bgmFlag == false && waitTime == 70)
		{
			// 適当に待ってからBGMを再生
			sound -> playBGMLoop();
			bgmFlag = true;
		}
		if( waitTime % 30 == 0 || waitTime == 0)
		{
			waitFlag = !waitFlag;
			logo[WaitLogo] -> setVisible( waitFlag);
		}
		escape[0] -> setOpacity( 25);
		escape[1] -> setOpacity( 25);
		waitTime++;
	}
	else if( master -> GetGameState() == GSTATE_PLAY_SET)
	{
		waitFlag = false;
		logo[WaitLogo] -> setVisible( false);
	}
	else if( master -> GetGameState() == GSTATE_PLAY_ACTION)
	{
		logo[ActionLogo] -> setVisible( true);
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
		logo[ActionLogo] -> setVisible( false);
	}
	else if(master->GetGameState() == GSTATE_CLEAR)
	{
		logo[WaitLogo]->setVisible(false);
	}

	//追加：ウェーブ中かつ残弾切れの時のみリロードUIを表示
	if(master->GetGameState() == GSTATE_PLAY && master->GetPlayerBullets() == 0)
	{
		logo[ReloadLogo] -> setVisible( true);
	}
	else
	{
		logo[ReloadLogo] -> setVisible( false);
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
void GameUILayer::setHP( int count)
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
void GameUILayer::setGameTime( float time)
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
void GameUILayer::setBulletCount( int count)
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


void GameUILayer::initHPGauge( void)
{
	// HPゲージの親となるノードを作成
	hpParent = Node::create();
	// HPゲージの枠を読み込み
	hp[HPFrame] = Sprite::createWithSpriteFrameName( "HPFrame.png");
	// HPゲージの背景を読み込み
	hp[HPBG] = Sprite::createWithSpriteFrameName( "HPGaugeBG.png");

	// ゲージ本体部分を読み込み配列に割り当て
	for( int i = 2, k = HPGaugeCount; i < HPGaugeCount; i++, k--)
	{
		// ファイル名用バッファ
		char buf[64];
		// ファイル名を生成
		sprintf( buf, "HPGauge_%02d.png", i - 1);
		// 配列の最後の要素から順番に画像データを保存　(後の増減の処理簡略化のため)
		hp[k - 1] = Sprite::createWithSpriteFrameName( buf);
	}

	// 全ての画像データにアクセス
	for( auto &p : hp)
	{
		// 描画位置設定
		p -> setPosition( Vec2( HPPositionX, HPPositionY));
		// 表示サイズを設定
		p -> setScale( 0.8);

		p -> retain();
		// 親ノードに接続
		hpParent -> addChild( p, 2);
	}

	hpParent -> retain();

	// 親ノードをレイヤーに接続
	addChild( hpParent);

	// ゲージ内のタイマーの親となるノードを作成
//	timeParent = Node::create();

	// 描画する数字分初期化
	for( int i = 0; i < TimeCount; i++)
	{
		// 0 ~ 9までの数値を初期化
		for( int k = 0; k < 10; k++)
		{
			// ファイル名用バッファ
			char buf[64];
			// ファイル名を生成
			sprintf( buf, "%d.png", k);
			// 桁数ごとに0 ~ 9までのスプライトを生成
			timeNumber[i][k] = Sprite::createWithSpriteFrameName( buf);
			// 表示するサイズを設定
			timeNumber[i][k] -> setScale( 0.16f);
			// 最初は全て非表示に
			timeNumber[i][k] -> setVisible( false);

			timeNumber[i][k] -> retain();
			// タイマーの親に接続
			
			addChild( timeNumber[i][k]);
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


//	timeParent -> retain();

	// 親ノードをレイヤーに接続
//	addChild( timeParent);
}

void GameUILayer::initBulletCounter( void)
{
	// 残弾アイコンの親となるノードを生成
	bulletParent = Node::create();
	// 残弾アイコンを読み込み
	bullet = Sprite::createWithSpriteFrameName( "BulletFrame.png");
	// 表示位置を設定
	bullet -> setPosition( Vec2( BulletFrameX, BulletFrameY));
	// 表示サイズを設定
	bullet -> setScale( 0.15f);
	// 親ノードに接続
	bulletParent -> addChild( bullet, 1);

	// 描画する数字分初期化
	for( int i = 0; i < BulletCount; i++)
	{
		// 0 ~ 9までの数値を初期化
		for( int k = 0; k < 10; k++)
		{
			// ファイル名用バッファ
			char buf[64];
			// ファイル名を生成
			sprintf( buf, "%d_.png", k);
			// 桁数ごとに0 ~ 9までのスプライトを生成
			bulletNumber[i][k] = Sprite::createWithSpriteFrameName( buf);
			// 表示するサイズを設定
			bulletNumber[i][k] -> setScale( 0.15f);
			// 最初は全て非表示に
			bulletNumber[i][k] -> setVisible( false);
			// 残弾アイコンの親ノードに接続
			bulletParent -> addChild( bulletNumber[i][k], 3);
		}
	}

	// 0 ~ 9までの数値分の設定
	for( int i = 0; i < 10; i++)
	{
		bulletNumber[0][i] -> setPosition( Vec2( 580, 55));
		bulletNumber[1][i] -> setPosition( Vec2( 615, 55));

		// 以下はマガジンの装弾数用のため小さく表示するように設定
		bulletNumber[2][i] -> setScale( 0.12f);
		bulletNumber[2][i] -> setPosition( Vec2( 670, 50));
		bulletNumber[3][i] -> setScale( 0.12f);
		bulletNumber[3][i] -> setPosition( Vec2( 700, 50));
	}

	// 初期設定の装弾数は 30 なので 3 と 0 のみ表示
	bulletNumber[2][3] -> setVisible( true);
	bulletNumber[3][0] -> setVisible( true);

	bulletParent -> retain();

	// 親ノードをレイヤーに接続
	addChild( bulletParent);
}


void GameUILayer::initOtherUI( void)
{
	// 画面サイズを取得
	auto visibleSize = Director::getInstance() -> getVisibleSize();

	// 回避アイコンを初期化
	for( int i = 0; i < EscapeCount; i++)
	{
		// スプライトを生成
		escape[i] = Sprite::createWithSpriteFrameName( "escape.png");
		// 表示サイズを変更
		escape[i] -> setScale( 0.4f);
		// レイヤーに接続
		addChild( escape[i]);
	}
	// 回避アイコンの表示位置設定 (左アイコン)
	escape[EscapeLeft] -> setPosition( Vec2( 130, 130));
	// 回避アイコンの表示位置設定 (右アイコン)
	escape[EscapeRight] -> setPosition( Vec2( 1150, 130));
	// 右側のアイコン用に左右反転アクションを生成
	auto flip = FlipX::create(true);
	// 右側のアイコンのみ左右反転
	escape[EscapeRight] -> runAction(flip);

	// 仕様変更のためカット
/*	pause = Sprite::create();

	for( int i = 0; i < EnemyAttackCount; i++)
	{
		for( int k = 0; k < 3; k++)
		{
			char buf[64];
			sprintf( buf, "enemy_attack_%02d.png", k + 1);
			enemyAttack[i][k] = Sprite::createWithSpriteFrameName( buf);
			enemyAttack[i][k] -> setVisible( false);
			addChild( enemyAttack[i][k]);
		}
	}

	for( int i = 0; i < DeleteCount; i++)
	{
		deleteLogo[i] = Sprite::createWithSpriteFrameName( "derete.png");
		deleteLogo[i] -> setVisible( false);
		addChild( deleteLogo[i]);
	}
*/

	// ゲーム内で使用するロゴを生成
	logo[ActionLogo] = Sprite::createWithSpriteFrameName( "action.png");
	logo[WaitLogo] = Sprite::createWithSpriteFrameName( "wait.png");
	logo[ReloadLogo] = Sprite::createWithSpriteFrameName( "rerode.png");

	// 各ロゴの初期化
	for( auto &p : logo)
	{
		// 表示位置を設定
		p -> setPosition( 1280 / 2, 250);
		// 表示サイズを設定
		p -> setScale( 0.3f);
		// 非表示状態に設定
		p -> setVisible( false);
		// レイヤーに接続
		addChild( p);
	}

	// レティクルの制御フラグを初期化
	// ウェーブ内での初射撃管理フラグ
	firstShotFlag = false;
	// 非射撃状態からの復帰フラグ
	returnEscapeFlag = false;
	// 復帰後の初射撃管理フラグ
	returnEscapeNoShotFlag = false;

	// アイドル状態レティクルを読み込み
	reticle[ReticleIdle] = Sprite::createWithSpriteFrameName( "reticle_idle.png");
	// 射撃状態レティクルを読み込み
	reticle[ReticleShot] = Sprite::createWithSpriteFrameName( "reticle_shot.png");
	// アイドル状態時の透明度を設定 (約20%)
	reticle[ReticleIdle] -> setOpacity( 50);

	// 各レティクルの初期化
	for( auto &p : reticle)
	{
		// 表示位置の設定
		p -> setPosition( Vec2( visibleSize.width / 2, visibleSize.height / 2));
		// 表示サイズの設定
		p -> setScale( 0.5f);
		// 非表示状態に設定
		p -> setVisible( false);
		// レイヤーに接続
		addChild( p);
	}
}



void GameUILayer::setReticlePoint( void)
{
	// タッチされた座標を保存
	static Vec2 pos;
	// 画面サイズを取得
	auto visibleSize = Director::getInstance() -> getVisibleSize();
	// ゲームマスターのインスタンスを取得
	auto master = GameMaster::GetInstance();

	// ウェーブ中以外はレティクル非表示
	if( master -> GetGameState() != GSTATE_PLAY && master -> GetGameState() != GSTATE_PLAY_ACTION)
	{
		// 全て非表示状態に設定
		for( auto &p : reticle) { p -> setVisible( false); }
		// 処理終了
		return;
	}

	// 画面をタッチされているか画面上をなぞっている場合
	if( master -> GetTouchState() == TSTATE_ON || master -> GetTouchState() == TSTATE_MOVE)
	{
		// 初射撃フラグを立てる
		firstShotFlag = true;
		// タッチ座標をローカルに保存
		pos = master -> GetTouchPos();
	}

	// プレイヤーが射撃中またはアイドル状態である時、1度でも射撃していた場合レティクルを射撃状態に変更
	if( ( master -> GetPlayerState() == PSTATE_SHOT || master -> GetPlayerState() == PSTATE_IDLE)
		&& firstShotFlag)// && !returnEscapeFlag)
	{
		// 回避状態からの復帰初射撃前
		if( returnEscapeNoShotFlag)
		{
			// 表示レティクルをアイドル状態に変更
			reticle[ReticleShot] -> setVisible( false);
			reticle[ReticleIdle] -> setVisible( true);
			// 画面をタッチされているか画面上をなぞっている場合
			if( master -> GetTouchState() == TSTATE_ON || master -> GetTouchState() == TSTATE_MOVE)
			{
				// 復帰後射撃前フラグを倒す
				returnEscapeNoShotFlag = false;
			}
		}
		else
		{
			// 射撃中の場合

			// 表示レティクルをショット状態に変更
			reticle[ReticleIdle] -> setVisible( false);
			reticle[ReticleShot] -> setVisible( true);

			// レティクル表示位置をタッチ位置の上に設定
			reticle[ReticleShot] -> setPosition( pos.x, pos.y + visibleSize.height * master -> reticleAjust);

			// レティクルが画面外へ出る場合
			if( reticle[ReticleShot] -> getPositionY() > visibleSize.height)
			{
				// 画面内に表示されるように補正
				reticle[ReticleShot] -> setPositionY( reticle[ReticleShot] -> getPositionY()
												- ( reticle[ReticleShot] -> getPositionY() - visibleSize.height));
			}
		}
	}
	else
	{
		// 表示レティクルをアイドル状態に変更
		reticle[ReticleShot] -> setVisible( false);
		reticle[ReticleIdle] -> setVisible( true);

		// 画面がタッチされていなくて
		if( master -> GetTouchState() == TSTATE_OFF)// && returnEscapeFlag)
		{
		//	returnEscapeFlag = false;
			firstShotFlag = true;
			returnEscapeNoShotFlag = true;
			pos = Vec2( visibleSize.width / 2, visibleSize.height / 2 - visibleSize.height * master -> reticleAjust);
		}
		else
		{
		//	returnEscapeFlag = true;
		}
	}
}
