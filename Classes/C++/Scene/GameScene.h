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
		virtual bool init();

		int playerNum;//

		static cocos2d::Scene* CreateScene();

		void update(float delta);
		void moveTime(float delta);

		int InitCamera(void);
		int UpdateCamera(void);

		int stage_num;//���݃X�e�[�W�ԍ��i���������ɗp����j
		int game_state;//�Q�[���̃X�e�[�g

		//�e�탌�C���[�̏������ɗp����
		//GameLayer* gameLayer();//�Q�[�����C���[�N���X���g�p����
		//UILayer * uiLayer();//�t�h���C���[���g�p����

		//static GameScene* sharedLayer();
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
