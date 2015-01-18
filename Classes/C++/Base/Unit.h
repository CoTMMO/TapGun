#ifndef __UNIT_H__
#define __UNIT_H__

#include "cocos2d.h"

#define MAX_UNIT 100

//using namespace cocos2d;

namespace TapGun
{

//���̒�`�͍���قȂ�w�b�_�t�@�C���Ɉڂ��ւ��܂�
#define FALSE -1
#define TRUE 1


	enum _UNIT_KIND_
	{
		UKIND_NONE,//���g�p
		UKIND_PLAYER1,//�v���C���[1
		UKIND_PLAYER2,//�v���C���[2
		UKIND_ENEMY,//�G�l�~�[
		UKIND_EBULLET,//�G�e
		UKIND_MAP,//�}�b�v���f�������݂�Unit�Ɋ܂߂Ă��܂�
		UKIND_NUM
	};



	//Unit�N���X
	//�v���C���[�E�G�l�~�[�E�e�E�}�b�v�f�[�^�ɗp���܂�
	class Unit
	{
	public:

		//�����o�ϐ�

		//==�t���O�n==
		int valid;//Unit�g�p�t���O�iTRUE/FALSE�j
		int kind;//Unit��ʁi_UNIT_KIND_�j

		//==�ϐ�==
		cocos2d::Vec3 pos;//���f�����W�E�E�E�폜�\��isprite3d�̍��W���g�p�j
		float speed;//�ړ����x�i�ړ��ʁj
		cocos2d::Vec3 speedVec;//�ړ��x�N�g���ispeed�����ƂɌv�Z����j
		cocos2d::Vec3 targetPos;//�ړ����̖ڕW���W�i���݂͎��L������e�̈ړ��ɗp���Ă��܂����A���ʂƔ��f����΍���폜���܂��j
		int frame;//Unit�ŗL�t���[��
		cocos2d::Vec3 collisionPos;//OBB�̕ӂ̒����i���݂͂P�̂ݒ�`�j

		//==�����蔻��֘A�N���X(��)==
		cocos2d::AABB aabbHead;//
		cocos2d::AABB aabbBody;//
		cocos2d::OBB obbHead;

		//==���f���E�A�j���[�V�����֘A�N���X==
		cocos2d::Sprite3D* sprite3d;//
		cocos2d::Node* wrapper;//���f���̐e�m�[�h�i���f������W�Ƃ��Ďg�p����B�ʏ��(0,0,0)���W�j
		cocos2d::Animation3D* animation;
		cocos2d::Animate3D* animate;

		//�����o�֐�
		void Init(void);//���l�̏������i����̓R���X�g���N�^�ɒu��������H�j
		int Init(int num, int utype);//���l�̏�����

		void SetCollision(void);//�����蔻���������

		void UpdatePos(void);//���x�����Ƃɍ��W�ړ��Ɠ����蔻��ړ�
		void UpdatePos(cocos2d::Vec3 pos);//�����̍��W�Ɉړ�


		//Character();
		//~Character();
	private:
	};
}
#endif //__UNIT_H__
