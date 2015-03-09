
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

		/**
		*	3Dスプライトの作成
		*
		*	@author	minaka
		*	@param	firstPath リソースファイル名
		*	@return	作成したスプライトへのポインタ
		*/
		static _Sprite3D* create( const std::string& firstPath);

		/**
		*	3Dスプライトの作成
		*
		*	@author	minaka
		*	@param	firstPath リソースファイル名
		*	@param	secondPath リソースファイル名
		*	@return	作成したスプライトへのポインタ
		*/
		static _Sprite3D* create( const std::string& firstPath, const std::string& secondPath);

		/**
		*	3Dスプライトの作成 (非同期読み込み)
		*
		*	@author	minaka
		*	@param	firstPath リソースファイル名
		*	@param	callback コールバック関数
		*	@param	callbackparam　コールバック関数の引数
		*	@return	作成したスプライトへのポインタ
		*/
		static void createAsync( const std::string& firstPath, const std::function<void(_Sprite3D*, void*)>& callback, void* callbackparam);

		/**
		*	3Dスプライトの作成 (非同期読み込み)
		*
		*	@author	minaka
		*	@param	firstPath リソースファイル名
		*	@param	secondPath リソースファイル名
		*	@param	callback コールバック関数
		*	@param	callbackparam　コールバック関数の引数
		*	@return	作成したスプライトへのポインタ
		*/
		static void createAsync( const std::string& firstPath, const std::string& secondPath, const std::function<void(_Sprite3D*, void*)>& callback, void* callbackparam);

		/**
		*	3Dモデルのアニメーション再生
		*
		*	@author	minaka
		*	@param	animeName アニメーション名
		*	@return	正常終了:0 エラー発生:-1
		*/
		int startAnimation( const std::string& animeName);

		/**
		*	3Dモデルのアニメーション再生（ループ）
		*
		*	@author	minaka
		*	@param	animeName アニメーション名
		*	@return	正常終了:0 エラー発生:-1
		*/
		int startAnimationLoop( const std::string& animeName);

		/**
		*	3Dモデルのアニメーション逆再生
		*
		*	@author	minaka
		*	@param	animeName アニメーション名
		*	@return	正常終了:0 エラー発生:-1
		*/
		int startAnimationReverse( const std::string& animeName);

		/**
		*	3Dモデルのアニメーション逆再生（ループ）
		*
		*	@author	minaka
		*	@param	animeName アニメーション名
		*	@return	正常終了:0 エラー発生:-1
		*/
		int startAnimationReverseLoop( const std::string& animeName);

		/**
		*	3Dモデルのアニメーショントリミング再生
		*
		*	@author	minaka
		*	@param	animeName アニメーション名
		*	@param	startTime トリミング開始フレーム
		*	@param	endTime トリミング終了フレーム
		*	@return	正常終了:0 エラー発生:-1
		*/
		int startAnimation( const std::string& animeName, float startTime, float endTime);

		/**
		*	3Dモデルのアニメーショントリミング再生（ループ）
		*
		*	@author	minaka
		*	@param	animeName アニメーション名
		*	@param	startTime トリミング開始フレーム
		*	@param	endTime トリミング終了フレーム
		*	@return	正常終了:0 エラー発生:-1
		*/
		int startAnimationLoop( const std::string& animeName, float startTime, float endTime);

		/**
		*	3Dモデルのアニメーショントリミング逆再生
		*
		*	@author	minaka
		*	@param	animeName アニメーション名
		*	@param	startTime トリミング開始フレーム
		*	@param	endTime トリミング終了フレーム
		*	@return	正常終了:0 エラー発生:-1
		*/
		int startAnimationReverse( const std::string& animeName, float startTime, float endTime);

		/**
		*	3Dモデルのアニメーショントリミング逆再生（ループ）
		*
		*	@author	minaka
		*	@param	animeName アニメーション名
		*	@param	startTime トリミング開始フレーム
		*	@param	endTime トリミング終了フレーム
		*	@return	正常終了:0 エラー発生:-1
		*/
		int startAnimationReverseLoop( const std::string& animeName, float startTime, float endTime);

		/**
		*	3Dモデルのアニメーション停止
		*
		*	@author	minaka
		*	@param	animeName アニメーション名
		*	@return	正常終了:0
		*	@date	1/3	Ver 1.0
		*/
		int stopAnimation( void);

		/**
		*	全てのアニメーションを停止
		*
		*	@author	minaka
		*	@return	正常終了:0
		*	@date	1/3	Ver 1.0
		*/
		int stopALLAnimation( void);

		/**
		*	3Dモデルのアニメーション再生速度を設定
		*
		*	@author	minaka
		*	@param	speed アニメーション速度
		*	@return	正常終了:0
		*/
		int setAnimationSpeed( float speed);

		/**
		*	3Dモデルのアニメーション状態チェック
		*
		*	@author	minaka
		*	@return	アニメーション中ではない:0　アニメーション中:1
		*/
		int checkAnimationState( void);

		/**
		*	3Dモデルのアニメーション情報解放
		*
		*	@author	minaka
		*	@date	1/3	Ver 1.0
		*/
		void releaseAnimation( void);

	CC_CONSTRUCTOR_ACCESS:
		_Sprite3D() {}

	protected:

	private:
		double time;
		cocos2d::Animation3D*           animation;
		cocos2d::Animate3D*             animate;
		std::map< const std::string, std::string>	modelAnimeList;

		/**
		*	3Dモデルデータ用アニメーション設定ファイルの読み込み
		*
		*	@author	minaka
		*	@param	animeName アニメーション名
		*	@return	正常終了:0 エラー発生:-1
		*/
		int createAnimation( const std::string& animeName, float startTime, float endTime, bool loopFlag, bool reverseFlag);

		/**
		*	3Dモデルデータ用アニメーション設定ファイルの読み込み
		*
		*	@author	minaka
		*	@param	fileName モデルデータ名
		*	@return	正常終了:0 エラー発生:-1
		*	@date	1/3	Ver 1.0
		*/
		int load3DModelAnimeData( const std::string& fileName);

		/**
		*	3Dスプライトの実体作成
		*
		*	@author	minaka
		*	@param	firstPath リソースファイル名
		*	@param	secondPath リソースファイル名
		*	@return	作成したスプライトへのポインタ
		*/
		static _Sprite3D* createObject( const char* firstPath, const char* secondPath);

		/**
		*	3Dスプライトの非同期作成
		*
		*	@author	minaka
		*	@param	firstPath リソースファイル名
		*	@param	secondPath リソースファイル名
		*	@param	callback コールバック関数
		*	@param	callbackparam　コールバック関数の引数
		*	@return	なし
		*/
		static void createObjectAsync( const char* firstPath, const char* secondPath, const std::function<void(_Sprite3D*, void*)>& callback, void* callbackparam);

		/**
		*	非同期読み込み処理 (cocos2d-xより移植)
		*
		*	@author	minaka
		*	@param	param 作成済みスプライト
		*	@return	なし
		*/
		void afterAsyncLoad( void* param);

		/**
		*	モデルデータの読み込み (cocos2d-xより移植)
		*
		*	@author	minaka
		*	@param	path モデルデータへのパス
		*	@return	bool 読み込み成功フラグ 成功 : true 失敗 : false
		*/
		bool loadFromCache( const std::string& path);

		/**
		*	ファイルパスの拡張子判定
		*
		*	@author	minaka
		*	@param	filePath 判定するファイルパス
		*	@return	ResouceType 拡張子別の定数
		*/
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
