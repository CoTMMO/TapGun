#ifndef __GAME_MODELSLAYER_H__
#define __GAME_MODELSLAYER_H__

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "Unit.h"

#else

#include "C++/Base/Unit.h"

#endif


namespace TapGun
{

	class GameModelsLayer : public cocos2d::Layer
	{
	public:

		//�����o�ϐ�
		int playerNum;


		virtual bool init();//���C���[�̃C���X�^���X����

		//�������n
		int InitLayer(void);//���C���[�̕ϐ�������
		void InitAllModels();//���f���S�̂̏�����

		int InitPlayer(int stage_num);//�v���C���[�̏�����
		int InitEnemy(int stage_num);//�G�l�~�[������
		int InitMap(int stage_num);//�}�b�v�̏�����


		void UpdateLayer();//���C���[�X�V

		//
		int GetPlayerNum();

		void UpdateModels();//�e�V�[������Ăяo�����

		//���f���\���p�̍\���̂Ɗ֐����ꎞ�I�ɂ܂Ƃ߂Ă��܂�
		int SearchFreeUnit();//
		Unit unit[MAX_UNIT];//


		//
		void update(float delta);//���ݎg�p���Ȃ�
		void moveTime(float delta);//���ݎg�p���Ȃ�

		CREATE_FUNC(GameModelsLayer);

	private:

		//�X�V�n
		void UpdatePlayer();//
		void UpdateEnemy();
		void UpdateBullets();

		void ShootBullet(int enemy_num);//
		void CheckHit( void);//�����蔻�菈��

	};
}

#endif //__GAME_MODELSLAYER_H__