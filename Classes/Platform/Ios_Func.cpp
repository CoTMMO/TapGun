
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
	string str = fileName;		// ローカルにコピー
	do {
		// ディレクトリ区切りを検索
		int point = str.rfind( '/', str.size());
		// 区切りなしの場合は終了
		if( point == string::npos) { break; }
		// 区切り文字より前の文字列を削除
		str.erase( 0, str.size() - ( str.size() - point));
		// もう一度ディレクトリ区切りを検索
		point = str.rfind( '/', str.size());
	}while( point != string::npos);	// 区切り文字がある限りループ
	return str;
}

// 以下の関数は他のプラットフォームとの互換性維持用

string getModelPath( const string& fileName)
{
	return getFilePath( fileName);
}

string getPicturePath( const string& fileName)
{
	return getFilePath( fileName);
}

string getParameterPath( const string& fileName)
{
	return getFilePath( fileName);
}

string getBGMPath( const string& fileName)
{
	return getFilePath( fileName);
}

string getSEPath( const string& fileName)
{
	return getFilePath( fileName);
}

string getVoicePath( const string& fileName)
{
	return getFilePath( fileName);
}

ifstream getFileStream( const string& fileName)
{
	string str = FileUtils::getInstance() -> fullPathForFilename( TapGun::getParameterPath( fileName));
	ifstream file( str, ios::in);
	if( file.fail()) { return nullptr; }
	return file;
}
