#include "GameModelsLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)


#include "GameMaster.h"
#include "Sprite3D.h"


#else

#include "C++/Base/Sprite3D.h"
#include "C++/Base/GameMaster.h"
#include "C++/Scene/GameModelsLayer.h"

#endif

#define COCOS_TMP//cocos2d��Sprite3d���g�p����ۂɒ�`����
#define RAY_TEST

USING_NS_CC;
using namespace TapGun;

GameMaster* GameParamObj;

/**
*	�Q�[���{�҂̃��f�����C���[�̏�����
*
*	@author	sasebon
*	@param	�Ȃ�
*	@return	�����������^�s��bool�l
*	@date	1/8 Ver 1.0
*/
bool GameModelsLayer::init()
{
	if(!Layer::init())
	{
		return false;
	}
	auto dispatcher = Director::getInstance()->getEventDispatcher();

	GameParamObj = GameMaster::GetInstance();//�Q�[���p�����[�^�N���X�̏�����
	return true;
}


/**
*	�Q�[���{�҂�UI���C���[�̊e�퐔�l������
*
*	@author	sasebon
*	@param	�Ȃ�
*	@return	�����������^�s��bool�l
*	@date	1/8 Ver 1.0
*/
int GameModelsLayer::InitLayer(void)
{
	InitAllModels();

	playerNum = -1;
	playerNum = InitPlayer(0);//�Ƃ肠��������0
	//InitMap(0);//���K�̃}�b�v�f�[�^���~��Ă���܂ł�������ۗ����܂�
	InitEnemy(0);

	return playerNum;
}

/**
*	�S���f��������
*
*	@author	sasebon
*	@param	�Ȃ�
*	@return	�Ȃ�
*	@date	1/8 Ver 1.0
*/
void GameModelsLayer::InitAllModels()
{
	for(int i = 0; i < MAX_UNIT; i++)
	{
		unit[i].Init();//�����o�ϐ�������
	}
}



/**
*	�v���C���[������
*
*	@author	sasebon
*	@param	�X�e�[�W�ԍ�
*	@return	����:Unit�z��̃v���C���[�ԍ� ���������s:-1
*	@date	1/8 Ver 1.0
*/
int GameModelsLayer::InitPlayer(int stage_num)
{
	int num = SearchFreeUnit();
	if(FALSE == num)
	{
		return FALSE;
	}
	unit[num].Init();//�����o�ϐ��̏����������Ă���

#ifdef	COCOS_TMP
	std::string fileName1 = "Graph/Models/mot_player_hide shot.c3t";
	std::string fileName2 = "Graph/Textures/box_tex.png";

	unit[num].sprite3d = cocos2d::Sprite3D::create(fileName1, fileName2);
#else
	std::string fileName1 = "mot_player_hide shot";
	std::string fileName2 = "box_tex.png";

	unit[num].sprite3d = TapGun::Sprite3D::create(fileName1, fileName2);
#endif
	unit[num].Init(num, UKIND_PLAYER1);
	playerNum = num;
	unit[num].wrapper = Node::create();//���f���̐e�m�[�h
	unit[num].wrapper->addChild(unit[num].sprite3d);//�e�m�[�h�ɃX�v���C�g��R�t��
	addChild(unit[num].wrapper);//�e�m�[�h�����C���[�ɕR�t��

	switch(stage_num)
	{
	case 0:

		unit[num].sprite3d->setScale(1.0f);
		unit[num].sprite3d->setRotation3D(Vec3(0.0f, 180.0f, 0.0f));//�v���C���[�͔��΂�����
		unit[num].sprite3d->setPosition3D(Vec3(2.0f, 0.0f, -8.5f));

		break;
	defalult:
		return FALSE;
		break;
	}


	//�����蔻��̒�`�i���j
	unit[num].collisionPos = Vec3(1.2, 3.0, 1.2);
	unit[num].SetCollision();

	//�A�j���[�V�����ǂݍ���
	//auto animation = Animation3D::create(fileName1);
	//auto animate = Animate3D::create(animation);
	//animate->setSpeed(1);

	//unit[num].sprite3d->runAction(RepeatForever::create(animate));

	return num;
}




/**
*	�G�l�~�[������
*
*	@author	sasebon
*	@param	�X�e�[�W�ԍ�
*	@return	����:1 ���������s:-1
*	@date	1/8 Ver 1.0
*/
int GameModelsLayer::InitEnemy(int stage_num)
{
	//���X�e�[�W�X�N���v�g�̓��e��ǂݍ���ŏ��������s���܂�

	int num = -1;

	switch(stage_num)
	{
	case 0:
		//�e�X�g�G�l�~�[�̓ǂݍ��݁F�P
		num = SearchFreeUnit();
		if(-1 == num)
		{
			return false;
		}

#ifdef	COCOS_TMP
		std::string fileName1 = "Graph/Models/mot_enemy_dei1.c3t";
		std::string fileName2 = "Graph/Textures/tex_boy.png";

		unit[num].sprite3d = cocos2d::Sprite3D::create(fileName1, fileName2);
#else
		std::string fileName1 = "mot_enemy_dei1";
		std::string fileName2 = "tex_boy.png";

		unit[num].sprite3d = TapGun::Sprite3D::create(fileName1, fileName2);
#endif
		unit[num].Init(num, UKIND_ENEMY);

		unit[num].wrapper = Node::create();//���f���̐e�m�[�h
		unit[num].wrapper->addChild(unit[num].sprite3d);
		addChild(unit[num].wrapper);

		unit[num].sprite3d->setScale(1.0f);
		unit[num].sprite3d->setPosition3D(Vec3(4.0f, 0.0f, -18.5f));

		//�����蔻��̒�`�i���j
		unit[num].collisionPos = Vec3(0.5, 0.5, 0.5);
		unit[num].SetCollision();



		//�A�j���[�V�����ǂݍ���
		{
			//auto animation = Animation3D::create("Graph/Models/mot_enemy_dei1_mot.c3b");
			//auto animate = Animate3D::create(animation);
			//animate->setSpeed(1);
			//unit[num].sprite3d->runAction(RepeatForever::create(animate));
		}


		//�e�X�g�G�l�~�[�̓ǂݍ��݁F�Q

		num = SearchFreeUnit();
		if(-1 == num)
		{
			return false;
		}
		unit[num].Init();//�����o�ϐ��̏����������Ă���

#ifdef	COCOS_TMP
		fileName1 = "Graph/Models/mot_enemy_dei1.c3t";
		fileName2 = "Graph/Textures/tex_boy.png";

		unit[num].sprite3d = cocos2d::Sprite3D::create(fileName1, fileName2);
#else
		fileName1 = "mot_enemy_dei1";
		fileName2 = "tex_boy.png";

		unit[num].sprite3d = ::Sprite3D::create(fileName1, fileName2);
#endif
		unit[num].Init(num, UKIND_ENEMY);

		unit[num].wrapper = Node::create();//���f���̐e�m�[�h
		unit[num].wrapper->addChild(unit[num].sprite3d);
		addChild(unit[num].wrapper);

		unit[num].sprite3d->setScale(1.0f);
		unit[num].sprite3d->setPosition3D(Vec3(3.0f, 0.0f, -60.5f));

		//�����蔻��̒�`�i���j
		unit[num].collisionPos = Vec3(0.5, 0.5, 0.5);
		unit[num].SetCollision();


		//�A�j���[�V�����ǂݍ���
		{
			//auto animation2 = Animation3D::create("Graph/Models/mot_enemy_dei1_mot.c3b");
			//auto animate2 = Animate3D::create(animation2);
			//animate2->setSpeed(1);
			//unit[num].sprite3d->runAction(RepeatForever::create(animate2));
		}

		break;
	defalut:
		break;
	}



	return TRUE;
}


/**
*	�X�e�[�W�I�u�W�F�N�g�̏�����
*
*	@author	sasebon
*	@param	�X�e�[�W�ԍ�
*	@return	����:1 ���������s:-1
*	@date	1/8 Ver 1.0
*/
int GameModelsLayer::InitMap(int stage_num)
{

	int num = SearchFreeUnit();
	if(-1 == num)
	{
		return false;
	}
	unit[num].Init();//�����o�ϐ��̏����������Ă���

#ifdef	COCOS_TMP
	std::string fileName1 = "Graph/Models/test_tenq.c3t";
	std::string fileName2 = "Graph/Textures/backGrund.png";

	unit[num].sprite3d = cocos2d::Sprite3D::create(fileName1, fileName2);
#else
	std::string fileName1 = "test_tenq";
	std::string fileName2 = "backGrund.png";

	unit[num].sprite3d = ::Sprite3D::create(fileName1, fileName2);
#endif
	unit[num].Init(num, UKIND_MAP);

	unit[num].wrapper = Node::create();//���f���̐e�m�[�h
	unit[num].wrapper->addChild(unit[num].sprite3d);
	addChild(unit[num].wrapper);
	//addChild(unit[num].sprite3d)map

	unit[num].sprite3d->setScale(1.0f);
	unit[num].sprite3d->setRotation3D(Vec3(0.0f, 0.0f, 0.0f));
	unit[num].sprite3d->setPosition3D(Vec3(0.0f, -5.5f, 0.0f));

	return TRUE;
}



void GameModelsLayer::UpdateModels()
{
	//float yrot = 1.0f;
	//Vec3 tmpVec3 = unit[1].sprite3d->getRotation3D();
	//tmpVec3.y += yrot;
	//unit[1].sprite3d->setRotation3D(tmpVec3);
}


void GameModelsLayer::UpdateLayer()
{
	UpdatePlayer();//�v���C���[�̍X�V
	UpdateEnemy();//�G�l�~�[�̍X�V
	UpdateBullets();//�G�e�̍X�V
	CheckHit();//�����蔻��̃`�F�b�N
}

void GameModelsLayer::UpdatePlayer()
{
	//�^�b�`���W�����ƂɍU�������̏������s��

	//�v���C���[���U���\�ȏꍇ�A�U���͈͂̍��W���^�b�`������U�����s��
}



void GameModelsLayer::UpdateEnemy()
{
	for(int i = 0; i < MAX_UNIT; i++)
	{
		if(-1 != unit[i].valid && UKIND_ENEMY == unit[i].kind)
		{
			unit[i].frame += 1;
			if(120 <= unit[i].frame)
			{
				ShootBullet(i);//i�Ԃ̃G�l�~�[���e�𔭎˂���
				unit[i].frame = 0;
			}
		}
	}
}



/*

*/
void GameModelsLayer::ShootBullet(int enemy_num)
{
	int num = SearchFreeUnit();
	if(FALSE != num)
	{
#ifdef	COCOS_TMP
		std::string fileName1 = "Graph/Models/tama.c3t";
		std::string fileName2 = "Graph/Textures/tama.png";

		unit[num].sprite3d = cocos2d::Sprite3D::create(fileName1, fileName2);
#else
		std::string fileName1 = "Graph/Models/mot_enemy_dei1.c3t";
		std::string fileName2 = "Graph/Textures/tex_boy.png";

		unit[num].sprite3d = TapGun::Sprite3D::create("tama", "tama.png");
#endif
		unit[num].Init(num, UKIND_EBULLET);

		unit[num].wrapper = Node::create();//���f���̐e�m�[�h
		unit[num].wrapper->addChild(unit[num].sprite3d);
		addChild(unit[num].wrapper);


		unit[num].sprite3d->setScale(0.02f);

		////�����蔻��̒�`�i���j
		//unit[num].aabbBody = unit[num].sprite3d->getAABB();
		//unit[num].aabbBody.set(Vec3(-0.3, -0.3, -0.3), Vec3(0.3, 0.3, 0.3));
		//unit[num].obbHead = OBB(unit[num].aabbBody);//
		////unit[num].obbHead.set();
		unit[num].collisionPos = Vec3(0.2, 0.2, 0.2);//�����蔻���`�̑傫����ݒ�
		unit[num].SetCollision();//�����蔻����Z�b�g

		//�e���������G�l�~�[�̍��W�ƁA�v���C���[�̍��W�����ɁA�e�̈ړ����������߂�
		Vec3 enemy_pos = unit[enemy_num].sprite3d->getPosition3D();
		Vec3 player_pos = unit[playerNum].sprite3d->getPosition3D();

		//unit[num].speed_vec = Vec3(enemy_pos.x, 0, 0);
		unit[num].speedVec = player_pos - enemy_pos;//���̕��@����������Ύg�p����

		//�x�N�g���̐��K�����s��
		float vx = unit[num].speedVec.x;
		float vz = unit[num].speedVec.z;

		float dist = sqrtf(vx * vx + vz * vz);//�񎟌��I�ȋ���
		vx = vx / dist;
		vz = vz / dist;

		//���K�����I�������A���x�������ĕ����x�N�g���̌v�Z�I��
		unit[num].speed = 0.8f;
		unit[num].speedVec.x = vx * unit[num].speed;
		unit[num].speedVec.z = vz * unit[num].speed;
		unit[num].speedVec.y = 0;//y�͍��̂Ƃ���0�ň���

		unit[num].sprite3d->setPosition3D(enemy_pos);
		unit[num].sprite3d->setPositionY(1.2f);
	}
}


/*

*/
void GameModelsLayer::UpdateBullets()
{
	//�S�Ă̓G�e���j�b�g���X�V
	for(int i = 0; i < MAX_UNIT; i++)
	{
		if(FALSE != unit[i].valid && UKIND_EBULLET == unit[i].kind)
		{
			unit[i].UpdatePos();//���W�ƈꏏ�ɓ����蔻����ړ�
			unit[playerNum].UpdatePos();//�����蔻��X�V

			//unit[i].sprite3d->getBoundingBox();



			//�v���C���[�Ƃ̓����蔻�������
			if(unit[playerNum].obbHead.intersects(unit[i].obbHead))
			{
				unit[i].sprite3d->setVisible(false);
			}
		}
	}
}



/**
*	�����蔻��̏���
*
*	@author	sasebon
*	@param	�Ȃ�
*	@return	�Ȃ�
*	@date	1/19 Ver 1.0
*/
void  GameModelsLayer::CheckHit(void)
{
	//========================================================
	//�v���C���[�U�������̉�����
	//�^�b�`���W�i�X�N���[�����W�j���烏�[���h���W�����߁A���C���΂�


#ifdef RAY_TEST

	/*@*/
	//���C�ƓG�̓����蔻�菈��
	const int pstate = GameParamObj->GetPlayerState();
	if(pstate == PSTATE_SHOT)
	{
		//���ӁF�G���d�Ȃ��đ��݂���ꍇ�ɔ����A�y�\�[�g�Ȃǂ̕��ёւ����s���K�v�����肻���ł�


		auto s = Director::getInstance()->getWinSize();//�E�B���h�E�T�C�Y���擾
		Vec3 rayStart = Vec3(0, 0, 0);
		Vec3 rayEnd = Vec3(0, 0, 0);
		Vec2 tPos = GameParamObj->GetTouchPos();//�^�b�`���W���擾

		Vec3 tmp_touch_pos = Vec3(tPos.x, tPos.y, -1.0f);//-1.0f == ������̋ߖʁinear plane�j

		Camera* cam3d = GameParamObj->GetCamera3D();
		cam3d->unproject(s, &tmp_touch_pos, &rayStart);//near plane��3�������W���擾
		rayStart = cam3d->getPosition3D();

		tmp_touch_pos.z = 1.0f;//1.0f == ������̉��ʁifar plane�j
		cam3d->unproject(s, &tmp_touch_pos, &rayEnd);//far plane��3�������W���擾

		Ray touch_ray;//�����C��ݒ�
		touch_ray._origin = rayStart;
		touch_ray._direction = rayEnd - rayStart;

		for(int i = 0; i < MAX_UNIT; i++)
		{
			if(-1 != unit[i].valid && UKIND_ENEMY == unit[i].kind)
			{
				if(touch_ray.intersects(unit[i].obbHead))//�^�b�`���W�̖@���ƓG�̓����蔻�肪�ڐG���������`�F�b�N
				{
					Vec3 rot = Vec3(0, 0, 0);
					rot = unit[i].sprite3d->getRotation3D();
					rot.y += 20.0f;
					unit[i].sprite3d->setRotation3D(rot);
				}
			}
		}
		GameParamObj->SetPlayerState(PSTATE_IDLE);
	}



	//	auto touch = item;
	//	auto location = touch->getLocationInView();
	//	if (_camera)
	//	{
	//		if (_sprite3D && _cameraType == CameraType::ThirdCamera && _bZoomOut == false && _bZoomIn == false && _bRotateLeft == false && _bRotateRight == false)
	//		{
	//			Vec3 nearP(location.x, location.y, -1.0f), farP(location.x, location.y, 1.0f);

	//			auto size = Director::getInstance()->getWinSize();
	//			_camera->unproject(size, &nearP, &nearP);
	//			_camera->unproject(size, &farP, &farP);
	//			Vec3 dir(farP - nearP);
	//			float dist = 0.0f;
	//			float ndd = Vec3::dot(Vec3(0, 1, 0), dir);
	//			if (ndd == 0)
	//				dist = 0.0f;
	//			float ndo = Vec3::dot(Vec3(0, 1, 0), nearP);
	//			dist = (0 - ndo) / ndd;
	//			Vec3 p = nearP + dist *  dir;

	//			if (p.x > 100)
	//				p.x = 100;
	//			if (p.x < -100)
	//				p.x = -100;
	//			if (p.z > 100)
	//				p.z = 100;
	//			if (p.z < -100)
	//				p.z = -100;

	//			_targetPos = p;
	//		}
	//}



#else

	/*@*/
	//���C�ƓG�̓����蔻�菈��
	const int pstate = GameParamObj->GetPlayerState();
	if (pstate == PSTATE_SHOT)
	{
		//���ӁF�G���d�Ȃ��đ��݂���ꍇ�ɔ����A�y�\�[�g�Ȃǂ̕��ёւ����s���K�v�����肻���ł�


		auto s = Director::getInstance()->getWinSize();//�E�B���h�E�T�C�Y���擾
		Vec3 rayStart = Vec3(0, 0, 0);
		Vec3 rayEnd = Vec3(0, 0, 0);
		Vec2 tPos = GameParamObj->GetTouchPos();//�^�b�`���W���擾

		Vec3 tmp_touch_pos = Vec3(tPos.x, tPos.y, -1.0f);//-1.0f == ������̋ߖʁinear plane�j

		Camera* cam3d = GameParamObj->GetCamera3D();
		cam3d->unproject(s, &tmp_touch_pos, &rayStart);//near plane��3�������W���擾
		rayStart = cam3d->getPosition3D();


		tmp_touch_pos.z = 1.0f;//1.0f == ������̉��ʁifar plane�j
		cam3d->unproject(s, &tmp_touch_pos, &rayEnd);//far plane��3�������W���擾

		Ray touch_ray;//�����C��ݒ�
		touch_ray._origin = rayStart;
		touch_ray._direction = rayEnd - rayStart;

		for (int i = 0; i < MAX_UNIT; i++)
		{
			if (-1 != unit[i].valid && UKIND_ENEMY == unit[i].kind)
			{
				if (touch_ray.intersects(unit[i].obbHead))//�^�b�`���W�̖@���ƓG�̓����蔻�肪�ڐG���������`�F�b�N
				{
					Vec3 rot = Vec3(0, 0, 0);
					rot = unit[i].sprite3d->getRotation3D();
					rot.y += 20.0f;
					unit[i].sprite3d->setRotation3D(rot);
				}
			}
		}
		GameParamObj->SetPlayerState(PSTATE_IDLE);
	}


#endif

	//========================================================
	//�G�̍U�������i�e�ƃv���C���[�̓����蔻��j
}


int GameModelsLayer::GetPlayerNum()
{
	return playerNum;
}

int GameModelsLayer::SearchFreeUnit()
{
	for(int i = 0; i < MAX_UNIT; i++)
	{
		if(FALSE == unit[i].valid)
		{
			return i;
		}
	}

	return -1;
}













/*
�w��t���[�����ƂɌĂяo�����
*/
void GameModelsLayer::moveTime(float delta)
{

}



void GameModelsLayer::update(float delta)
{

}