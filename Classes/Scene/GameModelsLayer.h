
#ifndef __GAME_MODELSLAYER_H__
#define __GAME_MODELSLAYER_H__

#include "cocos2d.h"

#include "Base/Unit.h"
#include "Base/Player.h"
#include "Stage/EnemyTable.h"

namespace TapGun
{

	enum _UNIT_NUM_
	{
		UNIT0_MAP = 0,
		UNIT1_ENEMY = 1,
		UNIT2_BULLET = 21,
		UNIT3_MAX = MAX_UNIT
	};

	typedef struct
	{
		cocos2d::Node* gNode;
		cocos2d::Node* lNode;
		cocos2d::Node* lNode2;
	}CalcNode;


	class GameModelsLayer : public cocos2d::Layer
	{
	public:
		Player player;

#ifdef DEBUG_CENTER
		Unit center;
#endif
		Unit calc;
		Unit unit[MAX_UNIT];//

		EnemyTable enemyTable[100];//敵の出現テーブル

		CalcNode cNode;
		cocos2d::Vec3 camTarget;//回避時のカメラの移動先座標
		cocos2d::Vec3 camCenter;//回避時のカメラの移動前座標

		int enemyStuck;//各ステージごとの残り敵数


		/**
		*	ゲーム本編のモデルレイヤーの初期化
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	初期化成功／不可のbool値
		*	@date	1/8 Ver 1.0
		*/
		virtual bool init();

		/**
		*	ゲーム本編のスプライトの生成
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	なし
		*	@date	2/4 Ver 1.0
		*/
		void LoadModels(void);


		/**
		*	ライトのセット
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	なし
		*	@date	2/5 Ver 1.0
		*/
		void SetLights(void);//ライトをセット

		/**
		*	ゲーム本編のUIレイヤーの各種数値初期化
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	なし
		*	@date	1/8 Ver 1.0
		*/
		void InitLayer(void);


		void InitAllModels(void);//モデル全体の初期化

		/**
		*	プレイヤー初期化
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	なし
		*	@date	3/9 Ver 1.0
		*/
		void InitPlayer(void);

		/**
		*	エネミー初期化
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	正常:1 初期化失敗:-1
		*	@date	3/9 Ver 1.0
		*/
		int InitEnemy(void);


		void InitBullet(void);

		/**
		*	ステージオブジェクトの初期化
		*
		*	@author	sasebon
		*	@param	ステージ番号
		*	@return	正常:1 初期化失敗:-1
		*	@date	1/8 Ver 1.0
		*/
		int InitMap();

		void checkNextEnemy(int enemyNum);//

		/**
		*	プレイヤーの回避中心計算
		*
		*	@author	sasebon
		*	@param	プレイヤーの角度
		*	@return	移動後のノード
		*	@date	2/4 Ver 1.0
		*/
		cocos2d::Vec2 calcRot(float pRot, int pSide);//角度計算

		/**
		*	カメラの回避座標計算
		*
		*	@author	sasebon
		*	@param	プレイヤー回転量（角度）、プレイヤーの左右の位置
		*	@return	移動後の座標
		*	@date	2/4 Ver 1.0
		*/
		cocos2d::Vec2 calcCamPos(float pRot, int pSide);

		/**
		*	カメラの回避座標計算
		*
		*	@author	sasebon
		*	@param	プレイヤー回転量（角度）、プレイヤーの左右の位置
		*	@return	移動後の座標
		*	@date	2/4 Ver 1.0
		*/
		cocos2d::Vec2 calcCamPos2(float pRot, int pSide);

		/**
		*	カメラの回避座標計算
		*
		*	@author	sasebon
		*	@param	プレイヤー回転量（角度）、プレイヤーの左右の位置
		*	@return	移動後の座標
		*	@date	2/4 Ver 1.0
		*/
		cocos2d::Vec2 calcCamPos3(float pRot, int pSide);


		/**
		*	ウェイト状態の更新
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	なし
		*	@date	1/23 Ver 1.0
		*/
		void UpdateWait(void);//

		/**
		*	プレイヤーの更新
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	なし
		*	@date	1/8 Ver 1.0
		*/
		void UpdatePlayer(void);//
		void UpdateEnemy(void);
		void UpdateBullets(void);

		/**
		*	プレイヤーの攻撃の当たり判定処理
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	なし
		*	@date	3/9 Ver 1.0
		*/
		void CheckPlayerAtk(void);


		void CheckEnemyAtk(void);//当たり判定処理（エネミーの攻撃）

		/**
		*	ウェーブ終了のチェックと次ウェーブの初期化
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	ウェーブ中:1 ウェーブ終了:-1
		*	@date	2/5 Ver 1.0
		*/
		int CheckNextWave(void);

		/**
		*	ステージごとのエネミーのセット
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	なし
		*	@date	1/8 Ver 1.0
		*/
		void SetNextWave(void);//

		/**
		*	画面外の敵の全消去
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	なし
		*	@date	2/5 Ver 1.0
		*/
		void ClearEnemies();//敵を全て非表示にする（ゲームオーバー状態にする前に使用する）

		/**
		*	プレイヤーの状態を死亡状態に変更
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	ウェーブ中:1 ウェーブ終了:-1
		*	@date	2/5 Ver 1.0
		*/
		void KillPlayer();

		CREATE_FUNC(GameModelsLayer);

	private:

		/**
		*	プレイヤーの攻撃状態の更新
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	なし
		*	@date	1/21 Ver 1.0
		*/
		void ActionShot(void);

		/**
		*	プレイヤーのアイドル状態の更新
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	なし
		*	@date	1/21 Ver 1.0
		*/
		void ActionIdle(void);

		/**
		*	プレイヤーの避け最中の更新
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	なし
		*	@date	1/21 Ver 1.0
		*/
		void ActionDodge(void);

		/**
		*	プレイヤーの隠れ状態の更新
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	なし
		*	@date	1/21 Ver 1.0
		*/
		void ActionHide(void);

		/**
		*	プレイヤーの飛び出し状態の更新
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	なし
		*	@date	1/21 Ver 1.0
		*/
		void ActionAppear(void);

		/**
		*	プレイヤーの食らい状態の更新
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	なし
		*	@date	1/21 Ver 1.0
		*/
		void ActionDamaged(void);

		/**
		*	プレイヤーの起き上がり状態の更新
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	なし
		*	@date	1/21 Ver 1.0
		*/
		void ActionRecover(void);

		/**
		*	プレイヤーのウェイト状態の更新
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	なし
		*	@date	1/21 Ver 1.0
		*/
		void ActionRun(void);

		/**
		*	プレイヤーの死亡状態の更新
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	なし
		*	@date	1/21 Ver 1.0
		*/
		void ActionDead(void);

		//エネミーの更新
		void ActionEStandby(int num);
		void ActionEIdle(int num);
//		void ActionEDodge(int num);
		void ActionEDamaged(int num);
//		void ActionERecover(int num);
		void ActionEDead(int num);
		void ActionEMove(int num);
		void ActionEAttack(int num);


		void SetEnemyAtk(int num);

		//flag 0はAI_MOVE,flag 1はAI_APPEAR
		void SetEnemyMove(int num,int flag);


		/**
		*	次の敵を出現させる
		*
		*	@author	sasebon
		*	@param	敵Unitの配列番号
		*	@return	なし
		*	@date	2/5 Ver 1.0
		*/
		void setNextEnemy(int num);

		//
		void ShootBullet(int enemy_num);//
		void ShootBullet(int enemy_num, int count);//

		/**
		*	非表示の敵（Unit）を検索
		*
		*	@author	sasebon
		*	@param	なし
		*	@return	敵Unitの配列番号
		*	@date	2/12 Ver 1.0
		*/
		int SearchFreeEnemy();


		//カメラの設定
		int ChangeCamera(int num);

		//計算用
		float getCross(cocos2d::Vec3 v1, cocos2d::Vec3 v2);
	};
}
#endif //__GAME_MODELSLAYER_H__
