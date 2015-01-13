#include "GameUILayer.h"

USING_NS_CC;
using namespace TapGun;

GameUILayer *gLayer_ui;
Scene *scene_ui;

/*
�C���N�������g
*/
//GameUILayer::UILayer()
//{
//	for(int i = 0; i < MAX_MODEL; i++)
//	{
//		Unit[MAX_MODEL].valid = 0;
//
//	}
//}


/*
 �g�p���Ă��Ȃ��̂ŁA����폜�\��
*/
Scene* GameUILayer::CreateScene()
{
	//Scene *scene = Scene::create();
	//UILayer *layer = GameUILayer::create();
	scene_ui = Scene::create();
	gLayer_ui = GameUILayer::create();
	scene_ui->addChild(gLayer_ui);
	return scene_ui;
}


/**
*	�Q�[���{�҂�UI���C���[��������
*
*	@author	sasebon
*	@param	�Ȃ�
*	@return	�����������^�s��bool�l
*	@date	1/8 Ver 1.0
*/
bool GameUILayer::init()
{
	if(!Layer::init())
	{
		return false;
	}

	auto dispatcher = Director::getInstance()->getEventDispatcher();


	this->scheduleUpdate();
	this->schedule(schedule_selector(GameUILayer::moveTime), 0.016f);

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
void GameUILayer::InitLayer(void)
{
	InitAllUI();
	SetUI();
}


int GameUILayer::SerchFreeUI()
{
	for(int i = 0; i < MAX_UI; i++)
	{
		if(-1 == valid[i])
		{
			return i;
		}
	}
	return -1;
}

void GameUILayer::SetUI()
{
	std::string fileName1 = "UI/ui_lifebar.png";

	//int num = SerchFreeUI();
	//if(-1 != num)
	//{
	//	valid[num] = 1;
	//	UISprite[num] = Sprite::create(fileName1);
	//	UISprite[num]->setPosition3D(Vec3(3, 2.1, -10.5));
	//	UISprite[num]->setScale(0.01f);
	//	addChild(UISprite[num]);
	//}

	int num = SerchFreeUI();

	//���C�t�o�[�̏�����
	if (-1 != num)
	{
		valid[num] = 1;
		UIBillBoard[num] = BillBoard::create(fileName1, BillBoard::Mode::VIEW_PLANE_ORIENTED);
		UIBillBoard[num]->setPosition3D(Vec3(-1.5, 0.3, -5.5));
		UIBillBoard[num]->setScale(0.001f);
		UINode[num] = Node::create();//���f���̐e�m�[�h
		UINode[num]->addChild(UIBillBoard[num]);

		addChild(UINode[num]);
	}
}


void GameUILayer::InitAllUI()
{
	for(int i = 0; i < MAX_UI; i++)
	{
		valid[i] = -1;
	}
}


void GameUILayer::UpdateUI(float camX, float camY, float camZ,float rotX,float rotY,float rotZ)
{
	Vec3 camPos;
	camPos.x = camX;
	camPos.y = camY;
	camPos.z = camZ;

	Vec3 camRot;
	camRot.x = rotX;
	camRot.y = rotY;
	camRot.z = rotZ;


	for(int i = 0; i < MAX_UI; i++)
	{
		if(-1 != valid[i])
		{
			UINode[i]->setPosition3D(camPos);
			UINode[i]->setRotation3D(camRot);
		}
	}
}






/*
 ���݂͐e�V�[����update�ōX�V�n�̊֐����Ăяo���Ă���̂ŁA���C���[�ŗL��moveTime�֐��͎g�p���Ă��܂���
*/
void GameUILayer::moveTime(float delta)
{

}


/*
���݂͐e�V�[����update�ōX�V�n�̊֐����Ăяo���Ă���̂ŁA���C���[�ŗL��Update�֐��͎g�p���Ă��܂���
*/
void GameUILayer::update(float delta)
{

}




bool GameUILayer::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{


	return true;
}


void GameUILayer::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	Director *pDirector;
	Point touchPoint;
	Node *pNode;
	Rect spRect;

}


void GameUILayer::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{

}


//void GameUILayer::onTouchCancelled = CC_CALLBACK_2(GameUILayer::onTouchCancelled, this);
//{
//
//	//��ʂ��^�b�`�������̏���
//
//}