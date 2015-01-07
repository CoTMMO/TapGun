
#include <iostream>
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "Errorfunc.h"
#include "Script.h"
#include "Sound.h"

#else

#include "C++/System/Errorfunc.h"
#include "C++/System/Script.h"
#include "C++/System/Sound.h"

#endif

USING_NS_CC;
using namespace std;
using namespace TapGun;

/**
 *	�X�N���v�g�Ǘ��N���X�̃C���X�^���X�擾
 *
 *	@author	minaka 
 *	@param	fileName �t�@�C����
 *	@return	����I��:0 �G���[����:-1
 *	@date	1/7 Ver 1.0
 */
Script* Script::getInstance( void)
{
	static Script* P;
	if( !P) P = new Script;
	return P;
}

/**
 *	�X�N���v�g�t�@�C���̓ǂݍ��݂Ɛݒ�
 *
 *	@author	minaka 
 *	@param	fileName �t�@�C����
 *	@param	layer �`��ݒ肷�郌�C���[
 *	@return	�G���[���b�Z�[�W
 *	@date	1/8 Ver 1.0
 */
string Script::loadScriptFile( const string& fileName, Layer* layer)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	string filePath = fileName + ".script";
#else
	string filePath = "Script/" + fileName + ".script";
#endif
	ifstream file( filePath, ios::in);
	if( file.fail()) 
	{
		string error = "�X�N���v�g�t�@�C�����J���܂��� �t�@�C��:" + fileName; 
		return error;
	}
	
	string str;
	lineCounter = 0;
	while( getline( file, str))
	{
		lineCounter++;
		string tmp;
		istringstream stream( str);
		getline( stream, tmp, ',');
		if( tmp == "Create" || tmp == "create")
		{
			string s = str;
			s.erase( 0, s.size() - ( s.size() - ( tmp.size() + 1)));
			functionNameCreate( s, layer);
		}
		else if( tmp == "Anime" || tmp == "anime")
		{
			string s = str;
			s.erase( 0, s.size() - ( s.size() - ( tmp.size() + 1)));
			functionNameAnime( s);
		}
		else if( tmp == "Print" || tmp == "print")
		{
			string s = str;
			s.erase( 0, s.size() - ( s.size() - ( tmp.size() + 1)));
			functionNamePrint( s, layer);
		}
		else if( tmp == "Play" || tmp == "play")
		{
			string s = str;
			s.erase( 0, s.size() - ( s.size() - ( tmp.size() + 1)));
			functionNamePlay( s);
		}
		else
		{
			string error = tmp + "���̖��߂͖������ł� ";
			string s = lineCounter + "�s�� �t�@�C��:" + fileName;;
			error = error + s;
			return error;
		}
	}
}

string Script::functionNameCreate( const string& scriptText, Layer* layer)
{
	FileNameFlag flag;
	string str;
	string command;
	string fileName = scriptText;
	int point = fileName.rfind( '.', fileName.size());

	if( point == string::npos)
	{
		flag = FileNameFlag::Text;
	}
	fileName.erase( 0, fileName.size() - ( fileName.size() - point));
	if( fileName == ".png" || fileName == ".jpg" || fileName == ".bmp")
	{
		flag = FileNameFlag::Picture;
	}
	else if( fileName == ".obj" || fileName == ".c3t" || fileName == ".c3b")
	{
		flag = FileNameFlag::Model;
	}
	else if( fileName == ".mp3" || fileName == ".wav")
	{
		flag = FileNameFlag::Sound;
	}
	else
	{
		string error = "�X�N���v�g�Ŏw�肳�ꂽ�t�@�C���͓ǂݍ��߂܂��� " + lineCounter;
		error = error + "�s�� �t�@�C��:" + fileName;
		return error;
	}

	istringstream stream( str);
	
	if( flag == FileNameFlag::Sound)
	{
		auto data = new InputSoundData;
		auto sound = Sound::getInstance();
		for( int i = 0; str.size() > 0; i++)
		{
			getline( stream, command, ',');
			str.erase( 0, str.size() - ( str.size() - ( command.size() + 1)));
			if( *str.data() == ',') break; // ','��2�q�����Ă����ꍇ�A�����I��
			
			if( i == 0) 
			{
				data -> dataName = command;
			}
			else
			{
				// Tag or Pos
				if( *command.data() == '#' || *command.data() == '��')
				{
					command.erase( 0, 1);
					data -> tag = command;
				}
				else
				{
					break;
				}
			}
		}
		sound -> loadSE( data -> dataName);
		soundList.push_back( data);
	}
	else
	{
		auto data = new InputSpriteData;
		for( int i = 0; str.size() > 0; i++)
		{
			getline( stream, command, ',');
			str.erase( 0, str.size() - ( str.size() - ( command.size() + 1)));
			if( *str.data() == ',') break; // ','��2�q�����Ă����ꍇ�A�����I��
			
			if( i == 0) 
			{
				data -> dataName = command;
			}
			else
			{
				if( *command.data() == '#' || *command.data() == '��')
				{
					command.erase( 0, 1);
					data -> tag = command;
				}
				else if( command.find( "posX", 0) != string::npos || command.find( "PosX", 0) != string::npos)
				{
					command.erase( 0, 5);
					data -> position.x = atof( command.c_str());
				}
				else if( command.find( "posY", 0) != string::npos || command.find( "PosY", 0) != string::npos)
				{
					command.erase( 0, 5);
					data -> position.y = atof( command.c_str());
				}
				else if( command.find( "posZ", 0) != string::npos || command.find( "PosZ", 0) != string::npos)
				{
					command.erase( 0, 5);
					data -> position.z = atof( command.c_str());
				}
				else if( command.find( "layer", 0) != string::npos || command.find( "Layer", 0) != string::npos)
				{
					command.erase( 0, 6);
					data -> layerNumber = atof( command.c_str());
				}
				else
				{
					break;
				}
			}
		}

		switch( flag)
		{
		case FileNameFlag::Text:
		{
			auto sprite = LabelTTF::create( data -> dataName, "Arial", 20);
			sprite -> setPosition( Vec2( data -> position.x, data -> position.y));
			layer -> addChild( sprite, data -> layerNumber);
			break;
		}
		case FileNameFlag::Picture:
		{
			auto sprite = Sprite::create( data -> dataName);
			sprite -> setPosition( Vec2( data -> position.x, data -> position.y));
			layer -> addChild( sprite, data -> layerNumber);
			break;
		}
		case FileNameFlag::Model:
		{
			auto sprite = Sprite3D::create( data -> dataName);
			sprite -> setPosition3D( data -> position);
			layer -> addChild( sprite, data -> layerNumber);
			break;
		}
		default:
			auto sprite = nullptr;
			break;
		};
		spriteList.push_back( data);
	}
}

string Script::functionNameAnime( const string& scriptText)
{
	return 0;
}

string Script::functionNamePrint( const string& scriptText, Layer* layer)
{
	return 0;
}

string Script::functionNamePlay( const string& scriptText)
{
	return 0;
}

