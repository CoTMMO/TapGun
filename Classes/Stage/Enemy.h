
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"

#include "Base/Sprite3D.h"


namespace TapGun
{
//���̒�`�͍���قȂ�w�b�_�t�@�C���Ɉڂ��ւ��܂�
#define FALSE -1
#define TRUE 1

	//�G�l�~�[�̍U���^�C�v
	enum _ENEMY_ATK_
	{
		AI_ATK_NONE = 0,//�U�����[�V�������s��Ȃ�
		AI_ATK_FAKE = -1,//�Њd�U�����s��
		AI_ATK_SSHOT = 7,//�����������s��
		AI_ATK_SLIDE = 4,//�T�C�h�W�����v�ˌ�
		AI_ATK_ACRO = 5,//�A�N���o�e�B�b�N
	};

	//�G�l�~�[�̏o�����@
	enum _ENEMY_APPEAR_
	{
		AI_APPEAR_RUN = 1,//����Ȃ���o��
		AI_APPEAR_WALK = 2,//����
		AI_APPEAR_SWALK = 3,//������
		AI_APPEAR_SLIDE = 4,//�T�C�h�W�����v
		AI_APPEAR_FJUMP = 6,//�O�W�����v
		AI_APPEAR_ACRO = 5,//�A�N���o�e�B�b�N
	};

	//�G�l�~�[�̈ړ����@
	enum _ENEMY_MOVE_
	{
		AI_MOVE_NONE = 0,//�����Ȃ�
		AI_MOVE_RUN = 1,//����Ȃ���o��
		AI_MOVE_WALK = 2,//����
		AI_MOVE_SWALK = 3,//������
		AI_MOVE_SLIDE = 4,//�T�C�h�W�����v
		AI_MOVE_ACRO = 5,//�A�N���o�e�B�b�N
	};

	//�G�l�~�[�̃��C�t�T�C�N��
	enum _ENEMY_LIFE_
	{
		AI_LIFE_ONCE = 11,//�A�N�V�����͈�x�̂�
		AI_LIFE_SWITCH = 12,//2�_�Ԃ��ړ�����
		AI_LIFE_STOP = 13,//�ŏI�n�_�łƂ܂�
	};

	class Enemy
	{
	public:
		cocos2d::Vec3 standbyPos;//
		cocos2d::Vec3 targetPos[3];

		int SetAI(int appear, int atk, int move, int life);
		void SetNextEnemy(int num1, int num2, int num3);
		void SetTargetPos(cocos2d::Vec3 pos[3]);
		int number;
		int kind;//�G�̎��
		int alive;//�����`�F�b�N
		int nextEnemiesNum[3];//���̓G���������Ƃ��ɁA���ɏo�Ă���G
		int finishFlag;//���̓G��������Ɓi�|���A�܂��͓�����j�ƃE�F�[�u�I���iTRUE/FALSE�j

		int hitpoint;
		//
		int AIappear;//�o���p�^�[��
		int AIAtk;
		int AIMove;
		int AILife;

		int nowShot;//���݂̃V���b�g��
		int maxShot;//�A�ˉ�
		//float atkFrame;//�U���Ԋu�i�b�j
		//float waitFrame;//�o�����Ă���ړ����n�߂�܂ł̎���
		float sleepTime;//
		float stsAtkFrame[3];//�U���Ԋu�i�b�j
		float stsWaitFrame;//�o�����Ă���ړ����n�߂�܂ł̎���

	private:

	};
}

#endif // __ENEMY_H__
