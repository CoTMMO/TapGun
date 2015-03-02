
#ifndef __GAME_CLEAR_LAYER_H__
#define __GAME_CLEAR_LAYER_H__

#include "cocos2d.h"

namespace TapGun
{
	class GameClearLayer : public cocos2d::Layer
	{
	public:
		bool init();

		void menuEndCallback( cocos2d::Ref* pSender);

		CREATE_FUNC( GameClearLayer);

	private:
	};
}

#endif // __GAME_CLEAR_LAYER_H__
