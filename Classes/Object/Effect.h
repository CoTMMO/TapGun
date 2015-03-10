
#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "cocos2d.h"

#include "Base/Sprite3D.h"


namespace TapGun
{
	class Effect
	{
	public:
		Effect( const Effect& p) = delete;
		Effect& operator=( const Effect& P) = delete;

		/*
		*	エフェクトオブジェクトへのインスタンスのゲッター
		*
		*	@author	minaka
		*	@param	なし
		*	@return	インスタンスへのポインタ
		 */
		static Effect* getInstance( void);

		/*
		*	プレイヤーマズルフラッシュの設定
		*
		*	@author	minaka
		*	@param	parentData 接続先モデルのポインタ
		*	@param	pointName 接続先ポイント名
		*	@return	なし
		 */
		void setPlayerMuzzle( cocos2d::Sprite3D* parentData, const std::string& pointName);

		/*
		*	敵マズルフラッシュの設定
		*
		*	@author	minaka
		*	@param	parentData 接続先モデルのポインタ
		*	@param	pointName1 接続先ポイント名
		*	@param	pointName2 接続先ポイント名
		*	@return	なし
		 */
		void setEnemyMuzzle( cocos2d::Sprite3D* parentData, const std::string& pointName1, const std::string& pointName2);

		/*
		*	敵マズルフラッシュの設定
		*
		*	@author	minaka
		*	@param	なし
		*	@return	なし
		 */
		void muzzleUpdate( void);

		/*
		*	ヒットエフェクトの表示設定
		*
		*	@author	minaka
		*	@param	sprite3d 表示先となるモデル
		*	@return	なし
		 */
		void setPlayerHitEffect( cocos2d::Sprite3D* sprite3d);

		/*
		*	ヒットエフェクトの生成と割り当て
		*
		*	@author	minaka
		*	@param	sprite3d 表示先となるモデル
		*	@param	point 表示する座標 (モデルからの相対座標)
		*	@return	なし
		 */
		void setPlayerHitEffect( cocos2d::Sprite3D* sprite3d, cocos2d::Vec3 point);

		/*
		*	ヒットエフェクトの生成と割り当て
		*
		*	@author	minaka
		*	@param	sprite3d 表示先となるモデル
		*	@return	なし
		 */
		void setEnemyHitEffect( cocos2d::Sprite3D* sprite3d);

		/*
		*	ヒットエフェクトの生成と割り当て
		*
		*	@author	minaka
		*	@param	sprite3d 表示先となるモデル
		*	@param	point 表示する座標 (モデルからの相対座標)
		*	@return	なし
		 */
		void setEnemyHitEffect( cocos2d::Sprite3D* sprite3d, cocos2d::Vec3 point);

	private:
		enum EffectNum					// エフェクトの最大数
		{
			PlayerNum =	1,
			EnemyNum = 32,

			PlayerMuzzleAnime = 3,
			EnemyMuzzleAnime = 2,
			EnemyMuzzleCount = 16,

			EffectGraphDataCount = 8,
		};

		enum EffectType					// エフェクトの種類
		{
			PlayerHitEffect,
			EnemyHitEffect,
		};

		struct EffectData				// 2Dエフェクト用構造体
		{
			bool flag;
			cocos2d::BillBoard* billboard;
			cocos2d::Node* node;
		};

		struct PlayerMuzzle				// プレイヤーマズルフラッシュ用構造体
		{
			bool shotFlag;
			int count;
			_Sprite3D* sprite3D[PlayerMuzzleAnime];
		};

		struct EnemyMuzzle				// 敵マズルフラッシュ用構造体
		{
			bool shotFlag;
			int count;
			_Sprite3D* sprite3DR[EnemyMuzzleAnime];
			_Sprite3D* sprite3DL[EnemyMuzzleAnime];
		};

		/*
		*	エフェクトの初期化
		*
		*	@author	minaka
		*	@param	なし
		 */
		Effect();

		/*
		*	ヒットエフェクトの生成と割り当て
		*
		*	@author	minaka
		*	@param	sprite3d 表示先となるモデル
		*	@param	point 表示する座標 (モデルからの相対座標)
		*	@param	effectType 作成するエフェクトの種類
		*	@return	なし
		 */
		void createHitEffect( cocos2d::Sprite3D* sprite3d, cocos2d::Vec3 vec, EffectType effectsType);

		EffectData *playerEffect[PlayerNum];
		EffectData *enemyEffect[EnemyNum];
		PlayerMuzzle *playerMuzzle;
		EnemyMuzzle *enemyMuzzle[EnemyMuzzleCount];
	};
}

#endif // __EFFECT_H__
