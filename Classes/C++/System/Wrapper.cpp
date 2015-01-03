
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "Wrapper.h"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "C++/System/Wrapper.h"

#else

#include "C++/System/Wrapper.h"

#endif

USING_NS_CC;
using namespace std;
using namespace TapGun;
using namespace CocosDenshion;

/**
 *	���b�p�[�N���X�̃C���X�^���X���擾
 *
 *	@author	minaka
 *	@return	�쐬�ς݂̃C���X�^���X�ւ̃|�C���^
 *	@date	12/28 Ver 1.0
 */
Wrapper* Wrapper::getInstance( void)
{
	static Wrapper* P;
	if( !P) P = new Wrapper;
	return P;
}

int Wrapper::loadSound( const string& fileName)
{
	return 0;
}
	
int Wrapper::playSound( const string& fileName)
{
	return 0;
}
	
int Wrapper::playSoundLoop( const string& fileName)
{
	return 0;
}
	
int Wrapper::stopSound( const string& fileName)
{
	return 0;
}
	
int Wrapper::releaseSound( const string& fileName)
{
	return 0;
}

