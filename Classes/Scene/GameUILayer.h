
#ifndef __GAME_UILAYER_H__
#define __GAME_UILAYER_H__

#include "cocos2d.h"

namespace TapGun
{
	class GameUILayer : public cocos2d::Layer
	{
	public:
		virtual bool init();

		void InitLayer();

		void UpdateLayer(void);

		CREATE_FUNC( GameUILayer);

		void setLogo( void);
		void setHP( int count);
		void setGameTime( float time);
		void setBulletCount( int count);
		void setReticlePoint( void);

	private:

		void initHPGauge( void);
		void initBulletCounter( void);
		void initOtherUI( void);

		enum HPGaugeValue		// HPゲージの設定に関する定数
		{
			HPFrame = 0,		// HPゲージ枠の配列番号
			HPBG = 1,			// HPゲージ背景の配列番号
			HPGaugeCount = 8,	// HPゲージの総スプライト数
			TimeCount = 5,		// タイマー用数値のスプライト数
			HPPositionX = 640,	// 描画位置X
			HPPositionY = 700,	// 描画位置
		};

		enum BulletCounterValue	// 残弾アイコンの設定に関する定数
		{
			BulletCount = 4,	// 残弾アイコンの総スプライト数
			BulletFrameX = 640,	// 描画位置X
			BulletFrameY = 60,	// 描画位置
		};

		enum OtherUIValue
		{
			EscapeLeft = 0,
			EscapeRight,
			EscapeCount,

			EnemyAttackCount = 5,
			DeleteCount = 5,

			ReticleIdle = 0,
			ReticleShot,
			ReticleCount,

			ActionLogo = 0,
			WaitLogo,
			ReloadLogo,
			GameLogoNum,
		};

		bool firstShotFlag;
		bool returnEscapeFlag;
		bool returnEscapeNoShotFlag;

		bool waitFlag;
		int waitTime;

		bool timeFlag;	// 1/100秒の制御フラグ
		int frame;		// 1/10秒以下の制御カウンタ

		int count01;
		int count001;

		cocos2d::Node *hpParent;
		cocos2d::Sprite *hp[HPGaugeCount];
		cocos2d::Node *timeParent;
		cocos2d::Sprite *timeNumber[TimeCount][10];
		cocos2d::Node *bulletParent;
		cocos2d::Sprite *bullet;
		cocos2d::Sprite *bulletNumber[BulletCount][10];
		cocos2d::Sprite *escape[EscapeCount];
//		cocos2d::Sprite *pause;
//		cocos2d::Sprite *enemyAttack[EnemyAttackCount][3];
//		cocos2d::Sprite *deleteLogo[DeleteCount];
		cocos2d::Sprite* logo[GameLogoNum];
		cocos2d::Sprite* reticle[ReticleCount];
	};
}
#endif //__GAME_UILAYER_H__
