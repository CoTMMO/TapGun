
#include <fstream>
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "EnemySettingFile.h"

#else

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
	string str;
	while( getline( file, str))
	{
		string name;
		string path;
		string tmp;
		istringstream stream(str);
		getline( stream, tmp, ',');
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		auto point = tmp.rfind( '/', tmp.size()) + 1;
		tmp.erase( 0, tmp.size() - ( tmp.size() - point));
		path = tmp;
	#else
		path = "Graph/Models/" + tmp;
	#endif
		getline( stream, tmp);
		name = tmp;
	}
	return;
}