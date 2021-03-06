
#ifndef __TEST_SCENE_H__
#define __TEST_SCENE_H__

#include "cocos2d.h"

namespace TapGun
{
	class Test : public cocos2d::Layer
	{
	public:
		static double frame;

		static cocos2d::Scene* createScene();

		virtual bool init();
		void update( float delta);
		void moveTime( float delta);
		
		//タッチイベントのリスナー
		cocos2d::EventListenerTouchOneByOne *listener;
		bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
		void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
		void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

		CREATE_FUNC(Test);

		cocos2d::BillBoard* billboard;
//		cocos2d::Sprite* sprite;
//		cocos2d::Animation* animation;
		
		void setEnemyHitEffect( cocos2d::Sprite3D* sprite3d);
		
	};
}

#endif // __TEST_SCENE_H__
