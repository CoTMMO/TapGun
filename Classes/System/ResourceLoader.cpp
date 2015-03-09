
#include "cocos2d.h"

#include "Platform.h"
#include "System/ResourceLoader.h"
#include "Base/Sprite3D.h"

USING_NS_CC;
using namespace std;
using namespace TapGun;

/**
*	メンバ変数の初期化
*
*	@author	minaka
*	@param	なし
*/
ResourceLoader::ResourceLoader()
{
	for( auto &p : loadFlag) { p = false; }
	typeCount[TypeMap] = Map;
	typeCount[TypeEnemy] = EnemyStart;
	typeCount[TypeBullet] = BulletStart;
	typeCount[TypePlayer] = Player;
}

/**
*	3Dモデルの非同期読み込みオブジェクトのインスタンスの生成
*
*	@author	minaka
*	@param	なし
*	@return	生成したオブジェクトへのポインタ
*/
ResourceLoader* ResourceLoader::getInstance( void)
{
	static ResourceLoader* p = nullptr;
	if( p == nullptr) { p = new ResourceLoader(); }
	return p;
}

/**
*	3Dモデルの非同期読み込み
*
*	@author	minaka
*	@param	fileName 読み込みたいモデルのファイルパス
*	@return	なし
*/
void ResourceLoader::loadModel( const string& filePath)
{
	_Sprite3D::createAsync( filePath, CC_CALLBACK_2( ResourceLoader::sprite3DLoadCallback, this, filePath), nullptr);
}

/**
*	3Dモデルの非同期読み込み
*
*	@author	minaka
*	@param	modelPath 読み込みたいモデルのファイルパス
*	@param	texturePath 読み込みたいテクスチャのファイルパス
*	@return	なし
*/
void ResourceLoader::loadModel( const string& modelPath, const string& texturePath)
{
	_Sprite3D::createAsync( modelPath, texturePath, CC_CALLBACK_2( ResourceLoader::sprite3DLoadCallback, this, modelPath), nullptr);
}

/**
*	読み込み済み3Dモデルのポインタ取得
*
*	@author	minaka
*	@param	count アクセスしたいモデルの番号 ※具体的な番号はヘッダーのModelNumberを参照
*	@return	読み込み済み : 実体へのポインタ　読み込まれていない : nullptr
*/
_Sprite3D* ResourceLoader::getSprite3D( int count)
{
	if( loadFlag[count] == true) { return models[count]; }
	else { return nullptr; }
}

/**
*	非同期読み込み完了モデルを配列に格納
*
*	@author	minaka
*	@param	sprite3D 読み込んだモデルデータへのポインタ
*	@param	param データの親ポインタ
*	@param	fileName 読み込んだモデルのファイル名
*	@return	読み込み済み : 実体へのポインタ　読み込まれていない : nullptr
*/
void ResourceLoader::sprite3DLoadCallback( _Sprite3D* sprite3D, void* param, const string& fileName)
{
	// ローカルにコピー
	string str = fileName;
	// ファイルパス制御クラスのインスタンスを取得
	auto access = FileAccess::getInstance();

	// 区切り文字を検索
	int point = str.rfind( '/', str.size());
	// 区切り文字より前方の文字列を削除
	str.erase( 0, str.size() - ( str.size() - point) + 1);

	// 区切り文字を検索
	point = str.rfind( '.', str.size());
	// 区切り文字より後方の文字列を削除
	if( point > 0) { str.erase( str.size() - ( str.size() - point), str.size()); }

	if( str == "stage")						// 実際のファイル名は"stage" プログラム内では"Map"
	{
		// 読み込みフラグを立てる
		loadFlag[typeCount[Map]] = true;
		// 読み込んだモデルを配列に格納
		models[typeCount[Map]] = sprite3D;
		// リファレンスカウンタを増加
		models[typeCount[Map]] -> retain();
	}
	else if( str == "enemy")
	{
		// 読み込みフラグを立てる
		loadFlag[typeCount[TypeEnemy]] = true;
		// 読み込んだモデルを配列に格納
		models[typeCount[TypeEnemy]] = sprite3D;
		// リファレンスカウンタを増加
		models[typeCount[TypeEnemy]] -> retain();
		// モデルにテクスチャを設定
		models[typeCount[TypeEnemy]] -> setTexture( access -> getModelPath( "Enemy/tex_teki.png"));
		models[typeCount[TypeEnemy]] -> getMeshByName( "gun") -> setTexture( access -> getModelPath( "Enemy/tex_boy_guns.png"));
		typeCount[TypeEnemy]++;
	}
	else if( str == "tama")
	{
		// 読み込みフラグを立てる
		loadFlag[typeCount[TypeBullet]] = true;
		// 読み込んだモデルを配列に格納
		models[typeCount[TypeBullet]] = sprite3D;
		// リファレンスカウンタを増加
		models[typeCount[TypeBullet]] -> retain();
		// モデルにテクスチャを設定
		auto child = (_Sprite3D*)models[typeCount[TypeBullet]] -> getChildByName( "tama1");
		child -> setTexture( access -> getModelPath( "Bullet/tama.png"));
		child = (_Sprite3D*)models[typeCount[TypeBullet]] -> getChildByName( "tama2");
		child -> setTexture( access -> getModelPath( "Bullet/tama_2.png"));

		typeCount[TypeBullet]++;
	}
	else if( str == "player")
	{
		// 読み込みフラグを立てる
		loadFlag[typeCount[TypePlayer]] = true;
		// 読み込んだモデルを配列に格納
		models[typeCount[TypePlayer]] = sprite3D;
		// リファレンスカウンタを増加
		models[typeCount[TypePlayer]] -> retain();
		// モデルにテクスチャを設定
		models[typeCount[TypePlayer]] -> setTexture( access -> getModelPath( "Player/tex_player_1.png"));
		models[typeCount[TypePlayer]] -> getMeshByName( "me") -> setTexture( access -> getModelPath( "Player/tex_player_2.png"));
		models[typeCount[TypePlayer]] -> getMeshByName( "body_3") -> setTexture( access -> getModelPath( "Player/tex_player_2.png"));
		models[typeCount[TypePlayer]] -> getMeshByName( "gun") -> setTexture( access -> getModelPath( "Player/tex_gun.png"));
		models[typeCount[TypePlayer]] -> getMeshByName( "mug") -> setTexture( access -> getModelPath( "Player/tex_gun.png"));
		models[typeCount[TypePlayer]] -> getMeshByName( "Cylinder014") -> setTexture( access -> getModelPath( "Player/tex_gun.png"));
	}
	else
	{
		log( "3DModelFileLoadError : FileName LoadPath no math");
	}
}
