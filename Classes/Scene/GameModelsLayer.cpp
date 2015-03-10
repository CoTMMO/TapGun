#include "GameModelsLayer.h"

#include "Base/Sprite3D.h"
#include "Base/GameMaster.h"
#include "System/Sound.h"
#include "System/ResourceLoader.h"
#include "Object/Effect.h"
#include "System/EnemySettingFile.h"


USING_NS_CC;
using namespace TapGun;

//GameModelsLayer
//GameModelsLayerの関数のうち、エネミーに以外に関連するものをまとめています
//エネミー関連はGameModelsLayerEにまとめています

int rotationCount;
float rotationR;//
Vec3 changeCameraPos;
Vec3 changeCameraRot;

bool GameModelsLayer::init()
{
	auto GM = GameMaster::GetInstance();

	if (!Layer::init())
	{
		return false;
	}
	auto dispatcher = Director::getInstance()->getEventDispatcher();

	GM = GameMaster::GetInstance();//ゲームパラメータクラスの初期化
	return true;
}


void GameModelsLayer::InitLayer(void)
{
	InitAllModels();

	InitPlayer();//とりあえず引数0
	InitMap();

	enemyTable->InitAll();//エネミー出現テーブルを初期化（現在はすべて0）

	InitEnemy();
	InitBullet();
}


void GameModelsLayer::LoadModels( void)
{
	auto EF = Effect::getInstance();//エフェクトのインスタンスの生成も同時に行う
	auto RL = ResourceLoader::getInstance();

	//プレイヤーのロード
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	std::string fileName1 = "player";
	std::string fileName2 = "Player.anime";
#else
	std::string fileName1 = "player/player";
	std::string fileName2 = "Player.anime";
#endif

	//スプライトとノードのcreate
	player.sprite3d = RL->getSprite3D(ResourceLoader::ModelNumber::Player);
	player.wrapper = Node::create();//モデルの親ノード
	player.leftNode = Node::create();//プレイヤーの回転軸の基準ノード（左）
	player.rightNode = Node::create();//プレイヤーの回転軸の基準ノード（右）
	player.centerNode = Node::create();
	player.colisionNode = Node::create();;//当たり判定基準用ノード

	cocos2d::Node* CamTargetNode;//プレイヤー基準でのカメラ注視点ノード（ウェイト中に使用する）
	cocos2d::Node* CamEyeNode;//プレイヤー基準でのカメラ視点ノード（ウェイト中に使用する）

	//スプライトとノードのaddChild
	player.wrapper->addChild(player.sprite3d);
	addChild(player.wrapper);
	player.wrapper->addChild(player.leftNode);
	player.wrapper->addChild(player.rightNode);
	player.wrapper->addChild(player.centerNode);
	player.sprite3d->addChild(player.colisionNode);

	//マップのロード
	//マップは0番に割り当て
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	fileName1 = "stage";
#else
	fileName1 = "Stage/stage";
#endif
	unit[UNIT0_MAP].sprite3d = RL->getSprite3D(ResourceLoader::ModelNumber::Map);//0番は現在マップに割り当て
	addChild(unit[UNIT0_MAP].sprite3d);


	//敵のロード
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	fileName1 = "enemy";
	fileName2 = "Enemy.anime";
#else
	fileName1 = "enemy/enemy";
	fileName2 = "Enemy.anime";
#endif
	for (int i = UNIT1_ENEMY; i < UNIT2_BULLET; i++)
	{
		//スプライトとノードのcreate
		unit[i].sprite3d = RL->getSprite3D(ResourceLoader::ModelNumber::EnemyStart + i - 1);
		unit[i].wrapper = Node::create();//親ノードも初期化
		unit[i].node1 = Node::create();//左手用ノード
		unit[i].node2 = Node::create();//右手用ノード
		unit[i].colisionNode = Node::create();//

		//スプライトとノードのaddChild
		unit[i].wrapper->addChild(unit[i].sprite3d);
		addChild(unit[i].wrapper);
		unit[i].sprite3d->addChild(unit[i].node2);
		unit[i].sprite3d->addChild(unit[i].node1);
		unit[i].sprite3d->addChild(unit[i].colisionNode);//当たり判定の基準ノードを3dスプライトに紐付け
	}
	//敵弾のロード
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	fileName1 = "tama";
#else
	fileName1 = "Bullet/tama";
#endif

	for (int i = UNIT2_BULLET; i < UNIT3_MAX; i++)
	{
		unit[i].sprite3d = RL->getSprite3D(i);
		unit[i].wrapper = Node::create();
		unit[i].node1 = Node::create();//弾の先端用ノード

		unit[i].wrapper->addChild(unit[i].sprite3d);
		addChild(unit[i].wrapper);
		unit[i].wrapper->addChild(unit[i].node1);
	}

	//座標計算用ノード
	cNode.gNode = Node::create();
	cNode.lNode = Node::create();
	cNode.lNode2 = Node::create();

	//座標計算用のノードをaddChild
	addChild(cNode.gNode);
	cNode.gNode->addChild(cNode.lNode);
	cNode.lNode->addChild(cNode.lNode2);
}


void GameModelsLayer::InitPlayer()
{
	auto GM = GameMaster::GetInstance();

	player.Init();//プレイヤーの情報を初期化

	//プレイヤー固有の初期化（今後場所を変更する）
	GM->playerHitFlag = TRUE;//食らい判定をONに
	GM->SetPlayerHP(STS_MAXPLAYERHP);//

	//回避座標の軸の定義
	player.leftNode->setPosition3D(Vec3(-0.6f, 0.0f, -0.6f));
	player.rightNode->setPosition3D(Vec3(0.6f, 0.0f, -0.6f));


	//ステージに応じた初期化
	player.sprite3d->setScale(1.0f);
	player.sprite3d->setRotation3D(GM->stagePoint[POINT_START].pRot);//プレイヤーは正面を向く
	player.wrapper->setPosition3D(GM->stagePoint[POINT_START].pPos);
	GM->sPoint = POINT_W1;//ステージ1に走る

	//当たり判定の定義（仮）
	player.collisionPos = Vec3(1.2, 3.0, 1.2);//範囲を指定して
	player.SetCollision();
	player.centerNode->setPosition3D(Vec3(0.0f, PLAYER_CENTER_Y, 0.0f));

	GM->SetPlayerBullets(STS_MAXBULLETS);//

	player.motProcTime = 0;
}



int GameModelsLayer::InitMap()
{
	unit[UNIT0_MAP].Init();//メンバ変数の初期化をしておく
	unit[UNIT0_MAP].sprite3d->setScale(1.0f);
	unit[UNIT0_MAP].sprite3d->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	unit[UNIT0_MAP].sprite3d->setPosition3D(Vec3(0.0f, 0.0f, 0.0f));
	unit[UNIT0_MAP].Init(0, UKIND_MAP);
	return TRUE;
}


void GameModelsLayer::UpdateWait()
{
	auto GM = GameMaster::GetInstance();

	switch (GM->waitFlag)
	{
	case 0://次の目標地点を決定する
	{
		//目標地点を取得し、移動処理の準備を行う
		player.targetPos = GM->stagePoint[GM->sPoint].pPos;//
		GM->SetPlayerState(PSTATE_RUN);

		//次の目標地点までの角度を計算し、プレイヤーをそちらのほうへ回転させる
		Vec3 vec = player.targetPos - player.wrapper->getPosition3D();//
		//ベクトルの正規化を行う
		vec.normalize();
		double r = atan2f(vec.z, vec.x);
		r = CC_RADIANS_TO_DEGREES(r);

		//次の目標地点への角度が求まった
		Vec3 rot = Vec3(0.0f, 90.0f - r, 0.0f);//

		if (PO_START == GM->stagePoint[GM->sPoint - 1].pointType)//ゲーム開始地点から移動する場合は回転処理をはさまない
		{
			GM->waitFlag = 3;

			player.speedVec = vec;//
			player.speed = STS_RUNSPEED;//スピードは後で調整する
			//ベクトルの正規化を行う
			//正規化が終わったら、速度をかけて方向ベクトルの計算終了
			player.speedVec.x *= player.speed;
			player.speedVec.z *= player.speed;

			player.speedVec.y = 0;//yは今のところ0で扱う
			player.sprite3d->setRotation3D(rot);//

			player.sprite3d->stopALLAnimation();
			player.sprite3d->startAnimationLoop("run");

			changeCameraPos = (Vec3(0.0f, 0.0f, 0.0f));
			changeCameraRot = (Vec3(0.0f, 0.0f, 0.0f));

			GM->SetCamera3DPos(Vec3(W_SETX, W_SETY, W_SETZ));//プレイヤー（親ノード）とカメラの位置関係をセット
			GM->SetCamera3DRot(Vec3(W_ROTX, W_ROTY, W_ROTZ));
		}
		else
		{
			GM->waitFlag = 1;

			Vec3 crot = GM->GetCamera3DRot();
			Vec3 cpos = GM->GetCamera3DPos();
			//カメラの位置が滑らかに変化する処理を入れる
			//ウェーブ用カメラからウェイト用カメラに位置と角度を入れ替える
			changeCameraPos = (Vec3(W_SETX, W_SETY, W_SETZ) - cpos) * 0.05f;
			changeCameraRot = (Vec3(W_ROTX, W_ROTY, W_ROTZ) - crot)* 0.05f;

			Vec3 prot = rot - player.sprite3d->getRotation3D();

			rotationR = prot.y;//戦闘時の角度との差を計算する

			rotationR *= 0.05f;//1フレームごとに変化する角度を計算
			rotationCount = 0;
		}
	}
		break;
	case 1://一定時間かけてカメラを動かす処理
	{
		if (20 > rotationCount)//回転中の動作
		{
			Vec3 tmp = player.sprite3d->getRotation3D();
			player.sprite3d->setRotation3D(tmp + Vec3(0, rotationR, 0));//今の角度に回転角度を足す

			GM->AddCamera3DPos(changeCameraPos);//プレイヤー（親ノード）とカメラの位置関係をセット
			GM->AddCamera3DRot(changeCameraRot);

			rotationCount++;
		}
		else//回転終了後の動作
		{
			GM->waitFlag = 3;

			player.speed = STS_RUNSPEED;//スピードは後で調整する
			player.speedVec = player.targetPos - player.wrapper->getPosition3D();//
			//ベクトルの正規化を行う
			player.speedVec.normalize();
			//正規化が終わったら、速度をかけて方向ベクトルの計算終了
			player.speedVec.x *= player.speed;
			player.speedVec.z *= player.speed;

			//キャラクターの向きを調整
			double r = atan2f(player.speedVec.z, player.speedVec.x);
			r = CC_RADIANS_TO_DEGREES(r);
			player.speedVec.y = 0;//yは今のところ0で扱う
			player.sprite3d->setRotation3D(Vec3(0.0f, 90.0f - r, 0.0f));//

			player.sprite3d->stopALLAnimation();
			player.sprite3d->startAnimationLoop("run");
		}
	}

		break;
	case 3://目標地点へと走る処理と、停止するまでの処理
	{
		UpdatePlayer();//プレイヤーの更新
		Vec3 tmpPos = player.wrapper->getPosition3D();
		tmpPos = player.targetPos - tmpPos;
		//一定以上目的地に近付いたら、カメラとプレイヤーを回転させる準備を行う

		//平面の距離を求める
		float d1 = sqrtf(tmpPos.x * tmpPos.x + tmpPos.y * tmpPos.y);
		//計算した平面上の距離と高さの距離を求める
		d1 = sqrtf((tmpPos.z * tmpPos.z) + (d1 * d1));


		//目標座標へ到達したら
		if (0.15f >= d1)
		{
			if (PO_CHANGE == GM->stagePoint[GM->sPoint].pointType)//ポイントが中継地点の場合
			{
				changeCameraPos = Vec3(0.0f, 0.0f, 0.0f);
				changeCameraRot = Vec3(0.0f, 0.0f, 0.0f);

				GM->waitFlag = 5;//回転させない
			}
			else if (PO_FINISH == GM->stagePoint[GM->sPoint].pointType)//ポイントがクリア地点の場合
			{
				changeCameraPos = Vec3(0.0f, 0.0f, 0.0f);
				changeCameraRot = Vec3(0.0f, 0.0f, 0.0f);

				GM->waitFlag = 5;//回転させない
			}
			else if (PO_BATTLE == GM->stagePoint[GM->sPoint].pointType)
			{
				//プレイヤーとカメラの位置関係を計算
				//Vec3 cP = GM->stagePoint[GM->sPoint].cPos - GM->stagePoint[GM->sPoint].pPos;
				//Vec3 cR = (GM->stagePoint[GM->sPoint].cRot) - GM->stagePoint[GM->sPoint].pRot;

				if (PSIDE_LEFT == GM->stagePoint[GM->sPoint].playerSide)
				{
					Vec3 cP = GM->stagePoint[GM->sPoint].cPos;
					Vec3 cR = GM->stagePoint[GM->sPoint].cRot;
					//changeCameraPos = Vec3(C_SETX_L - W_SETX, C_SETY_L - W_SETY, C_SETZ_L - W_SETZ) * 0.05f;
					//changeCameraRot = Vec3(C_ROTX_L - W_ROTX, C_ROTY_L - W_ROTY, C_ROTZ_L - W_ROTZ)* 0.05f;
					changeCameraPos = (cP - Vec3(W_SETX, W_SETY, W_SETZ)) * 0.05f;
					changeCameraRot = (cR - Vec3(W_ROTX, W_ROTY, W_ROTZ))* 0.05f;
					//changeCameraPos = cP * 0.05f;
					//changeCameraRot = cR * 0.05f;
				}
				else
				{
					changeCameraPos = Vec3(C_SETX_R - W_SETX, C_SETY_R - W_SETY, C_SETZ_R - W_SETZ) * 0.05f;
					changeCameraRot = Vec3(C_ROTX_R - W_ROTX, C_ROTY_R - W_ROTY, C_ROTZ_R - W_ROTZ)* 0.05f;
				}
				Vec3 tmp = player.sprite3d->getRotation3D();//プレイヤーの今の角度をもとに
				rotationR = GM->stagePoint[GM->sPoint].pRot.y - tmp.y;//戦闘時の角度との差を計算する
				rotationR *= 0.05f;//1フレームごとに変化する角度を計算
				rotationCount = 0;

				GM->waitFlag = 4;//回転させる
			}
			player.speed = 0.0f;
			player.speedVec = Vec3(0.0f, 0.0f, 0.0f);
		}
	}
		break;
	case 4://戦闘・中継ポイントで停止してから、プレイヤーを回転させる処理
	{
		//戦闘ポイント
		if (20 > rotationCount)//回転中の動作
		{
			Vec3 tmp = player.sprite3d->getRotation3D();
			player.sprite3d->setRotation3D(tmp + Vec3(0, rotationR, 0));//今の角度に回転角度を足す

			GM->AddCamera3DPos(changeCameraPos);//プレイヤー（親ノード）とカメラの位置関係をセット
			GM->AddCamera3DRot(changeCameraRot);

			rotationCount++;
		}
		else//回転終了後の動作
		{
			if (PO_BATTLE == GM->stagePoint[GM->sPoint].pointType)//次のウェーブに到達した
			{
				//座標と角度をセット
				player.sprite3d->setRotation3D(GM->stagePoint[GM->sPoint].pRot);
				player.wrapper->setPosition3D(GM->stagePoint[GM->sPoint].pPos);

				GM->playerSide = GM->stagePoint[GM->sPoint].playerSide;//現在が左右どちらに立っているかを確認

				//プレイヤーの回避用ノードをもとに、回避座標を取得する
				if (PSIDE_LEFT == GM->playerSide)
				{
					//目標角度をもとにノードの移動後座標を取得
					Vec2 pos = calcRot(GM->stagePoint[GM->sPoint].pRot.y, GM->stagePoint[GM->sPoint].playerSide);
					Vec3 newPos = player.wrapper->getPosition3D() + Vec3(pos.x, 0.0f, pos.y);
					player.wrapper->setPosition3D(newPos);//wrapper

					pos = calcRot(GM->stagePoint[GM->sPoint].pRot.y, GM->stagePoint[GM->sPoint].playerSide);
					newPos = player.sprite3d->getPosition3D() - Vec3(pos.x, 0.0f, pos.y);
					player.sprite3d->setPosition3D(newPos);//wrapper

					//目標角度をもとに、回避時のカメラ移動座標を計算
					//プレイヤーの親ノードに対する回避後カメラ座標の相対座標を計算
					Vec2 tmp = calcCamPos3(GM->stagePoint[GM->sPoint].pRot.y, PSIDE_LEFT);
					camTarget = Vec3(tmp.x, 0.0f, tmp.y);

					//回避時のカメラ移動前の座標を確保
					camCenter = player.wrapper->getPosition3D();

					player.sprite3d->stopAllActions();
					player.sprite3d->startAnimationLoop("idle_l");//
				}
				else
				{
					//目標角度をもとにノードの移動後座標を取得
					Vec2 pos = calcRot(GM->stagePoint[GM->sPoint].pRot.y, GM->stagePoint[GM->sPoint].playerSide);
					Vec3 newPos = player.wrapper->getPosition3D() + Vec3(pos.x, 0.0f, pos.y);
					player.wrapper->setPosition3D(newPos);//wrapper

					pos = calcRot(GM->stagePoint[GM->sPoint].pRot.y, GM->stagePoint[GM->sPoint].playerSide);
					newPos = player.sprite3d->getPosition3D() - Vec3(pos.x, 0.0f, pos.y);
					player.sprite3d->setPosition3D(newPos);

					//目標角度をもとに、回避時のカメラ移動座標を計算
					Vec2 tmp = calcCamPos3(GM->stagePoint[GM->sPoint].pRot.y, PSIDE_RIGHT);
					camTarget = Vec3(tmp.x, 0.0f, tmp.y);

					player.sprite3d->stopAllActions();
					player.sprite3d->startAnimationLoop("idle_r");//
				}
				GM->SetGameState(GSTATE_PLAY_SET);//戦闘ポイントに到着したら、ウェイトからプレイに移行
				GM->SetPlayerState(PSTATE_IDLE);

				//ゲームに関係する各種フラグの初期化
				GM->flgPlayerATK = FALSE;//プレイヤーの攻撃判定をOFFに
				GM->playerHitFlag = TRUE;//プレイヤーの食らい判定をONに
				GM->shotFlag = FALSE;//
			}
		}
	}
		break;
	case 5://戦闘・中継ポイント以外で停止した後の処理

		if (PO_FINISH == GM->stagePoint[GM->sPoint].pointType)
		{
			//player.sprite3d->setRotation3D(GM->stagePoint[GM->sPoint].pRot);
			player.wrapper->setPosition3D(GM->stagePoint[GM->sPoint].pPos);

			GM->waitFlag = 0;

			GM->SetGameState(GSTATE_CLEAR);
			GM->SetPlayerState(PSTATE_IDLE);//要チェック

			player.sprite3d->stopAllActions();
			player.sprite3d->startAnimationLoop("idle_l");//
			player.wrapper->setRotation3D(Vec3(0.0f, 30.0f, 0.0f));
		}
		else if (PO_CHANGE == GM->stagePoint[GM->sPoint].pointType)//ポイントが中継地点の場合
		{
			//player.sprite3d->setRotation3D(GM->stagePoint[GM->sPoint].pRot);
			player.wrapper->setPosition3D(GM->stagePoint[GM->sPoint].pPos);//座標は補正するが、角度は補正しない

			GM->sPoint++;//座標と角度が設定できたらポイントを先に進める
			GM->waitFlag = 0;
		}
		break;
	}
}


void GameModelsLayer::UpdatePlayer(void)
{
	auto GM = GameMaster::GetInstance();

	//1：プレイヤーの無敵時間の更新
	if (0 < GM->mutekiTime&& FALSE == GM->playerHitFlag)
	{
		//無敵時間が有効で、かつプレイヤーが回避状態でなければ処理を実行
		switch (GM->GetPlayerState())
		{
		case PSTATE_IDLE://アイドル状態
		case PSTATE_SHOT://攻撃中
			//一定時間が経過したら食らい判定をONにする
			GM->mutekiTime -= GM->loopTime;
			if (0 >= GM->mutekiTime)
			{
				GM->mutekiTime = 0;
				GM->playerHitFlag = TRUE;
			}
			break;
		case PSTATE_DODGE://隠れ中
			//回避に入った瞬間に無敵時間を0にする
			//(ただし回避中なので無敵のまま)
			GM->mutekiTime = 0;
			break;
		case PSTATE_HIDE://隠れている
			ActionHide();
			break;
		default:
			break;
		}
	}

	//2：プレイヤーの状態を取得して場合分け
	switch (GM->GetPlayerState())
	{
	case PSTATE_IDLE://アイドル状態
		ActionIdle();
		break;
	case PSTATE_SHOT:
		ActionShot();
		break;
	case PSTATE_DODGE://隠れ中
		ActionDodge();
		break;
	case PSTATE_HIDE://隠れている
		ActionHide();
		break;
	case PSTATE_APPEAR://隠れた状態から出る
		ActionAppear();
		break;
	case PSTATE_DAMAGED:
		ActionDamaged();
		break;
	case PSTATE_RECOVER:
		ActionRecover();
		break;
	case PSTATE_RUN://ウェイト状態

		break;
	case PSTATE_DEAD://死亡状態
		ActionDead();
		break;
	case PSTATE_CONTINUE://
		//ここに入るとゲームオーバーフラグがONになる
		break;
	}

	//もともとこちらが2だったので、問題が起これば元に戻す
	//3：プレイヤーのフレーム・座標更新
	player.Update(GM->loopTime);
	//マズルの更新
	auto ef = Effect::getInstance();
	ef->muzzleUpdate();
}


void GameModelsLayer::ActionIdle()
{
	auto GM = GameMaster::GetInstance();

	std::string shot;
	std::string h_shot;
	std::string h_reload;
	auto director = Director::getInstance();

	if (PSIDE_LEFT == GM->playerSide)
	{
		shot = "shot_l";
		h_shot = "h_shot_l";
		h_reload = "h_reload_l";
	}
	else
	{
		shot = "shot_r";
		h_shot = "h_shot_r";
		h_reload = "h_reload_r";
	}

	if (TSTATE_ON == GM->GetTouchState())//タッチされたら
	{
		auto s = Director::getInstance()->getWinSize();//画面サイズ取得
		const Vec2 tPos = GM->GetTouchPos();//タッチしたスクリーン座標を取得

		//		player.muzzleflagOn();

		int tmpFlag = 0;

		if (PSIDE_LEFT == GM->playerSide)//プレイヤーが左に立っている
		{
			//攻撃可能範囲をタッチしていれば
			if (tPos.x > s.width *(1.0f - BATTLE_FEILD_X))
			{
				tmpFlag = 1;//
			}
			//回避UI付近をタッチしたら
			else if (tPos.x >= s.width * HIDE_UI_X0 && tPos.x < s.width *HIDE_UI_X1
				&& tPos.y >= s.height*HIDE_UI_Y0 && tPos.y < s.height*HIDE_UI_Y1)
			{
				tmpFlag = 2;
			}
		}
		else//右に立っている
		{
			//攻撃可能範囲をタッチしていれば
			if (tPos.x < s.width * BATTLE_FEILD_X)
			{
				tmpFlag = 1;//
			}
			//回避UI付近をタッチしたら
			else if (tPos.x < s.width * (1.0f - HIDE_UI_X0) && tPos.x >= s.width *(1.0f - HIDE_UI_X1)
				&& tPos.y >= s.height*HIDE_UI_Y0 && tPos.y < s.height*HIDE_UI_Y1)
			{
				tmpFlag = 2;
			}
		}

		//タッチ状態に応じて分岐
		if (0 == tmpFlag)
		{
			//タッチしていない
		}
		else if (1 == tmpFlag)
		{
			//残弾に応じて処理を分ける
			if (0 >= GM->GetPlayerBullets())
			{
				//モーションは再生するが弾を出さない

			}
			else
			{
				//アニメーションを再生
				GM->SetPlayerState(PSTATE_SHOT);
				player.sprite3d->stopAllActions();
				player.sprite3d->startAnimation(shot);//射撃アニメーションを最後まで再生する

				GM->rapidFrame = -1.0f;//連射フレームを-1に初期化
				GM->flgPlayerATK = FALSE;//
				GM->shotFlag = FALSE;//

				//モーション管理用の時間を初期化
				player.motProcTime = 0;//モーションを再生してからの経過時間（ミリ秒）
				//				player.motPreTime = getNowTime();//PreTimeに現在時刻を代入
				//				player.motStartTime = getNowTime();


				GM->rapidFrame = 0.0f;
			}
		}
		else
		{
			//
			GM->SetPlayerState(PSTATE_DODGE);//回避に移行

			player.sprite3d->stopAllActions();
			player.sprite3d->startAnimationReverse(h_shot);//回避モーションを再生

			//モーション管理用の時間を初期化
			player.motProcTime = 0;//モーションを再生してからの経過時間（ミリ秒）
			player.setAnimEndTime(MACRO_FtoMS(STS_HIDEWAIT));//回避モーションにかかる時間をセット

		}
	}
}


void GameModelsLayer::ActionShot()
{
	auto GM = GameMaster::GetInstance();

	//モーションを再生してからの経過時間をある程度正確に取得するため、関数内で時間計算を行っています
	player.motProcTime += GM->loopTime;//モーションを再生してからの経過時間（ミリ秒）
	GM->rapidFrame += MACRO_StoF(GM->loopTime);//

	//プレイヤーの向きに応じて呼び出すアニメーションを変更する
	//とりあえずここで文字列作成
	std::string shot;
	std::string h_shot;
	std::string h_reload;
	if (PSIDE_LEFT == GM->playerSide)
	{
		shot = "shot_l";
		h_shot = "h_shot_l";
		h_reload = "h_reload_l";
	}
	else
	{
		shot = "shot_r";
		h_shot = "h_shot_r";
		h_reload = "h_reload_r";
	}

	//指定フレーム以上タッチしているかのチェック
	if (FALSE == GM->shotFlag && MACRO_StoF(player.motProcTime) >= STS_RAPIDSPEED)
	{
		GM->shotFlag = TRUE;

		auto ef = Effect::getInstance();
		ef->setPlayerMuzzle(player.sprite3d, "po_");
		player.sprite3d->stopAllActions();
		player.sprite3d->startAnimationLoop(shot, 0, MACRO_FtoS(7.0f));
	}

	if (TSTATE_ON == GM->GetTouchState() || TSTATE_MOVE == GM->GetTouchState())
	{
		if (0 >= GM->GetPlayerBullets())
		{
			//残弾がない場合
			GM->flgPlayerATK = FALSE;//攻撃判定をオフにする
			//アニメーションはそのまま行う
		}
		else
		{
			//残弾がある場合
			//一定フレームごとに攻撃判定をONにする
			if (STS_RAPIDSPEED < GM->rapidFrame)
			{
				GM->flgPlayerATK = TRUE;
				GM->AddPlayerBullets(-1);//弾数を減らす

				auto ef = Effect::getInstance();
				ef->setPlayerMuzzle(player.sprite3d, "po_");

				GM->rapidFrame = 0.0f;
			}
			else
			{
				GM->flgPlayerATK = FALSE;
			}
		}

		GM->SetPlayerState(PSTATE_SHOT);//ステート状態はそのまま
	}
	else if (TSTATE_RELEASE == GM->GetTouchState())//タッチを離したら
	{
		if (FALSE == GM->shotFlag)
		{
			//もしタッチ時間が指定フレーム以内なら
			GM->SetPlayerState(PSTATE_IDLE);//通常状態に戻す
			GM->flgPlayerATK = FALSE;//攻撃判定をオンにする

			//そのままアニメーションを終了する
		}
		else
		{
			GM->SetPlayerState(PSTATE_IDLE);//通常状態に戻す
			GM->flgPlayerATK = FALSE;//攻撃判定をオフにする

			//それ以外の場合、ショットの最後の部分まで再生する
			player.sprite3d->stopAllActions();
			player.sprite3d->startAnimation(shot);
		}
	}
	else
	{
		if (FALSE == GM->shotFlag)
		{
			//もしタッチ時間が5フレーム以内なら
			GM->SetPlayerState(PSTATE_IDLE);//通常状態に戻す
			GM->flgPlayerATK = FALSE;//攻撃判定をオフにする
			GM->shotFlag = FALSE;
			//そのままアニメーションを終了する
		}
		else
		{
			GM->SetPlayerState(PSTATE_IDLE);//通常状態に戻す
			GM->flgPlayerATK = FALSE;//攻撃判定をオフにする
			GM->shotFlag = FALSE;
			//ショットの最後の部分を再生する
			player.sprite3d->stopAllActions();
			player.sprite3d->startAnimation(shot);
		}
	}
}


void GameModelsLayer::ActionDodge(void)
{
	auto GM = GameMaster::GetInstance();

	//モーションを再生してからの経過時間をある程度正確に取得するため、関数内で時間計算を行っています
	player.motProcTime += GM->loopTime;//モーションを再生してからの経過時間（ミリ秒）

	//プレイヤーの向きに応じて呼び出すアニメーションを変更する
	//とりあえずここで文字列作成
	std::string shot;
	std::string h_shot;
	std::string h_reload;
	if (PSIDE_LEFT == GM->playerSide)
	{
		h_shot = "h_shot_l";
		h_reload = "h_reload_l";
	}
	else
	{
		h_shot = "h_shot_r";
		h_reload = "h_reload_r";
	}

	//１：ボタンホールド状態で回避フレームが終了するとActionHideに移行する
	//２：ボタンホールド状態が解除されればActionAppearに移行する
	//３：ボタンホールド継続状態であれば、モーションを継続する・・・（ボタンON,ボタンMOVE）

	int state = GM->GetTouchState();//現在のタッチ状態を取得

	//無敵時間の判定を行う
	if (STS_MUTEKISTART <= MACRO_StoF(player.motProcTime))//無敵開始フレームに達したら
	{
		GM->playerHitFlag = FALSE;//当たり判定を無効化する
	}

	//リロード判定を行う
	if (STS_RELOADSTART <= MACRO_StoF(player.motProcTime))//リロード開始フレームに達したら
	{
		GM->SetPlayerBullets(STS_MAXBULLETS);//弾数を回復する
	}

	//回避完了フレームに達したら
	//	if (STS_HIDEWAIT <= GM->hideFrame)
	//	if (0 == player.sprite3d->checkAnimationState())//アニメーションが終了したら
	if (player.getAnimEndTime() <= player.motProcTime)
	{
		GM->SetPlayerState(PSTATE_HIDE);//隠れている状態に移行

		//リロードモーションが終了した後は、リロードモーションを行う
		player.sprite3d->stopAllActions();
		player.sprite3d->startAnimation(h_reload);//リロードモーションを再生
		auto sound = Sound::getInstance();
		sound->playSE("Reload");

		//座標と角度をセットしてキャラクターの座標を補正
		if (PSIDE_LEFT == GM->playerSide)
		{
			player.sprite3d->setRotation3D(GM->stagePoint[GM->sPoint].pRot + Vec3(0.0f, 90.0f, 0.0f));
			player.wrapper->setRotation3D(Vec3(0.0f, -90.0f, 0.0f));

			//回避に合わせてカメラの座標を補正する
			player.cameraAjust = Vec3(camTarget.x * player.motProcTime, camTarget.y * player.motProcTime, camTarget.z * player.motProcTime);//ループごとの移動量を計算
		}
		else
		{
			player.sprite3d->setRotation3D(GM->stagePoint[GM->sPoint].pRot - Vec3(0.0f, 90.0f, 0.0f));
			player.wrapper->setRotation3D(Vec3(0.0f, 90.0f, 0.0f));

			//回避に合わせてカメラの座標を補正する
			player.cameraAjust = Vec3(camTarget.x * MACRO_FtoMS(STS_HIDEWAIT), camTarget.y * MACRO_FtoMS(STS_HIDEWAIT), camTarget.z * MACRO_FtoMS(STS_HIDEWAIT));//ループごとの移動量を計算
		}

		//モーション用カウンターを初期化
		player.motProcTime = 0;//経過時間を0に初期化
	}
	else
	{
		//回避完了前にホールド解除した場合
		//とりあえずTFLAG_CANCELをif内に入れておく
		if (TSTATE_RELEASE == state)
		{
			GM->SetPlayerState(PSTATE_APPEAR);//隠れている状態に移行
			//突撃モーションを再生

			int startTime = MACRO_FtoMS(STS_HIDEWAIT) - player.motProcTime;//回避モーションの残り時間をもとに、突撃モーションの開始時間を計算

			player.setAnimEndTime(MACRO_FtoMS(STS_HIDEWAIT) - startTime);//アニメーション終了までの残りミリ秒数をセット
			player.sprite3d->stopALLAnimation();//モーション終了
			player.sprite3d->startAnimation(h_shot, (startTime * 0.001f), MACRO_FtoS(STS_HIDEWAIT) - (startTime * 0.001f));//

			player.motProcTime = startTime;//
		}
		else if (TSTATE_MOVE == state || TSTATE_ON == state)//ホールド状態
		{
			//回避動作中は指定座標を軸に座標移動を行う
			if (PSIDE_LEFT == GM->playerSide)
			{
				//プレイヤーの座標の更新
				float rot = GM->loopTime * 90.0f / (MACRO_FtoMS(STS_HIDEWAIT));

				Vec3 tmp = player.wrapper->getRotation3D();
				tmp.y -= rot;// *loopTime;
				player.wrapper->setRotation3D(tmp);//毎フレームP親ノードの角度を更新する
				tmp = player.sprite3d->getRotation3D();
				tmp.y += rot;// *loopTime;
				player.sprite3d->setRotation3D(tmp);//プレイヤーの角度は逆に更新し、キャラクターの向きを正面に向かせる

				//回避に合わせてカメラの座標を補正する
				player.cameraAjust = Vec3(camTarget.x * player.motProcTime, camTarget.y * player.motProcTime, camTarget.z * player.motProcTime);//ループごとの移動量を計算
			}
			else
			{
				//プレイヤーの座標の更新
				float rot = GM->loopTime * 90.0f / (MACRO_FtoMS(STS_HIDEWAIT));//要チェック

				Vec3 tmp = player.wrapper->getRotation3D();
				tmp.y += rot;
				player.wrapper->setRotation3D(tmp);
				tmp = player.sprite3d->getRotation3D();
				tmp.y -= rot;
				player.sprite3d->setRotation3D(tmp);

				//回避に合わせてカメラの座標を補正する
				player.cameraAjust = Vec3(camTarget.x * MACRO_StoF(player.motProcTime), camTarget.y * MACRO_StoF(player.motProcTime), camTarget.z * MACRO_StoF(player.motProcTime));//ループごとの移動量を計算
			}
		}
		else
		{
			int a = 0;
			a = 0;
		}
	}
}



void GameModelsLayer::ActionHide(void)
{
	auto GM = GameMaster::GetInstance();

	//プレイヤーの向きに応じて呼び出すアニメーションを変更する
	//とりあえずここで文字列作成
	std::string shot;
	std::string h_shot;
	std::string h_reload;
	if (PSIDE_LEFT == GM->playerSide)
	{
		h_shot = "h_shot_l";
		h_reload = "h_reload_l";
	}
	else
	{
		h_shot = "h_shot_r";
		h_reload = "h_reload_r";
	}

	//ボタンが押しっぱなしであれば、回避状態を継続し、リロードアニメーションを再生
	//ボタンが離されれば、回避状態を終了して突撃状態へ移行

	if (TSTATE_ON == GM->GetTouchState() || TSTATE_MOVE == GM->GetTouchState())
	{
		//回避に合わせてカメラの座標を補正する
		player.cameraAjust = Vec3(camTarget.x * MACRO_FtoMS(STS_HIDEWAIT), camTarget.y * MACRO_FtoMS(STS_HIDEWAIT), camTarget.z * MACRO_FtoMS(STS_HIDEWAIT));//ループごとの移動量を計算
	}
	else if (TSTATE_RELEASE == GM->GetTouchState())//離されれば
	{
		GM->SetPlayerState(PSTATE_APPEAR);
		player.sprite3d->startAnimation(h_shot);
		//player.InitFrame();//フレームをリフレッシュ

		//モーション管理用時間の初期化
		player.motProcTime = 0;//経過時間は0
		player.setAnimEndTime(MACRO_FtoMS(STS_HIDEWAIT));//モーション終了までの残り時間を計算
		auto sound = Sound::getInstance();
		sound->stopSE("Reload");
	}
	else
	{
		//プレイヤーが隠れ状態になるタイミングで指を離した場合、このステートに入る
		//指を離しているので飛び出し状態に移行

		GM->SetPlayerState(PSTATE_APPEAR);
		player.sprite3d->startAnimation(h_shot);

		//モーション管理用時間の初期化
		player.motProcTime = 0;//経過時間は0
		player.setAnimEndTime(MACRO_FtoMS(STS_HIDEWAIT));//モーション終了までの残り時間を計算
		auto sound = Sound::getInstance();
		sound->stopSE("Reload");
	}
}


void GameModelsLayer::ActionAppear(void)
{
	auto GM = GameMaster::GetInstance();

	//モーションを再生してからの経過時間をある程度正確に取得するため、関数内で時間計算を行っています
	player.motProcTime += GM->loopTime;//モーションを再生してからの経過時間（ミリ秒）

	//プレイヤーの向きに応じて呼び出すアニメーションを変更する
	//とりあえずここで文字列作成
	std::string idle;
	if (PSIDE_LEFT == GM->playerSide)
	{
		idle = "idle_l";
	}
	else
	{
		idle = "idle_r";
	}

	//無敵時間の判定を行う
	if (STS_MUTEKISTART <= MACRO_StoF(player.motProcTime))//無敵終了フレームに達したら
	{
		GM->playerHitFlag = TRUE;//当たり判定を有効化する
	}

	//回避完了の判定
	if (player.getAnimEndTime() <= player.motProcTime)
	{
		//必要ならばモーションの停止を行う
		GM->SetPlayerState(PSTATE_IDLE);//アイドル状態に移行
		player.sprite3d->stopAllActions();

		//座標と角度をセットしてキャラクターの座標を補正
		player.sprite3d->setRotation3D(GM->stagePoint[GM->sPoint].pRot);
		player.wrapper->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));

		//座標と角度をセットしてカメラの座標を補正
		Vec2 tmp = calcCamPos3(GM->stagePoint[GM->sPoint].pRot.y, GM->playerSide);
		camTarget = Vec3(tmp.x, 0.0f, tmp.y);

		player.sprite3d->startAnimationLoop(idle);
	}
	else
	{
		if (PSIDE_LEFT == GM->playerSide)
		{
			//回避フレームに比例してカメラの回転を変化させる
			float rot = GM->loopTime * 90.0f / (MACRO_FtoMS(STS_HIDEWAIT));
			Vec3 tmp = player.wrapper->getRotation3D();
			tmp.y += rot;//
			player.wrapper->setRotation3D(tmp);//プレイヤーの親ノード（回避軸）の角度を更新する
			tmp = player.sprite3d->getRotation3D();
			tmp.y -= rot;//
			player.sprite3d->setRotation3D(tmp);//プレイヤー自身の角度を更新する

			//回避に合わせてカメラの座標を補正する
			player.cameraAjust =
				Vec3(camTarget.x * (MACRO_FtoMS(STS_HIDEWAIT) - player.motProcTime),
				camTarget.y * (MACRO_FtoMS(STS_HIDEWAIT) - player.motProcTime),
				camTarget.z * (MACRO_FtoMS(STS_HIDEWAIT) - player.motProcTime));//ループごとの移動量を計算
		}
		else
		{
			//回避フレームに比例してカメラの回転を変化させる
			float rot = GM->loopTime * 90.0f / (MACRO_FtoMS(STS_HIDEWAIT)) * 0.001f;
			Vec3 tmp = player.wrapper->getRotation3D();
			tmp.y -= rot;//
			player.wrapper->setRotation3D(tmp);//プレイヤーの親ノード（回避軸）の角度を更新する
			tmp = player.sprite3d->getRotation3D();
			tmp.y += rot;//
			player.sprite3d->setRotation3D(tmp);//プレイヤー自身の角度を更新する

			//回避に合わせてカメラの座標を補正する
			player.cameraAjust = Vec3(camTarget.x * (MACRO_FtoMS(STS_HIDEWAIT) - player.motProcTime),
				camTarget.y * (MACRO_FtoMS(STS_HIDEWAIT) - player.motProcTime),
				camTarget.z * (MACRO_FtoMS(STS_HIDEWAIT) - player.motProcTime)) * 0.001f;//ループごとの移動量を計算
		}
	}
}


void GameModelsLayer::ActionDamaged(void)
{
	auto GM = GameMaster::GetInstance();

	//モーションを再生してからの経過時間をある程度正確に取得するため、関数内で時間計算を行っています
	player.motProcTime += GM->loopTime;//モーションを再生してからの経過時間（秒）

	//プレイヤーの向きに応じて呼び出すアニメーションを変更する
	//とりあえずここで文字列作成
	std::string recov;
	if (PSIDE_LEFT == GM->playerSide)
	{
		recov = "recov_l";
	}
	else
	{
		recov = "recov_r";
	}

	//アニメーションの確認
	if (0 == player.sprite3d->checkAnimationState())
	{
		//食らいモーションが終了したら


		//必要ならばモーションの停止を行う
		GM->SetPlayerState(PSTATE_RECOVER);//起き上がり状態に移行
		player.sprite3d->stopAllActions();
		player.sprite3d->startAnimation(recov);//起き上がりモーションを再生

		//
		player.motProcTime = 0;//モーションを再生してからの経過時間（秒）
	}
	else
	{
		//
	}
}



void GameModelsLayer::ActionRecover(void)
{
	auto GM = GameMaster::GetInstance();

	//モーションを再生してからの経過時間をある程度正確に取得するため、関数内で時間計算を行っています
	player.motProcTime += GM->loopTime;//モーションを再生してからの経過時間（秒）

	//プレイヤーの向きに応じて呼び出すアニメーションを変更する
	//とりあえずここで文字列作成
	std::string idle;
	if (PSIDE_LEFT == GM->playerSide)
	{
		idle = "idle_l";
	}
	else
	{
		idle = "idle_r";
	}


	//アニメーションの確認
	if (0 == player.sprite3d->checkAnimationState())
	{
		//HPが0になったら
		if (0 < GM->GetPlayerHP())
		{
			//食らいモーションが終了したら
			GM->SetPlayerState(PSTATE_IDLE);//アイドル状態に移行
			player.sprite3d->stopAllActions();
			player.sprite3d->startAnimationLoop(idle);

			//GM->playerHitFlag = TRUE;//このタイミングでは食らい判定を戻さない
			GM->mutekiTime = STS_MUTEKITIME;//無敵時間をセット
			//モーション管理用の時間を初期化
			player.motProcTime = 0;//モーションを再生してからの経過時間（秒）
		}
		else
		{
			GM->SetPlayerState(PSTATE_DEAD);//死亡状態にする

			//必要ならばモーションの停止を行う
			player.sprite3d->stopAllActions();
			player.sprite3d->startAnimation("dei1");//

			//
			player.motProcTime = 0;//モーションを再生してからの経過時間（秒）

			//座標と角度をセットしてカメラの座標を補正
			ChangeCamera(2);
		}
	}
	else
	{
		//
	}
}


void GameModelsLayer::ActionRun(void)
{
}


void GameModelsLayer::ActionDead(void)
{
	auto GM = GameMaster::GetInstance();

	//モーションを再生してからの経過時間をある程度正確に取得するため、関数内で時間計算を行っています
	player.motProcTime += GM->loopTime;//モーションを再生してからの経過時間（秒）
}


void  GameModelsLayer::CheckPlayerAtk(void)
{
	auto GM = GameMaster::GetInstance();

	//攻撃判定フラグがONのときのみ攻撃判定を処理
	if (TRUE == GM->flgPlayerATK)
	{
		//注意：敵が重なって存在する場合に備え、Ｚソートなどの並び替えを行う必要がありそうです
		auto s = Director::getInstance()->getWinSize();//ウィンドウサイズを取得

		Vec3 rayStart = Vec3(0, 0, 0);//レイの始点
		Vec3 rayEnd = Vec3(0, 0, 0);//レイの終点

		Camera* cam3d = GM->GetCamera3D();//カメラのインスタンスを取得
		const Node* camNode = GM->GetCameraNode();//ノードのインスタンスを取得

		Vec3 tmpPos = cam3d->getPosition3D();//カメラ座標を保存
		Vec3 tmpRot = cam3d->getRotation3D();//カメラ回転を保存

		const Point pos = Vec2(tmpPos.x, tmpPos.z);
		const Point absolutePoint = camNode->convertToWorldSpace(pos);//カメラのx,zの絶対座標を取得

		Vec3 cPos = Vec3(pos.x, tmpPos.y, pos.y);//yは
		Vec2 tPos = GM->GetTouchPosInView();//タッチ座標を取得

		//レティクルの位置を加味したタッチ座標に修正
		tPos.y -= GM->reticleAjust * s.height;//レティクルの状態を
		if (tPos.y < 0.0f)
		{
			tPos.y = 0.0f;
		}

		Vec3 tmpPosNear = Vec3(tPos.x, tPos.y, -1.0f);//-1.0f == 視錘台の近面（near plane）
		Vec3 tmpPosFar = Vec3(tPos.x, tPos.y, 1.0f);//1.0f == 視錘台の遠面（far plane）


		//現在はレイの計算のためにカメラ位置を一時的に変更し、処理後に位置を元に戻しています
		cam3d->setPosition3D(cPos);

		cam3d->unproject(s, &tmpPosNear, &rayStart);//near planeの3次元座標を取得
		cam3d->unproject(s, &tmpPosFar, &rayEnd);

		Ray touchRay(rayStart, rayEnd);//仮レイを設定

		//敵とプレイヤーの距離が近い順に当たり判定を処理する
		int tmpArrayDir[UNIT2_BULLET - UNIT1_ENEMY];
		int tmpArrayNum[UNIT2_BULLET - UNIT1_ENEMY];
		for (int i = 0; i < UNIT2_BULLET - UNIT1_ENEMY; i++)
		{
			Vec3 pPos = player.wrapper->getPosition3D() + player.sprite3d->getPosition3D() + player.centerNode->getPosition3D();
			Vec3 dir = pPos - unit[i].sprite3d->getPosition3D();//プレイヤーと敵弾の差のベクトル
			//平面の距離を求める
			float dist = sqrtf(dir.x * dir.x + dir.y * dir.y);
			//計算した平面上の距離と高さの距離を求める
			dist = sqrtf((dir.z * dir.z) + (dist * dist));

			tmpArrayDir[i] = dist;
			tmpArrayNum[i] = i + UNIT1_ENEMY;
		}

		//距離でソート(シェーカーソート)
		int left = 0;
		int right = UNIT2_BULLET - UNIT1_ENEMY - 1;
		int tmp, shift;
		while (left < right)
		{
			for (int i = left; i < right; i++)
			{
				if (tmpArrayDir[i] >tmpArrayDir[i + 1])
				{
					tmp = tmpArrayDir[i];
					tmpArrayDir[i] = tmpArrayDir[i + 1];
					tmpArrayDir[i + 1] = tmp;

					tmp = tmpArrayNum[i];
					tmpArrayNum[i] = tmpArrayNum[i + 1];
					tmpArrayNum[i + 1] = tmp;

					shift = i;
				}
			}
			right = shift;
			for (int i = right; i > left; i--)
			{
				if (tmpArrayDir[i] < tmpArrayDir[i - 1])
				{
					tmp = tmpArrayDir[i];
					tmpArrayDir[i] = tmpArrayDir[i - 1];
					tmpArrayDir[i - 1] = tmp;

					tmp = tmpArrayNum[i];
					tmpArrayNum[i] = tmpArrayNum[i - 1];
					tmpArrayNum[i - 1] = tmp;

					shift = i;
				}
			}
			left = shift;
		}

		//ソート後
		//レイの当たり判定
		for (int i = 0; i < UNIT2_BULLET - UNIT1_ENEMY; i++)
		{
			int n = tmpArrayNum[i];
			//敵の
			if (TRUE == unit[n].visible
				&& UKIND_ENEMY == unit[n].kind
				&& unit[n].eState != ESTATE_DAMAGED
				&& unit[n].eState != ESTATE_DEAD
				&& unit[n].eState != ESTATE_SLEEP)
			{
				if (touchRay.intersects(unit[n].obbHead))//タッチ座標の法線と敵の当たり判定が接触したかをチェック
				{
					//被弾エフェクトの位置の計算
					float longDist = rayStart.distance(rayEnd);
					float shortDist = rayStart.distance(unit[n].sprite3d->getPosition3D());
					float r = shortDist / longDist;//レイの長さに対する、レイ視点と敵との距離の割合
					Vec3 tmpRay = (rayEnd - rayStart) * r * 0.8f;
					tmpRay += rayStart;
					tmpRay = tmpRay - unit[n].sprite3d->getPosition3D();

					unit[n].hitpoint -= 1;
					Effect::getInstance()->setEnemyHitEffect(unit[n].sprite3d, tmpRay);
					//１体に攻撃するとその時点でbreakする
					break;
				}
			}
		}
		//現在はレイの計算のためにカメラ位置を一時的に変更しているので、処理後に位置を元に戻しています
		cam3d->setPosition3D(tmpPos);//
		cam3d->setRotation3D(tmpRot);//
	}
}


void GameModelsLayer::KillPlayer()
{
	auto GM = GameMaster::GetInstance();

	GM->SetPlayerState(PSTATE_DEAD);

	//必要ならばモーションの停止を行う
	player.sprite3d->stopAllActions();
	player.sprite3d->startAnimation("dei1");//


	//プレイヤーの座標を指定位置に動かす
	//座標と角度をセットしてキャラクターの座標を補正
	player.sprite3d->setRotation3D(GM->stagePoint[GM->sPoint].pRot);
	player.wrapper->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));


	//座標と角度をセットしてカメラの座標を補正
	ChangeCamera(2);

	//その他必要な処理があればここに追記します
}


Vec2 GameModelsLayer::calcCamPos(float pRot, int pSide)
{
	Vec2 ret = Vec2(0.0f, 0.0f);
	cNode.gNode->setRotation(0.0f);
	cNode.gNode->setPosition(Vec2(0.0f, 0.0f));

	cNode.lNode->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	cNode.lNode->setPosition(Vec2(0.0f, 0.0f));

	cNode.lNode2->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	cNode.lNode2->setPosition(Vec2(0.0f, 0.0f));

	//プレイヤーの向きに応じて処理を変更する
	if (PSIDE_LEFT == pSide)
	{
		cNode.lNode->setPositionX(C_SETX_L);//
		cNode.lNode->setPositionY(C_SETZ_L);//
		cNode.lNode2->setPositionX(HIDECAMERA_X);
		cNode.lNode2->setPositionY(HIDECAMERA_Y);

		Vec2 test = cNode.lNode2->getPosition();

		cNode.gNode->setRotation(pRot);
		ret = cNode.lNode->convertToWorldSpace(cNode.lNode2->getPosition());//回転後のカメラの座標（プレイヤーとの相対位置）
		ret = cNode.gNode->convertToWorldSpace(ret);//回転後のカメラの座標（プレイヤーとの相対位置）
	}
	else
	{
		cNode.lNode->setPositionX(C_SETX_R);//
		cNode.lNode->setPositionY(C_SETZ_R);//
		cNode.lNode2->setPositionX(-HIDECAMERA_X);
		cNode.lNode2->setPositionY(HIDECAMERA_Y);

		cNode.gNode->setRotation(pRot);
		ret = cNode.lNode->convertToWorldSpace(cNode.lNode2->getPosition());//回転後のカメラの座標（プレイヤーとの相対位置）
		ret = cNode.gNode->convertToWorldSpace(ret);//回転後のカメラの座標（プレイヤーとの相対位置）
	}
	return ret;
}


Vec2 GameModelsLayer::calcCamPos2(float pRot, int pSide)
{
	//プレイヤーの先頭位置と角度から、カメラの回避時の座標を計算する
	//回避時のカメラはプレイヤーに追従せず、この関数で計算した座標へ向かって個別に移動します
	Vec2 pos = Vec2(0.0f, 0.0f);
	cNode.gNode->setRotation(0.0f);
	cNode.gNode->setPosition(Vec2(0.0f, 0.0f));

	cNode.lNode->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	cNode.lNode->setPosition(Vec2(0.0f, 0.0f));

	cNode.lNode2->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	cNode.lNode2->setPosition(Vec2(0.0f, 0.0f));

	//プレイヤーの向きに応じて処理を変更する
	if (PSIDE_LEFT == pSide)
	{
		cNode.lNode->setPositionX(HIDEPOINT_X);//カメラの親ノード
		cNode.lNode->setPositionY(HIDEPOINT_Y);//〃
		cNode.lNode2->setPositionX(-HIDECAMERA_X);//回避後の座標
		cNode.lNode2->setPositionY(HIDECAMERA_Y);

		cNode.gNode->setRotation(pRot);

		pos = cNode.gNode->convertToWorldSpace(cNode.lNode2->getPosition());//回転後のlNode2の座標を取得
	}
	else
	{
		cNode.lNode->setPositionX(HIDEPOINT_X);//カメラの親ノード
		cNode.lNode->setPositionY(HIDEPOINT_Y);//〃
		cNode.lNode2->setPositionX(HIDECAMERA_X);//回避後の座標
		cNode.lNode2->setPositionY(HIDECAMERA_Y);

		cNode.gNode->setRotation(pRot);

		pos = cNode.gNode->convertToWorldSpace(cNode.lNode2->getPosition());//回転後のlNode2の座標を取得
	}
	return pos;
}


Vec2 GameModelsLayer::calcCamPos3(float pRot, int pSide)
{
	Vec2 ret = Vec2(0.0f, 0.0f);
	cNode.gNode->setRotation(0.0f);
	cNode.gNode->setPosition(Vec2(0.0f, 0.0f));

	cNode.lNode->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	cNode.lNode->setPosition(Vec2(0.0f, 0.0f));

	cNode.lNode2->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	cNode.lNode2->setPosition(Vec2(0.0f, 0.0f));

	//プレイヤーの向きに応じて処理を変更する
	if (PSIDE_LEFT == pSide)
	{
		cNode.lNode->setPositionX(HIDECAMERA_X);//カメラの親ノードの回避後の座標
		cNode.lNode->setPositionY(HIDECAMERA_Y);//カメラの親ノードの回避後の座標

		cNode.gNode->setRotation(pRot);//カメラの親ノードをプレイヤーの向きだけ回転させる

		ret = cNode.gNode->convertToWorldSpace(cNode.lNode->getPosition());//回転後のlNodeの座標を取得

		//1フレーム当たりの移動量が計算できた
		//ret.x = ret.x / STS_HIDEWAIT;
		//ret.y = ret.y / STS_HIDEWAIT;

		//1秒当たりの移動量を計算する
		ret.x = ret.x / MACRO_FtoMS(STS_HIDEWAIT);
		ret.y = ret.y / MACRO_FtoMS(STS_HIDEWAIT);
	}
	else
	{
		cNode.lNode->setPositionX(-HIDECAMERA_X);//カメラの親ノードの回避後の座標
		cNode.lNode->setPositionY(HIDECAMERA_Y);//カメラの親ノードの回避後の座標

		cNode.gNode->setRotation(pRot);//カメラの親ノードをプレイヤーの向きだけ回転させる

		ret = cNode.gNode->convertToWorldSpace(cNode.lNode->getPosition());//回転後のlNodeの座標を取得
		//1フレーム当たりの移動量が計算できた
		//ret.x = ret.x / STS_HIDEWAIT;
		//ret.y = ret.y / STS_HIDEWAIT;

		//1秒当たりの移動量を計算する
		ret.x = ret.x / MACRO_FtoMS(STS_HIDEWAIT);
		ret.y = ret.y / MACRO_FtoMS(STS_HIDEWAIT);
	}
	return ret;
}


void GameModelsLayer::SetLights()
{
	auto GM = GameMaster::GetInstance();

	//ライトをレイヤーにセット
	auto camera = GM->GetCamera3D();

	int dark = 128;
	auto ambient = AmbientLight::create(Color3B(dark, dark, dark));
	ambient->setLightFlag(cocos2d::LightFlag::LIGHT2);//数字は適当
	camera->addChild(ambient);

	int color = 255;
	auto direction = DirectionLight::create(Vec3(-5.0f, -5.0f, 3.0f), Color3B(color, color, color));
	direction->setPosition3D(Vec3(50.0f, 100.0f, -100.0f));
	direction->setLightFlag(cocos2d::LightFlag::LIGHT3);//数字は適当

	camera->addChild(direction);

}



int GameModelsLayer::ChangeCamera(int num)
{
	//ゲームの状態変化に合わせてカメラを切り替えることができます
	//カメラの座標と角度も変更できますが、あくまでカメラの初期位置であり、移動と回転は他で行う必要があります
	//カメラの動きを注視点管理で統一したら、この関数は使わなくする予定です

	auto GM = GameMaster::GetInstance();

	switch (num)
	{
	case 0://0番はOPデモ用の初期設定

		break;
	case 1://1番はゲーム本編の初期設定

		break;
	case 2://2番は死亡用の初期設定

		//カメラの位置を任意の値に設定する
		GM->SetCamera3DPos(Vec3(KE_POSX, KE_POSY, KE_POSZ));
		GM->SetCameraTarget(Vec3(KT_POSX, KT_POSY, KT_POSZ));

		//カメラ座標と注視点からカメラ角度を設定する
		GM->CalcCameraRot();
		break;
	}
	return TRUE;
}



Vec2 GameModelsLayer::calcRot(float pRot, int pSide)
{
	Vec2 ret = Vec2(0.0f, 0.0f);
	cNode.gNode->setRotation(0.0f);
	cNode.gNode->setPosition(Vec2(0.0f, 0.0f));

	cNode.lNode->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	cNode.lNode->setPosition(Vec2(0.0f, 0.0f));

	//プレイヤーの向きに応じて処理を変更する
	if (PSIDE_LEFT == pSide)
	{
		cNode.lNode->setPositionX(HIDEPOINT_X);//回避座標を代入
		cNode.lNode->setPositionY(HIDEPOINT_Y);//回避座標を代入

		cNode.gNode->setRotation(pRot);

		ret = cNode.gNode->convertToWorldSpace(cNode.lNode->getPosition());//回転後のlNodeの座標を取得
	}
	else
	{
		cNode.lNode->setPositionX(-HIDEPOINT_X);//回避座標を代入
		cNode.lNode->setPositionY(HIDEPOINT_Y);//回避座標を代

		cNode.gNode->setRotation(pRot);
		ret = cNode.gNode->convertToWorldSpace(cNode.lNode->getPosition());//回転後のlNodeの座標を取得
	}
	return ret;
}


float GameModelsLayer::getCross(cocos2d::Vec3 v1, cocos2d::Vec3 v2)
{
	Vec2 vec1 = Vec2(v1.x, v1.z);
	Vec2 vec2 = Vec2(v2.x, v2.z);
	return vec1.cross(vec2);
}