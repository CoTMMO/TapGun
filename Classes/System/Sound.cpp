
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "Platform.h"
#include "System/Sound.h"

USING_NS_CC;
using namespace std;
using namespace TapGun;
using namespace CocosDenshion;

Sound::Sound()
{
}

Sound* Sound::getInstance( void)
{
	static Sound* P;
	if( !P) P = new Sound;
	return P;
}

void Sound::loadBGM( const string& fileName)
{
	Sound::getInstance() -> bgmFileName = FileAccess::getInstance() -> getBGMPath( fileName);
	SimpleAudioEngine::getInstance() -> preloadBackgroundMusic( Sound::getInstance() -> bgmFileName.c_str());
}

void Sound::loadBGM( const string& fileName, float Volume)
{
	Sound::getInstance() -> loadBGM( fileName);
	SimpleAudioEngine::getInstance() -> setBackgroundMusicVolume( ( Volume / 100));
}

int Sound::playBGM( void)
{
	string str = Sound::getInstance() -> bgmFileName;
	if( str.length() != NULL && !Sound::getInstance() -> playCheckBGM())
	{
		SimpleAudioEngine::getInstance() -> playBackgroundMusic( str.c_str(), false);
		return 0;
	}
	return -1;
}

int Sound::playBGM( float Volume)
{
	SimpleAudioEngine::getInstance() -> setBackgroundMusicVolume( ( Volume / 100));
	if( Sound::getInstance() -> playBGM()) { return -1; }
	return 0;
}

int Sound::playBGMLoop( void)
{
	string str = Sound::getInstance() -> bgmFileName;
	if( str.length() != NULL)
	{
		SimpleAudioEngine::getInstance() -> playBackgroundMusic( str.c_str(), true);
		return 0;
	}
	return -1;
}

int Sound::playBGMLoop( float Volume)
{
	SimpleAudioEngine::getInstance() -> setBackgroundMusicVolume( ( Volume / 100));
	if( Sound::getInstance() -> playBGMLoop()) { return -1; }
	return 0;
}

void Sound::setBGMVolume( float Volume)
{
	SimpleAudioEngine::getInstance() -> setBackgroundMusicVolume( ( Volume / 100));
}

void Sound::stopBGM( void)
{
	SimpleAudioEngine::getInstance() -> stopBackgroundMusic();
}

void Sound::pauseBGM( void)
{
	SimpleAudioEngine::getInstance() -> pauseBackgroundMusic();
}

void Sound::resumeBGM( void)
{
	SimpleAudioEngine::getInstance() -> resumeBackgroundMusic();
}

void Sound::restartBGM( void)
{
	SimpleAudioEngine::sharedEngine() -> rewindBackgroundMusic();
}

int Sound::playCheckBGM( void)
{
	if( SimpleAudioEngine::sharedEngine() -> isBackgroundMusicPlaying())
	{
		return 1;		// çƒê∂íÜ
	}
	else
	{
		return 0;
	}
}

int Sound::loadSE( const string& fileName)
{
	SoundData *data = new SoundData;
	string str = static_cast<string>(fileName);

	if( str.size() < 4) return -1;
	data -> filePath = FileAccess::getInstance() -> getSEPath( fileName).c_str();
	SimpleAudioEngine::getInstance() -> preloadEffect( data -> filePath.c_str());

	while( int point = str.rfind( '/', str.size()) != -1)
	{
		str.erase( 0, point);
	}
	data -> fileName = str;

	data -> ID = seDataList.size() - 1;

	seDataList.push_back( data);
	return 0;
}

int Sound::loadSE( const string& fileName, float Volume)
{
	SimpleAudioEngine::getInstance() -> setEffectsVolume( ( Volume / 100));
	if( Sound::getInstance() -> loadSE( fileName)) { return -1; }
	return 0;
}

int Sound::playSE( const string& fileName)
{
	for( int i = 0; i < seDataList.size(); i++)
	{
		if( seDataList[i] -> fileName == fileName)
		{
			SimpleAudioEngine::getInstance() -> playEffect( seDataList[i] -> filePath.c_str());
			return 0;
		}
	}
	return -1;
}

int Sound::playSE( const string& fileName, float Volume)
{
	SimpleAudioEngine::getInstance() -> setEffectsVolume( ( Volume / 100));
	if( Sound::getInstance() -> playSE( fileName)) { return -1; }
	return 0;
}

int Sound::setSEVolume( float Volume)
{
	SimpleAudioEngine::getInstance() -> setEffectsVolume( ( Volume / 100));
}

int Sound::stopSE( const string& fileName)
{
	for( int i = 0; i < seDataList.size(); i++)
	{
		if( seDataList[i] -> fileName == fileName)
		{
			SimpleAudioEngine::getInstance() -> stopEffect( seDataList[i] -> ID);
			return 0;
		}
	}
	return -1;
}

int Sound::pauseSE( const string& fileName)
{
	for( int i = 0; i < seDataList.size(); i++)
	{
		if( seDataList[i] -> fileName == fileName)
		{
			SimpleAudioEngine::getInstance() -> pauseEffect( seDataList[i] -> ID);
			return 0;
		}
	}
	return -1;
}


int Sound::resumeSE( const string& fileName)
{
	for( int i = 0; i < seDataList.size(); i++)
	{
		if( seDataList[i] -> fileName == fileName)
		{
			SimpleAudioEngine::getInstance() -> pauseEffect( seDataList[i] -> ID);
			return 0;
		}
	}
	return -1;
}

void Sound::stopSEAll( void)
{
	SimpleAudioEngine::getInstance() -> stopAllEffects();
}

void Sound::pauseSEAll( void)
{
	SimpleAudioEngine::getInstance() -> pauseAllEffects();
}

void Sound::resumeSEAll( void)
{
	SimpleAudioEngine::getInstance() -> resumeAllEffects();
}

int Sound::releaseSE( const string& fileName)
{
	for( int i = 0; i < seDataList.size(); i++)
	{
		if( seDataList[i] -> fileName == fileName)
		{
			SimpleAudioEngine::getInstance() -> unloadEffect( seDataList[i] -> filePath.c_str());
			return 0;
		}
	}
	return -1;
}

void Sound::releaseSEAll( void)
{
	for( int i = 0; i < seDataList.size(); i++)
	{
		SimpleAudioEngine::getInstance() -> unloadEffect( seDataList[i] -> filePath.c_str());
	}
	vector<SoundData*>().swap(seDataList);
}
