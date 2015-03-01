
#ifndef __CONTINUE_LAYER_H__
#define __CONTINUE_LAYER_H__

#include "cocos2d.h"

namespace TapGun
{
	class ContinueLayer : public cocos2d::Layer
	{
	public:
		bool init();
		void update( float delta);
		void moveTime( float delta);

		void menuStartCallback( cocos2d::Ref* pSender);
		void menuEndCallback( cocos2d::Ref* pSender);

		CREATE_FUNC( ContinueLayer);

	private:
		enum Menu
		{
			Restart = 0,
			End,
			MenuNum,

			BG = 0,
			Logo,
			SpriteNum
		};

		cocos2d::Menu* menu[MenuNum];
		cocos2d::Sprite* sprite[SpriteNum];
	};
}

#endif // __CONTINUE_LAYER_H__
