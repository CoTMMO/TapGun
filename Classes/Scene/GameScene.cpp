
#include "GameScene.h"

//各レイヤーを管理するソースコードをインクルードしておく

#include "Scene/TitleScene.h"
#include "Scene/GameModelsLayer.h"
#include "Scene/GameUILayer.h"
#include "Scene/ContinueLayer.h"
#include "Base/GameMaster.h"
#include "System/Sound.h"


USING_NS_CC;
using namespace TapGun;

Sound* sound;



/*
GameScene
（将来的にGameSceneなどに名前を変更する）
ゲーム本編の更新処理を行う
ここで処理された内容を元に、モデルデータを扱うレイヤーとＵＩを扱うレイヤーで描画を行う
*/

GameMaster* GameMasterS;//とりあえず名前を変えるか名前空間で区別する

GameModelsLayer* gGameLayer;
GameUILayer* gUILayer;
ContinueLayer* gContinueLayer;

static GameScene *multiSceneLayerInstance;

/**
*	ゲーム管理シーンクリエイト
*
*	@author	sasebon
*	@param	なし
*	@return	シーンのポインタ
*	@date	1/8 Ver 1.0
*/
Scene* GameScene::CreateScene()
{
	Scene *scene = Scene::create();//GameSceneのシーンを作成
	GameScene *layer = GameScene::create();//上記シーンに

	scene->addChild(layer);

	return scene;
}


/**
*	ゲームシーン初期化
*
*	@author	sasebon
*	@param	なし
*	@return	正常:1 初期化失敗:-1
*	@date	1/8 Ver 1.0
*/
bool GameScene::init()
{
	//レイヤー初期化
	if (!Layer::init())
	{
		return false;
	}

	//ゲームレイヤーを作成
	gGameLayer = GameModelsLayer::create();
	this->addChild(gGameLayer);

	//UIレイヤーを作成
	gUILayer = GameUILayer::create();
	this->addChild(gUILayer);

	//コンティニューを作成
	gContinueLayer = ContinueLayer::create();
	this->addChild(gContinueLayer);
	gContinueLayer->setVisible(false);

	GameMasterS = GameMaster::GetInstance();//ゲームパラメータクラスのインスタンス生成
	GameMasterS->InitScreenSize();//スクリーンサイズのセット

	//現在はタッチイベントのリスナーをここに用意しています
	auto dispatcher = Director::getInstance()->getEventDispatcher();

	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	this->scheduleUpdate();

	GameMasterS->loopTime = 16;
	//
	GameMasterS->reticleAjust = 0.1f;//

	GameMasterS->SetGameState(GSTATE_CREATE);

	return true;
}


/**
*	ゲームシーンのカメラ初期化
*
*	@author	sasebon
*	@param	なし
*	@return	正常:1
*	@date	1/8 Ver 1.0
*/
int GameScene::InitCamera()
{
	auto s = Director::getInstance()->getWinSize();

	//プレイヤーの座標取得はとりあえずこのような形で記述しています
	Vec3 cameraPos = gGameLayer->player.wrapper->getPosition3D() - gGameLayer->player.sprite3d->getPosition3D();

	//ノードを意識する座標
	GameMasterS->SetCameraNodePos(cameraPos);//ノードは常にプレイヤーの座標に一致
	GameMasterS->SetCameraNodeRot(gGameLayer->player.sprite3d->getRotation3D());//ノード回転もプレイヤーをもとに設定

	GameMasterS->SetCamera3DPos(Vec3(C_SETX_L, C_SETY_L, C_SETZ_L));//プレイヤー（親ノード）とカメラの位置関係をセット
	GameMasterS->SetCamera3DRot(Vec3(C_ROTX_L, C_ROTY_L, C_ROTZ_L));

	GameMasterS->GetCameraNode();
	GameMasterS->Get3DCamInstance();

	return TRUE;
}



void GameScene::CreateCamera()
{
	auto s = Director::getInstance()->getWinSize();

	//2D用カメラの実装
	if (NULL != gUILayer)
	{
		GameMasterS->InitCamera2D();//カメラを初期化
		gUILayer->setCameraMask(CAMFLAG_DEFAULT);
		addChild(GameMasterS->Get2DCamInstance());
	}

	//3D用カメラの実装
	if (NULL != gGameLayer)
	{
		GameMasterS->InitCamera3D();//カメラを初期化（ノードにaddChildもする）
		gGameLayer->setCameraMask(CAMFLAG_3D);

		addChild(GameMasterS->GetCamNodeInstance());
	}
}



/**
*	指定フレームごとのゲームシーン更新（現在使用していません）
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
void GameScene::moveTime(float delta)
{

}



/**
*	ゲームシーン更新（ゲームタイミング更新にはこちらを使用します）
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
void GameScene::update(float delta)
{
	GameMasterS->UpdateTouchManager();//タッチ情報を更新


	//ループにかかった時間を計測(秒)
	GameMasterS->loopTime = delta * 1000;

	//現在のゲームの状態でゲーム分岐
	switch (GameMasterS->GetGameState())
	{
		//ゲームの残り時間はGameModelsLayer内で管理します
	case GSTATE_CREATE:

		GameMasterS->InitParam();//ゲームパラメータの初期化
		if (NULL != gGameLayer)//現在は子レイヤーをクリエイトしたかを確認する
		{
			gGameLayer->LoadModels();//スプライトの生成
		}
		if (NULL != gUILayer)//現在は子レイヤーをクリエイトしたかを確認する
		{
			//gUILayer->LoadUISprite();//
		}

		CreateCamera();
		InitCamera();

		if(NULL != gGameLayer)//現在は子レイヤーをクリエイトしたかを確認する
		{
			gGameLayer->SetLights();//ライトのセット
		}

		break;
	case GSTATE_INIT:

//		GameMasterS->InitParam();

		if (NULL != gGameLayer)//現在は子レイヤーをクリエイトしたかを確認する
		{
			gGameLayer->InitLayer();//
		}
		if (NULL != gUILayer)//現在は子レイヤーをクリエイトしたかを確認する
		{
			gUILayer->InitLayer();//
		}

		GameMasterS->SetGameState(GSTATE_OP);
		break;

	case GSTATE_OP:
		//シーン切り替え用の変数
		timeCount += GameMasterS->loopTime;//
		if (timeCount >= TIME_OP)
		{
			GameMasterS->SetGameState(GSTATE_WAIT);
			GameMasterS->SetPlayerState(PSTATE_RUN);
			//OP終了したら、最初のウェーブ定義を読み込む
			gGameLayer->SetNextWave();
		}
		else
		{
			UpdateCamera();//モデルの移動をもとにカメラ移動
			timeCount = 0;
		}

		break;
	case GSTATE_WAIT:
		if(NULL != gGameLayer)//現在は子レイヤーをクリエイトしたかを確認する
		{
			//移動方向の指定・プレイヤーとカメラの移動
			gGameLayer->UpdateWait();

			//敵はウェイト中に動き始める（演出が出来るように変更）
			gGameLayer->UpdateEnemy();
			gGameLayer->UpdateBullets();
		}
		if (NULL != gUILayer)//現在は子レイヤーをクリエイトしたかを確認する
		{
			gUILayer->UpdateLayer();
		}
		UpdateCamera();//モデルの移動をもとにカメラ移動

		break;
	case GSTATE_PLAY_SET://ウェイト終了後プレイ前の処理

		//敵の配置を行う
//		gGameLayer->SetEnemy();
		GameMasterS->SetGameState(GSTATE_PLAY_ACTION);
		timeCount = 0;
		break;
	case GSTATE_PLAY_ACTION:

		//シーン切り替え用の変数
		timeCount += GameMasterS->loopTime;//

		if (timeCount >= TIME_ACTION_UI)
		{
			//一定時間経過したらウェーブを開始する
			timeCount = 0;
			GameMasterS->SetGameState(GSTATE_PLAY);
		}
		else
		{
			if (NULL != gUILayer)//現在は初期化チェック確認する
			{
				gUILayer->UpdateLayer();
			}
		}
		break;
	case GSTATE_PLAY:

		if(PSTATE_DEAD != GameMasterS->GetPlayerState())
		{
			//死亡していない場合
			//ゲーム内時間の更新はここで行う
			GameMasterS->gameActionTime -= GameMasterS->loopTime;
			if(GameMasterS->gameActionTime <= 0)
			{
				//ゲームの残り時間が0になったらゲームオーバー
				GameMasterS->SetGameState(GSTATE_TIMEOVER);//
				gGameLayer->ClearEnemies();//全ての敵を非表示にする
				gGameLayer->KillPlayer();//プレイヤーを死亡状態にする
			}
			gGameLayer->CheckNextWave();//ウェーブ終了のチェック
		}
		else
		{
			//プレイヤーがダメージ死亡したら
			GameMasterS->SetGameState(GSTATE_DEADOVER);//ダメージ死亡
			gGameLayer->ClearEnemies();//全ての敵を非表示にする
			break;
		}


		gGameLayer->UpdatePlayer();//プレイヤーの更新
		gGameLayer->UpdateEnemy();//エネミーの更新
		gGameLayer->UpdateBullets();//敵弾の更新
		gGameLayer->CheckPlayerAtk();//当たり判定とダメージのチェック
		gGameLayer->CheckEnemyAtk();//当たり判定とダメージのチェック

		if (NULL != gUILayer)//現在は初期化チェック確認する
		{
			gUILayer->UpdateLayer();
		}

		UpdateCamera();//モデルの移動をもとにカメラ移動

		break;
	case GSTATE_TIMEOVER://プレイヤーがタイムアップで死亡

		timeCount += GameMasterS->loopTime;//
		if (timeCount >= TIME_DEAD_UI)
		{
			//GameMasterS->SetGameState(GSTATE_CONTINUE_INIT);//
			GameMasterS->SetGameState(GSTATE_END);//
		}
		else
		{
			gGameLayer->UpdatePlayer();//プレイヤーの更新
			gGameLayer->UpdateBullets();//弾は画面外へ飛ばす

			if (NULL != gUILayer)//現在は子レイヤーをクリエイトしたかを確認する
			{
				gUILayer->UpdateLayer();
			}
			UpdateCamera();//モデルの移動をもとにカメラ移動
		}

		break;
	case GSTATE_DEADOVER://プレイヤーがダメージで死亡

		timeCount += GameMasterS->loopTime;//
		if (timeCount >= TIME_DEAD_UI)
		{
			//GameMasterS->SetGameState(GSTATE_CONTINUE_INIT);//
			GameMasterS->SetGameState(GSTATE_END);//
		}
		else
		{
			gGameLayer->UpdatePlayer();//プレイヤーの更新
			gGameLayer->UpdateBullets();//弾は画面外へ飛ばす

			if (NULL != gUILayer)//現在は子レイヤーをクリエイトしたかを確認する
			{
				gUILayer->UpdateLayer();
			}
			UpdateCamera();//モデルの移動をもとにカメラ移動
		}
		break;
	case GSTATE_PAUSE:
		//ポーズ中は専用のレイヤーを描画する？
		//モデルの更新処理を制限する



		break;
	case GSTATE_CONTINUE_INIT:
		//コンティニュー前の準備

		gContinueLayer->setVisible(true);//コンティニューレイヤーを表示
		GameMasterS->SetGameState(GSTATE_CONTINUE);//

		break;
	case GSTATE_CONTINUE:


		GameMasterS->SetGameState(GSTATE_CONTINUE_END);//

		break;
	case GSTATE_CONTINUE_END:
		//
		gContinueLayer->setVisible(false);//コンティニューレイヤーを非表示

//		GameMasterS->SetGameState(GSTATE_INIT);//
		GameMasterS->SetGameState(GSTATE_END);//

		break;
	case GSTATE_GAMEOVER:
		//タイトルに戻る
		EndToTitle();
		break;
	case GSTATE_CLEAR:
		//クリア時の処理
		timeCount += GameMasterS->loopTime;//
		if (timeCount >= TIME_DEAD_UI)
		{
			//一定時間経ったらゲーム終了へ
			EndToTitle();
		}
		else
		{
			//
		}

		break;
	case GSTATE_END:
		//タイトルに戻る
		EndToTitle();
		break;
	case GSTATE_EVENT:
		//

		break;
	}
}




/**
*	タイトルへ戻る
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	3/2 Ver 1.0
*/
void GameScene::EndToTitle()
{
	//BGMを停止
	auto sound = Sound::getInstance();
	sound->stopBGM();
	sound->releaseBGM();

	removeFromParent();
	auto scene = TitleScene::createScene();
	auto tran = TransitionFade::create(1, scene, ccc3(0, 0, 0));
	Director::getInstance()->replaceScene(tran);
}


/**
*	ゲームシーンのカメラ更新
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
void GameScene::UpdateCamera()
{
	if (NULL != gUILayer)
	{
		//
	}

	if (NULL != gGameLayer)
	{
		//プレイヤーの座標取得はとりあえずこのような形で記述しています
		Vec3 cameraPos;
		Vec3 cameraRot;

		//ゲームの状態でカメラの位置を動かします
		//一部のステートはさらにプレイヤーの状態でカメラの位置を動かします
		switch (GameMasterS->GetGameState())
		{
		case GSTATE_OP:

			cameraPos = gGameLayer->player.wrapper->getPosition3D() + gGameLayer->player.sprite3d->getPosition3D();
			cameraRot = gGameLayer->player.wrapper->getRotation3D() + gGameLayer->player.sprite3d->getRotation3D();

			//プレイヤーの座標にカメラのノードを置く
			GameMasterS->SetCameraNodePos(cameraPos);

			//カメラを公転させる
			cameraRot.y -= 180.0f;//プレイヤーは180度回転させているので補正を行う
			GameMasterS->SetCameraNodeRot(cameraRot);

			break;
		case GSTATE_WAIT:

			cameraPos = gGameLayer->player.wrapper->getPosition3D() + gGameLayer->player.sprite3d->getPosition3D();
			cameraRot = gGameLayer->player.wrapper->getRotation3D() + gGameLayer->player.sprite3d->getRotation3D();

			//プレイヤーの座標にカメラのノードを置く
			GameMasterS->SetCameraNodePos(cameraPos);

			//カメラを公転させる
			cameraRot.y -= 180.0f;//プレイヤーは180度回転させているので補正を行う
			GameMasterS->SetCameraNodeRot(cameraRot);

			break;
		case GSTATE_PLAY:

			//プレイ時はさらにプレイヤーの状態でカメラの位置を調整します
			if (PSTATE_DODGE == GameMasterS->GetPlayerState()
				|| PSTATE_HIDE == GameMasterS->GetPlayerState()
				|| PSTATE_APPEAR == GameMasterS->GetPlayerState()
				)
			{
				cameraPos = gGameLayer->player.wrapper->getPosition3D() + gGameLayer->player.sprite3d->getPosition3D() + gGameLayer->player.cameraAjust;
				cameraRot = gGameLayer->player.wrapper->getRotation3D() + gGameLayer->player.sprite3d->getRotation3D();

				//プレイヤーの座標にカメラのノードを置く
				//GameMasterS->SetCameraNodePos(gGameLayer->player.cameraAjust);
				GameMasterS->SetCameraNodePos(cameraPos);

				//カメラを公転させる
				cameraRot.y -= 180.0f;//プレイヤーは180度回転させているので補正を行う
				GameMasterS->SetCameraNodeRot(cameraRot);
			}
			else
			{
				cameraPos = gGameLayer->player.wrapper->getPosition3D() + gGameLayer->player.sprite3d->getPosition3D();
				cameraRot = gGameLayer->player.wrapper->getRotation3D() + gGameLayer->player.sprite3d->getRotation3D();
				//プレイヤーの座標にカメラのノードを置く
				GameMasterS->SetCameraNodePos(cameraPos);
				//カメラを公転させる
				cameraRot.y -= 180.0f;//プレイヤーは180度回転させているので補正を行う
				GameMasterS->SetCameraNodeRot(cameraRot);
			}
			break;

		case GSTATE_TIMEOVER:
		case GSTATE_DEADOVER:

			cameraPos = gGameLayer->player.wrapper->getPosition3D() + gGameLayer->player.sprite3d->getPosition3D();
//			cameraRot = gGameLayer->player.wrapper->getRotation3D() + gGameLayer->player.sprite3d->getRotation3D();
			//プレイヤーの座標にカメラのノードを置く
			GameMasterS->SetCameraNodePos(cameraPos);

			//カメラの回転は注視点基準なのでVec3(0,0,0)
			break;
		case GSTATE_CLEAR:
			cameraPos = gGameLayer->player.wrapper->getPosition3D() + gGameLayer->player.sprite3d->getPosition3D();
			cameraRot = gGameLayer->player.sprite3d->getRotation3D();
			//プレイヤーの座標にカメラのノードを置く
			GameMasterS->SetCameraNodePos(cameraPos);
			//カメラを公転させる
			cameraRot.y -= 180.0f;//プレイヤーは180度回転させているので補正を行う
			GameMasterS->SetCameraNodeRot(cameraRot);
			break;
		default:
			cameraPos = gGameLayer->player.wrapper->getPosition3D() + gGameLayer->player.sprite3d->getPosition3D();
			cameraRot = gGameLayer->player.wrapper->getRotation3D() + gGameLayer->player.sprite3d->getRotation3D();
			//プレイヤーの座標にカメラのノードを置く
			GameMasterS->SetCameraNodePos(cameraPos);
			//カメラを公転させる
			cameraRot.y -= 180.0f;//プレイヤーは180度回転させているので補正を行う
			GameMasterS->SetCameraNodeRot(cameraRot);
			break;
		}
	}
}



bool GameScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	GameMasterS->SetTouchPos(pTouch);//タッチ座標を取得してセット
	GameMasterS->SetTouchFlag(TFLAG_ON);
	
	return true;
}


void GameScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	GameMasterS->SetTouchPos(pTouch);//タッチ座標を取得してセット
	GameMasterS->SetTouchFlag(TFLAG_MOVE);

	Director *pDirector;
	Point touchPoint;
	Rect spRect;

	static float rx;
	static float ry;
	static float rz;

	auto s = Director::getInstance()->getWinSize();//画面サイズ取得
	//タッチ座標を取得する
	pDirector = Director::getInstance();
	touchPoint = pDirector->convertToGL(pTouch->getLocationInView());

}



void GameScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	GameMasterS->SetTouchPos(pTouch);//タッチ座標を取得してセット
	GameMasterS->SetTouchFlag(TFLAG_RELEASE);
}