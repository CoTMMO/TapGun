
#ifndef __RESOURCE_LOADER_SCENE_H__
#define __RESOURCE_LOADER_SCENE_H__

#include "cocos2d.h"

#include "Base/Sprite3D.h"

namespace TapGun
{
	class ResourceLoader
	{
	public:
		enum ModelNumber
		{
			Map = 0,
			EnemyStart = 1,
			EnemyEnd = 20,
			BulletStart = 21,
			BulletEnd = 70,
			Player,
			ModelMax,
		};

		ResourceLoader( const ResourceLoader& p) = delete;
		ResourceLoader& operator=( const ResourceLoader& P) = delete;
		static ResourceLoader* getInstance( void);

		void loadModel( const std::string& fileName);
		void loadModel( const std::string& modelPath, const std::string& texturePath);

		_Sprite3D* getSprite3D( int count);

	private:
		enum ModelTypeCount
		{
			TypeMap,
			TypeEnemy,
			TypeBullet,
			TypePlayer,
			TypeCountNum,
		};

		ResourceLoader();
		void sprite3DLoadCallback( _Sprite3D* sprite3D, void* param, const std::string& fileName);

		bool loadFlag[ModelMax];
		int typeCount[TypeCountNum];
		_Sprite3D* models[ModelMax];
	};
}

#endif // __RESOURCE_LOADER_SCENE_H__
