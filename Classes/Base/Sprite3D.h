
#ifndef __SPRITE3D_H__
#define __SPRITE3D_H__

#include "cocos2d.h"

namespace TapGun
{
	enum ResouceType
	{
		NoExt,
		Model,
		Anime,
		Error,
		Num,
	};

	class _Sprite3D : public cocos2d::Sprite3D
	{
	public:
		static _Sprite3D* create( const std::string& firstPath);
		static _Sprite3D* create( const std::string& firstPath, const std::string& secondPath);
		
		static void createAsync( const std::string& firstPath, const std::function<void(_Sprite3D*, void*)>& callback, void* callbackparam);
		static void createAsync( const std::string& firstPath, const std::string& secondPath, const std::function<void(_Sprite3D*, void*)>& callback, void* callbackparam);

		int startAnimation( const std::string& animeName);
		int startAnimation(const std::string& animeName, float startTime, float endTime);
		int startAnimationLoop( const std::string& animeName);
		int startAnimationLoop(const std::string& animeName, float startTime, float endTime);
		int startAnimationReverse( const std::string& animeName);
		int startAnimationReverse(const std::string& animeName, float startTime, float endTime);
		int startAnimationReverseLoop( const std::string& animeName);
		int startAnimationReverseLoop(const std::string& animeName, float startTime, float endTime);
		
		int stopAnimation( void);
		int stopALLAnimation( void);
		
		int setAnimationSpeed( float speed);

		int checkAnimationState( void);

		void releaseAnimation( void);

	CC_CONSTRUCTOR_ACCESS:
		_Sprite3D() {}

	protected:

	private:
		double time;
		cocos2d::Animation3D*           animation;
		cocos2d::Animate3D*             animate;
		std::map< const std::string, std::string>	modelAnimeList;

		int createAnimation( const std::string& animeName, float startTime, float endTime, bool loopFlag, bool reverseFlag);

		int load3DModelAnimeData( const std::string& fileName);
		int load3DModelTextureData( const std::string& fileName);
		static _Sprite3D* createObject( const char* firstPath, const char* secondPath);
		static void createObjectAsync( const char* firstPath, const char* secondPath, const std::function<void(_Sprite3D*, void*)>& callback, void* callbackparam);
		void afterAsyncLoad( void* param);
		bool loadFromCache( const std::string& path);
		static ResouceType checkResourcePath( const std::string& filePath);

		struct AsyncLoadParam
		{
			std::function<void(_Sprite3D*, void*)> afterLoadCallback; // callback after load
			void*                           callbackParam;
			bool                            result; // sprite load result
			std::string                     modlePath;
			std::string                     texPath; //
			cocos2d::MeshDatas* meshdatas;
			cocos2d::MaterialDatas* materialdatas;
			cocos2d::NodeDatas*   nodeDatas;
		};
		AsyncLoadParam             _asyncLoadParam;
	};
}

#endif // __SPRITE3D_H__
