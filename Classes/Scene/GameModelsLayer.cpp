#include "GameModelsLayer.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "GameMaster.h"
#include "Sprite3D.h"
#include "Sound.h"
#include "ResourceLoader.h"

#else

#include "Base/Sprite3D.h"
#include "Base/GameMaster.h"
#include "System/Sound.h"
#include "System/ResourceLoader.h"

#endif


USING_NS_CC;
using namespace TapGun;

GameMaster* GameMasterM;//変数名は今後考慮する

int rotationCount;
float rotationR;//
Vec3 changeCameraPos;
Vec3 changeCameraRot;
 
/**
*	ゲーム本編のモデルレイヤーの初期化
*
*	@author	sasebon
*	@param	なし
*	@return	初期化成功／不可のbool値
*	@date	1/8 Ver 1.0
*/
bool GameModelsLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}
	auto dispatcher = Director::getInstance()->getEventDispatcher();

	GameMasterM = GameMaster::GetInstance();//ゲームパラメータクラスの初期化
	return true;
}



/**
*	ゲーム本編のUIレイヤーの各種数値初期化
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
void GameModelsLayer::InitLayer(void)
{
	//	LoadModels();//スプライトの生成
	//	player.createMuzzle(player.sprite3d);
	//	player.muzzleFlagOff();
	InitAllModels();

	InitPlayer(0);//とりあえず引数0
	InitMap(0);//正規のマップデータが降りてくるまでいったん保留します

	enemyTable->InitAll();//エネミー出現テーブルを初期化（現在はすべて0）

	InitEnemy(0);
	InitBullet();

	//timeval構造体の初期化
	nowTV = new timeval();
	preTV = new timeval();
	memset(nowTV, 0, sizeof(timeval));
	memset(preTV, 0, sizeof(timeval));
}



/**
*	ゲーム本編のスプライトの生成
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	2/4 Ver 1.0
*/
void GameModelsLayer::LoadModels()
{
	//各ユニットのノードなど、createする必要があるものは同時に行う
	//addChildも同時に行う

	//プレイヤーのロード
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	std::string fileName1 = "player";
	std::string fileName2 = "Player.anime";
#else
	std::string fileName1 = "player/player";
	std::string fileName2 = "Player.anime";
#endif

	auto aa = ResourceLoader::getInstance();

	//スプライトとノードのcreate
//	player.sprite3d = aa->getSprite3D(ResourceLoader::ModelNumber::Player);
	player.sprite3d = _Sprite3D::create(fileName1, fileName2);
	player.wrapper = Node::create();//モデルの親ノード
	player.leftNode = Node::create();//プレイヤーの回転軸の基準ノード（左）
	player.rightNode = Node::create();//プレイヤーの回転軸の基準ノード（右）
	player.centerNode = Node::create();
	player.colisionNode = Node::create();;//当たり判定基準用ノード

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
	unit[UNIT0_MAP].sprite3d = _Sprite3D::create(fileName1);//0番は現在マップに割り当て
//	unit[UNIT0_MAP].sprite3d = aa->getSprite3D(ResourceLoader::ModelNumber::Map);//0番は現在マップに割り当て
	addChild(unit[UNIT0_MAP].sprite3d);


	//敵のロード
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	fileName1 = "enemy";
	fileName2 = "Enemy.anime";
#else
	fileName1 = "enemy/enemy";
	fileName2 = "Enemy.anime";
#endif
	for(int i = UNIT1_ENEMY; i < UNIT2_BULLET; i++)
	{
		//スプライトとノードのcreate
		unit[i].sprite3d = _Sprite3D::create(fileName1, fileName2);//1番~20番を敵に割り当て
//		unit[i].sprite3d = aa->getSprite3D(ResourceLoader::ModelNumber::EnemyStart + i - 1);
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

	for(int i = UNIT2_BULLET; i < UNIT3_MAX; i++)
	{
		unit[i].sprite3d = _Sprite3D::create(fileName1);
//		unit[i].sprite3d = aa->getSprite3D(ResourceLoader::ModelNumber::BulletStart + i - 1);
		unit[i].wrapper = Node::create();
		unit[i].node1 = Node::create();//弾の先端用ノード
		//		unit[i].colisionNode = Node::create();

		unit[i].wrapper->addChild(unit[i].sprite3d);
		addChild(unit[i].wrapper);
		unit[i].wrapper->addChild(unit[i].node1);
		//		unit[i].sprite3d->addChild(unit[i].colisionNode);//当たり判定の基準ノードを3dスプライトに紐付け
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


/**
*	プレイヤー初期化
*
*	@author	sasebon
*	@param	ステージ番号
*	@return	なし
*	@date	1/8 Ver 1.0
*/
void GameModelsLayer::InitPlayer(int wave_num)
{
	player.Init();//プレイヤーの情報を初期化

	//プレイヤー固有の初期化（今後場所を変更する）
	GameMasterM->playerHitFlag = TRUE;//
	GameMasterM->SetPlayerHP(STS_MAXPLAYERHP);//

	//回避座標の軸の定義
	player.leftNode->setPosition3D(Vec3(-0.6f, 0.0f, -0.6f));
	player.rightNode->setPosition3D(Vec3(0.6f, 0.0f, -0.6f));


	//ステージに応じた初期化
	player.sprite3d->setScale(1.0f);
	player.sprite3d->setRotation3D(GameMasterM->stagePoint[POINT_START].pRot);//プレイヤーは正面を向く
	player.wrapper->setPosition3D(GameMasterM->stagePoint[POINT_START].pPos);
	GameMasterM->sPoint = POINT_W1;//ステージ1に走る

	//当たり判定の定義（仮）
	player.collisionPos = Vec3(1.2, 3.0, 1.2);//範囲を指定して
	player.SetCollision();
	player.centerNode->setPosition3D(Vec3(0.0f, PLAYER_CENTER_Y, 0.0f));

	GameMasterM->SetPlayerBullets(STS_MAXBULLETS);//

	//最初はアイドル状態から始まるので、時間を取得せずともよい
	player.motStartTime = 0.0f;
	player.motProcTime = 0.0f;
	player.motPreTime = 0.0f;
}



/**
*	エネミー初期化
*
*	@author	sasebon
*	@param	ステージ番号
*	@return	正常:1 初期化失敗:-1
*	@date	1/8 Ver 1.0
*/
int GameModelsLayer::InitEnemy(int stage_num)
{
	//全てのエネミーユニットを初期化
	//エネミーのセットはsetEnemyで行う

	for(int i = UNIT1_ENEMY; i < UNIT2_BULLET; i++)
	{
		unit[i].Init(i, UKIND_ENEMY);
		unit[i].visible = FALSE;
		unit[i].sprite3d->setVisible(false);
		unit[i].collisionPos = Vec3(0.8f, 1.5f, 0.8f);//範囲を指定して
		unit[i].SetCollision();//
		unit[i].eState = ESTATE_NONE;

		unit[i].node2->setPosition3D(Vec3(0.0f, 0.0f, 0.0f));//
		unit[i].node1->setPosition3D(Vec3(0.0f, 0.0f, 0.0f));//
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
	//現在ウェーブで場合分け
	switch (GameMasterM->wave)
	{
	case 0://ウェーブ1

		enemyTable->InitAll();//エネミー出現テーブルを初期化（）
		enemyTable->finishNumber = 11;//10体の敵が出る

		//敵の初期化
		for(int i = UNIT1_ENEMY; i < UNIT2_BULLET; i++)
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
			for(int j = 0; j < 3; j++)
			{
				unit[i].nextEnemies[j] = -1;
			}

			//
			for(int j = 0; j < 3; j++)
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
		enemyTable->targetPos[3][1] = (Vec3(12.21f, 0.0f, -12.327f));
		enemyTable->targetPos[3][2] = (Vec3(9.705f, 0.0f, -8.381f));

		//enemyTable->targetPos[4][0] = (Vec3(11.385f, 0.0f, -7.781f));
		//enemyTable->targetPos[4][1] = (Vec3(9.705f, 0.0f, -8.381f));
		enemyTable->targetPos[4][2] = (Vec3(10.673f, 0.0f, -13.275f));

		//enemyTable->targetPos[5][2] = (Vec3(9.705f, 0.0f, -8.381f));
		//enemyTable->targetPos[5][1] = (Vec3(11.385f, 0.0f, -10.211f));
		enemyTable->targetPos[5][2] = (Vec3(11.55f, 0.0f, -9.23f));

		//enemyTable->targetPos[6][1] = (Vec3(11.385f, 0.0f, -7.781f));
		//enemyTable->targetPos[6][2] = (Vec3(9.705f, 0.0f, -8.381f));
		enemyTable->targetPos[6][2] = (Vec3(10.613f, 0.0f, -10.226f));

		//enemyTable->targetPos[9][1] = (Vec3(11.385f, 0.0f, -7.781f));
		//enemyTable->targetPos[9][2] = (Vec3(9.705f, 0.0f, -8.381f));
		enemyTable->targetPos[7][2] = (Vec3(12.21f, 0.0f, -12.327f));

		//enemyTable->targetPos[7][1] = (Vec3(11.385f, 0.0f, -7.781f));
		//enemyTable->targetPos[7][2] = (Vec3(9.705f, 0.0f, -8.381f));
		enemyTable->targetPos[8][2] = (Vec3(11.504f, 0.0f, -9.227f));

		//enemyTable->targetPos[8][2] = (Vec3(9.705f, 0.0f, -13.016f));
		enemyTable->targetPos[9][1] = (Vec3(10.673f, 0.0f, -13.275f));
		enemyTable->targetPos[9][2] = (Vec3(15.682f, 0.0f, -13.016f));


		//エネミーの初期座標をセット
		//エネミーの最初のターゲット座標をセット
		//エネミーの行動パターンをセット
		enemyTable->enemyData[0].standbyPos = enemyTable->standbyPos[0];
		enemyTable->enemyData[0].SetTargetPos(enemyTable->targetPos[0]);
		enemyTable->enemyData[0].SetNextEnemy(2, -1, -1);
		enemyTable->enemyData[0].alive = TRUE;
		enemyTable->enemyData[0].SetAI(AI_APPEAR_RUN, AI_ATK_NONE, AI_MOVE_WALK, AI_LIFE_SWITCH);
//		enemyTable->enemyData[0].SetAI(AI_APPEAR_SWALK, AI_ATK_NONE, AI_MOVE_WALK, AI_LIFE_SWITCH);
		enemyTable->enemyData[0].sleepTime = 1000;
		enemyTable->enemyData[0].hitpoint = 5;
		//enemyTable->enemyData[0].stsAtkFrame[0] = 
		enemyTable->enemyData[0].stsAtkFrame[1] = 2000;
		enemyTable->enemyData[0].stsAtkFrame[2] = 2000;


		enemyTable->enemyData[1].standbyPos = enemyTable->standbyPos[1];
		enemyTable->enemyData[1].SetTargetPos(enemyTable->targetPos[1]);
		enemyTable->enemyData[1].SetNextEnemy(3, -1, -1);
		enemyTable->enemyData[1].alive = TRUE;
		enemyTable->enemyData[1].SetAI(AI_APPEAR_RUN, AI_ATK_NONE, AI_MOVE_SWALK, AI_LIFE_SWITCH);
//		enemyTable->enemyData[1].SetAI(AI_APPEAR_SWALK, AI_ATK_NONE, AI_MOVE_SWALK, AI_LIFE_SWITCH);
		enemyTable->enemyData[1].sleepTime = 1000;
		enemyTable->enemyData[1].hitpoint = 5;
		//enemyTable->enemyData[0].stsAtkFrame[0] = 
		enemyTable->enemyData[1].stsAtkFrame[1] = 4000;
		enemyTable->enemyData[1].stsAtkFrame[2] = 4000;


		enemyTable->enemyData[2].standbyPos = enemyTable->standbyPos[2];
		enemyTable->enemyData[2].SetTargetPos(enemyTable->targetPos[2]);
		enemyTable->enemyData[2].SetNextEnemy(4, -1, -1);
		enemyTable->enemyData[2].alive = TRUE;
		enemyTable->enemyData[2].SetAI(AI_APPEAR_RUN,  AI_ATK_FAKE, AI_MOVE_NONE, AI_LIFE_STOP);
		enemyTable->enemyData[2].sleepTime = 1000;
		enemyTable->enemyData[2].hitpoint = 5;
		//enemyTable->enemyData[0].stsAtkFrame[0] = 
		//enemyTable->enemyData[2].stsAtkFrame[1] = 1000;
		enemyTable->enemyData[2].stsAtkFrame[2] = 4000;


		enemyTable->enemyData[3].standbyPos = enemyTable->standbyPos[3];
		enemyTable->enemyData[3].SetTargetPos(enemyTable->targetPos[3]);
		enemyTable->enemyData[3].SetNextEnemy(5, -1, -1);
		enemyTable->enemyData[3].alive = TRUE;
		enemyTable->enemyData[3].SetAI(AI_APPEAR_RUN,  AI_ATK_FAKE, AI_MOVE_NONE, AI_LIFE_STOP);
		enemyTable->enemyData[3].sleepTime = 1000;
		enemyTable->enemyData[3].hitpoint = 5;
		//enemyTable->enemyData[0].stsAtkFrame[0] = 
		//enemyTable->enemyData[2].stsAtkFrame[1] = 1000;
		enemyTable->enemyData[3].stsAtkFrame[2] = 4000;


		enemyTable->enemyData[4].standbyPos = enemyTable->standbyPos[4];
		enemyTable->enemyData[4].SetTargetPos(enemyTable->targetPos[4]);
		enemyTable->enemyData[4].SetNextEnemy(6, -1, -1);
		enemyTable->enemyData[4].alive = TRUE;
		enemyTable->enemyData[4].SetAI(AI_APPEAR_RUN,  AI_ATK_FAKE, AI_MOVE_NONE, AI_LIFE_STOP);
		enemyTable->enemyData[4].sleepTime = 1000;
		enemyTable->enemyData[4].hitpoint = 5;
		//enemyTable->enemyData[0].stsAtkFrame[0] = 
		//enemyTable->enemyData[2].stsAtkFrame[1] = 1000;
		enemyTable->enemyData[4].stsAtkFrame[2] = 5000;


		//要チェック
		enemyTable->enemyData[5].standbyPos = enemyTable->standbyPos[5];
		enemyTable->enemyData[5].SetTargetPos(enemyTable->targetPos[5]);
		enemyTable->enemyData[5].SetNextEnemy(7, -1, -1);
		enemyTable->enemyData[5].alive = TRUE;
		enemyTable->enemyData[5].SetAI(AI_APPEAR_RUN, AI_ATK_FAKE, AI_MOVE_WALK, AI_LIFE_STOP);
		enemyTable->enemyData[5].sleepTime = 1000;
		enemyTable->enemyData[5].hitpoint = 5;
		//enemyTable->enemyData[0].stsAtkFrame[0] = 
		enemyTable->enemyData[5].stsAtkFrame[1] = 3000;
		enemyTable->enemyData[5].stsAtkFrame[2] = 3000;


		enemyTable->enemyData[6].standbyPos = enemyTable->standbyPos[6];
		enemyTable->enemyData[6].SetTargetPos(enemyTable->targetPos[6]);
		enemyTable->enemyData[6].SetNextEnemy(8, -1, -1);
		enemyTable->enemyData[6].alive = TRUE;
		enemyTable->enemyData[6].SetAI(AI_APPEAR_SWALK, AI_ATK_FAKE, AI_MOVE_NONE, AI_LIFE_STOP);
		enemyTable->enemyData[6].sleepTime = 1000;
		enemyTable->enemyData[6].hitpoint = 5;
		//enemyTable->enemyData[0].stsAtkFrame[0] = 
		//->enemyData[5].stsAtkFrame[1] = 1000;
		enemyTable->enemyData[6].stsAtkFrame[2] = 3000;


		//サイドジャンプ行動を行う
		//要チェック
		enemyTable->enemyData[7].standbyPos = enemyTable->standbyPos[7];
		enemyTable->enemyData[7].SetTargetPos(enemyTable->targetPos[7]);
		enemyTable->enemyData[7].SetNextEnemy(9, -1, -1);
		enemyTable->enemyData[7].alive = TRUE;
		enemyTable->enemyData[7].SetAI(AI_APPEAR_SJUMP, AI_ATK_SJUMP, AI_MOVE_SJUMP, AI_LIFE_SWITCH);//サイドジャンプモーションをアニメーション管理で分割します
		enemyTable->enemyData[7].sleepTime = 1000;
		enemyTable->enemyData[7].hitpoint = 5;
		//enemyTable->enemyData[0].stsAtkFrame[0] = 
		//->enemyData[5].stsAtkFrame[1] = 1000;
		enemyTable->enemyData[7].stsAtkFrame[2] = 500;


		enemyTable->enemyData[8].standbyPos = enemyTable->standbyPos[8];
		enemyTable->enemyData[8].SetTargetPos(enemyTable->targetPos[8]);
		enemyTable->enemyData[8].SetNextEnemy(-1, -1, -1);
		enemyTable->enemyData[8].finishFlag = TRUE;
		enemyTable->enemyData[8].alive = TRUE;
		enemyTable->enemyData[8].SetAI(AI_APPEAR_WALK, AI_ATK_FAKE, AI_MOVE_NONE, AI_LIFE_STOP);
		enemyTable->enemyData[8].sleepTime = 1000;
		enemyTable->enemyData[8].hitpoint = 5;
		//enemyTable->enemyData[0].stsAtkFrame[0] = 
		//->enemyData[5].stsAtkFrame[1] = 1000;
		enemyTable->enemyData[8].stsAtkFrame[2] = 3000;

		//要チェック
		//アクロバット移動
		enemyTable->enemyData[9].standbyPos = enemyTable->standbyPos[9];
		enemyTable->enemyData[9].SetTargetPos(enemyTable->targetPos[9]);
		enemyTable->enemyData[9].SetNextEnemy(-1, -1, -1);
		enemyTable->enemyData[9].finishFlag = TRUE;
		enemyTable->enemyData[9].alive = TRUE;
		enemyTable->enemyData[9].SetAI(AI_APPEAR_ACRO, AI_ATK_ACRO,AI_MOVE_RUN, AI_LIFE_ONCE);//
		enemyTable->enemyData[9].sleepTime = 1000;
		enemyTable->enemyData[9].hitpoint = 5;
		enemyTable->enemyData[9].stsAtkFrame[2] = 3000;


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
*	ステージオブジェクトの初期化
*
*	@author	sasebon
*	@param	ステージ番号
*	@return	正常:1 初期化失敗:-1
*	@date	1/8 Ver 1.0
*/
int GameModelsLayer::InitMap(int stage_num)
{
	unit[UNIT0_MAP].Init();//メンバ変数の初期化をしておく
	unit[UNIT0_MAP].sprite3d->setScale(1.0f);
	unit[UNIT0_MAP].sprite3d->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	unit[UNIT0_MAP].sprite3d->setPosition3D(Vec3(0.0f, 0.0f, 0.0f));
	unit[UNIT0_MAP].Init(0, UKIND_MAP);
	return TRUE;
}


/**
*	ウェイト状態の更新
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/23 Ver 1.0
*/
void GameModelsLayer::UpdateWait()
{
	switch (GameMasterM->waitFlag)
	{
	case 0:
	{
		//目標地点を取得し、移動処理の準備を行う
		player.targetPos = GameMasterM->stagePoint[GameMasterM->sPoint].pPos;//
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
		GameMasterM->waitFlag = 1;
		GameMasterM->SetPlayerState(PSTATE_RUN);
		//カメラの位置が滑らかに変化する処理を入れる
		GameMasterM->SetCamera3DPos(Vec3(W_SETX, W_SETY, W_SETZ));//プレイヤー（親ノード）とカメラの位置関係をセット
		GameMasterM->SetCamera3DRot(Vec3(W_ROTX, W_ROTY, W_ROTZ));


		Vec3 tmp = player.sprite3d->getRotation3D();//プレイヤーの今の角度をもとに
		rotationR = GameMasterM->stagePoint[GameMasterM->sPoint].pRot.y - tmp.y;//戦闘時の角度との差を計算する
		rotationR *= TIME_WROT * 0.01f;//1ミリ秒ごとに変化する角度を計算
		rotationCount = 0;


	}
		break;
	case 1:

		//一定時間かけてカメラを動かす処理




		GameMasterM->waitFlag = 2;
		break;
	case 2:
	{
			  UpdatePlayer();//プレイヤーの更新
			  Vec3 tmpPos = player.wrapper->getPosition3D();
			  tmpPos = player.targetPos - tmpPos;
			  //一定以上目的地に近付いたら、カメラとプレイヤーを回転させる準備を行う


			  //平面の距離を求める
			  float d1 = sqrtf(tmpPos.x * tmpPos.x + tmpPos.y * tmpPos.y);
			  //計算した平面上の距離と高さの距離を求める
			  d1 = sqrtf((tmpPos.z * tmpPos.z) + (d1 * d1));

			  if(0.15f >= d1)
			  {
				  GameMasterM->waitFlag = 3;
				  Vec3 tmp = player.sprite3d->getRotation3D();//プレイヤーの今の角度をもとに
				  rotationR = GameMasterM->stagePoint[GameMasterM->sPoint].pRot.y - tmp.y;//戦闘時の角度との差を計算する
				  rotationR *= 0.05;//1フレームごとに変化する角度を計算
				  rotationCount = 0;

				  if(POINT_CHANGE == GameMasterM->stagePoint[GameMasterM->sPoint].pointType)
				  {
					  changeCameraPos = Vec3(0.0f, 0.0f, 0.0f);
					  changeCameraRot = Vec3(0.0f, 0.0f, 0.0f);
				  }
				  else if(POINT_FINISH == GameMasterM->stagePoint[GameMasterM->sPoint].pointType)
				  {
					  changeCameraPos = Vec3(0.0f, 0.0f, 0.0f);
					  changeCameraRot = Vec3(0.0f, 0.0f, 0.0f);
				  }
				  else if(POINT_BATTLE == GameMasterM->stagePoint[GameMasterM->sPoint].pointType)
				  {
					  //プレイヤーとカメラの位置関係を計算
					  Vec3 cP = GameMasterM->stagePoint[GameMasterM->sPoint].cPos - GameMasterM->stagePoint[GameMasterM->sPoint].pPos;
					  Vec3 cR = (GameMasterM->stagePoint[GameMasterM->sPoint].cRot) - GameMasterM->stagePoint[GameMasterM->sPoint].pRot;

					  if(PSIDE_LEFT == GameMasterM->stagePoint[GameMasterM->sPoint].playerSide)
					  {
						  changeCameraPos = Vec3(C_SETX_L - W_SETX, C_SETY_L - W_SETY, C_SETZ_L - W_SETZ) * 0.05;
						  changeCameraRot = Vec3(C_ROTX_L - W_ROTX, C_ROTY_L - W_ROTY, C_ROTZ_L - W_ROTZ)* 0.05;
					  }
					  else
					  {
						  changeCameraPos = Vec3(C_SETX_R - W_SETX, C_SETY_R - W_SETY, C_SETZ_R - W_SETZ) * 0.05;
						  changeCameraRot = Vec3(C_ROTX_R - W_ROTX, C_ROTY_R - W_ROTY, C_ROTZ_R - W_ROTZ)* 0.05;
					  }
				  }
				  player.speed = 0.0f;
				  player.speedVec = Vec3(0.0f, 0.0f, 0.0f);
			  }
	}
		break;
	case 3:
	{
		if (20 > rotationCount)//回転中の動作
		{
			Vec3 tmp = player.sprite3d->getRotation3D();
			player.sprite3d->setRotation3D(tmp + Vec3(0, rotationR, 0));//今の角度に回転角度を足す

			GameMasterM->AddCamera3DPos(changeCameraPos);//プレイヤー（親ノード）とカメラの位置関係をセット
			GameMasterM->AddCamera3DRot(changeCameraRot);

			rotationCount++;
		}
		else//回転終了後の動作
		{
			if (POINT_CHANGE == GameMasterM->stagePoint[GameMasterM->sPoint].pointType)//進行方向切り替え
			{
				player.sprite3d->setRotation3D(GameMasterM->stagePoint[GameMasterM->sPoint].pRot);
				player.wrapper->setPosition3D(GameMasterM->stagePoint[GameMasterM->sPoint].pPos);

				GameMasterM->sPoint++;//座標と角度が設定できたらポイントを先に進める
				GameMasterM->waitFlag = 0;
			}
			else if (POINT_BATTLE == GameMasterM->stagePoint[GameMasterM->sPoint].pointType)//次のウェーブに到達した
			{
				//座標と角度をセット
				player.sprite3d->setRotation3D(GameMasterM->stagePoint[GameMasterM->sPoint].pRot);
				player.wrapper->setPosition3D(GameMasterM->stagePoint[GameMasterM->sPoint].pPos);

				GameMasterM->playerSide = GameMasterM->stagePoint[GameMasterM->sPoint].playerSide;//現在が左右どちらに立っているかを確認

				//プレイヤーの回避用ノードをもとに、回避座標を取得する
				if (PSIDE_LEFT == GameMasterM->playerSide)
				{
					//目標角度をもとにノードの移動後座標を取得
					Vec2 pos = calcRot(GameMasterM->stagePoint[GameMasterM->sPoint].pRot.y, GameMasterM->stagePoint[GameMasterM->sPoint].playerSide);
					Vec3 newPos = player.wrapper->getPosition3D() + Vec3(pos.x, 0.0f, pos.y);
					player.wrapper->setPosition3D(newPos);//wrapper

					pos = calcRot(GameMasterM->stagePoint[GameMasterM->sPoint].pRot.y, GameMasterM->stagePoint[GameMasterM->sPoint].playerSide);
					newPos = player.sprite3d->getPosition3D() - Vec3(pos.x, 0.0f, pos.y);
					player.sprite3d->setPosition3D(newPos);//wrapper

					//目標角度をもとに、回避時のカメラ移動座標を計算
					//プレイヤーの親ノードに対する回避後カメラ座標の相対座標を計算
					Vec2 tmp = calcCamPos3(GameMasterM->stagePoint[GameMasterM->sPoint].pRot.y, PSIDE_LEFT);
					camTarget = Vec3(tmp.x, 0.0f, tmp.y);

					//回避時のカメラ移動前の座標を確保
					camCenter = player.wrapper->getPosition3D();
				}
				else
				{
					//目標角度をもとにノードの移動後座標を取得
					Vec2 pos = calcRot(GameMasterM->stagePoint[GameMasterM->sPoint].pRot.y, GameMasterM->stagePoint[GameMasterM->sPoint].playerSide);
					Vec3 newPos = player.wrapper->getPosition3D() + Vec3(pos.x, 0.0f, pos.y);
					player.wrapper->setPosition3D(newPos);//wrapper

					pos = calcRot(GameMasterM->stagePoint[GameMasterM->sPoint].pRot.y, GameMasterM->stagePoint[GameMasterM->sPoint].playerSide);
					newPos = player.sprite3d->getPosition3D() - Vec3(pos.x, 0.0f, pos.y);
					player.sprite3d->setPosition3D(newPos);//wrapper


					//目標角度をもとに、回避時のカメラ移動座標を計算
					Vec2 tmp = calcCamPos3(GameMasterM->stagePoint[GameMasterM->sPoint].pRot.y, PSIDE_RIGHT);
					camTarget = Vec3(tmp.x, 0.0f, tmp.y);
				}
				GameMasterM->SetGameState(GSTATE_PLAY_SET);//戦闘ポイントに到着したら、ウェイトからプレイに移行
				player.sprite3d->stopAllActions();
				GameMasterM->SetPlayerState(PSTATE_IDLE);

				//ゲームに関係する各種フラグの初期化
				GameMasterM->flgPlayerATK = FALSE;//プレイヤーの攻撃判定をOFFに
				GameMasterM->playerHitFlag = TRUE;//プレイヤーの食らい判定をONに
				GameMasterM->shotFlag = FALSE;//

			}
			else if (POINT_CLEAR == GameMasterM->stagePoint[GameMasterM->sPoint].pointType)//クリアしたら
			{

			}
			else
			{
				int a = 0;
			}
		}
	}
		break;
	case 4:

		break;
	}
}



/**
*	プレイヤーの更新
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
void GameModelsLayer::UpdatePlayer(void)
{
	//タッチ座標をもとに攻撃や回避の処理を行う
	//この関数内では、タッチ状態とタッチフラグの更新は行わないこと。

	//1：プレイヤーの状態を取得して場合分け
	switch(GameMasterM->GetPlayerState())
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

	//もともとこちらが1だったので、問題が起これば元に戻す
	//2：プレイヤーのフレーム・座標更新
	player.Update(GameMasterM->loopTime);
	//マズルの更新
	//	player.muzzleUpdate();
}


/**
*	プレイヤーのアイドル状態の更新
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/21 Ver 1.0
*/
void GameModelsLayer::ActionIdle()
{
	//プレイヤーの向きに応じて呼び出すアニメーションを変更する
	//とりあえずここで文字列作成
	std::string shot;
	std::string h_shot;
	std::string h_reload;

	auto director = Director::getInstance();


	if(PSIDE_LEFT == GameMasterM->playerSide)
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

	if(TSTATE_ON == GameMasterM->GetTouchState())//タッチされたら
	{
		auto s = Director::getInstance()->getWinSize();//画面サイズ取得
		const Vec2 tPos = GameMasterM->GetTouchPos();//タッチしたスクリーン座標を取得

		//		player.muzzleflagOn();

		int tmpFlag = 0;

		if(PSIDE_LEFT == GameMasterM->playerSide)//プレイヤーが左に立っている
		{
			//攻撃可能範囲をタッチしていれば
			if(tPos.x > s.width *(1.0f - BATTLE_FEILD_X))
			{
				tmpFlag = 1;//
			}
			//回避UI付近をタッチしたら
			else if(tPos.x >= s.width * HIDE_UI_X0 && tPos.x < s.width *HIDE_UI_X1
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
		if(0 == tmpFlag)
		{
			//タッチしていない
		}
		else if(1 == tmpFlag)
		{
			//残弾に応じて処理を分ける
			if(0 >= GameMasterM->GetPlayerBullets())
			{
				//モーションは再生するが弾を出さない

			}
			else
			{
				//アニメーションを再生
				GameMasterM->SetPlayerState(PSTATE_SHOT);
				player.sprite3d->stopAllActions();
				player.sprite3d->startAnimation(shot);//射撃アニメーションを最後まで再生する

				GameMasterM->rapidFrame = -1.0f;//連射フレームを-1に初期化
				GameMasterM->flgPlayerATK = FALSE;//
				GameMasterM->shotFlag = FALSE;//

				//モーション管理用の時間を初期化
				player.motProcTime = 0;//モーションを再生してからの経過時間（ミリ秒）
				player.motPreTime = getNowTime();//PreTimeに現在時刻を代入
				player.motStartTime = getNowTime();


				GameMasterM->rapidFrame = 0.0f;
			}
		}
		else
		{
			//
			GameMasterM->SetPlayerState(PSTATE_DODGE);//回避に移行

			player.sprite3d->stopAllActions();
			player.sprite3d->startAnimationReverse(h_shot);//回避モーションを再生

			//モーション管理用の時間を初期化
			player.motProcTime = 0;//モーションを再生してからの経過時間（ミリ秒）
			player.motPreTime = getNowTime();//PreTimeに現在時刻を代入
			player.motStartTime = getNowTime();
			player.setAnimEndTime(MACRO_FtoS(STS_HIDEWAIT));//回避モーションにかかる時間をセット

		}
	}
}



/**
*	プレイヤーの攻撃状態の更新
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/21 Ver 1.0
*/
void GameModelsLayer::ActionShot()
{
	//モーションを再生してからの経過時間をある程度正確に取得するため、関数内で時間計算を行っています
	player.motProcTime = getNowTime() - player.motStartTime;//モーションを再生してからの経過時間（ミリ秒）
	int lTime = getNowTime() - player.motPreTime;
	player.motPreTime = getNowTime();//PreTimeに現在時刻を代入

	GameMasterM->rapidFrame += MACRO_StoF(lTime);//

	//プレイヤーの向きに応じて呼び出すアニメーションを変更する
	//とりあえずここで文字列作成
	std::string shot;
	std::string h_shot;
	std::string h_reload;
	if (PSIDE_LEFT == GameMasterM->playerSide)
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

	auto sound = Sound::getInstance();

	//指定フレーム以上タッチしているかのチェック
	if (FALSE == GameMasterM->shotFlag && MACRO_StoF(player.motProcTime) >= STS_RAPIDSPEED)
	{
		GameMasterM->shotFlag = TRUE;
		player.sprite3d->stopAllActions();
		player.sprite3d->startAnimationLoop(shot, 0, MACRO_FtoS(7.0f));
	}


	if (TSTATE_ON == GameMasterM->GetTouchState() || TSTATE_MOVE == GameMasterM->GetTouchState())
	{
		if (0 >= GameMasterM->GetPlayerBullets())
		{
			//残弾がない場合
			GameMasterM->flgPlayerATK = FALSE;//攻撃判定をオフにする
			//アニメーションはそのまま行う
		}
		else
		{
			//残弾がある場合
			//一定フレームごとに攻撃判定をONにする
			if (STS_RAPIDSPEED < GameMasterM->rapidFrame)
			{
				GameMasterM->flgPlayerATK = TRUE;
				GameMasterM->AddPlayerBullets(-1);//弾数を減らす
				//音声はフラグ成立時に鳴らす
				//sound->playSE("Shot.wav");
				GameMasterM->rapidFrame = 0.0f;
			}
			else
			{
				GameMasterM->flgPlayerATK = FALSE;
			}
		}

		GameMasterM->SetPlayerState(PSTATE_SHOT);//ステート状態はそのまま
	}
	else if (TSTATE_RELEASE == GameMasterM->GetTouchState())//タッチを離したら
	{
		if (FALSE == GameMasterM->shotFlag)
		{
			//もしタッチ時間が指定フレーム以内なら
			GameMasterM->SetPlayerState(PSTATE_IDLE);//通常状態に戻す
			GameMasterM->flgPlayerATK = FALSE;//攻撃判定をオンにする

			//そのままアニメーションを終了する
		}
		else
		{
			GameMasterM->SetPlayerState(PSTATE_IDLE);//通常状態に戻す
			GameMasterM->flgPlayerATK = FALSE;//攻撃判定をオフにする

			//それ以外の場合、ショットの最後の部分まで再生する
			player.sprite3d->stopAllActions();
			player.sprite3d->startAnimation(shot);
		}
	}
	else
	{
		if (FALSE == GameMasterM->shotFlag)
		{
			//もしタッチ時間が5フレーム以内なら
			GameMasterM->SetPlayerState(PSTATE_IDLE);//通常状態に戻す
			GameMasterM->flgPlayerATK = FALSE;//攻撃判定をオフにする
			GameMasterM->shotFlag = FALSE;
			//そのままアニメーションを終了する
		}
		else
		{
			GameMasterM->SetPlayerState(PSTATE_IDLE);//通常状態に戻す
			GameMasterM->flgPlayerATK = FALSE;//攻撃判定をオフにする
			GameMasterM->shotFlag = FALSE;
			//ショットの最後の部分を再生する
			player.sprite3d->stopAllActions();
			player.sprite3d->startAnimation(shot);
		}
	}
}



/**
*	プレイヤーの避け最中の更新
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/21 Ver 1.0
*/
void GameModelsLayer::ActionDodge(void)
{
	//モーションを再生してからの経過時間をある程度正確に取得するため、関数内で時間計算を行っています
	player.motProcTime = getNowTime() - player.motStartTime;//モーションを再生してからの経過時間（ミリ秒）
	int lTime = getNowTime() - player.motPreTime;
	player.motPreTime = getNowTime();//PreTimeに現在時刻を代入

	//プレイヤーの向きに応じて呼び出すアニメーションを変更する
	//とりあえずここで文字列作成
	std::string shot;
	std::string h_shot;
	std::string h_reload;
	if (PSIDE_LEFT == GameMasterM->playerSide)
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

	int state = GameMasterM->GetTouchState();//現在のタッチ状態を取得

	//無敵時間の判定を行う
	if (STS_MUTEKISTART <= MACRO_StoF(player.motProcTime))//無敵開始フレームに達したら
	{
		GameMasterM->playerHitFlag = FALSE;//当たり判定を無効化する
	}

	//リロード判定を行う
	if (STS_RELOADSTART <= MACRO_StoF(player.motProcTime))//リロード開始フレームに達したら
	{
		GameMasterM->SetPlayerBullets(STS_MAXBULLETS);//弾数を回復する
	}

	//回避完了フレームに達したら
	//	if (STS_HIDEWAIT <= GameMasterM->hideFrame)
	//	if (0 == player.sprite3d->checkAnimationState())//アニメーションが終了したら
	if (player.getAnimEndTime() <= player.motProcTime)
	{
		GameMasterM->SetPlayerState(PSTATE_HIDE);//隠れている状態に移行

		//リロードモーションが終了した後は、リロードモーションを行う
		player.sprite3d->stopAllActions();
		player.sprite3d->startAnimation(h_reload);//リロードモーションを再生

		//モーション用カウンターを初期化
		player.motProcTime = 0.0f;//経過時間を0に初期化
		player.motPreTime = getNowTime();
		player.motStartTime = getNowTime();

		//座標と角度をセットしてキャラクターの座標を補正
		if (PSIDE_LEFT == GameMasterM->playerSide)
		{
			player.sprite3d->setRotation3D(GameMasterM->stagePoint[GameMasterM->sPoint].pRot + Vec3(0.0f, 90.0f, 0.0f));
			player.wrapper->setRotation3D(Vec3(0.0f, -90.0f, 0.0f));

			//回避に合わせてカメラの座標を補正する
			player.cameraAjust = Vec3(camTarget.x * MACRO_FtoS(STS_HIDEWAIT), camTarget.y * MACRO_FtoS(STS_HIDEWAIT), camTarget.z * MACRO_FtoS(STS_HIDEWAIT));//ループごとの移動量を計算
		}
		else
		{
			player.sprite3d->setRotation3D(GameMasterM->stagePoint[GameMasterM->sPoint].pRot - Vec3(0.0f, 90.0f, 0.0f));
			player.wrapper->setRotation3D(Vec3(0.0f, 90.0f, 0.0f));

			//回避に合わせてカメラの座標を補正する
			player.cameraAjust = Vec3(camTarget.x * MACRO_FtoS(STS_HIDEWAIT), camTarget.y * MACRO_FtoS(STS_HIDEWAIT), camTarget.z * MACRO_FtoS(STS_HIDEWAIT));//ループごとの移動量を計算
		}
	}
	else
	{
		//回避完了前にホールド解除した場合
		//とりあえずTFLAG_CANCELをif内に入れておく
		if (TSTATE_RELEASE == state)
		{
			GameMasterM->SetPlayerState(PSTATE_APPEAR);//隠れている状態に移行
			//突撃モーションを再生

			int startTime = MACRO_FtoS(STS_HIDEWAIT) - player.motProcTime;//回避モーションの残り時間をもとに、突撃モーションの開始時間を計算

			player.setAnimEndTime(MACRO_FtoS(STS_HIDEWAIT) - startTime);//アニメーション終了までの残りミリ秒数をセット
			player.sprite3d->stopALLAnimation();//モーション終了
			player.sprite3d->startAnimation(h_shot, startTime, MACRO_FtoS(STS_HIDEWAIT));//

			//
			player.motStartTime = getNowTime();//現在時刻
			player.motProcTime = startTime;//
		}
		else if (TSTATE_MOVE == state || TSTATE_ON == state)//ホールド状態
		{
			//回避動作中は指定座標を軸に座標移動を行う
			if (PSIDE_LEFT == GameMasterM->playerSide)
			{
				//プレイヤーの座標の更新
				float rot = lTime * 90.0f / (MACRO_FtoS(STS_HIDEWAIT));

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
				float rot = lTime * 90.0f / (MACRO_FtoS(STS_HIDEWAIT)) * 0.001f;//要チェック

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


/**
*	プレイヤーの隠れ状態の更新
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/21 Ver 1.0
*/
void GameModelsLayer::ActionHide(void)
{
	//プレイヤーの向きに応じて呼び出すアニメーションを変更する
	//とりあえずここで文字列作成
	std::string shot;
	std::string h_shot;
	std::string h_reload;
	if (PSIDE_LEFT == GameMasterM->playerSide)
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

	if (TSTATE_ON == GameMasterM->GetTouchState() || TSTATE_MOVE == GameMasterM->GetTouchState())
	{
		//タッチ中はリロードを再生、再生後はモーション停止
		player.cameraAjust = Vec3(camTarget.x * MACRO_FtoS(STS_HIDEWAIT), camTarget.y * MACRO_FtoS(STS_HIDEWAIT), camTarget.z * MACRO_FtoS(STS_HIDEWAIT)) * 0.001f;//ループごとの移動量を計算
	}
	else if (TSTATE_RELEASE == GameMasterM->GetTouchState())//離されれば
	{
		GameMasterM->SetPlayerState(PSTATE_APPEAR);
		player.sprite3d->startAnimation(h_shot);
		//		player.InitFrame();//フレームをリフレッシュ

		//モーション管理用時間の初期化
		player.motStartTime = getNowTime();//スタートは現在時刻 * 0.001f//現在時刻を取得
		player.motPreTime = getNowTime();//前フレームの時刻も現在時刻
		player.motProcTime = 0;//経過時間は0
		player.setAnimEndTime(MACRO_FtoS(STS_HIDEWAIT));//モーション終了までの残り時間を計算
	}
}

/**
*	プレイヤーの飛び出し状態の更新
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/21 Ver 1.0
*/
void GameModelsLayer::ActionAppear(void)
{
	//モーションを再生してからの経過時間をある程度正確に取得するため、関数内で時間計算を行っています
	player.motProcTime = getNowTime() - player.motStartTime;//モーションを再生してからの経過時間（ミリ秒）
	int lTime = getNowTime() - player.motPreTime;//1ループにかかった時間を計算
	player.motPreTime = getNowTime();//PreTimeに現在時刻を代入


	//プレイヤーの向きに応じて呼び出すアニメーションを変更する
	//とりあえずここで文字列作成
	std::string idle;
	if (PSIDE_LEFT == GameMasterM->playerSide)
	{
		idle = "idle_l";
	}
	else
	{
		idle = "idle_r";
	}

	//１：突撃終了フレームが終了するとActionIdleに移行する
	int flag = GameMasterM->GetTouchFlag();//現在のタッチ状態を取得

	//無敵時間の判定を行う
	if (STS_MUTEKISTART <= MACRO_StoF(player.motProcTime))//無敵終了フレームに達したら
	{
		GameMasterM->playerHitFlag = TRUE;//当たり判定を有効化する
	}

	//回避完了の判定
	//	if (STS_HIDEWAIT <= GameMasterM->hideFrame)//突撃フレームに達したら
	//	if(0 == player.sprite3d->checkAnimationState())//アニメーションが終了したら
	if (player.getAnimEndTime() <= player.motProcTime)
	{
		//必要ならばモーションの停止を行う
		GameMasterM->SetPlayerState(PSTATE_IDLE);//アイドル状態に移行
		player.sprite3d->stopAllActions();


		//座標と角度をセットしてキャラクターの座標を補正
		player.sprite3d->setRotation3D(GameMasterM->stagePoint[GameMasterM->sPoint].pRot);
		player.wrapper->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));

		//座標と角度をセットしてカメラの座標を補正
		Vec2 tmp = calcCamPos3(GameMasterM->stagePoint[GameMasterM->sPoint].pRot.y, GameMasterM->playerSide);
		camTarget = Vec3(tmp.x, 0.0f, tmp.y);


		player.sprite3d->startAnimationLoop(idle);
	}
	else
	{
		if (PSIDE_LEFT == GameMasterM->playerSide)
		{
			//回避フレームに比例してカメラの回転を変化させる
			float rot = lTime * 90.0f / (MACRO_FtoS(STS_HIDEWAIT)) * 0.001f;
			Vec3 tmp = player.wrapper->getRotation3D();
			tmp.y += rot;//
			player.wrapper->setRotation3D(tmp);//プレイヤーの親ノード（回避軸）の角度を更新する
			tmp = player.sprite3d->getRotation3D();
			tmp.y -= rot;//
			player.sprite3d->setRotation3D(tmp);//プレイヤー自身の角度を更新する

			//回避に合わせてカメラの座標を補正する
			player.cameraAjust = 
				Vec3(camTarget.x * (MACRO_FtoS(STS_HIDEWAIT) - player.motProcTime),
				camTarget.y * (MACRO_FtoS(STS_HIDEWAIT) - player.motProcTime),
				camTarget.z * (MACRO_FtoS(STS_HIDEWAIT) - player.motProcTime)) * 0.001f;//ループごとの移動量を計算
		}
		else
		{
			//回避フレームに比例してカメラの回転を変化させる
			float rot = lTime * 90.0f / (MACRO_FtoS(STS_HIDEWAIT)) * 0.001f;
			Vec3 tmp = player.wrapper->getRotation3D();
			tmp.y -= rot;//
			player.wrapper->setRotation3D(tmp);//プレイヤーの親ノード（回避軸）の角度を更新する
			tmp = player.sprite3d->getRotation3D();
			tmp.y += rot;//
			player.sprite3d->setRotation3D(tmp);//プレイヤー自身の角度を更新する

			//回避に合わせてカメラの座標を補正する
			player.cameraAjust = Vec3(camTarget.x * (MACRO_FtoS(STS_HIDEWAIT) - player.motProcTime),
				camTarget.y * (MACRO_FtoS(STS_HIDEWAIT) - player.motProcTime),
				camTarget.z * (MACRO_FtoS(STS_HIDEWAIT) - player.motProcTime)) * 0.001f;//ループごとの移動量を計算
		}
	}
}

/**
*	プレイヤーの食らい状態の更新
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/21 Ver 1.0
*/
void GameModelsLayer::ActionDamaged(void)
{
	//モーションを再生してからの経過時間をある程度正確に取得するため、関数内で時間計算を行っています
	player.motProcTime = getNowTime() - player.motStartTime;//モーションを再生してからの経過時間（秒）
	int lTime = getNowTime() - player.motPreTime;//1ループにかかった時間を計算
	player.motPreTime = getNowTime();//PreTimeに現在時刻を代入

	//プレイヤーの向きに応じて呼び出すアニメーションを変更する
	//とりあえずここで文字列作成
	std::string recov;
	if (PSIDE_LEFT == GameMasterM->playerSide)
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
		GameMasterM->SetPlayerState(PSTATE_RECOVER);//起き上がり状態に移行
		player.sprite3d->stopAllActions();
		player.sprite3d->startAnimation(recov);//起き上がりモーションを再生

		//
		player.motProcTime = 0;//モーションを再生してからの経過時間（秒）
		player.motPreTime = getNowTime();//PreTimeに現在時刻を代入
		player.motStartTime = getNowTime();//PreTimeに現在時刻を代入
	}
	else
	{
		//
	}
}


/**
*	プレイヤーの起き上がり状態の更新
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/21 Ver 1.0
*/
void GameModelsLayer::ActionRecover(void)
{
	//モーションを再生してからの経過時間をある程度正確に取得するため、関数内で時間計算を行っています
	player.motProcTime = getNowTime() - player.motStartTime;//モーションを再生してからの経過時間（秒）
	int lTime = getNowTime() - player.motPreTime;//1ループにかかった時間を計算
	player.motPreTime = getNowTime();//PreTimeに現在時刻を代入


	//プレイヤーの向きに応じて呼び出すアニメーションを変更する
	//とりあえずここで文字列作成
	std::string idle;
	if(PSIDE_LEFT == GameMasterM->playerSide)
	{
		idle = "idel_l";
	}
	else
	{
		idle = "idel_r";
	}


	//アニメーションの確認
	if(0 == player.sprite3d->checkAnimationState())
	{
		//HPが0になったら
		if(0 < GameMasterM->GetPlayerHP())
		{
			//食らいモーションが終了したら
			GameMasterM->SetPlayerState(PSTATE_IDLE);//アイドル状態に移行
			player.sprite3d->stopAllActions();
			player.sprite3d->startAnimationLoop(idle);

			GameMasterM->playerHitFlag = TRUE;//当たり判定を戻す

			//モーション管理用の時間を初期化
			player.motProcTime = 0;//モーションを再生してからの経過時間（秒）
			player.motPreTime = getNowTime();//PreTimeに現在時刻を代入
			player.motStartTime = getNowTime();//StartTimeに現在時刻を代入
		}
		else
		{
			GameMasterM->SetPlayerState(PSTATE_DEAD);//死亡状態にする

			//必要ならばモーションの停止を行う
			player.sprite3d->stopAllActions();
			player.sprite3d->startAnimation("dei1");//

			//
			player.motProcTime = 0;//モーションを再生してからの経過時間（秒）
			player.motPreTime = getNowTime();//PreTimeに現在時刻を代入
			player.motStartTime = getNowTime();//PreTimeに現在時刻を代入

			//座標と角度をセットしてカメラの座標を補正
			ChangeCamera(2);
		}
	}
	else
	{
		//
	}
}

/**
*	プレイヤーのウェイト状態の更新
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/21 Ver 1.0
*/
void GameModelsLayer::ActionRun(void)
{
}




/**
*	プレイヤーの死亡状態の更新
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/21 Ver 1.0
*/
void GameModelsLayer::ActionDead(void)
{
	//
	//モーションを再生してからの経過時間をある程度正確に取得するため、関数内で時間計算を行っています
	player.motProcTime = getNowTime() - player.motStartTime;//モーションを再生してからの経過時間（秒）
	int lTime = getNowTime() - player.motPreTime;//1ループにかかった時間を計算
	player.motPreTime = getNowTime();//PreTimeに現在時刻を代入

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
	//
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
	if (num != -1)
	{
		//弾を一度初期化する
		unit[num].Init(num, UKIND_EBULLET);

		unit[num].sprite3d->setScale(0.5f);//

		//弾を撃ったエネミーの座標と、プレイヤーの座標を元に、弾の移動方向を求める
		Vec3 enemyPos = unit[enemy_num].sprite3d->getPosition3D();

		//プレイヤーに向けて弾を発射する
		unit[num].speedVec = GameMasterM->stagePoint[GameMasterM->sPoint].pPos - enemyPos;//

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

		unit[num].sprite3d->setPosition3D(enemyPos);
		unit[num].sprite3d->setPositionY(1.2f);

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
	//全ての敵弾ユニットを更新
	for(int num = UNIT2_BULLET; num <= UNIT3_MAX; num++)
	{
		//画面に出ている弾のみを更新
		if(TRUE == unit[num].visible)
		{
			unit[num].Update(GameMasterM->loopTime);//座標と一緒に当たり判定を移動

			//指定時間が経過したら消去処理
			if(4.0f <= unit[num].GetTime())
			{
				unit[num].speed = 0.0f;
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
*	@date	1/19 Ver 1.0
*/
void  GameModelsLayer::CheckHit(void)
{
	//========================================================
	//プレイヤー攻撃処理
	//タッチ座標（スクリーン座標）からワールド座標を求め、レイを飛ばす

	//レイと敵の当たり判定処理
	const int pstate = GameMasterM->GetPlayerState();

	//攻撃判定フラグがONのときのみ攻撃判定を処理
	if(TRUE == GameMasterM->flgPlayerATK)
	{
		//注意：敵が重なって存在する場合に備え、Ｚソートなどの並び替えを行う必要がありそうです
		auto s = Director::getInstance()->getWinSize();//ウィンドウサイズを取得

		Vec3 rayStart = Vec3(0, 0, 0);//レイの始点
		Vec3 rayEnd = Vec3(0, 0, 0);//レイの終点

		Camera* cam3d = GameMasterM->GetCamera3D();//カメラのインスタンスを取得
		const Node* camNode = GameMasterM->GetCameraNode();//ノードのインスタンスを取得

		Vec3 tmpPos = cam3d->getPosition3D();//カメラ座標を保存
		Vec3 tmpRot = cam3d->getRotation3D();//カメラ回転を保存

		const Point pos = Vec2(tmpPos.x, tmpPos.z);
		const Point absolutePoint = camNode->convertToWorldSpace(pos);//カメラのx,zの絶対座標を取得

		Vec3 cPos = Vec3(pos.x, tmpPos.y, pos.y);//yは
		Vec2 tPos = GameMasterM->GetTouchPosInView();//タッチ座標を取得

		tPos.y -= GameMasterM->reticleAjust * s.height;//レティクルの状態を
		if(tPos.y < 0.0f)
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
		for(int i = 0; i < UNIT2_BULLET - UNIT1_ENEMY; i++)
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
		while(left < right)
		{
			for(int i = left; i < right; i++)
			{
				if(tmpArrayDir[i] >tmpArrayDir[i + 1])
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
			for(int i = right; i > left; i--)
			{
				if(tmpArrayDir[i] < tmpArrayDir[i - 1])
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
		for(int i = 0; i < UNIT2_BULLET - UNIT1_ENEMY; i++)
		{
			int n = tmpArrayNum[i];
			//敵の
			if(TRUE == unit[n].visible
				&& UKIND_ENEMY == unit[n].kind
				&& unit[n].eState != ESTATE_DAMAGED
				&& unit[n].eState != ESTATE_DEAD
				&& unit[n].eState != ESTATE_SLEEP)
			{
				if(touchRay.intersects(unit[n].obbHead))//タッチ座標の法線と敵の当たり判定が接触したかをチェック
				{
					unit[n].hitpoint -= 1;
					//１体に攻撃するとその時点でbreakする
					break;
				}
			}
		}
		//現在はレイの計算のためにカメラ位置を一時的に変更しているので、処理後に位置を元に戻しています
		cam3d->setPosition3D(tmpPos);//
		cam3d->setRotation3D(tmpRot);//
	}

	//========================================================
	//敵の攻撃処理（弾とプレイヤーの当たり判定）
	if(TRUE == GameMasterM->playerHitFlag)
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
					GameMasterM->AddPlayerHP(-1);//ダメージを計算
					GameMasterM->playerHitFlag = FALSE;//食らい判定を無効化

					player.sprite3d->stopALLAnimation();//すべてのアニメーションを中断して
					switch(GameMasterM->playerSide)
					{
					case PSIDE_LEFT:
						player.sprite3d->startAnimation("hit_l");//食らいモーションを再生
						break;
					case PSIDE_RIGHT:
						player.sprite3d->startAnimation("hit_r");//食らいモーションを再生
						break;
					}
					GameMasterM->SetPlayerState(PSTATE_DAMAGED);//


					//座標と角度をセットしてキャラクターの座標を補正
					player.sprite3d->setRotation3D(GameMasterM->stagePoint[GameMasterM->sPoint].pRot);
					player.wrapper->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));

					//座標と角度をセットしてカメラの座標を補正
					Vec2 tmp = calcCamPos3(GameMasterM->stagePoint[GameMasterM->sPoint].pRot.y, GameMasterM->playerSide);
					camTarget = Vec3(tmp.x, 0.0f, tmp.y);

					//タイムとフレームの初期化
					//モーション管理用の時間を初期化
					player.motProcTime = 0;//モーションを再生してからの経過時間（秒）
					player.motPreTime = getNowTime();//PreTimeに現在時刻を代入
					player.motStartTime = getNowTime();//StartTimeに現在時刻を代入

					//一度でも攻撃を受けるとブレーク
					break;
				}
			}
		}
	}

	if(FALSE == GameMasterM->playerHitFlag && (PSTATE_DAMAGED == GameMasterM->GetPlayerState() || PSTATE_RECOVER == GameMasterM->GetPlayerState())
		)
	{
		//当たり判定がオフの時も、プレイヤーが食らいモーションを受けているときは弾とプレイヤーの当たり判定を処理する
		//（演出のための処理）
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
*	非表示の敵（Unit）を検索
*
*	@author	sasebon
*	@param	なし
*	@return	敵Unitの配列番号
*	@date	2/12 Ver 1.0
*/
int GameModelsLayer::SearchFreeEnemy()
{
	for(int i = UNIT1_ENEMY; i <= UNIT2_BULLET; i++)
	{
		if(FALSE == unit[i].visible)
		{
			return i;
		}
	}
	return -1;
}



/**
*	プレイヤーの回避中心計算
*
*	@author	sasebon
*	@param	プレイヤーの角度
*	@return	移動後のノード
*	@date	2/4 Ver 1.0
*/
Vec2 GameModelsLayer::calcRot(float pRot, int pSide)
{
	//
	Vec2 ret = Vec2(0.0f, 0.0f);
	cNode.gNode->setRotation(0.0f);
	cNode.gNode->setPosition(Vec2(0.0f, 0.0f));

	cNode.lNode->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	cNode.lNode->setPosition(Vec2(0.0f, 0.0f));

	//プレイヤーの向きに応じて処理を変更する
	if(PSIDE_LEFT == pSide)
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



/**
*	カメラの回避座標計算
*
*	@author	sasebon
*	@param	プレイヤー回転量（角度）、プレイヤーの左右の位置
*	@return	移動後の座標
*	@date	2/4 Ver 1.0
*/
Vec2 GameModelsLayer::calcCamPos(float pRot, int pSide)
{
	//
	Vec2 ret = Vec2(0.0f, 0.0f);
	cNode.gNode->setRotation(0.0f);
	cNode.gNode->setPosition(Vec2(0.0f, 0.0f));

	cNode.lNode->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	cNode.lNode->setPosition(Vec2(0.0f, 0.0f));

	cNode.lNode2->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	cNode.lNode2->setPosition(Vec2(0.0f, 0.0f));

	//プレイヤーの向きに応じて処理を変更する
	if(PSIDE_LEFT == pSide)
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



/**
*	カメラの回避座標計算
*
*	@author	sasebon
*	@param	プレイヤー回転量（角度）、プレイヤーの左右の位置
*	@return	移動後の座標
*	@date	2/4 Ver 1.0
*/
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
	if(PSIDE_LEFT == pSide)
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



/**
*	カメラの回避座標計算
*
*	@author	sasebon
*	@param	プレイヤー回転量（角度）、プレイヤーの左右の位置
*	@return	移動後の座標
*	@date	2/4 Ver 1.0
*/
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
	if(PSIDE_LEFT == pSide)
	{
		cNode.lNode->setPositionX(HIDECAMERA_X);//カメラの親ノードの回避後の座標
		cNode.lNode->setPositionY(HIDECAMERA_Y);//カメラの親ノードの回避後の座標

		cNode.gNode->setRotation(pRot);//カメラの親ノードをプレイヤーの向きだけ回転させる

		ret = cNode.gNode->convertToWorldSpace(cNode.lNode->getPosition());//回転後のlNodeの座標を取得

		//1フレーム当たりの移動量が計算できた
		//ret.x = ret.x / STS_HIDEWAIT;
		//ret.y = ret.y / STS_HIDEWAIT;

		//1秒当たりの移動量を計算する
		ret.x = ret.x / MACRO_FtoS(STS_HIDEWAIT);
		ret.y = ret.y / MACRO_FtoS(STS_HIDEWAIT);
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
		ret.x = ret.x / MACRO_FtoS(STS_HIDEWAIT);
		ret.y = ret.y / MACRO_FtoS(STS_HIDEWAIT);
	}
	return ret;
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
	auto sound = Sound::getInstance();
	auto random = rand() % 4;
	for(int num = UNIT1_ENEMY; num < UNIT2_BULLET; num++)
	{
		if(TRUE == unit[num].visible)//エネミーが表示されていれば
		{
			unit[num].Update(GameMasterM->loopTime);//フレーム・座標・当たり判定の更新

			auto director = Director::getInstance();
			auto loopTime = director->getDeltaTime();


			//敵キャラクターのhpが0になったら
			if(unit[num].hitpoint <= 0 && ESTATE_DAMAGED != unit[num].eState && ESTATE_DEAD != unit[num].eState)
			{
				unit[num].eState = ESTATE_DAMAGED;//ダメージ状態に変更
			}


			switch(unit[num].eState)
			{
			case ESTATE_SLEEP://敵が休眠状態

				unit[num].sleepTime -= GameMasterM->loopTime;
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



/**
*	次の敵を出現させる
*
*	@author	sasebon
*	@param	敵Unitの配列番号
*	@return	なし
*	@date	2/5 Ver 1.0
*/
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
			unit[n].nowTargetPos = 0;//0番のターゲットへ向かって移動する
			unit[n].tableNum = nextNum;

			//AIの種類をセット
			unit[n].AIAppear = enemyTable->enemyData[nextNum].AIappear;
			unit[n].AIAtk = enemyTable->enemyData[nextNum].AIAtk;
			unit[n].AIMove = enemyTable->enemyData[nextNum].AIMove;
			unit[n].AILife = enemyTable->enemyData[nextNum].AILife;

			//敵のステータスをセット
			for(int i = 0; i < 3; i++)
			{
				unit[n].stsAtkFrame[i] = enemyTable->enemyData[nextNum].stsAtkFrame[i];
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



/**
*	ウェーブ終了のチェック
*
*	@author	sasebon
*	@param	なし
*	@return	ウェーブ中:1 ウェーブ終了:-1
*	@date	2/5 Ver 1.0
*/
int GameModelsLayer::CheckNextStage(void)
{
	if(GSTATE_PLAY == GameMasterM->GetGameState())
	{
		for(int i = 0; i <= 100; i++)//
		{
			if(TRUE == enemyTable->enemyData[i].alive)
			{
				//1体でも敵が生きていれば
				return FALSE;
			}
		}
		player.sprite3d->stopALLAnimation();//現在のモーションを終了し

		GameMasterM->SetGameState(GSTATE_WAIT);//次のウェイトへ進む
		GameMasterM->wave += 1;//次のステージへ進む
		GameMasterM->waitFlag = 0;//
		GameMasterM->sPoint += 1;
	}
	else
	{
		//戦闘中で無ければ常にfalse
		return FALSE;
	}

	return TRUE;
}



/**
*	画面外の敵の全消去
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	2/5 Ver 1.0
*/
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


/**
*	プレイヤーの状態を死亡状態に変更
*
*	@author	sasebon
*	@param	なし
*	@return	ウェーブ中:1 ウェーブ終了:-1
*	@date	2/5 Ver 1.0
*/
void GameModelsLayer::KillPlayer()
{
	GameMasterM->SetPlayerState(PSTATE_DEAD);

	//必要ならばモーションの停止を行う
	player.sprite3d->stopAllActions();
	player.sprite3d->startAnimation("dei1");//


	//プレイヤーの座標を指定位置に動かす
	//座標と角度をセットしてキャラクターの座標を補正
	player.sprite3d->setRotation3D(GameMasterM->stagePoint[GameMasterM->sPoint].pRot);
	player.wrapper->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));


	//座標と角度をセットしてカメラの座標を補正
	ChangeCamera(2);

	//その他必要な処理があればここに追記します
}


/**
*	現在時刻を取得（ミリ秒）
*
*	@author	sasebon
*	@param	なし
*	@return	現在時刻（ミリ秒）
*	@date	2/5 Ver 1.0
*/
int GameModelsLayer::getNowTime(void)
{
	//現在時刻を取得
	gettimeofday(nowTV, nullptr);
	//時間差を計算
	nowTime = (nowTV->tv_sec * 1000.0f + nowTV->tv_usec * 0.001f);

	return nowTime;
}







//
void GameModelsLayer::ActionEStandby(int num)
{
	//向きの設定
	//スリープ状態からスタンバイ状態に移行するときは、targetPosの値から移動目標を自動で判断する
	for(int i = 0; i < 3; i++)
	{
		//現在はtargetPos配列に有効座標が入っているものとして処理を組んでいます
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
	case AI_APPEAR_SJUMP:
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
	case AI_APPEAR_SJUMP:
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


//移動
void GameModelsLayer::ActionEMove(int num)
{
	Vec3 tmpPos = unit[num].sprite3d->getPosition3D();
	//目標地点との距離を求める
	float d1 = tmpPos.distance(unit[num].targetPos[unit[num].nowTargetPos]);
	//一定以上目的地に近付いたら
	if(0.05f >= d1)
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

		Vec3 tmpPos = GameMasterM->stagePoint[GameMasterM->sPoint].pPos;//
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


//アイドル
void GameModelsLayer::ActionEIdle(int num)
{
	unit[num].atkFrame -= GameMasterM->loopTime;
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
			}

			break;
		case AI_ATK_FAKE://威嚇攻撃を行う
		case AI_ATK_SSHOT://立ち撃ちを行う
		case AI_ATK_SJUMP://サイドジャンプ射撃
		case AI_ATK_ACRO://アクロバティック
		case AI_ATK_NUM:

			//アイドル時間を設定
			SetEnemyAtk(num);

			break;
		}


	}
	else
	{

	}
}


//
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


//
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
	//アニメーションが終了したら
	//どの敵キャラクターも
	if(0 == unit[num].sprite3d->checkAnimationState())
	{
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
			unit[num].eState = ESTATE_IDLE;//アイドル状態を開始
			unit[num].sprite3d->stopALLAnimation();//現在のモーションを終了
			unit[num].sprite3d->startAnimationLoop("idle");//
			unit[num].atkFrame = unit[num].stsAtkFrame[unit[num].nowTargetPos];//アイドル時間を設定
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
			unit[num].atkFrame -= GameMasterM->loopTime;//アタックフレームを減少させていく
			if(0.0f >= unit[num].atkFrame && STS_ENEMY_MAXSHOT > unit[num].nowShot)
			{
				unit[num].atkFrame = 0.15f;//次の弾発射までの時間を設定
				unit[num].nowShot++;
			}
			break;
		case AI_ATK_SSHOT://立ち撃ちを行う

			//タイミングを合わせて射撃を行う
			unit[num].atkFrame -= GameMasterM->loopTime;//アタックフレームを減少させていく
			if(0.0f >= unit[num].atkFrame && STS_ENEMY_MAXSHOT > unit[num].nowShot)
			{
				//フレームが0になったら
				ShootBullet(num);
				unit[num].atkFrame = 0.15f;//次の弾発射までの時間を設定
				unit[num].nowShot++;
			}
			break;
		case AI_ATK_SJUMP://サイドジャンプ射撃

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

		//攻撃しない敵はAIのライフサイクルに従って動きを変更
		if(AI_LIFE_ONCE == unit[num].AILife)
		{
			//一度のみ
			//要チェック
			//アニメーションの終了
		}
		else if(AI_LIFE_STOP == unit[num].AILife)
		{
			//アイドル状態を繰り返す
			unit[num].eState = ESTATE_IDLE;

			//要チェック
			//アニメーションの終了

		}
		else if(AI_LIFE_SWITCH == unit[num].AILife)
		{
			//現在目標座標をもとに、次の目標座標へ移動する
			unit[num].eState = ESTATE_IDLE;
		}

		//アイドル時間を設定
		unit[num].atkFrame = unit[num].stsAtkFrame[unit[num].nowTargetPos];

		break;
	case AI_ATK_FAKE://威嚇攻撃を行う

		unit[num].eState = ESTATE_ATTACK1;//アタック状態に移る
		unit[num].sprite3d->stopALLAnimation();//現在のモーションを終了し
		unit[num].sprite3d->startAnimation("shot");

		//要チェック　アニメーションに合わせた弾の発射タイミング
		//unit[num].atkFrame = 1000;//弾を発射するまでの残りフレームとして扱う
		break;
	case AI_ATK_SSHOT://立ち撃ちを行う

		unit[num].eState = ESTATE_ATTACK1;//アタック状態に移る
		unit[num].sprite3d->stopALLAnimation();//現在のモーションを終了し
		unit[num].sprite3d->startAnimation("shot");

		//要チェック　アニメーションに合わせた弾の発射タイミング
		unit[num].atkFrame = 1000;//弾を発射するまでの残りフレームとして扱う
		break;
	case AI_ATK_SJUMP://サイドジャンプ射撃//要チェック

		//要チェック

		unit[num].eState = ESTATE_ATTACK1;//アタック状態に移る
		unit[num].sprite3d->stopALLAnimation();//現在のモーションを終了し
		unit[num].sprite3d->startAnimation("shot");
		//要チェック　アニメーションに合わせた弾の発射タイミング
		unit[num].atkFrame = 1.0f;//弾を発射するまでの残りフレーム

		break;
	case AI_ATK_ACRO://アクロバティック//要チェック

		//要チェック

		unit[num].eState = ESTATE_ATTACK1;//アタック状態に移る
		unit[num].sprite3d->stopALLAnimation();//現在のモーションを終了し
		unit[num].sprite3d->startAnimation("shot");
		//要チェック　アニメーションに合わせた弾の発射タイミング
		unit[num].atkFrame = 1.0f;//弾を発射するまでの残りフレーム

		break;
	default:

		//要チェック
		unit[num].eState = ESTATE_ATTACK1;//アタック状態に移る
		unit[num].sprite3d->stopALLAnimation();//現在のモーションを終了し
		unit[num].sprite3d->startAnimation("shot");
		//要チェック　アニメーションに合わせた弾の発射タイミング
		unit[num].atkFrame = 1.0f;//弾を発射するまでの残りフレーム
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
	case AI_MOVE_SJUMP://動かない
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
	case AI_MOVE_SJUMP://
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


/*

*/
void GameModelsLayer::update(float delta)
{

}


/*
指定フレームごとに呼び出される
*/
void GameModelsLayer::moveTime(float delta)
{

}







/**
*	カメラ設定の変更
*
*	@author	sasebon
*	@param	設定番号
*	@return	適切な引数:1,不正な引数:-1
*	@date	2/24 Ver 1.0
*/
int GameModelsLayer::ChangeCamera(int num)
{
	//ゲームの状態変化に合わせてカメラを切り替えることができます
	//カメラの座標と角度も変更できますが、あくまでカメラの初期位置であり、移動と回転は他で行う必要があります
	//カメラの動きを注視点管理で統一したら、この関数は使わなくする予定です

	switch(num)
	{
	case 0://0番はOPデモ用の初期設定

		break;
	case 1://1番はゲーム本編の初期設定

		break;
	case 2://2番は死亡用の初期設定

		//カメラの位置を任意の値に設定する
		GameMasterM->SetCamera3DPos(Vec3(KE_POSX, KE_POSY, KE_POSZ));
		GameMasterM->SetCameraTarget(Vec3(KT_POSX, KT_POSY, KT_POSZ));

		//カメラ座標と注視点からカメラ角度を設定する
		GameMasterM->CalcCameraRot();
		break;
	}
	return TRUE;
}


float GameModelsLayer::getCross(cocos2d::Vec3 v1, cocos2d::Vec3 v2)
{
	Vec2 vec1 = Vec2(v1.x, v1.z);
	Vec2 vec2 = Vec2(v2.x, v2.z);
	return vec1.cross(vec2);
}

