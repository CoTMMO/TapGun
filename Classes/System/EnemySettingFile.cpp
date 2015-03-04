
#include <fstream>
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "EnemySettingFile.h"
#include "Enemy.h"

#else

#include "Stage/Enemy.h"
#include "System/EnemySettingFile.h"

#endif

USING_NS_CC;
using namespace std;
using namespace TapGun;

EnemySettingFile* EnemySettingFile::getInstance( void)
{
	static EnemySettingFile* p = nullptr;
	if( !p) { p = new EnemySettingFile; }
	return p;
}

void EnemySettingFile::loadTableFile( const string& fileName)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	std::string dir = FileUtils::getInstance() -> fullPathForFilename( fileName);
	ifstream file( dir, ios::in);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	string filePath = "Parameter/EnemySettingFile/" + fileName;
	string fileData = FileUtilsAndroid::getInstance() -> getStringFromFile( filePath);
	stringstream file(fileData);
#else
	string filePath = "Parameter/EnemySettingFile/" + fileName;
	ifstream file( filePath, ios::in);
#endif
	if( file.fail()) { return; }

	int loadCount = 0;
	string str;
	for( fileLineCount = 1; getline( file, str); fileLineCount++)
	{
		if( fileLineCount > 6)
		{
			dataList[loadCount];
			loadCount++;
			fileLineCount = 1;
		}

		istringstream stream( str);
		string tmp;
		WaveData* data = new WaveData;
		int x;
		int y;
		int z;

		switch( fileLineCount)
		{
		case 1:
		case 3:
		case 5:
			break;

		case 2:
			getline( stream, tmp, ',');
			data -> Num = atoi( tmp.c_str());

			getline( stream, tmp, ',');
			tmp.erase( 0, 2);
			x = atoi( tmp.c_str());
			getline( stream, tmp, ',');
			y = atoi( tmp.c_str());
			getline( stream, tmp, ',');
			z = atoi( tmp.c_str());
			data -> startPos = Vec3( x, y, z);

			getline( stream, tmp, ',');
			data -> sleepTime = atoi( tmp.c_str());

			for( int i = 0; i < 3; i++)
			{
				getline( stream, tmp, ',');
				tmp.erase( 0, 2);
				x = atoi( tmp.c_str());
				getline( stream, tmp, ',');
				y = atoi( tmp.c_str());
				getline( stream, tmp, ',');
				z = atoi( tmp.c_str());
				data -> targetPos[i] = Vec3( x, y, z);
			}
			getline( stream, tmp, ',');
			data -> waitToAtack = atoi( tmp.c_str());
			break;

		case 4:
			getline( stream, tmp, ',');
			for( int i = 0; i < 3; i++)
			{
				getline( stream, tmp, ',');
				x = atoi( tmp.c_str());
				data -> waitToMove[i] = x;
			}
			for( int i = 0; i < 3; i++)
			{
				getline( stream, tmp, ',');
				x = atoi( tmp.c_str());
				data -> nextEnemies[i] = x;
			}
			break;

		case 6:
			getline( stream, tmp, ',');

			break;

		default:
			break;
		}
	}
	return;
}
