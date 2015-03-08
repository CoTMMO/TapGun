
#include "Android_Func.h"

USING_NS_CC;
using namespace std;
using namespace TapGun;

/*
*	ios用に文字列を加工
*
*	@author	minaka
*	@param	fileName 	加工前文字列
*	@return	std::string	加工後文字列
 */
static string getFilePath( const string& fileName)
{
	int	point = fileName.rfind( '/', fileName.size());	// ディレクトリ区切りを検索
	string str = fileName.substr( point + 1);			// 必要な部分のみ切り取り
	return str;
}

// 以下の関数は他のプラットフォームとの互換性維持用

FileAccess::FileAccess() {}

FileAccess* FileAccess::getInstance( void)
{
	static FileAccess *p = nullptr;
	if( !p) { p = new FileAccess; }
	return p;
}

string FileAccess::getModelPath( const string& fileName)
{
	return getFilePath( fileName);
}

string FileAccess::getPicturePath( const string& fileName)
{
	return getFilePath( fileName);
}

string FileAccess::getAnimationPath( const string& fileName)
{
	return getFilePath( fileName);
}

string FileAccess::getEnemySettingFilePath( const string& fileName)
{
	return getFilePath( fileName);
}

string FileAccess::getBGMPath( const string& fileName)
{
	return getFilePath( fileName);
}

string FileAccess::getSEPath( const string& fileName)
{
	return getFilePath( fileName);
}

string FileAccess::getVoicePath( const string& fileName)
{
	return getFilePath( fileName);
}

string FileAccess::getFileStream( const string& fileName)
{
	string fileData = FileUtils::getInstance() -> getStringFromFile( fileName);
	return fileData;
}
