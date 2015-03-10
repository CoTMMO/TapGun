#ifndef __UNIT_H__
#define __UNIT_H__

#include "cocos2d.h"

#include <stdio.h>
#include <fstream>

#include "Base/Sprite3D.h"


#define MAX_UNIT 70


namespace TapGun
{

	//この定義は今後異なるヘッダファイルに移し替えます
#define FALSE -1
#define TRUE 1


	enum _UNIT_KIND_
	{
		UKIND_NONE,//未使用
		UKIND_ENEMY,//エネミー
		UKIND_EBULLET,//敵弾
		UKIND_MAP,//マップモデルも現在はUnitに含めています
		UKIND_NUM
	};

	enum _ENEMY_KIND_
	{
		EKIND_TWIN,//ツインマシンガン
		EKIND_NUM//
	};

	enum _ENEMY_STATE_
	{
		ESTATE_IDLE,
		ESTATE_STANDBY,
		ESTATE_MOVE,

		ESTATE_NONE,//
		ESTATE_SLEEP,
		ESTATE_ATTACK1,
		ESTATE_DAMAGED,
		ESTATE_DEAD,//死亡
		ESTATE_END,//敵の終了処理
		ESTATE_NUM
	};

	//Unitクラス
	//プレイヤー・エネミー・弾・マップデータに用います
	class Unit
	{
	public:

		//メンバ変数
		//==フラグ系==
		int visible;//ユニット表示フラグ( == 使用フラグ)

		//AI管理
		int AIAppear;//
		int AIMove[3];//
		int AIAtk[3];//エネミーの攻撃モーション
		int AILife;//AIの生き方

		int nextEnemies[3];//このエネミーを倒した次に出てくるエネミーの番号

		//パラメータ
		int kind;//Unit種別（_UNIT_KIND_）
		int hitpoint;//
		int nowShot;
		int maxShot;//

		int eState;//敵の状態
		int oldState;//敵の状態

		//管理フレーム(必要であれば時間をdoubleに変更する)
		//int atkFrame;//攻撃までのカウント（ミリ秒）
		int waitTime;//各種待ち時間（ミリ秒）

		int stsWaitToMove[3];//移動までの待ち時間定義（ミリ秒）
		int stsWaitToAtk[3];//攻撃までの待ち時間定義（ミリ秒）

		cocos2d::Vec3 StandbyPos;//待機座標
		int tableNum;


		cocos2d::Vec3 pos;//モデル座標・・・削除予定（sprite3dの座標を使用）
		float speed;//移動速度（移動量）
		cocos2d::Vec3 speedVec;//移動ベクトル（speedをもとに計算する）
		cocos2d::Vec3 targetPos[3];//移動時の目標座標
		int nowTarget;//何番目の目標座標に向けて走るかの設定
		int maxTarget;//移動先が何箇所あるか（1~3）
		cocos2d::Vec3 collisionPos;//OBBの辺の長さ（現在は１つのみ定義）

		//==当たり判定関連クラス(仮)==

		cocos2d::Node* colisionNode;//当たり判定基準用ノード

		//当たり判定用ボックスは敵のみ使用する（プレイヤーと敵弾では使用しない）
		cocos2d::AABB aabbHead;//
		cocos2d::AABB aabbBody;//
		cocos2d::OBB obbHead;

		//==モデル・アニメーション関連クラス==
		TapGun::_Sprite3D* sprite3d;
		cocos2d::Node* wrapper;//モデルの親ノード（モデル基準座標として使用する。通常は(0,0,0)座標）
		cocos2d::Node* node1;//座標取得用ノード（敵の場合は左手、敵弾の場合は弾の先端の座標を表す）
		cocos2d::Node* node2;//座標取得用ノード（敵の場合は右手の座標を表す）

		cocos2d::Animation3D* animation;
		cocos2d::Animate3D* animate;

		//メンバ関数
		void Init(void);//数値の初期化（今後はコンストラクタに置き換える？）
		int Init(int num, int utype);//数値の初期化

		void SetCollision(void);//当たり判定を初期化
		void SetTargetPos(cocos2d::Vec3 pos[3]);//
		void Update(void);//速度をもとに座標移動と当たり判定移動
		void Update(int loopTime);//
		void SetPos(cocos2d::Vec3 pos);//引数の座標に移動

		void SetAnimation(const std::string& animeName, const int speed);//

		void InitTime(void);//時間を初期化
		int GetTime(void);//キャラクター固有時間の取得
		void SetTime(int time);//時間のセット（ミリ秒）
	private:

		int time;//Unit固有時間（ミリ秒）
	};
}
#endif //__UNIT_H__
