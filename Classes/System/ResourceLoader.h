
#ifndef __RESOURCE_LOADER_SCENE_H__
#define __RESOURCE_LOADER_SCENE_H__

#include "cocos2d.h"

#include "Base/Sprite3D.h"

namespace TapGun
{
	class ResourceLoader
	{
	public:
		enum ModelNumber		// 非同期読み込み配列の区切り番号
		{
			Map = 0,
			EnemyStart = 1,
			EnemyEnd = 20,
			BulletStart = 21,
			BulletEnd = 70,
			Player = 71,
			ModelMax,
		};

		ResourceLoader( const ResourceLoader& p) = delete;
		ResourceLoader& operator=( const ResourceLoader& P) = delete;

		/**
		 *	3Dモデルの非同期読み込みオブジェクトのインスタンスの生成
		 *
		 *	@author	minaka
		 *	@param	なし
		 *	@return	生成したオブジェクトへのポインタ
		 */
		static ResourceLoader* getInstance( void);

		/**
		 *	3Dモデルの非同期読み込み
		 *
		 *	@author	minaka
		 *	@param	fileName 読み込みたいモデルのファイルパス
		 *	@return	なし
		 */
		void loadModel( const std::string& fileName);

		/**
		 *	3Dモデルの非同期読み込み
		 *
		 *	@author	minaka
		 *	@param	fileName 読み込みたいモデルのファイルパス
		 *	@param	texturePath 読み込みたいテクスチャのファイルパス
		 *	@return	なし
		 */
		void loadModel( const std::string& modelPath, const std::string& texturePath);

		/**
		 *	読み込み済み3Dモデルのポインタ取得
		 *
		 *	@author	minaka
		 *	@param	count アクセスしたいモデルの番号 ※具体的な番号はヘッダーのModelNumberを参照
		 *	@return	読み込み済み : 実体へのポインタ　読み込まれていない : nullptr
		 */
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

		/**
		 *	メンバ変数の初期化
		 *
		 *	@author	minaka
		 *	@param	なし
		 */
		ResourceLoader();

		/**
		 *	非同期読み込み完了モデルを配列に格納
		 *
		 *	@author	minaka
		 *	@param	sprite3D 読み込んだモデルデータへのポインタ
		 *	@param	param データの親ポインタ
		 *	@param	fileName 読み込んだモデルのファイル名
		 *	@return	読み込み済み : 実体へのポインタ　読み込まれていない : nullptr
		 */
		void sprite3DLoadCallback( _Sprite3D* sprite3D, void* param, const std::string& fileName);

		bool loadFlag[ModelMax];		// 読み込み完了判定フラグ
		int typeCount[TypeCountNum];	// モデル別読み込み数カウント
		_Sprite3D* models[ModelMax];	// 読み込んだモデルへのポインタ
	};
}

#endif // __RESOURCE_LOADER_SCENE_H__
