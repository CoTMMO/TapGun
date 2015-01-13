#include "GameModelsLayer.h"

#define PI 3.1415926

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)



#else




#endif

USING_NS_CC;
using namespace TapGun;

static GameModelsLayer *multiSceneLayerInstance;

#define A

/*
�C���N�������g
*/
//GameModelsLayer::GameLayer()
//{
//	for(int i = 0; i < MAX_MODEL; i++)
//	{
//		unit[MAX_MODEL].valid = 0;
//
//	}
//}

Scene* GameModelsLayer::CreateScene()
{
	Scene *scene = Scene::create();
	GameModelsLayer *layer = GameModelsLayer::create();
	scene->addChild(layer);
	return scene;
}


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
	multiSceneLayerInstance = this;

	auto dispatcher = Director::getInstance()->getEventDispatcher();

	//auto listener = EventListenerTouchOneByOne::create();

	/*
	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameModelsLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameModelsLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameModelsLayer::onTouchEnded, this);

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	*/

	//this->scheduleUpdate();
	//this->schedule(schedule_selector(GameModelsLayer::moveTime), 0.016f);

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
	for(int i = 0; i < MAX_MODEL; i++)
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
	std::string fileName1 = "mot_player_hide shot";
	std::string fileName2 = "box_tex.png";

	int num = SearchFreeUnit();
	if(FALSE == num)
	{
		return FALSE;
	}
	unit[num].Init();//�����o�ϐ��̏����������Ă���
	unit[num].sprite3d = Sprite3D::create(fileName1, fileName2);
	unit[num].Init(num,UKIND_PLAYER1);
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
	unit[num].collision_vec = Vec3(1.2, 3.0, 1.2);
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
	std::string fileName1 = "mot_enemy_dei1";
	std::string fileName2 = "tex_boy.png";

	switch(stage_num)
	{
	case 0:


		//�e�X�g�G�l�~�[�̓ǂݍ��݁F�P
		num = SearchFreeUnit();
		if(-1 == num)
		{
			return false;
		}

		unit[num].sprite3d = Sprite3D::create(fileName1, fileName2);
		unit[num].Init(num, UKIND_ENEMY);

		unit[num].wrapper = Node::create();//���f���̐e�m�[�h
		unit[num].wrapper->addChild(unit[num].sprite3d);
		addChild(unit[num].wrapper);

		unit[num].sprite3d->setScale(1.0f);
		unit[num].sprite3d->setPosition3D(Vec3(4.0f, 0.0f, -18.5f));

		unit[num].aabbBody = unit[num].sprite3d->getAABB();//


		//�A�j���[�V�����ǂݍ���
		{
			//auto animation = Animation3D::create(fileName1);
			//auto animate = Animate3D::create(animation);
			//animate->setSpeed(1);
			//unit[num].sprite3d->runAction(RepeatForever::create(animate));
		}


		//�e�X�g�G�l�~�[�̓ǂݍ��݁F�Q
		fileName1 = "mot_enemy_dei2";
		fileName2 = "tex_boy.png";

		num = SearchFreeUnit();
		if(-1 == num)
		{
			return false;
		}
		unit[num].Init();//�����o�ϐ��̏����������Ă���

		unit[num].sprite3d = Sprite3D::create(fileName1, fileName2);
		unit[num].Init(num, UKIND_ENEMY);

		unit[num].wrapper = Node::create();//���f���̐e�m�[�h
		unit[num].wrapper->addChild(unit[num].sprite3d);
		addChild(unit[num].wrapper);

		unit[num].sprite3d->setScale(1.0f);
		unit[num].sprite3d->setPosition3D(Vec3(3.0f, 0.0f, -23.5f));

		//�A�j���[�V�����ǂݍ���
		{
			//auto animation2 = Animation3D::create(fileName1);
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
	std::string fileName1 = "map.c3t";
	std::string fileName2 = "stage_tex.png";

	int num = SearchFreeUnit();
	if(-1 == num)
	{
		return false;
	}
	unit[num].Init();//�����o�ϐ��̏����������Ă���

#ifdef A//
	unit[num].sprite3d = Sprite3D::create(fileName1);
	unit[num].sprite3d->setTexture(fileName2);
#elif defined B//
	unit[num].sprite3d = Sprite3D::create(fileName1, fileName2);
#endif
	unit[num].valid = 1;
	unit[num].kind = UKIND_MAP;

	unit[num].wrapper = Node::create();//���f���̐e�m�[�h
	unit[num].wrapper->addChild(unit[num].sprite3d);
	addChild(unit[num].wrapper);
	//addChild(unit[num].sprite3d);

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



void GameModelsLayer::UpdatePlayer()
{
	//

}



void GameModelsLayer::UpdateEnemy()
{
	for(int i = 0; i < MAX_MODEL; i++)
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
		unit[num].sprite3d = Sprite3D::create("tama", "tama.png");
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
		unit[num].collision_vec = Vec3(0.2, 0.2, 0.2);//�����蔻���`�̑傫����ݒ�
		unit[num].SetCollision();//�����蔻����Z�b�g

		//�e���������G�l�~�[�̍��W�ƁA�v���C���[�̍��W�����ɁA�e�̈ړ����������߂�
		Vec3 enemy_pos = unit[enemy_num].sprite3d->getPosition3D();
		Vec3 player_pos = unit[playerNum].sprite3d->getPosition3D();

		//unit[num].speed_vec = Vec3(enemy_pos.x, 0, 0);
		unit[num].speed_vec = player_pos - enemy_pos;//���̕��@����������Ύg�p����

		//�x�N�g���̐��K�����s��
		float vx = unit[num].speed_vec.x;
		float vz = unit[num].speed_vec.z;

		float dist = sqrtf(vx * vx + vz * vz);//�񎟌��I�ȋ���
		vx = vx / dist;
		vz = vz / dist;

		//���K�����I�������A���x�������ĕ����x�N�g���̌v�Z�I��
		unit[num].speed = 0.8f;
		unit[num].speed_vec.x = vx * unit[num].speed;
		unit[num].speed_vec.z = vz * unit[num].speed;
		unit[num].speed_vec.y = 0;//y�͍��̂Ƃ���0�ň���

		unit[num].sprite3d->setPosition3D(enemy_pos);
		unit[num].sprite3d->setPositionY(1.2f);

	}
}


/*

*/
void GameModelsLayer::UpdateBullets()
{
	//�S�Ă̓G�e���j�b�g���X�V
	for(int i = 0; i < MAX_MODEL; i++)
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



int GameModelsLayer::GetPlayerNum()
{
	return playerNum;
}


int GameModelsLayer::SearchFreeUnit()
{
	for(int i = 0; i < MAX_MODEL; i++)
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


/*
bool GameModelsLayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	return true;
}


void GameModelsLayer::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{

}



void GameModelsLayer::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{

}


//void GameModelsLayer::onTouchCancelled = CC_CALLBACK_2(GameModelsLayer::onTouchCancelled, this);
//{
//
//	//��ʂ��^�b�`�������̏���
//
//}

*/