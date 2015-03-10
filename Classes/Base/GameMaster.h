#ifndef __GAMEMASTER_H__
#define __GAMEMASTER_H__

#include "cocos2d.h"


namespace TapGun
{

	//���̒�`�͍���قȂ�w�b�_�t�@�C���Ɉڂ��ւ��܂�
#define FALSE -1
#define TRUE 1

//�v���C���[�̃p�����[�^��`
#define STS_MAXPLAYERHP 6//�v���C���[�̍ő�HP
#define STS_RAPIDSPEED 8//�v���C���[�̘A�ˑ��x�i�t���[���j
#define STS_MAXBULLETS 30//�v���C���[�̍ő�e��
#define STS_MUTEKITIME 2000 //���G����(�~���b)

#define STS_RUNSPEED 0.005f//�v���C���[�̃E�F�C�g���̑��x
#define STS_HIDEWAIT 25//������[�V�������I������܂ł̑S�̃t���[��
#define STS_HIDESTART 1//����{�^���������Ă��������[�V�������n�܂�܂ł̎���
#define STS_MUTEKISTART 0//������[�V�������n�܂��Ă��疳�G���ԂɈڍs����܂ł̎���
#define STS_APPEARSTART 1//����{�^���𗣂��Ă���ˌ����[�V�������n�܂�܂ł̎���
#define STS_MUTEKIEND (STS_HIDEWAIT - STS_MUTEKISTART)//�ˌ����[�V�������n�܂��Ă��疳�G���Ԃ��I������܂ł̎���
#define STS_RELOADSTART 12//������[�V�������n�܂��Ă��烊���[�h���s����܂ł̎���



//�G�̃p�����[�^��`
#define STS_ENEMY_HP 6//�G��HP
#define STS_ENEMY_MAXSHOT 5//�G�̘A�ː�

#define STS_ENEMY_RUNSPEED 0.005f//�G�̑��葬�x
#define STS_ENEMY_WALKSPEED 0.0015f//�G�̕������x
#define STS_ENEMY_BWALKSPEED 0.0012f//�G�̌ジ���葬�x
#define STS_ENEMY_SWALKSPEED 0.0005f//�G�̉��������x
#define STS_ENEMY_SJUMPSPEED 0.0012f//�G�̃X���C�h�ړ��̊�{���x�i�����ɂ���ĕω�����j
#define STS_ENEMY_ACROSPEED 0.00005f//�G�̃A�N���o�b�g�ړ��̊�{���x�i�����ɂ���ĕω�����j
#define STS_ENEMY_FJUMPSPEED 0.003f//�O�W�����v���x

#define POS_ENEMY_LPOX -0.3f//�G�̍��̏e�̃|�C���g
#define POS_ENEMY_LPOY 1.25f//�G�̍��̏e�̃|�C���g
#define POS_ENEMY_LPOZ 0.1f//�G�̍��̏e�̃|�C���g
#define POS_ENEMY_RPOX 0.3f//�G�̉E�̏e�̃|�C���g
#define POS_ENEMY_RPOY 1.25f//�G�̉E�̏e�̃|�C���g
#define POS_ENEMY_RPOZ 0.1f//�G�̉E�̏e�̃|�C���g

#define STS_EBULLET_SPEED 0.019f//�G�̒e�̑��x
#define STS_EBULLET_VANISH_TIME 3000//�G�̒e�̑��x
#define STS_SSHOT_START 1000//SShot���̒e�̔��ˊJ�n�܂ł̑҂�����
#define STS_SSHOT_SPAN 68//SShot���̒e�̔��ˊJ�n�܂ł̑҂�����

#define ENEMY_STOP_DIST 0.19f//�ړ����̓G�͖ڕW�n�_�Ƃ̋��������̐��l�����ɂȂ�ƒ�~����


#define BATTLE_FEILD_X 0.8//�^�b�`�ōU���\�ȉ�ʊ���


//�����蔻��n��`
#define PLAYER_CENTER_Y 1.2f//�v���C���[�̓����蔻�荂���i�G�e���ڕW�Ƃ��鍂���j


//����{�^���̓����蔻���`�i�{�^���̃X�v���C�g�Ɠ����蔻��𕪂��Ď������܂��j
#define HIDE_UI_RECT_X 0.18f//�����蔻���`
#define HIDE_UI_RECT_Y 0.18f//�����蔻���`
#define HIDE_UI_POS_X 0.09f//�����蔻�蒆�S
#define HIDE_UI_POS_Y 0.2f//�����蔻�蒆�S


//�ȏ�̃X�e�[�^�X�����Ƃɍ��W���v�Z����
#define HIDE_UI_X0 (HIDE_UI_RECT_X * 0.5f - HIDE_UI_POS_X)
#define HIDE_UI_Y0 (HIDE_UI_RECT_Y * 0.5f - HIDE_UI_POS_Y)
#define HIDE_UI_X1 (HIDE_UI_RECT_X * 0.5f + HIDE_UI_POS_X)
#define HIDE_UI_Y1 (HIDE_UI_RECT_Y * 0.5f + HIDE_UI_POS_Y)


#define PERSE 22.296f//�J��������p

//Action���̃J�����̐ݒ�(�E)
#define C_SETX_R -0.62f
#define C_SETY_R 1.51f
#define C_SETZ_R 5.3f
#define C_ROTX_R -4.0f
#define C_ROTY_R 10.0f
#define C_ROTZ_R 0.0f


//Action���̃J�����̐ݒ�(��)
#define C_SETX_L 0.0f
#define C_SETY_L 1.61f
#define C_SETZ_L 4.8f
#define C_ROTX_L 0.0f
#define C_ROTY_L -13.0f
#define C_ROTZ_L 0.0f


//Wait���̃J�����̐ݒ�
#define W_PERSE 45//�J��������p
#define W_SETX 0.0f
#define W_SETY 1.61f
#define W_SETZ 4.0f

#define W_ROTX 0.0f
#define W_ROTY 0.0f
#define W_ROTZ 0.0f


//���S���̃J�����̏����ݒ�
#define KE_POSX 0.0f//Kill Eye Pos
#define KE_POSY 2.8f
#define KE_POSZ -6.0f

#define KT_POSX 0.0f//Kill Target Pos
#define KT_POSY 0.0f
#define KT_POSZ 0.0f


//3DSMAX�̃J�����p�x��ʏ�p�x�ɕϊ����邽�߂̃}�N��
#define MACRO_CROT_X(x) (x - 90)
#define MACRO_CROT_Y(y) (y + 180)

	//�~���b���t���[���ɕϊ�/�t���[�����~���b�ɕϊ�
#define MACRO_StoF(second) (second * 60.0f * 0.001f)
#define MACRO_FtoMS(frame) (frame * 0.01666f * 1000)
#define MACRO_FtoS(frame) (frame * 0.01666f)


	//�v���C���[��������鎞�̎��̍��W�i�v���C���[���猩�����΍��W�A�������j
	//�v���C���[��0�x��Ŕz�u���Ă���̂ŁAX��Y�����J�����Ƃ��ꂼ�ꔽ�]���Ă��܂�
#define HIDEPOINT_X 0.35f
#define HIDEPOINT_Y 0.35f

	//����������̃J�����̈ړ���̍��W�i�J�������猩�����΍��W�A�������j
#define HIDECAMERA_X 1.1f
#define HIDECAMERA_Y -1.1f

//�Q�[���֘A����
#define TIME_MAXTIME 180000//�Q�[���{�҂̍ő厞�ԁi�~���b�j
#define TIME_ACTION_UI 1800//Action��UI��\�����鎞�ԁi�~���b�j
#define TIME_DEAD_UI 3000//���S���̃J�����\�����鎞�ԁi�~���b�j
#define TIME_OP 0//OP�̎��ԁi�~���b�j�E�E�E���݂�OP���Ȃ��̂�0�~���b

#define TIME_WROT 300//


	enum _CAMERA_FLAG_
	{
		//CAMFLAG_DEFAULT = CameraFlag::DEFAULT,//
		//CAMFLAG_3D = CameraFlag::USER1
		CAMFLAG_DEFAULT = 1,//
		CAMFLAG_3D = 2
	};

	enum _GAME_STATE_
	{
		GSTATE_CREATE,//���C���[�������ɌĂ΂��
		GSTATE_INIT,//
		GSTATE_OP,//OP���
		GSTATE_WAIT,//�E�F�C�g��
		GSTATE_PLAY_SET,//�퓬�J�n�O�̑҂����ԁi�G�̔z�u�ɂ̂ݎg�p����j
		GSTATE_PLAY_ACTION,//�퓬�J�n�O�̑҂����ԁiAction��UI��`�悷��Ƃ��Ɏg�p����j
		GSTATE_PLAY,
		GSTATE_PAUSE,
		GSTATE_TIMEOVER,//�^�C���I�[�o�[�ɂ�鉉�o
		GSTATE_DEADOVER,//�_���[�W���S�ɂ�鉉�o

		GSTATE_CONTINUE_INIT,//�R���e�B�j���[��ʑO�̏���
		GSTATE_CONTINUE,//�R���e�B�j���[���邩�ǂ����̑���͂����ōs��
		GSTATE_CONTINUE_END,//�R���e�B�j���[�I����̏�����

		GSTATE_GAMEOVER,//
		GSTATE_CLEAR,//
		GSTATE_END,//�Q�[���V�[�����I�����ă^�C�g���֖߂�

		GSTATE_EVENT,//���[�r�[�C�x���g�Ȃǂ�i�s������Ƃ��Ɏg�p����H�i���ݖ��g�p�j
		GSTATE_NUM
	};

	enum _PLAYER_STATE_
	{
		PSTATE_IDLE,
		PSTATE_SHOT,
		//		PSTATE_PLAY_SET,//�A�N�V�����̏�����
		PSTATE_DODGE,//�B�ꒆ
		PSTATE_HIDE,//�B��Ă���
		PSTATE_APPEAR,//�B�ꂽ��Ԃ���o��
		PSTATE_DAMAGED,//�U����H�����
		PSTATE_RECOVER,//��e����̉�
		PSTATE_RUN,
		PSTATE_DEAD,//�v���C���[�̎��S
		PSTATE_CONTINUE,//�v���C���[���S��̑���҂�
		PSTATE_NUM
	};

	enum _PLAYERSIDE_
	{
		PSIDE_RIGHT,
		PSIDE_LEFT
	};

	enum _TOUCH_STATE_
	{
		TSTATE_OFF,
		TSTATE_ON,
		TSTATE_MOVE,
		TSTATE_RELEASE
	};

	enum _TOUCH_FLAG_
	{
		TFLAG_OFF,
		TFLAG_ON,
		TFLAG_MOVE,
		TFLAG_RELEASE,
		TFLAG_CANCEL,
		TFLAG_NUM
	};

	//
	enum _STAGE_POINT_DEF_
	{
		PO_NONE,//�g�p���Ȃ�
		PO_START,//�X�^�[�g�n�_
		PO_BATTLE,//�o�g���}�X
		PO_CHANGE,//�����]��
		PO_FINISH,
	};

	enum _STAGE_POINT_
	{
		//W = �E�F�[�u
		//L = ���[�g

		POINT_START,
		//POINT_L1_0,�͎g�p���Ȃ�
		POINT_W1,

		POINT_L2a,
		POINT_L2b,
		POINT_L2c,
		POINT_L2d,

		//POINT_W2,

		//POINT_L3_1,
		//POINT_W3,
		//POINT_W3_2,
		//POINT_W4,
		POINT_END,
	};


	//�v���C���[�̐i�s���W���`����\����
	typedef struct
	{
		cocos2d::Vec3 pPos;//�v���C���[�̈ʒu
		cocos2d::Vec3 pRot;//�v���C���[�̊p�x
		cocos2d::Vec3 cPos;//�J�����̈ʒu
		cocos2d::Vec3 cRot;//�J�����̊p�x

		int pointType;
		int playerSide;
		cocos2d::Point hidePoint;//��𓮍�̎��ƂȂ���W
	}StagePoint;

	class GameMaster
	{
	public:
		//�e��public�ϐ��͍���private�ɒu�������Ă����܂�

		//�^�C��
		int gameActionTime;//�E�F�[�u�̎c�莞�ԁi�~���b�j

		/*
		timeval* nowTV;//���ݎ���(timeval)
		timeval* preTV;//�O�t���[���܂ł̎���(timeval)
		int nowTime;//���ݎ����i�~���b�j
		int preTime;//1���[�v�O�̎����i�~���b�j
		*/
		int loopTime;//���[�v�ɗv�������ԁi�~���b�j

		int shotFlag;//�v���C���[�̍U���A�j���[�V�����p�t���O(5�t���[���ȏ�^�b�`���Ă��邩�ǂ���)

		//�e��t���O�i���private�ɏC������j
		int waitFlag;//�E�F�C�g���[�h��i�s������t���O
		int sPoint;//���݂̃X�e�[�W�|�C���g
		float rapidFrame;//�A�ˑ҂�����
		int wave;//���݃E�F�[�u
		StagePoint stagePoint[100];//�v���C���[�̐i�s���W���`����\����

		int playerSide;//�v���C���[�����E�ǂ���ɗ����Ă��邩�̃t���O
		int playerHitFlag;//�v���C���[�̖��G��Ԃ̃t���O�iTRUE/FALSE�j

		int flgPlayerATK;//�v���C���[�̍U������������s�����iTRUE/FALSE�j
		int mutekiTime;//���G����(�~���b)

		float reticleAjust;//�w�̈ʒu�ƃ��e�B�N���̈ʒu�̍��i��ʂɑ΂��銄���j


		//�֐�
		GameMaster(const GameMaster &P) = delete;
		GameMaster& operator= (const GameMaster &P) = delete;
		static GameMaster* GetInstance(void);

		static cocos2d::Camera* Get2DCamInstance(void);
		static cocos2d::Camera* Get3DCamInstance(void);
		static cocos2d::Node* GetCamNodeInstance(void);

		void InitScreenSize(void);

		void InitParam(void);
		void InitParam(int wave);

		void InitCamera2D(void);
		void InitCamera3D(void);

		//2D�J�����p
		void SetCamera2DPos(cocos2d::Vec3 pos);//2D�J�����̈ʒu��ύX���邱�Ƃ͂��܂�Ȃ��̂ŕK�v�Ȃ��H
		cocos2d::Camera* GetCamera2D(void);//2D�p�J�����̃N���X�|�C���^��Ԃ�

		//3D�J�����p
		void SetCamera3DPos(cocos2d::Vec3 pos);
		void SetCamera3DRot(cocos2d::Vec3 rot);
		void AddCamera3DPos(cocos2d::Vec3 pos);
		void AddCamera3DRot(cocos2d::Vec3 rot);
		cocos2d::Vec3 GetCamera3DPos(void);
		cocos2d::Vec3 GetCamera3DRot(void);
		//3D�J�����̃m�[�h
		void SetCameraNodePos(cocos2d::Vec3 pos);
		void SetCameraNodeRot(cocos2d::Vec3 rot);
		void AddCameraNodePos(cocos2d::Vec3 pos);
		void AddCameraNodeRot(cocos2d::Vec3 rot);
		cocos2d::Vec3 GetCameraNodePos(void);
		cocos2d::Vec3 GetCameraNodeRot(void);
		//3D�J�����̒����_
		void SetCameraTarget(cocos2d::Vec3 pos);
		cocos2d::Vec3 GetCameraTarget(void);

		cocos2d::Camera* GetCamera3D(void);
		const cocos2d::Node* GetCameraNode(void);

		void SetPlayerState(int state);

		//�^�b�`�֘A
		void UpdateTouchManager(void);//�^�b�`�Ǘ��N���X���X�V
		void SetTouchPos(cocos2d::Touch* tch);//�^�b�`���W���擾
		int SetTouchFlag(int state);//�^�b�`�t���O���擾
		cocos2d::Vec2 GetTouchPosInView(void);//�^�b�`���W��Ԃ�
		cocos2d::Vec2 GetTouchPos(void);//�^�b�`���W��Ԃ�

		int GetTouchFlag(void);//�^�b�`�t���O��Ԃ�
		int GetTouchState(void);//�^�b�`��Ԃ�Ԃ�

		int GetPlayerState(void);

		//�Q�[����Ԃ̃Z�b�g�Ǝ擾
		int GetGameState(void);
		void SetGameState(int gState);

		//�v���C���[HP�̃Q�b�^�[�ƃZ�b�^�[
		int GetPlayerHP(void);
		int SetPlayerHP(int value);
		int AddPlayerHP(int value);

		//�v���C���[�e���̃Q�b�^�[�ƃZ�b�^�[
		int GetPlayerBullets(void);
		int SetPlayerBullets(int value);
		int AddPlayerBullets(int value);

		void CalcCameraRot(void);

		//���݃Q�[�����Ԃ̃Q�b�^�[�ƃZ�b�^�[
		int GetGameTime(void);
		int setGameTime(int time);
		int AddGameTime(int time);

	private:
		int playerState;//�v���C���[�̏��
		int gameState;//�Q�[���̏��

		cocos2d::Touch* touch;//�^�b�`�Ǘ�
		int touchState;//�^�b�`�̏��
		int touchFlag;//

		//�v���C���[�̃X�e�[�^�X
		int playerHP;//�v���C���[�̗̑�
		int nowBullets;//�v���C���[�̌��ݒe��

		cocos2d::Size screenSize;

		static cocos2d::Camera* camera2D;
		static cocos2d::Camera* camera3D;
		static cocos2d::Node* CamNode;

		//3D�J�����̃^�[�Q�b�g�ʒu���擾�ł��Ȃ��̂ŁA��������GameMaster.h�ɏ����܂�
		cocos2d::Vec3 camTarget;//3D�J�����̒����_
		//
		cocos2d::Vec2 setHidePoint(StagePoint stagePoint);

		GameMaster() {}
	};
}

#endif // __GAMEMASTER_H__
