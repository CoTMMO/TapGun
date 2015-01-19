#include "GameScene.h"

//�e���C���[���Ǘ�����\�[�X�R�[�h���C���N���[�h���Ă���

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "GameModelsLayer.h"
#include "GameUILayer.h"
#include"GameMaster.h"

#else

#include "C++/Scene/GameModelsLayer.h"
#include "C++/Scene/GameUILayer.h"
#include "C++/Base/GameMaster.h"

#endif

USING_NS_CC;
using namespace TapGun;

/*
GameScene
�i�����I��GameScene�Ȃǂɖ��O��ύX����j
�Q�[���{�҂̍X�V�������s��
�����ŏ������ꂽ���e�����ɁA���f���f�[�^���������C���[�Ƃt�h���������C���[�ŕ`����s��
*/



GameMaster* GameParamObj2;//�Ƃ肠�������O��ς��邩���O��Ԃŋ�ʂ���

GameModelsLayer* gGameLayer;
GameUILayer* gUILayer;
 
static GameScene *multiSceneLayerInstance;
 


/**
*	�Q�[���Ǘ��V�[���N���G�C�g
*
*	@author	sasebon
*	@param	�Ȃ�
*	@return	�V�[���̃|�C���^
*	@date	1/8 Ver 1.0
*/
Scene* GameScene::CreateScene()
{
	Scene *scene = Scene::create();//GameScene�̃V�[�����쐬
	GameScene *layer = GameScene::create();//��L�V�[����

	scene->addChild(layer);

	return scene;
};


//�Q�[�����C���[�̏������֐�
//GameLayer* GameScene::gameLayer()
//{
//	cocos2d::Node* layer = this->getChildByTag(GameLayerTag);
//	return (GameLayer *)layer;
//}


//UILayer* GameScene::uiLayer()
//{
//	cocos2d::Node* layer = GameScene::sharedLayer()->getChildByTag(UILayerTag);
//	return (UILayer *)layer;
//}


/**
*	�Q�[���V�[��������
*
*	@author	sasebon
*	@param	�Ȃ�
*	@return	����:1 ���������s:-1
*	@date	1/8 Ver 1.0
*/
bool GameScene::init()
{
	//���C���[������
	if(!Layer::init())
	{
		return false;
	}

	//�Q�[�����C���[���쐬
	gGameLayer = GameModelsLayer::create();
	this->addChild(gGameLayer);


	//UI���C���[���쐬
	gUILayer = GameUILayer::create();
	this->addChild(gUILayer);


	GameParamObj2 = GameMaster::GetInstance();//�Q�[���p�����[�^�N���X�̃C���X�^���X����
	GameParamObj2->InitScreenSize();//�X�N���[���T�C�Y�̃Z�b�g
	GameParamObj2->InitParam();//�Q�[���p�����[�^�̏�����


	//���݂̓^�b�`�C�x���g�̃��X�i�[�������ɗp�ӂ��Ă��܂�
	auto dispatcher = Director::getInstance()->getEventDispatcher();

	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//setTouchMode(Touch::DispatchMode::ONE_BY_ONE);

	this->scheduleUpdate();
	this->schedule(schedule_selector(GameScene::moveTime), 0.016f);//1�b60F�ŃQ�[���X�V

	return true;
}



/**
*	�Q�[���V�[���̃J����������
*
*	@author	sasebon
*	@param	�Ȃ�
*	@return	����:1
*	@date	1/8 Ver 1.0
*/
int GameScene::InitCamera()
{

	auto s = Director::getInstance()->getWinSize();


	//2D�p�J�����̎���
	if(NULL != gUILayer)
	{
		GameParamObj2->InitCamera2D();//�J������������
		gUILayer->setCameraMask(CAMFLAG_DEFAULT);
		int a = gUILayer->getCameraMask();
		addChild(GameParamObj2->Get2DCamInstance());
	}

	//3D�p�J�����̎���
	if(NULL != gGameLayer)
	{
		GameParamObj2->InitCamera3D();//�J������������
		gGameLayer->setCameraMask(CAMFLAG_3D);
		int b = gGameLayer->getCameraMask();

		//�v���C���[�̍��W�擾�͂Ƃ肠�������̂悤�Ȍ`�ŋL�q���Ă��܂�
		Vec3 cameraPos = gGameLayer->unit[playerNum].sprite3d->getPosition3D();

		cameraPos.x += 0.8;// += 0.5f;
		cameraPos.y += 1.5f;// += 1.5f;
		cameraPos.z += 4.0f;// += 3.1f;

		GameParamObj2->SetCamera3DPos(cameraPos);
		GameParamObj2->SetCamera3DRot(Vec3(0.0f, 0.0f, 0.0f));
		addChild(GameParamObj2->Get3DCamInstance());//add camera to the scene

	}
	return TRUE;
}



//�X�V�n


/**
*	�w��t���[�����Ƃ̃Q�[���V�[���X�V
*
*	@author	sasebon
*	@param	�Ȃ�
*	@return	�Ȃ�
*	@date	1/8 Ver 1.0
*/
void GameScene::moveTime(float delta)
{

	switch (GameParamObj2->GetGameState())
	{

	case GSTATE_INIT://�q���C���[���̕ϐ����������s��

		if (NULL != gGameLayer)//���݂͎q���C���[���N���G�C�g���������m�F����
		{
			playerNum = gGameLayer->InitLayer();//
		}
		if (NULL != gUILayer)//���݂͎q���C���[���N���G�C�g���������m�F����
		{
			gUILayer->InitLayer();//
		}

		InitCamera();
		GameParamObj2->SetGameState(GSTATE_PLAY);
		break;

	case GSTATE_PLAY:
		if (NULL != gGameLayer)//���݂͏������`�F�b�N�m�F����
		{
			gGameLayer->UpdateLayer();//���C���[�̍X�V(���݂̓^�b�`���W�ƃJ�����\���̂������Ƃ��ēn���Ă��܂�)
		}
		if (NULL != gUILayer)//���݂͏������`�F�b�N�m�F����
		{

		}
		UpdateCamera();//���f���̈ړ������ƂɃJ�����ړ�

		break;

	}
}


/**
*	�Q�[���V�[���X�V�i���ݎg�p���Ă��܂���j
*
*	@author	sasebon
*	@param	�Ȃ�
*	@return	�Ȃ�
*	@date	1/8 Ver 1.0
*/
void GameScene::update(float delta)
{

}



/**
*	�Q�[���V�[���̃J�����X�V
*
*	@author	sasebon
*	@param	�Ȃ�
*	@return	�Ȃ�
*	@date	1/8 Ver 1.0
*/
int GameScene::UpdateCamera()
{
	if(NULL != gUILayer)
	{

	}

	if(NULL != gGameLayer)
	{
		static float rot;
		Camera* cm2 = GameParamObj2->GetCamera3D();
		Camera* cm = GameParamObj2->Get3DCamInstance();
		rot = -0.01f;
		//gGameLayer->setRotation3D(Vec3(0.0f, rot, 0.0f));
	}
	return TRUE;
}



bool GameScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	GameParamObj2->SetTouchPos(pTouch->getLocation());//�^�b�`���W���擾���ăZ�b�g

	GameParamObj2->SetPlayerState(PSTATE_SHOT);//�v���C���[�̏�Ԃ��u�ˌ��v��Ԃɂ���
	return true;
}


void GameScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{

	Director *pDirector;
	Point touchPoint;
	Rect spRect;

	static float rx;
	static float ry;
	static float rz;

	auto s = Director::getInstance()->getWinSize();//��ʃT�C�Y�擾
	//�^�b�`���W���擾����
	pDirector = Director::getInstance();
	touchPoint = pDirector->convertToGL(pTouch->getLocationInView());

	if(touchPoint.x >= s.width * 2 / 3 && (touchPoint.y >= s.height / 3 && touchPoint.y < s.height * 2 / 3))
	{
		//Vec3 tmpVec = camera3->getPosition3D();
		//tmpVec.y += 0.2f;
		//camera3->setPosition3D(tmpVec);
		//ry -= 1.0f;
		//if(NULL != gGameLayer)
		//{
		//	camera3->setRotation3D(Vec3(0.0f, ry, 0.0f));
		//}
		//if(NULL != gUILayer)
		//{
		//	camera2->setRotation3D(Vec3(0.0f, ry, 0.0f));
		//}
	}
	else if(touchPoint.x < s.width / 3 && (touchPoint.y >= s.height / 3 && touchPoint.y < s.height * 2 / 3))
	{
		//Vec3 tmpVec = camera3->getPosition3D();
		//tmpVec.y -= 0.2f;
		//camera3->setPosition3D(tmpVec);
		//ry += 1.0f;
		//if(NULL != gGameLayer)
		//{
		//	camera3->setRotation3D(Vec3(0.0f, ry, 0.0f));
		//}
		//if(NULL != gUILayer)
		//{
		//	camera2->setRotation3D(Vec3(0.0f, ry, 0.0f));
		//}
	}
	else if(touchPoint.y < s.height / 3 && (touchPoint.x >= s.width / 3 && touchPoint.x < s.width * 2 / 3))
	{
		//Vec3 tmpVec = camera3->getPosition3D();
		//tmpVec.z += 0.2f;
		//camera3->setPosition3D(tmpVec);
	}
	else if(touchPoint.y >= s.height * 2 / 3 && (touchPoint.x >= s.width / 3 && touchPoint.x < s.width * 2 / 3))
	{
		//Vec3 tmpVec = camera3->getPosition3D();
		//tmpVec.z -= 0.2f;
		//camera3->setPosition3D(tmpVec);
	}
}



void GameScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	
}

