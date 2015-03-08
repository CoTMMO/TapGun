
#include "Win32_Func.h"

USING_NS_CC;
using namespace std;
using namespace TapGun;

FileAccess::FileAccess() {}

FileAccess* FileAccess::getInstance( void)
{
	static FileAccess *p = nullptr;
	if( !p) { p = new FileAccess; }
	return p;
}

string FileAccess::getModelPath( const string& fileName)
{
	string str = "Graph/Models/" + fileName;
	return str;
}

string FileAccess::getPicturePath( const string& fileName)
{
	string str = "Graph/Pictures/" + fileName;
	return str;
}

string FileAccess::getAnimationPath( const string& fileName)
{
	string str = "Parameter/Animation/" + fileName;
	return str;
}

string FileAccess::getEnemySettingFilePath( const string& fileName)
{
	string str = "Parameter/EnemySettingFile/" + fileName;
	return str;
}

string FileAccess::getBGMPath( const string& fileName)
{
	string str = "Sound/BGM/" + fileName + ".mp3";
	return str;
}

string FileAccess::getSEPath( const string& fileName)
{
	string str = "Sound/SE/" + fileName + ".mp3";
	return str;
}

string FileAccess::getVoicePath( const string& fileName)
{
	string str = "Sound/Voice/" + fileName + ".mp3";
	return str;
}

string FileAccess::getFileStream( const string& fileName)
{
	string fileData = FileUtils::getInstance() -> getStringFromFile( fileName);
	return fileData;
}
