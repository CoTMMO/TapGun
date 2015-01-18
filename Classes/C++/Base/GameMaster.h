#ifndef __GAMEMASTER_H__
#define __GAMEMASTER_H__

#include "cocos2d.h"

namespace TapGun
{
	enum _CAMERA_FLAG_
	{
		CAMFLAG_DEFALUT = 1,
		CAMFLAG_3D = 2,
	};

	enum _GAME_STATE_
	{
		GSTATE_INIT,
		GSTATE_PAUSE,
		GSTATE_PLAY,
		GSTATE_NUM
	};

	enum _PLAYER_STATE_
	{
		PSTATE_IDLE,
		PSTATE_SHOT,
		PSTATE_DAMAGED,
		PSTATE_RUN,
		PSTATE_DEAD,
		PSTATE_NUM
	};

	enum _ENEMY_STATE_
	{
		ESTATE_IDLE,
		ESTATE_ATTACK1,
		ESTATE_ATTACK2,
		ESTATE_ATTACK3,
		ESTATE_DAMAGED,
		ESTATE_DEAD,
		ESTATE_NUM
	};

	class GameMaster
	{
	public:
		//�e��ϐ��͓K�Xget�֐��Aset�֐��ł̏����Ɉڂ��ւ��Ă����܂�


		//�ϐ�


		//�֐�
		GameMaster(const GameMaster &P) = delete;
		GameMaster& operator= (const GameMaster &P) = delete;
		static GameMaster* GetInstance(void);


		static cocos2d::Camera* Get3DCamInstance(void);
		static cocos2d::Camera* Get2DCamInstance(void);

		void InitScreenSize(void);


		void InitParam(void);
		void InitParam(int wave);

		void InitCamera2D(void);
		void InitCamera3D(void);


		////////////////////////////////////////
		//setParam(���݂�void�^�Ő錾���Ă��܂�)

		//2D�J�����p
		void SetCamera2DPos(cocos2d::Vec3 pos);//2D�J�����̈ʒu��ύX���邱�Ƃ͂��܂�Ȃ��̂ŕK�v�Ȃ��H

		//3D�J�����p
		void SetCamera3DPos(cocos2d::Vec3 pos);
		void SetCamera3DRot(cocos2d::Vec3 rot);
		void AddCamera3DPos(cocos2d::Vec3 pos);
		void AddCamera3DRot(cocos2d::Vec3 rot);


		void SetGameState(int state);
		void SetPlayerState(int state);
		void SetTouchPos(cocos2d::Vec2 tPos);



		////////////////////////////////////////
		//getParam

		//2D�J�����p
		cocos2d::Camera* GetCamera2D(void);//2D�p�J�����̃N���X��Ԃ�


		//3D�J�����p
		cocos2d::Camera* GetCamera3D(void);



		cocos2d::Vec2 GetTouchPos(void);
		int GetPlayerState(void);
		int GetGameState(void);

	private:
		cocos2d::Size screenSize;
		int wave;//���݃E�F�[�u
		int playerState;//�v���C���[�̏��
		int gameState;//�Q�[���̏��
		cocos2d::Vec2 touchPos;//�^�b�`���W�i���݂̓V���O���^�b�`�ɑΉ��j
		static cocos2d::Camera* camera2D;
		static cocos2d::Camera* camera3D;

		GameMaster() {}


	};
}

#endif // __GAMEMASTER_H__
