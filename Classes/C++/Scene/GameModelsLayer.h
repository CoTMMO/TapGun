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


		virtual bool init();
		//static cocos2d::Scene* CreateScene();

		int InitLayer(void);

		int InitPlayer(int stage_num);//

		int InitMap(int stage_num);
		int InitEnemy(int stage_num);

		int GetPlayerNum();

		void update(float delta);
		void moveTime(float delta);

		void UpdateModels();//�e�V�[������Ăяo�����

		//���f���\���p�̍\���̂Ɗ֐����ꎞ�I�ɂ܂Ƃ߂Ă��܂�
		void InitAllModels();//
		int SearchFreeUnit();//
		Unit unit[MAX_UNIT];//

		int playerNum;


		void UpdateLayer();//���C���[�X�V

		CREATE_FUNC(GameModelsLayer);

	private:
		void UpdatePlayer();//

		void UpdateEnemy();
		void UpdateBullets();

		void ShootBullet(int enemy_num);

	};
}

#endif //__GAME_MODELSLAYER_H__