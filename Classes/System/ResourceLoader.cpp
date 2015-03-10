
#include "cocos2d.h"

#include "Platform.h"
#include "System/ResourceLoader.h"
#include "Base/Sprite3D.h"

USING_NS_CC;
using namespace std;
using namespace TapGun;

ResourceLoader::ResourceLoader()
{
	for( auto &p : loadFlag) { p = false; }
	typeCount[TypeMap] = Map;
	typeCount[TypeEnemy] = EnemyStart;
	typeCount[TypeBullet] = BulletStart;
	typeCount[TypePlayer] = Player;
}

ResourceLoader* ResourceLoader::getInstance( void)
{
	static ResourceLoader* p = nullptr;
	if( p == nullptr) { p = new ResourceLoader(); }
	return p;
}

void ResourceLoader::loadModel( const string& filePath)
{
	_Sprite3D::createAsync( filePath, CC_CALLBACK_2( ResourceLoader::sprite3DLoadCallback, this, filePath), nullptr);
}

void ResourceLoader::loadModel( const string& modelPath, const string& texturePath)
{
	_Sprite3D::createAsync( modelPath, texturePath, CC_CALLBACK_2( ResourceLoader::sprite3DLoadCallback, this, modelPath), nullptr);
}

_Sprite3D* ResourceLoader::getSprite3D( int count)
{
	if( loadFlag[count] == true) { return models[count]; }
	else { return nullptr; }
}

void ResourceLoader::sprite3DLoadCallback( _Sprite3D* sprite3D, void* param, const string& fileName)
{
	// ���[�J���ɃR�s�[
	string str = fileName;
	// �t�@�C���p�X����N���X�̃C���X�^���X���擾
	auto access = FileAccess::getInstance();

	// ��؂蕶��������
	int point = str.rfind( '/', str.size());
	// ��؂蕶�����O���̕�������폜
	str.erase( 0, str.size() - ( str.size() - point) + 1);

	// ��؂蕶��������
	point = str.rfind( '.', str.size());
	// ��؂蕶��������̕�������폜
	if( point > 0) { str.erase( str.size() - ( str.size() - point), str.size()); }

	if( str == "stage")						// ���ۂ̃t�@�C������"stage" �v���O�������ł�"Map"
	{
		// �ǂݍ��݃t���O�𗧂Ă�
		loadFlag[typeCount[Map]] = true;
		// �ǂݍ��񂾃��f����z��Ɋi�[
		models[typeCount[Map]] = sprite3D;
		// ���t�@�����X�J�E���^�𑝉�
		models[typeCount[Map]] -> retain();
	}
	else if( str == "enemy")
	{
		// �ǂݍ��݃t���O�𗧂Ă�
		loadFlag[typeCount[TypeEnemy]] = true;
		// �ǂݍ��񂾃��f����z��Ɋi�[
		models[typeCount[TypeEnemy]] = sprite3D;
		// ���t�@�����X�J�E���^�𑝉�
		models[typeCount[TypeEnemy]] -> retain();
		// ���f���Ƀe�N�X�`����ݒ�
		models[typeCount[TypeEnemy]] -> setTexture( access -> getModelPath( "Enemy/tex_teki.png"));
		models[typeCount[TypeEnemy]] -> getMeshByName( "gun") -> setTexture( access -> getModelPath( "Enemy/tex_boy_guns.png"));
		typeCount[TypeEnemy]++;
	}
	else if( str == "tama")
	{
		// �ǂݍ��݃t���O�𗧂Ă�
		loadFlag[typeCount[TypeBullet]] = true;
		// �ǂݍ��񂾃��f����z��Ɋi�[
		models[typeCount[TypeBullet]] = sprite3D;
		// ���t�@�����X�J�E���^�𑝉�
		models[typeCount[TypeBullet]] -> retain();
		// ���f���Ƀe�N�X�`����ݒ�
		auto child = (_Sprite3D*)models[typeCount[TypeBullet]] -> getChildByName( "tama1");
		child -> setTexture( access -> getModelPath( "Bullet/tama.png"));
		child = (_Sprite3D*)models[typeCount[TypeBullet]] -> getChildByName( "tama2");
		child -> setTexture( access -> getModelPath( "Bullet/tama_2.png"));

		typeCount[TypeBullet]++;
	}
	else if( str == "player")
	{
		// �ǂݍ��݃t���O�𗧂Ă�
		loadFlag[typeCount[TypePlayer]] = true;
		// �ǂݍ��񂾃��f����z��Ɋi�[
		models[typeCount[TypePlayer]] = sprite3D;
		// ���t�@�����X�J�E���^�𑝉�
		models[typeCount[TypePlayer]] -> retain();
		// ���f���Ƀe�N�X�`����ݒ�
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
