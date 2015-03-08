
#ifndef __IOS_FUNCTION_H__
#define __IOS_FUNCTION_H__

#include <fstream>
#include "cocos2d.h"

namespace TapGun
{
	std::string getModelPath( const std::string& fileName);
	std::string getPicturePath( const std::string& fileName);
	std::string getParameterPath( const std::string& fileName);
	std::string getBGMPath( const std::string& fileName);
	std::string getSEPath( const std::string& fileName);
	std::string getVoicePath( const std::string& fileName);
	std::ifstream getFileStream( const std::string& fileName);
}

#endif // __IOS_FUNCTION_H__
