#include"Unit.h"
using namespace TapGun;
USING_NS_CC;


/**
*	Unit�N���X�̃����o�ϐ���������
*
*	@author	sasebon
*	@param	�Ȃ�
*	@return	�Ȃ�
*	@date	1/8 Ver 1.0
*/
void Unit::Init( void)
{
	//���������e�͐����X�V���܂�

	//�Ǘ��t���O
	valid = FALSE;
	kind = UKIND_NONE;//Unit��ʂ�NONE��

	//�ϐ�
	pos = Vec3(0, 0, 0);//���f�����W
	speed = 0.0f;//�ړ����x
	speedVec = Vec3(0, 0, 0);//�ړ��x�N�g��
	targetPos = Vec3(0, 0, 0);//�ړ��ڕW
	collisionPos = Vec3(0, 0, 0);//�����蔻��iOBB�j�̊e��

	frame = 0;//�Ǘ��t���[��

}



/**
*	Unit�N���X�̃����o�ϐ���������
*
*	@author	sasebon
*	@param	Unit�z��̔ԍ�,Unit���
*	@return	����I��:1 �������G���[:-1
*	@date	1/8 Ver 1.0
*/
int Unit::Init(int num, int utype)
{
	//���������e�͐����X�V���܂�

	//num�Ԃ�Unit�\���̂��g�p����Ă��邩����������Ă��Ȃ��A�܂���Unit��ʂ��s���A�܂��͔z��O�̏ꍇ�̓G���[
	if(FALSE != valid || 0 > utype || UKIND_NUM <= utype || 0 > num || MAX_UNIT <= num)
	{
		return FALSE;
	}

	//�t���O�̏�����
	valid = TRUE;
	kind = utype;

	//�ϐ�
	pos = Vec3(0, 0, 0);
	speed = 0.0f;
	speedVec = Vec3(0, 0, 0);
	targetPos = Vec3(0, 0, 0);

	frame = 0;//�Ǘ��t���[��

	//���f���̎�ʂɂ���ē����蔻��̐ݒ���s��
	//�G��e�̎�ށi����ʁj�ɂ���Ă���ɏ�����������
	switch(utype)
	{
	case UKIND_ENEMY://�G�l�~�[

		break;
	case UKIND_EBULLET://�G�e


		break;
	default:

		break;
	}


	//�K�v�Ȃ�΃��f����A�j���[�V�����֘A�̃f�[�^������������
}




/**
*	�����蔻��̏�����
*
*	@author	sasebon
*	@param	�Ȃ�
*	@return	�Ȃ�
*	@date	1/8 Ver 1.0
*/
void Unit::SetCollision(void)
{
	//�����蔻��̒�`�i���j
	aabbBody = sprite3d->getAABB();

	//�����蔻��̈ړ�
	Vec3 collision_center = sprite3d->getPosition3D();

	Vec3 collision_min = collision_center - collisionPos * 0.5f;
	Vec3 collision_max = collision_center + collisionPos * 0.5f;

	aabbBody.set(collision_min, collision_max);
	obbHead = OBB(aabbBody);//
}




/**
*	���x�����Ƃɂ���Unit�ړ�
*
*	@author	sasebon
*	@param	�Ȃ�
*	@return	�Ȃ�
*	@date	1/8 Ver 1.0
*/
void Unit::UpdatePos(void)
{
	pos = sprite3d->getPosition3D();
	pos += speedVec;
	sprite3d->setPosition3D(pos);

	//�����蔻��̈ړ�
	Vec3 collision_min = pos - collisionPos / 2;
	Vec3 collision_max = pos + collisionPos / 2;

	aabbBody.set(collision_min, collision_max);
	obbHead = OBB(aabbBody);//
}



/**
*	�������W�ւ�Unit�ړ�
*
*	@author	sasebon
*	@param	���W
*	@return	�Ȃ�
*	@date	1/8 Ver 1.0
*/
void Unit::UpdatePos(Vec3 pos_vec)
{
	pos = pos_vec;
	sprite3d->setPosition3D(pos_vec);
}
