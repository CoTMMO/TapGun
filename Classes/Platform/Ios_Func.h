
#ifndef __IOS_FUNCTION_H__
#define __IOS_FUNCTION_H__

#include <fstream>
#include "cocos2d.h"

namespace TapGun
{
	class FileAccess
	{
	public:
		FileAccess( const FileAccess& p) = delete;
		FileAccess& operator=( const FileAccess& P) = delete;
		static FileAccess* getInstance( void);

		std::string getModelPath( const std::string& fileName);
		std::string getPicturePath( const std::string& fileName);
		std::string getAnimationPath( const std::string& fileName);
		std::string getEnemySettingFilePath( const std::string& fileName);
		std::string getBGMPath( const std::string& fileName);
		std::string getSEPath( const std::string& fileName);
		std::string getVoicePath( const std::string& fileName);
		std::string getFileStream( const std::string& fileName);

	private:
		FileAccess();
	};
}

#endif // __IOS_FUNCTION_H__
