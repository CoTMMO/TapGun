
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
		AI_ATK_NONE,//�U�����[�V�������s��Ȃ�
		AI_ATK_FAKE,//�Њd�U�����s��
		AI_ATK_SSHOT,//�����������s��
		AI_ATK_SJUMP,//�T�C�h�W�����v�ˌ�
		AI_ATK_ACRO,//�A�N���o�e�B�b�N
		AI_ATK_NUM
	};

	//�G�l�~�[�̏o�����@
	enum _ENEMY_APPEAR_
	{
		AI_APPEAR_RUN,//����Ȃ���o��
		AI_APPEAR_WALK,//����
		AI_APPEAR_SWALK,//������
		AI_APPEAR_SLIDE,//�T�C�h�W�����v
		AI_APPEAR_FJUMP,//�O�W�����v
		AI_APPEAR_ACRO,//�A�N���o�e�B�b�N
		AI_APPEAR_NUM
	};

	//�G�l�~�[�̈ړ����@
	enum _ENEMY_MOVE_
	{
		AI_MOVE_NONE,//�����Ȃ�
		AI_MOVE_RUN,//����Ȃ���o��
		AI_MOVE_WALK,//����
		AI_MOVE_SWALK,//������
		AI_MOVE_SJUMP,//�����Ȃ�
		AI_MOVE_ACRO,//�A�N���o�e�B�b�N
		AI_MOVE_NUM
	};

	//�G�l�~�[�̃��C�t�T�C�N��
	enum _ENEMY_LIFE_
	{
		AI_LIFE_ONCE,//�A�N�V�����͈�x�̂�
		AI_LIFE_SWITCH,//2�_�Ԃ��ړ�����
		AI_LIFE_STOP,//�ŏI�n�_�łƂ܂�
		AI_LIFE_NUM,
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
