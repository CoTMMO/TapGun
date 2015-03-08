
#include "cocos2d.h"
#include "3d/CCMesh.h"
#include "base/CCAsyncTaskPool.h"

#include "Base/Sprite3D.h"
#include "Platform.h"

#if ( _DEBUG || DEBUG)

#define MODEL_EXTENSION (".c3t")

#else

#define MODEL_EXTENSION (".c3b")

#endif

USING_NS_CC;
using namespace std;
using namespace TapGun;

/**
*	3Dスプライトの作成
*
*	@author	minaka
*	@param	firstPath リソースファイル名
*	@return	作成したスプライトへのポインタ
*/
_Sprite3D* _Sprite3D::create( const string& firstPath)
{
	return createObject( firstPath.c_str(), nullptr);
}

/**
*	3Dスプライトの作成
*
*	@author	minaka
*	@param	firstPath リソースファイル名
*	@param	secondPath リソースファイル名
*	@return	作成したスプライトへのポインタ
*/
_Sprite3D* _Sprite3D::create( const string& firstPath, const string& secondPath)
{
	return createObject( firstPath.c_str(), secondPath.c_str());
}

/**
*	3Dスプライトの作成 (非同期読み込み)
*
*	@author	minaka
*	@param	firstPath リソースファイル名
*	@param	callback コールバック関数
*	@param	callbackparam　コールバック関数の引数
*	@return	作成したスプライトへのポインタ
*/
void _Sprite3D::createAsync( const string& modelPath, const function<void(_Sprite3D*, void*)>& callback, void* callbackparam)
{
	createObjectAsync( modelPath.c_str(), "", callback, callbackparam);
}

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
void _Sprite3D::createAsync( const string& modelPath, const string& texturePath, const function<void(_Sprite3D*, void*)>& callback, void* callbackparam)
{
	createObjectAsync( modelPath.c_str(), texturePath.c_str(), callback, callbackparam);
}

/**
*	3Dスプライトの実体作成
*
*	@author	minaka
*	@param	firstPath リソースファイル名
*	@param	secondPath リソースファイル名
*	@return	作成したスプライトへのポインタ
*/
_Sprite3D* _Sprite3D::createObject( const char* firstPath, const char* secondPath)
{
	string filePath;							// ファイルのディレクトリパスを格納
	auto sprite = new (nothrow) _Sprite3D();	// スプライト生成
	auto access = FileAccess::getInstance();	// ファイルパス制御クラスのインスタンスを取得
	vector<string> str;							// 文字列配列を使用し後の処理を簡略化
	bool Flag[ResouceType::Num] = { false };	// 読み込み制御フラグを生成

	// 文字列を配列化しアクセスを簡単に
	if( &firstPath == nullptr) return nullptr;
	else str.push_back( firstPath);
	if( secondPath != nullptr) str.push_back( secondPath);

	for( int i = 0; i < str.size(); i++)
	{
		auto type = checkResourcePath( str[i]); // 拡張子を判定し以後の処理を分岐
		switch( type)
		{
		case ResouceType::NoExt:
		case ResouceType::Model:
			filePath = access -> getModelPath( str.at(i));	// ファイルのパスを取得
			// 拡張子無しの場合は補完
			if( type == ResouceType::NoExt) { filePath = filePath + MODEL_EXTENSION; }

			// フラグチェックし二重読み込み防止
			if( Flag[ResouceType::NoExt] == false && Flag[ResouceType::Model] == false)
			{
				// スプライトの実体作成
				if( sprite && sprite -> initWithFile( filePath))
				{
					sprite -> _contentSize = sprite -> getBoundingBox().size;
					sprite -> autorelease();
				}
				// フラグ処理
				if( Flag[ResouceType::Model] == false) { Flag[ResouceType::Model] = true; }
				else if( Flag[ResouceType::NoExt] == false) { Flag[ResouceType::NoExt] = true; }
			}
			else { return nullptr; }
			break;

		case ResouceType::Anime:
			// フラグチェックし二重読み込み防止
			if( Flag[ResouceType::Anime] == false)
			{
				// モデルに紐付くアニメーションデータを読み込む
				sprite -> load3DModelAnimeData( str[i]);
				// フラグ処理
				Flag[ResouceType::Anime] = true;
			}
			else { return nullptr; }
			break;

		default:
			break;
		}
	}
	// モデルの読み込みが行われなかった場合
	if( Flag[ResouceType::NoExt] == false && Flag[ResouceType::Model] == false)
	{
		// 実体なし
		CC_SAFE_DELETE( sprite);
		return nullptr;
	}
	return sprite;
}

/**
*	モデルデータの読み込み (cocos2d-xより移植)
*
*	@author	minaka
*	@param	path モデルデータへのパス
*	@return	bool 読み込み成功フラグ 成功 : true 失敗 : false
*/
bool _Sprite3D::loadFromCache( const string& path)
{
	auto spritedata = Sprite3DCache::getInstance() -> getSpriteData( path);
	if( spritedata)
	{
		for( auto it : spritedata -> meshVertexDatas) { _meshVertexDatas.pushBack( it); }
		_skeleton = Skeleton3D::create( spritedata -> nodedatas -> skeleton);
		CC_SAFE_RETAIN( _skeleton);

		for( const auto& it : spritedata -> nodedatas -> nodes)
		{
			if( it) { createNode( it, this, *(spritedata -> materialdatas), spritedata -> nodedatas -> nodes.size() == 1); }
		}

		for( const auto& it : spritedata -> nodedatas -> skeleton)
		{
			if( it) { createAttachSprite3DNode( it,*(spritedata -> materialdatas)); }
		}

		for( ssize_t i = 0; i < _meshes.size(); i++) { _meshes.at( i) -> setGLProgramState( spritedata -> glProgramStates.at( i)); }
		return true;
	}
	return false;
}

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
void _Sprite3D::createObjectAsync( const char* firstPath, const char* secondPath, const function<void(_Sprite3D*, void*)>& callback, void* callbackparam)
{
	string filePath;							// ファイルのディレクトリパスを格納
	auto sprite = new (nothrow) _Sprite3D();	// スプライト生成
	auto access = FileAccess::getInstance();	// ファイルパス制御クラスのインスタンスを取得
	vector<string> str;							// 文字列配列を使用し後の処理を簡略化
	bool Flag[ResouceType::Num] = { false };	// 読み込み制御フラグを生成

	// 文字列を配列化しアクセスを簡単に
	if( &firstPath == nullptr) return;
	else str.push_back( firstPath);
	if( secondPath != nullptr) str.push_back( secondPath);

	for( int i = 0; i < str.size(); i++)
	{
		auto type = checkResourcePath( str[i]); // 拡張子を判定し以後の処理を分岐
		switch( type)
		{
		case ResouceType::NoExt:
		case ResouceType::Model:
			filePath = access -> getModelPath( str[i]);	// ファイルのパスを取得
			// 拡張子無しの場合は補完
			if( type == ResouceType::NoExt) { filePath = filePath + MODEL_EXTENSION; }

			// フラグチェックし二重読み込み防止
			if( Flag[ResouceType::NoExt] == false && Flag[ResouceType::Model] == false)
			{
				// スプライトの実体作成
				if( sprite -> loadFromCache( filePath))
				{
					sprite -> autorelease();
//					if( !str[i].empty()) { sprite -> setTexture( texturePath); }
					callback( sprite, callbackparam);
					Flag[ResouceType::NoExt] = true;
					break;
				}

				sprite -> _asyncLoadParam.afterLoadCallback = callback;
				if( str[i].size() > 4) { sprite -> _asyncLoadParam.texPath = access -> getModelPath(""); }
				else { sprite -> _asyncLoadParam.texPath = ""; }
				sprite -> _asyncLoadParam.modlePath = filePath;
				sprite -> _asyncLoadParam.callbackParam = callbackparam;
				sprite -> _asyncLoadParam.materialdatas = new (nothrow) MaterialDatas();
				sprite -> _asyncLoadParam.meshdatas = new (nothrow) MeshDatas();
				sprite -> _asyncLoadParam.nodeDatas = new (nothrow) NodeDatas();
				AsyncTaskPool::getInstance() -> enqueue( AsyncTaskPool::TaskType::TASK_IO, CC_CALLBACK_1( _Sprite3D::afterAsyncLoad, sprite), (void*)(&sprite -> _asyncLoadParam), [sprite]()
				{
					sprite -> _asyncLoadParam.result = sprite -> loadFromFile( sprite -> _asyncLoadParam.modlePath, sprite -> _asyncLoadParam.nodeDatas, sprite -> _asyncLoadParam.meshdatas, sprite -> _asyncLoadParam.materialdatas);
				});

				// フラグ処理
				if( Flag[ResouceType::Model] == false) { Flag[ResouceType::Model] = true; }
				else if( Flag[ResouceType::NoExt] == false) { Flag[ResouceType::NoExt] = true; }
			}
			else { return; }
			break;

		case ResouceType::Anime:
			// フラグチェックし二重読み込み防止
			if( Flag[ResouceType::Anime] == false)
			{
				// モデルに紐付くアニメーションデータを読み込む
				sprite -> load3DModelAnimeData( str[i]);
				// フラグ処理
				Flag[ResouceType::Anime] = true;
			}
			else { return; }
			break;

		default:
			break;
		}
	}
	// モデルの読み込みが行われなかった場合
	if( Flag[ResouceType::NoExt] == false && Flag[ResouceType::Model] == false)
	{
		// 実体なし
		CC_SAFE_DELETE( sprite);
		return;
	}
}

/**
*	非同期読み込み処理 (cocos2d-xより移植)
*
*	@author	minaka
*	@param	param 作成済みスプライト
*	@return	なし
*/
void _Sprite3D::afterAsyncLoad( void* param)
{
	_Sprite3D::AsyncLoadParam* asyncParam = (_Sprite3D::AsyncLoadParam*)param;
	autorelease();
	if( asyncParam)
	{
		if( asyncParam -> result)
		{
			_meshes.clear();
			_meshVertexDatas.clear();
			CC_SAFE_RELEASE_NULL( _skeleton);
			removeAllAttachNode();

			//create in the main thread
			auto& meshdatas = asyncParam -> meshdatas;
			auto& materialdatas = asyncParam -> materialdatas;
			auto& nodeDatas = asyncParam -> nodeDatas;
			if( initFrom( *nodeDatas, *meshdatas, *materialdatas))
			{
				auto spritedata = Sprite3DCache::getInstance() -> getSpriteData( asyncParam -> modlePath);
				if( spritedata == nullptr)
				{
					//add to cache
					auto data = new (std::nothrow) Sprite3DCache::Sprite3DData();
					data -> materialdatas = materialdatas;
					data -> nodedatas = nodeDatas;
					data -> meshVertexDatas = _meshVertexDatas;
					for( const auto mesh : _meshes) { data -> glProgramStates.pushBack( mesh -> getGLProgramState()); }

					Sprite3DCache::getInstance() -> addSprite3DData( asyncParam -> modlePath, data);
					meshdatas = nullptr;
					materialdatas = nullptr;
					nodeDatas = nullptr;
				}
			}
			delete meshdatas;
			delete materialdatas;
			delete nodeDatas;

			if( asyncParam -> texPath != "") { setTexture( asyncParam -> texPath); }
		}
		else
		{
			CCLOG( "file load failed: %s ", asyncParam -> modlePath.c_str());
		}
		asyncParam -> afterLoadCallback( this, asyncParam -> callbackParam);
	}
}

/**
*	ファイルパスの拡張子判定
*
*	@author	minaka
*	@param	filePath 判定するファイルパス
*	@return	ResouceType 拡張子別の定数
*/
ResouceType _Sprite3D::checkResourcePath( const string& filePath)
{
	string str = filePath;	// ローカルにコピー
	// 拡張子判定不可
	if( str.size() < 4) return ResouceType::Error;
	// '.'までの文字数を計測
	int point = str.rfind( '.', str.size());
	// '.'なしの場合拡張子なしと判定
	if( point == string::npos) return ResouceType::NoExt;
	// 文字列を拡張子のみに
	str.erase( 0, str.size() - ( str.size() - point));
	// 拡張子のみとなった文字列で判定
	if( str == ".obj" || str == ".c3t" || str == ".c3b") return ResouceType::Model;
	else if( str == ".anime") return ResouceType::Anime;
}

/**
*	3Dモデルデータ用アニメーション設定ファイルの読み込み
*
*	@author	minaka
*	@param	fileName モデルデータ名
*	@return	正常終了:0 エラー発生:-1
*	@date	1/3	Ver 1.0
*/
int _Sprite3D::load3DModelAnimeData( const string& fileName)
{
	// ファイルパス制御クラスのインスタンスを取得
	auto access = FileAccess::getInstance();
	// 設定ファイルを入力ストリームに設定
	stringstream file( access -> getFileStream( access -> getAnimationPath( fileName)));
	// エラー判定
	if( file.fail())
	{
		log( "Animation File Load Error");
		return -1;
	}

	string str;						// ファイルの文字列を格納
	while( getline( file, str))
	{
		string path;				// モデルファイルパスを保存
		string tmp;					// 読み込んだ文字列の一時保存
		istringstream stream(str);	// 文字列を編集可能に

		getline( stream, tmp, ',');	// カンマ区切りで読み込み
		path = access -> getModelPath( tmp);	// ストリームから取得した文字列を元に読み込み可能なパスを生成
		getline( stream, tmp);		// ファイルの残りを読み込み
		// Windows環境でコンパイルした時に発生する改行コードを検索
		int point = tmp.rfind( '\r', tmp.size());
		if( point != string::npos)
		{
			// 開業コードが存在していれば削除
			tmp.erase( point, str.size());
		}
		modelAnimeList[tmp] = path;	// 連想配列に格納
	}
	return 0;
}

/**
*	3Dモデルデータ用アニメーション設定ファイルの読み込み
*
*	@author	minaka
*	@param	animeName アニメーション名
*	@return	正常終了:0 エラー発生:-1
*/
int _Sprite3D::createAnimation( const string& animeName, float startTime, float endTime, bool loopFlag, bool reverseFlag)
{
	string str = modelAnimeList[animeName];

	if( str == "")	// ファイル名チェック
	{
		log( "Animation DataName Error");
		return -1;
	}

	// animationを作成
	animation = Animation3D::create( str);

	// animationのチェック
	if( animation == nullptr)
	{
		log( "Animation create Error");
		return -1;
	}

	// animateを作成 (時間指定がある場合トリミング作成)
	if( startTime == NULL && endTime == NULL) { animate = Animate3D::create( animation); }
	else { animate = Animate3D::create( animation, startTime, endTime); }

	// 逆再生フラグがオンの場合逆再生設定
	if( reverseFlag) { animate -> setSpeed( -1); }

	// ループ再生フラグがオンの場合ループ再生設定
	if( loopFlag) { runAction( RepeatForever::create( animate)); }
	else { runAction( animate); }

	// 正常終了
	return 0;
}

/**
*	3Dモデルのアニメーション再生
*
*	@author	minaka
*	@param	animeName アニメーション名
*	@return	正常終了:0 エラー発生:-1
*/
int _Sprite3D::startAnimation( const string& animeName)
{
	if( createAnimation( animeName, NULL, NULL, false, false)) { return -1; }
	return 0;
}

/**
*	3Dモデルのアニメーション再生（ループ）
*
*	@author	minaka
*	@param	animeName アニメーション名
*	@return	正常終了:0 エラー発生:-1
*/
int _Sprite3D::startAnimationLoop( const string& animeName)
{
	if( createAnimation( animeName, NULL, NULL, true, false)) { return -1; }
	return 0;
}

/**
*	3Dモデルのアニメーション逆再生
*
*	@author	minaka
*	@param	animeName アニメーション名
*	@return	正常終了:0 エラー発生:-1
*/
int _Sprite3D::startAnimationReverse( const string& animeName)
{
	if( createAnimation( animeName, NULL, NULL, false, true)) { return -1; }
	return 0;
}

/**
*	3Dモデルのアニメーション逆再生（ループ）
*
*	@author	minaka
*	@param	animeName アニメーション名
*	@return	正常終了:0 エラー発生:-1
*/
int _Sprite3D::startAnimationReverseLoop( const string& animeName)
{
	if( createAnimation( animeName, NULL, NULL, true, true)) { return -1; }
	return 0;
}

/**
*	3Dモデルのアニメーショントリミング再生
*
*	@author	minaka
*	@param	animeName アニメーション名
*	@param	startTime トリミング開始フレーム
*	@param	endTime トリミング終了フレーム
*	@return	正常終了:0 エラー発生:-1
*/
int _Sprite3D::startAnimation( const string& animeName, float startTime, float endTime)
{
	if( createAnimation( animeName, startTime, endTime, false, false)) { return -1; }
	return 0;
}

/**
*	3Dモデルのアニメーショントリミング再生（ループ）
*
*	@author	minaka
*	@param	animeName アニメーション名
*	@param	startTime トリミング開始フレーム
*	@param	endTime トリミング終了フレーム
*	@return	正常終了:0 エラー発生:-1
*/
int _Sprite3D::startAnimationLoop( const string& animeName, float startTime, float endTime)
{
	if( createAnimation( animeName, startTime, endTime, true, false)) { return -1; }
	return 0;
}

/**
*	3Dモデルのアニメーショントリミング逆再生
*
*	@author	minaka
*	@param	animeName アニメーション名
*	@param	startTime トリミング開始フレーム
*	@param	endTime トリミング終了フレーム
*	@return	正常終了:0 エラー発生:-1
*/
int _Sprite3D::startAnimationReverse( const string& animeName, float startTime, float endTime)
{
	if( createAnimation( animeName, startTime, endTime, false, true)) { return -1; }
	return 0;
}

/**
*	3Dモデルのアニメーショントリミング逆再生（ループ）
*
*	@author	minaka
*	@param	animeName アニメーション名
*	@param	startTime トリミング開始フレーム
*	@param	endTime トリミング終了フレーム
*	@return	正常終了:0 エラー発生:-1
*/
int _Sprite3D::startAnimationReverseLoop( const string& animeName, float startTime, float endTime)
{
	if( createAnimation( animeName, startTime, endTime, true, true)) { return -1; }
	return 0;
}

/**
*	3Dモデルのアニメーション停止
*
*	@author	minaka
*	@param	animeName アニメーション名
*	@return	正常終了:0
*	@date	1/3	Ver 1.0
*/
int _Sprite3D::stopAnimation( void)
{
	stopAction( animate);
	return 0;
}

/**
*	全てのアニメーションを停止
*
*	@author	minaka
*	@return	正常終了:0
*	@date	1/3	Ver 1.0
*/
int _Sprite3D::stopALLAnimation( void)
{
	stopAllActions();
	return 0;
}

/**
*	3Dモデルのアニメーション再生速度を設定
*
*	@author	minaka
*	@param	speed アニメーション速度
*	@return	正常終了:0
*/
int _Sprite3D::setAnimationSpeed( float speed)
{
	animate -> setSpeed( speed);
	return 0;
}

/**
*	3Dモデルのアニメーション状態チェック
*
*	@author	minaka
*	@return	アニメーション中ではない:0　アニメーション中:1
*/
int _Sprite3D::checkAnimationState( void)
{
	if( numberOfRunningActions() == 0 ) { return 0; }
	else { return 1; }
}

/**
*	3Dモデルのアニメーション情報解放
*
*	@author	minaka
*	@date	1/3	Ver 1.0
*/
void _Sprite3D::releaseAnimation( void)
{
	map< const string, string>().swap( modelAnimeList);
}
