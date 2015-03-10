
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

		enum HPGaugeValue		// HP�Q�[�W�̐ݒ�Ɋւ���萔
		{
			HPFrame = 0,		// HP�Q�[�W�g�̔z��ԍ�
			HPBG = 1,			// HP�Q�[�W�w�i�̔z��ԍ�
			HPGaugeCount = 8,	// HP�Q�[�W�̑��X�v���C�g��
			TimeCount = 5,		// �^�C�}�[�p���l�̃X�v���C�g��
			HPPositionX = 640,	// �`��ʒuX
			HPPositionY = 700,	// �`��ʒu
		};

		enum BulletCounterValue	// �c�e�A�C�R���̐ݒ�Ɋւ���萔
		{
			BulletCount = 4,	// �c�e�A�C�R���̑��X�v���C�g��
			BulletFrameX = 640,	// �`��ʒuX
			BulletFrameY = 60,	// �`��ʒu
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

		bool timeFlag;	// 1/100�b�̐���t���O
		int frame;		// 1/10�b�ȉ��̐���J�E���^

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
