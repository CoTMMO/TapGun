#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "cocos2d.h"

using namespace cocos2d;

namespace TapGun
{

	enum _CHAR_KIND_
	{
		CKIND_NONE,
		CKIND_PLAYER,
		CKIND_ENEMY,
		CKIND_EBULLET,//�G�e
		CKIND_MAP
	};


	class Character
	{
	public:

		int valid;
		int kind;
		int frame;//

		Sprite3D* sprite3d;//���f���̃��[�J�����W
		Node* wrapper;//���f���̐e�m�[�h�i���f������W�Ƃ��Ďg�p����B�ʏ��(0,0,0)���W�j
		Animation3D* animation;
		Animate3D* animate;
		Vec3 pos;//���f�����W
		Vec3 target_pos;//���f�����ړ�����Ƃ��̖ڕW���W�i���L������e�̈ړ��ɗp����j
		Vec3 speed_vec;//�ړ����x
		float speed;//

		void Init(void);//���l�̏������i����̓R���X�g���N�^�ɒu��������H�j

		//Character();
		//~Character();
	private:
	};
}
#endif //__CHARACTER_H__
