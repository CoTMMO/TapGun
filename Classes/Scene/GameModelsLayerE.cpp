#include "GameModelsLayer.h"

#include "Base/Sprite3D.h"
#include "Base/GameMaster.h"
#include "System/Sound.h"
#include "System/ResourceLoader.h"
#include "Object/Effect.h"


USING_NS_CC;
using namespace TapGun;


/**
*	全モデル初期化
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
void GameModelsLayer::InitAllModels()
{
	for (int i = 0; i < MAX_UNIT; i++)
	{
		unit[i].Init();//メンバ変数初期化
	}
}



int GameModelsLayer::InitEnemy()
{
	//全てのエネミーユニットを初期化
	//エネミーのセットはsetEnemyで行う

	for(int i = UNIT1_ENEMY; i < UNIT2_BULLET; i++)
	{
		unit[i].Init(i, UKIND_ENEMY);
		unit[i].visible = FALSE;
		unit[i].sprite3d->setVisible(false);
		unit[i].collisionPos = Vec3(0.9f, 1.6f, 0.9f);//範囲を指定して
		unit[i].SetCollision();//
		unit[i].eState = ESTATE_NONE;

		unit[i].node1->setPosition3D(Vec3(POS_ENEMY_LPOX, POS_ENEMY_LPOY, POS_ENEMY_LPOZ));//
		unit[i].node2->setPosition3D(Vec3(POS_ENEMY_RPOX, POS_ENEMY_RPOY, POS_ENEMY_RPOZ));//
	}
	return TRUE;
}


/**
*	敵弾初期化
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
void GameModelsLayer::InitBullet()
{
	//全てのエネミーユニットを初期化
	//エネミーのセットはsetEnemyで行う
	for(int i = UNIT2_BULLET; i < UNIT3_MAX; i++)
	{
		unit[i].Init(i, UKIND_EBULLET);
		unit[i].sprite3d->setVisible(false);
		unit[i].eState = ESTATE_NONE;
	}
}


/**
*	ステージごとのエネミーのセット
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
void GameModelsLayer::SetEnemy(void)
{
	auto GM = GameMaster::GetInstance();

	//現在ウェーブで場合分け
	switch (GM->wave)
	{
	case 0://ウェーブ1

		enemyTable->InitAll();//エネミー出現テーブルを初期化（）
		enemyTable->finishNumber = 12;//10体の敵が出る

		//敵の初期化
		for (int i = UNIT1_ENEMY; i < UNIT2_BULLET; i++)
		{
			//
			unit[i].sprite3d->setVisible(false);//敵モデル非表示
			unit[i].visible = FALSE;//敵モデル非表示
			unit[i].sprite3d->setPosition3D(Vec3(0.0f, 0.0f, 0.0f));//敵モデル非表示
			unit[i].eState = ESTATE_SLEEP;
			unit[i].tableNum = -1;
			unit[i].hitpoint = 5;
			unit[i].nowShot = 0;
			unit[i].maxShot = STS_ENEMY_MAXSHOT;

			//
			unit[i].AIMove = -1;
			unit[i].AIAtk = -1;
			unit[i].AIAppear = -1;//
			unit[i].AILife = -1;

			//
			for (int j = 0; j < 3; j++)
			{
				unit[i].nextEnemies[j] = -1;
			}

			//
			for (int j = 0; j < 3; j++)
			{
				unit[i].stsAtkFrame[j] = 0.0f;
			}

			unit[i].atkFrame = 0.0f;//
			unit[i].eWaitFrame = 0.0f;//出現までの待ちフレーム
			unit[i].sleepTime = 0.0f;
		}

		//敵の出現ポイントをセット
		enemyTable->standbyPos[0] = (Vec3(16.665f, 0.0f, -14.551f));
		enemyTable->standbyPos[1] = (Vec3(15.682f, 0.0f, -13.016f));
		enemyTable->standbyPos[2] = (Vec3(12.660f, 0.0f, -5.490f));
		enemyTable->standbyPos[3] = (Vec3(9.705f, 0.0f, -16.823f));
		enemyTable->standbyPos[4] = (Vec3(16.665f, 0.0f, -14.551f));

		enemyTable->standbyPos[5] = (Vec3(15.682f, 0.0f, -13.016f));
		enemyTable->standbyPos[6] = (Vec3(9.19f, 0.0f, -16.823f));
		enemyTable->standbyPos[7] = (Vec3(15.682f, 0.0f, -13.016f));
		enemyTable->standbyPos[8] = (Vec3(12.66f, 0.0f, -5.49f));
		enemyTable->standbyPos[9] = (Vec3(7.402f, 0.0f, -14.74f));


		//敵の目標地点をセット
		//enemyTable->targetPos[0][0] = (Vec3(11.385f, 0.0f, -10.211f));
		enemyTable->targetPos[0][1] = (Vec3(10.613f, 0.0f, -10.226f));
		enemyTable->targetPos[0][2] = (Vec3(10.247f, 0.0f, -7.78f));

		//enemyTable->targetPos[1][0] = (Vec3(9.705f, 0.0f, -8.381f));
		enemyTable->targetPos[1][1] = (Vec3(10.673f, 0.0f, -13.275f));
		enemyTable->targetPos[1][2] = (Vec3(11.645f, 0.0f, -12.424f));

		//enemyTable->targetPos[2][0] = (Vec3(11.645f, 0.0f, -12.424f));
		//enemyTable->targetPos[2][1] = (Vec3(11.385f, 0.0f, -10.211f));
		enemyTable->targetPos[2][2] = (Vec3(11.504f, 0.0f, -9.227f));

		//enemyTable->targetPos[2][2] = (Vec3(9.705f, 0.0f, -8.381f));
		//enemyTable->targetPos[3][1] = (Vec3(12.21f, 0.0f, -12.327f));
		enemyTable->targetPos[3][2] = (Vec3(9.705f, 0.0f, -8.381f));

		//enemyTable->targetPos[4][0] = (Vec3(11.385f, 0.0f, -7.781f));
		//enemyTable->targetPos[4][1] = (Vec3(9.705f, 0.0f, -8.381f));
		enemyTable->targetPos[4][2] = (Vec3(10.673f, 0.0f, -13.275f));


		//enemyTable->targetPos[5][1] = (Vec3(11.385f, 0.0f, -10.211f));
		enemyTable->targetPos[5][1] = (Vec3(10.55f, 0.0f, -11.23f));
		enemyTable->targetPos[5][2] = (Vec3(11.55f, 0.0f, -9.23f));

		//enemyTable->targetPos[6][1] = (Vec3(11.385f, 0.0f, -7.781f));
		//enemyTable->targetPos[6][2] = (Vec3(9.705f, 0.0f, -8.381f));
		enemyTable->targetPos[6][2] = (Vec3(10.613f, 0.0f, -10.226f));

		//enemyTable->targetPos[6][1] = (Vec3(11.385f, 0.0f, -7.781f));
		//enemyTable->targetPos[6][2] = (Vec3(9.705f, 0.0f, -8.381f));
		enemyTable->targetPos[10][2] = (Vec3(11.813f, 0.0f, -12.226f));

		//サイドジャンプ
		//enemyTable->targetPos[9][1] = (Vec3(11.385f, 0.0f, -7.781f));
		enemyTable->targetPos[7][1] = (Vec3(12.21f, 0.0f, -12.327f));//サイドジャンプの着地位置
		enemyTable->targetPos[7][2] = (Vec3(12.21f, 0.0f, -12.327f));//その場に留まって射撃を行う

		//画面手前からの敵
		//enemyTable->targetPos[7][1] = (Vec3(11.385f, 0.0f, -7.781f));
		//enemyTable->targetPos[7][2] = (Vec3(9.705f, 0.0f, -8.381f));
		enemyTable->targetPos[8][2] = (Vec3(11.704f, 0.0f, -8.25f));


		//アクロバット
		//enemyTable->targetPos[8][2] = (Vec3(9.705f, 0.0f, -13.016f));
		enemyTable->targetPos[9][1] = (Vec3(10.673f, 0.0f, -13.275f));//アクロバット開始地点まで走る
		enemyTable->targetPos[9][2] = (Vec3(12.14f, 0.0f, -11.95f));//ここまでアクロバットで移動


		//エネミーの初期座標をセット
		//エネミーの最初のターゲット座標をセット
		//エネミーの行動パターンをセット
		enemyTable->enemyData[0].standbyPos = enemyTable->standbyPos[0];
		enemyTable->enemyData[0].SetTargetPos(enemyTable->targetPos[0]);
		enemyTable->enemyData[0].SetNextEnemy(2, -1, -1);
		enemyTable->enemyData[0].alive = TRUE;
		enemyTable->enemyData[0].SetAI(AI_APPEAR_RUN, AI_ATK_NONE, AI_MOVE_WALK, AI_LIFE_SWITCH);
		enemyTable->enemyData[0].sleepTime = 1000;
		enemyTable->enemyData[0].hitpoint = 5;
		enemyTable->enemyData[0].stsAtkFrame[1] = 2000;
		enemyTable->enemyData[0].stsAtkFrame[2] = 2000;


		enemyTable->enemyData[1].standbyPos = enemyTable->standbyPos[1];
		enemyTable->enemyData[1].SetTargetPos(enemyTable->targetPos[1]);
		enemyTable->enemyData[1].SetNextEnemy(3, -1, -1);
		enemyTable->enemyData[1].alive = TRUE;
		enemyTable->enemyData[1].SetAI(AI_APPEAR_RUN, AI_ATK_NONE, AI_MOVE_SWALK, AI_LIFE_SWITCH);
		enemyTable->enemyData[1].sleepTime = 1000;
		enemyTable->enemyData[1].hitpoint = 5;
		enemyTable->enemyData[1].stsAtkFrame[1] = 3000;
		enemyTable->enemyData[1].stsAtkFrame[2] = 3000;


		enemyTable->enemyData[2].standbyPos = enemyTable->standbyPos[2];
		enemyTable->enemyData[2].SetTargetPos(enemyTable->targetPos[2]);
		enemyTable->enemyData[2].SetNextEnemy(4, -1, -1);
		enemyTable->enemyData[2].alive = TRUE;
		enemyTable->enemyData[2].SetAI(AI_APPEAR_RUN,  AI_ATK_FAKE, AI_MOVE_NONE, AI_LIFE_STOP);
		enemyTable->enemyData[2].sleepTime = 1000;
		enemyTable->enemyData[2].hitpoint = 5;
		enemyTable->enemyData[2].stsAtkFrame[2] = 2500;


		enemyTable->enemyData[3].standbyPos = enemyTable->standbyPos[3];
		enemyTable->enemyData[3].SetTargetPos(enemyTable->targetPos[3]);
		enemyTable->enemyData[3].SetNextEnemy(5, -1, -1);
		enemyTable->enemyData[3].alive = TRUE;
		enemyTable->enemyData[3].SetAI(AI_APPEAR_RUN, AI_ATK_SSHOT, AI_MOVE_NONE, AI_LIFE_STOP);
		enemyTable->enemyData[3].sleepTime = 1000;
		enemyTable->enemyData[3].hitpoint = 5;
		enemyTable->enemyData[3].stsAtkFrame[2] = 2500;


		enemyTable->enemyData[4].standbyPos = enemyTable->standbyPos[4];
		enemyTable->enemyData[4].SetTargetPos(enemyTable->targetPos[4]);
		enemyTable->enemyData[4].SetNextEnemy(6, 10, -1);
		enemyTable->enemyData[4].alive = TRUE;
		enemyTable->enemyData[4].SetAI(AI_APPEAR_RUN,  AI_ATK_FAKE, AI_MOVE_NONE, AI_LIFE_STOP);
		enemyTable->enemyData[4].sleepTime = 1000;
		enemyTable->enemyData[4].hitpoint = 5;
		enemyTable->enemyData[4].stsAtkFrame[2] = 2500;


		//要チェック
		enemyTable->enemyData[5].standbyPos = enemyTable->standbyPos[5];
		enemyTable->enemyData[5].SetTargetPos(enemyTable->targetPos[5]);
		enemyTable->enemyData[5].SetNextEnemy(7, -1, -1);
		enemyTable->enemyData[5].alive = TRUE;
		enemyTable->enemyData[5].SetAI(AI_APPEAR_RUN, AI_ATK_SSHOT, AI_MOVE_WALK, AI_LIFE_STOP);
		enemyTable->enemyData[5].sleepTime = 1000;
		enemyTable->enemyData[5].hitpoint = 5;
		enemyTable->enemyData[5].stsAtkFrame[1] = 3000;
		enemyTable->enemyData[5].stsAtkFrame[2] = 3000;


		//二体出てくる敵
		enemyTable->enemyData[6].standbyPos = enemyTable->standbyPos[6];
		enemyTable->enemyData[6].SetTargetPos(enemyTable->targetPos[6]);
		enemyTable->enemyData[6].SetNextEnemy(8, -1, -1);
		enemyTable->enemyData[6].alive = TRUE;
		enemyTable->enemyData[6].SetAI(AI_APPEAR_RUN, AI_ATK_SSHOT, AI_MOVE_NONE, AI_LIFE_STOP);
		enemyTable->enemyData[6].sleepTime = 980;
		enemyTable->enemyData[6].hitpoint = 5;
		enemyTable->enemyData[6].stsAtkFrame[2] = 2000;

		enemyTable->enemyData[10].standbyPos = enemyTable->standbyPos[6];//とりあえず6番と同じ所からスタート
		enemyTable->enemyData[10].SetTargetPos(enemyTable->targetPos[10]);
		enemyTable->enemyData[10].SetNextEnemy(-1, -1, -1);
		enemyTable->enemyData[10].alive = TRUE;
		enemyTable->enemyData[10].SetAI(AI_APPEAR_RUN, AI_ATK_SSHOT, AI_MOVE_NONE, AI_LIFE_STOP);
		enemyTable->enemyData[10].sleepTime = 1600;
		enemyTable->enemyData[10].hitpoint = 5;
		enemyTable->enemyData[10].stsAtkFrame[2] = 2500;


		//サイドジャンプ行動を行う
		//要チェック
		enemyTable->enemyData[7].standbyPos = enemyTable->standbyPos[7];
		enemyTable->enemyData[7].SetTargetPos(enemyTable->targetPos[7]);
		enemyTable->enemyData[7].SetNextEnemy(9, -1, -1);
		enemyTable->enemyData[7].alive = TRUE;
		enemyTable->enemyData[7].SetAI(AI_APPEAR_SLIDE, AI_ATK_SLIDE, AI_MOVE_NONE, AI_LIFE_STOP);//サイドジャンプモーションをアニメーション管理で分割します
		enemyTable->enemyData[7].sleepTime = 1000;
		enemyTable->enemyData[7].hitpoint = 5;
		enemyTable->enemyData[7].stsAtkFrame[1] = 100;//着地後攻撃は素早い
		enemyTable->enemyData[7].stsAtkFrame[2] = 3000;//その後は隙を作る


		//画面手前から出てくる敵
		enemyTable->enemyData[8].standbyPos = enemyTable->standbyPos[8];
		enemyTable->enemyData[8].SetTargetPos(enemyTable->targetPos[8]);
		enemyTable->enemyData[8].SetNextEnemy(-1, -1, -1);
		enemyTable->enemyData[8].finishFlag = TRUE;
		enemyTable->enemyData[8].alive = TRUE;
		enemyTable->enemyData[8].SetAI(AI_APPEAR_RUN, AI_ATK_SSHOT, AI_MOVE_NONE, AI_LIFE_STOP);
		enemyTable->enemyData[8].sleepTime = 1000;
		enemyTable->enemyData[8].hitpoint = 5;
		//enemyTable->enemyData[0].stsAtkFrame[0] = 
		//->enemyData[5].stsAtkFrame[1] = 1000;
		enemyTable->enemyData[8].stsAtkFrame[2] = 3000;

		//要チェック
		//アクロバット移動の代わりに移動攻撃の繰り返しに修正
		enemyTable->enemyData[9].standbyPos = enemyTable->standbyPos[9];
		enemyTable->enemyData[9].SetTargetPos(enemyTable->targetPos[9]);
		enemyTable->enemyData[9].SetNextEnemy(-1, -1, -1);
		enemyTable->enemyData[9].finishFlag = TRUE;
		enemyTable->enemyData[9].alive = TRUE;
		enemyTable->enemyData[9].SetAI(AI_APPEAR_RUN, AI_ATK_SSHOT,AI_MOVE_RUN, AI_LIFE_SWITCH);//
		enemyTable->enemyData[9].sleepTime = 1000;
		enemyTable->enemyData[9].hitpoint = 5;
		enemyTable->enemyData[9].stsAtkFrame[1] = 300;
		enemyTable->enemyData[9].stsAtkFrame[2] = 1200;


		//敵の出現定義が済んだら、最初に出現させる敵のセットを行う
		unit[1].eState = ESTATE_SLEEP;
		unit[1].sprite3d->setVisible(false);//敵モデル非表示
		unit[1].visible = TRUE;//敵モデル表示
		unit[1].sprite3d->setPosition3D(enemyTable->enemyData[0].standbyPos);//敵モデル非表示
		unit[1].SetTargetPos(enemyTable->enemyData[0].targetPos);
		unit[1].StandbyPos = enemyTable->enemyData[0].standbyPos;
		for(int i = 0; i < 3; i++)
		{
			unit[1].stsAtkFrame[i] = enemyTable->enemyData[0].stsAtkFrame[i];
		}
		unit[1].tableNum = 0;
		unit[1].AIAppear = enemyTable->enemyData[0].AIappear;
		unit[1].AIAtk = enemyTable->enemyData[0].AIAtk;
		unit[1].AIMove = enemyTable->enemyData[0].AIMove;
		unit[1].AILife = enemyTable->enemyData[0].AILife;


		unit[1].stsWaitFrame = enemyTable->enemyData[0].stsWaitFrame;

		unit[2].eState = ESTATE_SLEEP;
		unit[2].sprite3d->setVisible(false);//敵モデル非表示
		unit[2].visible = TRUE;//敵モデル表示
		unit[2].sprite3d->setPosition3D(enemyTable->enemyData[1].standbyPos);//敵モデル非表示
		unit[2].SetTargetPos(enemyTable->enemyData[1].targetPos);
		unit[2].StandbyPos = enemyTable->enemyData[1].standbyPos;
		unit[2].tableNum = 1;

		unit[2].AIAppear = enemyTable->enemyData[1].AIappear;
		unit[2].AIAtk = enemyTable->enemyData[1].AIAtk;
		unit[2].AIMove = enemyTable->enemyData[1].AIMove;
		unit[2].AILife = enemyTable->enemyData[1].AILife;
		for(int i = 0; i < 3; i++)
		{
			unit[2].stsAtkFrame[i] = enemyTable->enemyData[1].stsAtkFrame[i];
		}
		unit[2].stsWaitFrame = enemyTable->enemyData[1].stsWaitFrame;

		break;
	case 1://ウェーブ2


		break;
	case 2://ウェーブ2


		break;
	default:
		break;
	}
}



/**
*	敵に弾を発射させる
*
*	@author	sasebon
*	@param	弾を撃った敵の番号
*	@return	なし
*	@date	2/12 Ver 1.0
*/
void GameModelsLayer::ShootBullet(int enemy_num)
{
	auto GM = GameMaster::GetInstance();

	int num = -1;
	for (int i = UNIT2_BULLET; i < UNIT3_MAX; i++)
	{
		if (FALSE == unit[i].visible)
		{
			//表示されていない弾用構造体の配列番号を取得する
			num = i;
			break;
		}
	}

	//
	if (num != -1)
	{
		//弾を一度初期化する
		unit[num].Init(num, UKIND_EBULLET);

		unit[num].sprite3d->setScale(0.5f);//

		//弾を撃ったエネミーの座標と、プレイヤーの座標を元に、弾の移動方向を求める
		Vec3 enemyPos = unit[enemy_num].sprite3d->getPosition3D();

		//プレイヤーに向けて弾を発射する
		unit[num].speedVec = GM->stagePoint[GM->sPoint].pPos - enemyPos;//

		//ベクトルの正規化を行う
		unit[num].speedVec.normalize();

		//キャラクターの向きを調整
		double r = atan2f(unit[num].speedVec.z, unit[num].speedVec.x);
		r = CC_RADIANS_TO_DEGREES(r);

		//		unit[num].sprite3d->setRotation3D(Vec3(-90.0f, 90.0f - r, 0.0f));//
		unit[num].sprite3d->setRotation3D(Vec3(0.0f, 90.0f - r, 0.0f));//

		//正規化が終わったら、速度をかけて方向ベクトルの計算終了
		unit[num].speed = STS_EBULLET_SPEED;
		unit[num].speedVec.x *= unit[num].speed;
		unit[num].speedVec.z *= unit[num].speed;
		unit[num].speedVec.y = 0.0f;

		unit[num].sprite3d->setPosition3D(enemyPos);
		unit[num].sprite3d->setPositionY(1.2f);

		//弾を画面に描画する
		unit[num].visible = TRUE;
		unit[num].sprite3d->setVisible(true);
	}
}




/**
*	敵に弾を発射させる
*
*	@author	sasebon
*	@param	弾を撃った敵の番号,発射したい弾数
*	@return	なし
*	@date	2/12 Ver 1.0
*/
void GameModelsLayer::ShootBullet(int enemy_num, int count)
{
	auto GM = GameMaster::GetInstance();

	int num = -1;
	for (int i = UNIT2_BULLET; i < UNIT3_MAX; i++)
	{
		if (FALSE == unit[i].visible)
		{
			//表示されていない弾用構造体の配列番号を取得する
			num = i;
			break;
		}
	}

	//
	if (num != -1)
	{
		//弾を一度初期化する
		unit[num].Init(num, UKIND_EBULLET);

		unit[num].sprite3d->setScale(0.5f);//

		//弾を撃ったエネミーの座標と、プレイヤーの座標を元に、弾の移動方向を求める
		Vec3 enemyPos;
		if (0 == (count % 2))
		{
			enemyPos = unit[enemy_num].node1->getPosition3D() + unit[enemy_num].sprite3d->getPosition3D();
		}
		else
		{
			enemyPos = unit[enemy_num].node2->getPosition3D() + unit[enemy_num].sprite3d->getPosition3D();
		}


		//プレイヤーに向けて弾を発射する
		unit[num].speedVec = GM->stagePoint[GM->sPoint].pPos - enemyPos;//
		//ベクトルの正規化を行う
		unit[num].speedVec.normalize();

		//キャラクターの向きを調整
		double r = atan2f(unit[num].speedVec.z, unit[num].speedVec.x);
		r = CC_RADIANS_TO_DEGREES(r);

		unit[num].sprite3d->setRotation3D(Vec3(0.0f, 90.0f - r, 0.0f));//

		//正規化が終わったら、速度をかけて方向ベクトルの計算終了
		unit[num].speed = STS_EBULLET_SPEED;
		unit[num].speedVec.x *= unit[num].speed;
		unit[num].speedVec.z *= unit[num].speed;
		unit[num].speedVec.y = 0.0f;

		unit[num].sprite3d->setPosition3D(enemyPos);

		//弾を画面に描画する
		unit[num].visible = TRUE;
		unit[num].sprite3d->setVisible(true);
	}
}



/**
*	敵弾の更新処理
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	2/12 Ver 1.0
*/
void GameModelsLayer::UpdateBullets()
{
	auto GM = GameMaster::GetInstance();

	//全ての敵弾ユニットを更新
	for (int num = UNIT2_BULLET; num <= UNIT3_MAX; num++)
	{
		//画面に出ている弾のみを更新
		if (TRUE == unit[num].visible)
		{
			unit[num].Update(GM->loopTime);//座標と一緒に当たり判定を移動

			//指定時間が経過したら消去処理
			if (STS_EBULLET_VANISH_TIME <= unit[num].GetTime())
			{
				unit[num].speed = 0.0f;//スピードを0にする
				unit[num].visible = FALSE;
				unit[num].sprite3d->setVisible(false);
			}
		}
	}
}



/**
*	当たり判定の処理
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	3/9 Ver 1.0
*/
void  GameModelsLayer::CheckEnemyAtk(void)
{
	auto GM = GameMaster::GetInstance();

	if(TRUE == GM->playerHitFlag)
	{
		//プレイヤーの当たり判定が存在する場合
		//全ての敵弾ユニットを更新
		for(int i = UNIT2_BULLET; i < UNIT3_MAX; i++)
		{
			if(TRUE == unit[i].visible)
			{
				//プレイヤーとの当たり判定を処理

				//プレイヤーのグローバル座標を取得
				Vec3 pPos = player.wrapper->getPosition3D() + player.sprite3d->getPosition3D() + player.centerNode->getPosition3D();
				Vec3 dir = pPos - unit[i].sprite3d->getPosition3D();//プレイヤーと敵弾の差のベクトル

				//平面の距離を求める
				float d1 = sqrtf(dir.x * dir.x + dir.y * dir.y);
				//計算した平面上の距離と高さの距離を求める
				d1 = sqrtf((dir.z * dir.z) + (d1 * d1));


				//弾とプレイヤーの位置を元に当たり判定の処理
				if(0.4f > d1)
				{
					//接触した場合は_sprite3Dの解放を行う
					unit[i].sprite3d->setVisible(false);
					unit[i].visible = FALSE;
					unit[i].speedVec = Vec3(0.0f, 0.0f, 0.0f);

					//プレイヤーの状態を食らい判定にする
					GM->AddPlayerHP(-1);//ダメージを計算
					GM->playerHitFlag = FALSE;//食らい判定を無効化

					player.sprite3d->stopALLAnimation();//すべてのアニメーションを中断して
					switch(GM->playerSide)
					{
					case PSIDE_LEFT:
						player.sprite3d->startAnimation("hit_l");//食らいモーションを再生
						break;
					case PSIDE_RIGHT:
						player.sprite3d->startAnimation("hit_r");//食らいモーションを再生
						break;
					}
					GM->SetPlayerState(PSTATE_DAMAGED);//


					//座標と角度をセットしてキャラクターの座標を補正
					player.sprite3d->setRotation3D(GM->stagePoint[GM->sPoint].pRot);
					player.wrapper->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));

					//座標と角度をセットしてカメラの座標を補正
					Vec2 tmp = calcCamPos3(GM->stagePoint[GM->sPoint].pRot.y, GM->playerSide);
					camTarget = Vec3(tmp.x, 0.0f, tmp.y);

					//タイムとフレームの初期化
					//モーション管理用の時間を初期化
					player.motProcTime = 0;//モーションを再生してからの経過時間（秒）

					//一度でも攻撃を受けるとブレーク
					break;
				}
			}
		}
	}

	if((FALSE == GM->playerHitFlag && (PSTATE_DAMAGED == GM->GetPlayerState() || PSTATE_RECOVER == GM->GetPlayerState()))
		|| (FALSE == GM->playerHitFlag && 0 <= GM->mutekiTime && (PSTATE_SHOT == GM->GetPlayerState() || PSTATE_IDLE == GM->GetPlayerState()))
		)
	{
		//（演出のための処理）
		//当たり判定がオフの時かつ、食らい状態または復帰状態
		//当たり判定がオフの時かつ、無敵時間が残っている状態かつ、アイドル状態またはショット状態

		//全ての敵弾ユニットを更新
		for(int i = UNIT2_BULLET; i < UNIT3_MAX; i++)
		{
			if(TRUE == unit[i].visible)
			{
				//プレイヤーとの当たり判定を処理

				//プレイヤーのグローバル座標を取得
				Vec3 pPos = player.wrapper->getPosition3D() + player.sprite3d->getPosition3D() + player.centerNode->getPosition3D();

				Vec3 dir = pPos - unit[i].sprite3d->getPosition3D();//プレイヤーと敵弾の差のベクトル

				//平面の距離を求める
				float d1 = sqrtf(dir.x * dir.x + dir.y * dir.y);
				//計算した平面上の距離と高さの距離を求める
				d1 = sqrtf((dir.z * dir.z) + (d1 * d1));
				if(0.4f > d1)
				{
					//接触した場合は_sprite3Dの解放を行う
					unit[i].sprite3d->setVisible(false);
					unit[i].visible = FALSE;
					unit[i].speedVec = Vec3(0.0f, 0.0f, 0.0f);
				}
			}
		}
	}
}




/**
*	エネミーの更新
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
void GameModelsLayer::UpdateEnemy()
{
	auto GM = GameMaster::GetInstance();

	for(int num = UNIT1_ENEMY; num < UNIT2_BULLET; num++)
	{
		if(TRUE == unit[num].visible)//エネミーが表示されていれば
		{
			unit[num].Update(GM->loopTime);//フレーム・座標・当たり判定の更新

			//敵キャラクターのhpが0になったら
			if(unit[num].hitpoint <= 0 && ESTATE_DAMAGED != unit[num].eState && ESTATE_DEAD != unit[num].eState)
			{
				unit[num].eState = ESTATE_DAMAGED;//ダメージ状態に変更
			}


			switch(unit[num].eState)
			{
			case ESTATE_SLEEP://敵が休眠状態

				unit[num].sleepTime -= GM->loopTime;
				if(unit[num].sleepTime <= 0.0f)
				{
					unit[num].eState = ESTATE_STANDBY;
					unit[num].sprite3d->setVisible(true);
				}
				break;
			case ESTATE_STANDBY://目標地点への移動開始
				ActionEStandby(num);
				break;
			case ESTATE_IDLE://アイドル状態
				ActionEIdle(num);
				break;
			case ESTATE_MOVE://移動状態
				ActionEMove(num);
				break;
			case ESTATE_ATTACK1://攻撃
				ActionEAttack(num);
				break;
			case ESTATE_DAMAGED://被弾
				ActionEDamaged(num);
				break;
			case ESTATE_DEAD://死亡
				ActionEDead(num);
				break;
			case ESTATE_END://死んだ敵の処分
				//敵が死んだら次の敵を確認する
				//
				break;
			}
		}
	}
}




void GameModelsLayer::setNextEnemy(int num)
{
	enemyTable->enemyData[unit[num].tableNum].alive = FALSE;//num番の敵を倒した扱いにする

	//現在のウェーブと、死亡した敵の番号をもとに、次のモデルを準備する
	for(int i = 0; i < 3; i++)
	{
		const int nextNum = enemyTable->enemyData[unit[num].tableNum].nextEnemiesNum[i];
		if(-1 == nextNum)//次のモデルが設定されていなければ
		{
			continue;
		}
		else
		{//次の敵をスタンバイ状態にする
			int n = SearchFreeEnemy();

			unit[n].eState = ESTATE_SLEEP;
			unit[n].sprite3d->setVisible(true);//敵モデル表示
			unit[n].visible = TRUE;//敵モデル表示
			unit[n].sprite3d->setPosition3D(enemyTable->enemyData[nextNum].standbyPos);
			unit[n].StandbyPos = enemyTable->enemyData[nextNum].standbyPos;
			unit[n].SetTargetPos(enemyTable->enemyData[nextNum].targetPos);
			unit[n].nowTargetPos = 0;//ターゲットはスリープ解除したときに決定する
			unit[n].tableNum = nextNum;

			//AIの種類をセット
			unit[n].AIAppear = enemyTable->enemyData[nextNum].AIappear;
			unit[n].AIAtk = enemyTable->enemyData[nextNum].AIAtk;
			unit[n].AIMove = enemyTable->enemyData[nextNum].AIMove;
			unit[n].AILife = enemyTable->enemyData[nextNum].AILife;

			//敵のステータスをセット
			for(int j = 0; j < 3; j++)
			{
				unit[n].stsAtkFrame[j] = enemyTable->enemyData[nextNum].stsAtkFrame[j];
			}
			unit[n].stsWaitFrame = enemyTable->enemyData[nextNum].stsWaitFrame;
			unit[n].hitpoint = enemyTable->enemyData[nextNum].hitpoint;

			//弾の管理
			unit[i].nowShot = 0;
			unit[i].maxShot = enemyTable->enemyData[nextNum].maxShot;
			//フレーム
			unit[n].sleepTime = enemyTable->enemyData[nextNum].sleepTime;
			//

		}
	}
}


void GameModelsLayer::ActionEStandby(int num)
{
	//向きの設定
	//スリープ状態からスタンバイ状態に移行するときは、targetPosの値から移動目標を自動で判断する
	for(int i = 0; i < 3; i++)
	{
		//現在はtargetPos配列に必ず有効座標が入っているものとして処理を組んでいます
		if(Vec3(-999.0f, -999.0f, -999.0f) != unit[num].targetPos[i])
		{
			unit[num].nowTargetPos = i;
			break;
		}
	}

	//出現モーションに応じて動き方を変える
	switch(unit[num].AIAppear)
	{
	case AI_APPEAR_RUN:
		unit[num].speed = STS_ENEMY_RUNSPEED;//
		break;
	case AI_APPEAR_WALK:
		unit[num].speed = STS_ENEMY_WALKSPEED;
		break;
	case AI_APPEAR_SWALK:
		unit[num].speed = STS_ENEMY_SWALKSPEED;
		break;
	case AI_APPEAR_SLIDE:
		unit[num].speed = STS_ENEMY_SJUMPSPEED;//
		break;
	case AI_APPEAR_ACRO:
		unit[num].speed = STS_ENEMY_ACROSPEED;//
		break;
	}

	unit[num].speedVec = unit[num].targetPos[unit[num].nowTargetPos] - unit[num].sprite3d->getPosition3D();
	unit[num].speedVec.normalize();//ベクトルの正規化を行う
	double r = atan2(unit[num].speedVec.z, unit[num].speedVec.x);
	r = CC_RADIANS_TO_DEGREES(r);

	//正規化が終わったら、速度をかけて方向ベクトルの計算終了
	unit[num].speedVec.x *= unit[num].speed;
	unit[num].speedVec.z *= unit[num].speed;
	unit[num].speedVec.y = 0;//yは今のところ0で扱う

	float gai = 0;

	//出現モーションに応じて動き方を変える
	switch(unit[num].AIAppear)
	{
	case AI_APPEAR_RUN:
		unit[num].speed = STS_ENEMY_RUNSPEED;//
		unit[num].sprite3d->setRotation3D(Vec3(0.0f, 90.0f - r, 0.0f));//
		unit[num].sprite3d->startAnimationLoop("run");
		unit[num].eState = ESTATE_MOVE;//待機が終わったら移動
		break;
	case AI_APPEAR_WALK:
		unit[num].speed = STS_ENEMY_WALKSPEED;
		unit[num].sprite3d->setRotation3D(Vec3(0.0f, 90.0f - r, 0.0f));//

		unit[num].sprite3d->startAnimationLoop("walk");
		unit[num].eState = ESTATE_MOVE;//待機が終わったら移動
		break;
	case AI_APPEAR_SWALK:
		//横移動
		unit[num].speed = STS_ENEMY_SWALKSPEED;
		//要チェック
		//外積をもとに
		gai = getCross(unit[num].sprite3d->getPosition3D(), unit[num].targetPos[unit[num].nowTargetPos]);
		if(gai <= 0)
		{
			unit[num].sprite3d->setRotation3D(Vec3(0.0f, 180.0f - r, 0.0f));//横歩きなので正面を向かせる
			unit[num].sprite3d->startAnimationLoop("swalk_l");
		}
		else
		{
			unit[num].sprite3d->setRotation3D(Vec3(0.0f, 90.0f - r, 0.0f));//横歩きなので正面を向かせる
			unit[num].sprite3d->startAnimationLoop("swalk_r");
		}

		unit[num].eState = ESTATE_MOVE;//待機が終わったら移動
		break;
	case AI_APPEAR_SLIDE:
		//横移動
		//要チェック
		unit[num].speed = STS_ENEMY_SJUMPSPEED;//
		gai = getCross(unit[num].sprite3d->getPosition3D(), unit[num].targetPos[unit[num].nowTargetPos]);
		if(gai <= 0)
		{
			unit[num].sprite3d->setRotation3D(Vec3(0.0f, 180.0f - r, 0.0f));//横歩きなので正面を向かせる
			unit[num].sprite3d->startAnimationLoop("slide");
		}
		else
		{
			unit[num].sprite3d->setRotation3D(Vec3(0.0f, 90.0f - r, 0.0f));//横歩きなので正面を向かせる
			unit[num].sprite3d->startAnimationLoop("slide");
		}
		unit[num].eState = ESTATE_MOVE;//待機が終わったら移動

		break;
	case AI_APPEAR_ACRO:
		//横移動
		//要チェック
		unit[num].speed = STS_ENEMY_ACROSPEED;//
		gai = getCross(unit[num].sprite3d->getPosition3D(), unit[num].targetPos[unit[num].nowTargetPos]);
		if(gai <= 0)
		{
			unit[num].sprite3d->setRotation3D(Vec3(0.0f, 180.0f - r, 0.0f));//横歩きなので正面を向かせる
			unit[num].sprite3d->startAnimation("sroll2");
		}
		else
		{
			unit[num].sprite3d->setRotation3D(Vec3(0.0f, 90.0f - r, 0.0f));//横歩きなので正面を向かせる
			unit[num].sprite3d->startAnimation("sroll2");
		}
		unit[num].eState = ESTATE_MOVE;//待機が終わったら移動
		break;
	}
}


void GameModelsLayer::ActionEMove(int num)
{
	auto GM = GameMaster::GetInstance();

	Vec3 tmpPos = unit[num].sprite3d->getPosition3D();
	//目標地点との距離を求める
	float d1 = tmpPos.distance(unit[num].targetPos[unit[num].nowTargetPos]);
	//一定以上目的地に近付いたら
	if(0.19f >= d1)
	{
		//エネミーのライフサイクルで場合分け
		switch(unit[num].AILife)
		{
		case AI_LIFE_ONCE://
			//現在の目的地が最終ポイントであれば、キャラクターを消す
			//それ以外であればアイドル
			if(2 == unit[num].nowTargetPos)
			{
				//敵を消去して、次テーブルを検索
				unit[num].speed = 0.0f;//
				unit[num].speedVec = Vec3(0.0f, 0.0f, 0.0f);
				unit[num].sprite3d->stopALLAnimation();

				unit[num].sprite3d->setVisible(false);
				unit[num].visible = FALSE;
				setNextEnemy(num);//次の敵の出現チェック

				return;
//				break;//ここでブレーク：要チェック
			}
			else
			{
				//それ以外の場合はアイドルに移行
			}
		case AI_LIFE_SWITCH://2点間を移動する
		case AI_LIFE_STOP://最終地点でとまる
			//アイドル

			break;
		}

		//要チェック：登場後即攻撃するキャラクターは、アイドルではなく攻撃に移行させる

		Vec3 tmpPos = GM->stagePoint[GM->sPoint].pPos;//
		unit[num].sprite3d->setPosition3D(unit[num].targetPos[unit[num].nowTargetPos]);//敵をポジションにセット

		tmpPos = tmpPos - unit[num].sprite3d->getPosition3D();//プレイヤーの位置へのベクトルを計算
		tmpPos.normalize();//ベクトルの正規化を行う

		double r = atan2(tmpPos.z, tmpPos.x);
		r = CC_RADIANS_TO_DEGREES(r);
		unit[num].sprite3d->setRotation3D(Vec3(0.0f, 90.0f - r, 0.0f));//

		unit[num].speed = 0.0f;
		unit[num].speedVec = Vec3(0.0f, 0.0f, 0.0f);//移動終了

		//どのキャラクターもまずはアイドル状態に移行する
		unit[num].eState = ESTATE_IDLE;//アイドル状態を開始
		unit[num].sprite3d->stopALLAnimation();//現在のモーションを終了
		unit[num].sprite3d->startAnimationLoop("idle");//
		unit[num].atkFrame = unit[num].stsAtkFrame[unit[num].nowTargetPos];//アイドル時間を設定
		//アニメーションの管理フレームを初期化する
	}
	else
	{

	}
}


void GameModelsLayer::ActionEIdle(int num)
{
	auto GM = GameMaster::GetInstance();

	unit[num].atkFrame -= GM->loopTime;
	//アイドル時間が消費されたら
	if(unit[num].atkFrame <= 0)
	{
		//エネミーの攻撃をセット
		//攻撃しない敵については関数内でステートの更新を行う

		switch(unit[num].AIAtk)
		{
		case AI_ATK_NONE://攻撃モーションを行わない

			//AIの行動パターンをもとに動きを変更する
			if(AI_LIFE_ONCE == unit[num].AILife)
			{
				//目標地点をスイッチする
				unit[num].nowTargetPos += 1;//要チェック
				SetEnemyMove(num);
			}
			else if(AI_LIFE_SWITCH == unit[num].AILife)
			{
				//目標地点をスイッチして再度動かす
				if(2 == unit[num].nowTargetPos)
				{
					unit[num].nowTargetPos = 1;
				}
				else if(1 == unit[num].nowTargetPos)
				{
					unit[num].nowTargetPos = 2;
				}

				SetEnemyMove(num);
			}
			else if(AI_LIFE_STOP == unit[num].AILife)
			{
				//要チェック
				////アイドル状態に移行する
				//unit[num].eState = ESTATE_IDLE;//アイドル状態を開始
				//unit[num].sprite3d->stopALLAnimation();//現在のモーションを終了
				//unit[num].sprite3d->startAnimationLoop("idle");//
				//unit[num].atkFrame = unit[num].stsAtkFrame[unit[num].nowTargetPos];//アイドル時間を設定

				//最終地点に行っていなければ移動、最終地点にいる場合は停止
				if(2 != unit[num].nowTargetPos)
				{
					unit[num].nowTargetPos += 1;
					SetEnemyMove(num);
				}
			}

			break;
		case AI_ATK_FAKE://威嚇攻撃を行う
		case AI_ATK_SSHOT://立ち撃ちを行う
		case AI_ATK_ACRO://アクロバティック

			//アイドル時間を設定
			SetEnemyAtk(num);

			break;
		case AI_ATK_SLIDE://サイドジャンプ射撃

			//アイドル時間を設定
			SetEnemyAtk(num);

			break;

		}
	}
	else
	{

	}
}


void GameModelsLayer::ActionEDamaged(int num)
{
	//HPが0になったら

	//ジャンプする敵については専用処理を記述する

	//それ以外の一般的な敵
	//速度を0にする
	unit[num].speed = 0.0f;//
	unit[num].speedVec = Vec3(0.0f, 0.0f, 0.0f);


	unit[num].sprite3d->stopALLAnimation();
	unit[num].sprite3d->startAnimation("dei1");
	unit[num].eState = ESTATE_DEAD;
}


void GameModelsLayer::ActionEDead(int num)
{
	if (0 == unit[num].sprite3d->checkAnimationState())//死亡アニメーションが終了したら
	{
		unit[num].sprite3d->setVisible(false);
		unit[num].visible = FALSE;
		setNextEnemy(num);//次の敵の出現チェック
	}
}



/*

*/
void GameModelsLayer::ActionEAttack(int num)
{
	auto GM = GameMaster::GetInstance();

	if(0 == unit[num].sprite3d->checkAnimationState())
	{
		unit[num].nowShot = 0;

		//AIの行動パターンをもとに動きを変更する
		if(AI_LIFE_ONCE == unit[num].AILife)
		{
			//目標地点をスイッチする
			unit[num].nowTargetPos += 1;//要チェック
			SetEnemyMove(num);
		}
		else if(AI_LIFE_SWITCH == unit[num].AILife)
		{
			//目標地点をスイッチして再度動かす
			if(2 == unit[num].nowTargetPos)
			{
				unit[num].nowTargetPos = 1;
			}
			else if(1 == unit[num].nowTargetPos)
			{
				unit[num].nowTargetPos = 2;
			}
			SetEnemyMove(num);
		}
		else if(AI_LIFE_STOP == unit[num].AILife)
		{
			//アイドル状態に移行する
			if(2 > unit[num].nowTargetPos)
			{
				//最終地点に着いていない場合は目標地点を更新する
				unit[num].nowTargetPos += 1;
				//その後移動
				SetEnemyMove(num);
			}
			else
			{
				//すでに最終地点の場合はアイドル状態に
				unit[num].eState = ESTATE_IDLE;//アイドル状態を開始
				unit[num].sprite3d->stopALLAnimation();//現在のモーションを終了
				unit[num].sprite3d->startAnimationLoop("idle");//
				unit[num].atkFrame = unit[num].stsAtkFrame[unit[num].nowTargetPos];//アイドル時間を設定
			}
		}
	}
	else
	{
		//敵の攻撃モーションごとに弾の発射タイミングなどを選択
		switch(unit[num].AIAtk)
		{
		case AI_ATK_NONE://攻撃しない
			//
			break;
		case AI_ATK_FAKE://威嚇攻撃を行う

			//タイミングを合わせて射撃を行う
			unit[num].atkFrame -= GM->loopTime;//アタックフレームを減少させていく
			if(0 >= unit[num].atkFrame && STS_ENEMY_MAXSHOT > unit[num].nowShot)
			{
				Effect::getInstance() -> setEnemyMuzzle( unit[num].sprite3d, "Po_1", "Po_2");
				unit[num].atkFrame = STS_SSHOT_SPAN;//次の弾発射までの時間を設定
				unit[num].nowShot++;
			}
			break;
		case AI_ATK_SSHOT://立ち撃ちを行う

			//タイミングを合わせて射撃を行う
			unit[num].atkFrame -= GM->loopTime;//アタックフレームを減少させていく
			if(0 >= unit[num].atkFrame && STS_ENEMY_MAXSHOT > unit[num].nowShot)
			{
				Effect::getInstance() -> setEnemyMuzzle( unit[num].sprite3d, "Po_1", "Po_2");
				//フレームが0になったら
				unit[num].atkFrame = STS_SSHOT_SPAN;//次の弾発射までの時間を設定
				unit[num].nowShot++;
				ShootBullet(num, unit[num].nowShot);
			}
			break;
		case AI_ATK_SLIDE://サイドジャンプ射撃

			//タイミングを合わせて射撃を行う
			unit[num].atkFrame -= GM->loopTime;//アタックフレームを減少させていく
			if(0 >= unit[num].atkFrame && STS_ENEMY_MAXSHOT > unit[num].nowShot)
			{
				Effect::getInstance()->setEnemyMuzzle(unit[num].sprite3d, "Po_1", "Po_2");
				//フレームが0になったら
				unit[num].atkFrame = STS_SSHOT_SPAN;//次の弾発射までの時間を設定
				unit[num].nowShot++;
				ShootBullet(num, unit[num].nowShot);
			}
			break;
		case AI_ATK_ACRO://アクロバティック
			break;
		default:
			break;
		}
	}
}



void GameModelsLayer::SetEnemyAtk(int num)
{
	//攻撃モーションに応じて動きを変化
	switch(unit[num].AIAtk)
	{
		//
	case AI_ATK_NONE:

		//攻撃しない敵はここには入ってこない

		break;
	case AI_ATK_FAKE://威嚇攻撃を行う

		unit[num].eState = ESTATE_ATTACK1;//アタック状態に移る
		unit[num].sprite3d->stopALLAnimation();//現在のモーションを終了し
		unit[num].sprite3d->startAnimation("shot");

		//要チェック　アニメーションに合わせた弾の発射タイミング
		unit[num].atkFrame = STS_SSHOT_START;//弾を発射するまでの残りフレームとして扱う
		break;
	case AI_ATK_SSHOT://立ち撃ちを行う

		unit[num].eState = ESTATE_ATTACK1;//アタック状態に移る
		unit[num].sprite3d->stopALLAnimation();//現在のモーションを終了し
		unit[num].sprite3d->startAnimation("shot");

		//要チェック　アニメーションに合わせた弾の発射タイミング
		unit[num].atkFrame = STS_SSHOT_START;//弾を発射するまでの残りフレームとして扱う
		break;
	case AI_ATK_SLIDE://サイドジャンプ射撃//要チェック

		//要チェック

		unit[num].eState = ESTATE_ATTACK1;//アタック状態に移る
		unit[num].sprite3d->stopALLAnimation();//現在のモーションを終了し
		unit[num].sprite3d->startAnimation("shot");
		//要チェック　アニメーションに合わせた弾の発射タイミング
		unit[num].atkFrame = STS_SSHOT_START;//弾を発射するまでの残りフレーム

		break;
	case AI_ATK_ACRO://アクロバティック//要チェック

		//要チェック

		unit[num].eState = ESTATE_ATTACK1;//アタック状態に移る
		unit[num].sprite3d->stopALLAnimation();//現在のモーションを終了し
		unit[num].sprite3d->startAnimation("shot");
		//要チェック　アニメーションに合わせた弾の発射タイミング
		unit[num].atkFrame = STS_SSHOT_START;//弾を発射するまでの残りフレーム

		break;
	default:

		//要チェック
		unit[num].eState = ESTATE_ATTACK1;//アタック状態に移る
		unit[num].sprite3d->stopALLAnimation();//現在のモーションを終了し
		unit[num].sprite3d->startAnimation("shot");
		//要チェック　アニメーションに合わせた弾の発射タイミング
		unit[num].atkFrame = STS_SSHOT_START;//弾を発射するまでの残りフレーム
		break;
	}
}



void GameModelsLayer::SetEnemyMove(int num)
{
	//敵の移動をセット
	//すでに目標位置は更新されています
	switch(unit[num].AIMove)
	{
	case AI_MOVE_NONE://動かない
		break;
	case AI_MOVE_RUN://走り
		unit[num].speed = STS_ENEMY_RUNSPEED;//
		break;
	case AI_MOVE_WALK://歩き
		unit[num].speed = STS_ENEMY_WALKSPEED;
		break;
	case AI_MOVE_SWALK://横歩き
		unit[num].speed = STS_ENEMY_SWALKSPEED;
		break;
	case AI_MOVE_SLIDE://動かない
		unit[num].speed = STS_ENEMY_SJUMPSPEED;//
		break;
	case AI_MOVE_ACRO://アクロバティック
		unit[num].speed = STS_ENEMY_ACROSPEED;//
		break;
	}
	unit[num].speedVec = unit[num].targetPos[unit[num].nowTargetPos] - unit[num].sprite3d->getPosition3D();
	unit[num].speedVec.normalize();//ベクトルの正規化を行う
	double r = atan2(unit[num].speedVec.z, unit[num].speedVec.x);
	r = CC_RADIANS_TO_DEGREES(r);

	//正規化が終わったら、速度をかけて方向ベクトルの計算終了
	unit[num].speedVec.x *= unit[num].speed;
	unit[num].speedVec.z *= unit[num].speed;
	unit[num].speedVec.y = 0;//yは今のところ0で扱う

	float gai = 0;


	//アニメーション終了
	unit[num].sprite3d->stopALLAnimation();


	//目標地点が決まったら動き方をもとに
	switch(unit[num].AIMove)
	{
	case AI_MOVE_NONE://動かない

		break;
	case AI_MOVE_RUN://走り
		unit[num].sprite3d->setRotation3D(Vec3(0.0f, 90.0f - r, 0.0f));//
		unit[num].sprite3d->startAnimationLoop("run");
		break;
	case AI_MOVE_WALK://歩き
		unit[num].sprite3d->setRotation3D(Vec3(0.0f, 90.0f - r, 0.0f));//

		unit[num].sprite3d->startAnimationLoop("walk");
		break;
	case AI_MOVE_SWALK://横歩き
		//横移動
		//要チェック
		//unit[num].sprite3d->setRotation3D(Vec3(0.0f, 180.0f - r, 0.0f));//横歩きなので正面を向かせる

		//外積をもとに
		gai = getCross(unit[num].sprite3d->getPosition3D(), unit[num].targetPos[unit[num].nowTargetPos]);
		if(gai <= 0)
		{
			unit[num].sprite3d->startAnimationLoop("swalk_r");
			unit[num].sprite3d->setRotation3D(Vec3(0.0f, -90.0f - r, 0.0f));//横歩きなので正面を向かせる
		}
		else
		{
			unit[num].sprite3d->startAnimationLoop("swalk_l");
			unit[num].sprite3d->setRotation3D(Vec3(0.0f, 90.0f - r, 0.0f));//横歩きなので正面を向かせる
		}
		break;
	case AI_MOVE_SLIDE://
		//横移動
		//要チェック
		unit[num].sprite3d->setRotation3D(Vec3(0.0f, 180.0f - r, 0.0f));//

		break;
	case AI_MOVE_ACRO://アクロバティック
		//横移動
		//要チェック
		unit[num].sprite3d->setRotation3D(Vec3(0.0f, 180.0f - r, 0.0f));//


		break;
	}
	unit[num].eState = ESTATE_MOVE;
}


void GameModelsLayer::ClearEnemies()
{
	for (int i = UNIT1_ENEMY; i < UNIT2_BULLET; i++)
	{
		//現在アクティブなキャラクターを全てオフにする
		if (TRUE == unit[i].visible)
		{
			unit[i].visible = FALSE;
			unit[i].sprite3d->setVisible(false);

			//必要な初期化処理があればここに追記します
		}
	}
}


int GameModelsLayer::CheckNextWave(void)
{
	auto GM = GameMaster::GetInstance();

	if (GSTATE_PLAY == GM->GetGameState())
	{
		for (int i = 0; i <= 100; i++)//
		{
			if (TRUE == enemyTable->enemyData[i].alive)
			{
				//1体でも敵が生きていれば
				return FALSE;
			}
		}

		//プレイヤーの座標を指定位置に動かす
		//座標と角度をセットしてキャラクターの座標を補正
		player.sprite3d->setRotation3D(GM->stagePoint[GM->sPoint].pRot);
		player.wrapper->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));

		std::string idle;
		if (PSIDE_LEFT == GM->playerSide)
		{
			idle = "idle_l";
		}
		else
		{
			idle = "idle_r";
		}
		GM->SetGameState(GSTATE_WAIT);//次のウェイトへ進む
		GM->wave += 1;//次のステージへ進む
		GM->waitFlag = 0;//
		GM->sPoint += 1;

		//
		player.sprite3d->stopALLAnimation();
		player.sprite3d->startAnimation(idle);
	}
	else
	{
		//戦闘中で無ければ常にfalse
		return FALSE;
	}

	return TRUE;
}


int GameModelsLayer::SearchFreeEnemy()
{
	for (int i = UNIT1_ENEMY; i <= UNIT2_BULLET; i++)
	{
		if (FALSE == unit[i].visible)
		{
			return i;
		}
	}
	return -1;
}

