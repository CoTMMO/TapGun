#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"


namespace TapGun
{
	/* Scene Enumration */
	typedef enum
	{
		GameLayerTag,
		UILayerTag
	} GameSceneTag;


	enum _GAME_STATE_
	{
		GSTATE_INIT,
		GSTATE_PAUSE,
		GSTATE_PLAY,
		GSTATE_NUM
	};
	
	class GameScene : public cocos2d::Layer
	{
	public:

		//�����o�ϐ�

		//==�t���O�n==
		int game_state;//�Q�[���̌��݃X�e�[�g


		//==�ϐ��n==
		cocos2d::Vec2 touch_pos;//���^�b�`���W�i���݂̓V���O���^�b�`�̂݁j
		int playerNum;//Unit�z��̃v���C���[�ԍ��i0�ŌŒ肵�Ă��悢�H�j

		//�����I�ɂ͊Ǘ��N���X�̃����o�ϐ��Ɋ܂߂܂�
		int stage_num;//���݃X�e�[�W�ԍ��i���������ɗp����j
		int player_state;//

		//==�����o�֐�==
		static cocos2d::Scene* CreateScene();
		virtual bool init();//�V�[���N���G�C�g���̏������֐��i�����I�ɌĂяo�����j

		void update(float delta);//�X�V�֐��i���݂͎g�p���Ă��Ȃ��j
		void moveTime(float delta);//�w��t���[�����Ƃ̍X�V�֐��i��������g�p����j

		int InitCamera(void);//�J�������W�X�V�֐��i�q���C���[��������Ɏg�p����j
		int UpdateCamera(void);//�J�������W�̍X�V�i�q���C���[�N���G�C�g��Ɏg�p����j


		//�e�탌�C���[�̏������ɗp����
		//GameLayer* gameLayer();//�Q�[�����C���[�N���X���g�p����
		//UILayer * uiLayer();//�t�h���C���[���g�p����

		//static GameScene* sharedLayer();

		//�^�b�`�C�x���g�̃��X�i�[
		cocos2d::EventListenerTouchOneByOne *listener;
		bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
		void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
		void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

	private:

		//void UpdatePlayer( void);
		//void UpdateEnemy( void);

		//void menuCloseCallback(CCObject* pSender);

		CREATE_FUNC(GameScene);
	};

}

#endif // __GAME_SCENE_H__
