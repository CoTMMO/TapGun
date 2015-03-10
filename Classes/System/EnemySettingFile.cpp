
#include <fstream>
#include "cocos2d.h"

#include "Platform.h"
#include "Stage/Enemy.h"
#include "System/EnemySettingFile.h"

#define TIME_CHANGER( time) ( time * 1000)	// �b�P�ʂ�time���~���b�P�ʂɕϊ�

USING_NS_CC;
using namespace std;
using namespace TapGun;

EnemySettingFile::EnemySettingFile()
{
}

EnemySettingFile* EnemySettingFile::create( const string& fileName)
{
	// �I�u�W�F�N�g����
	auto settingFile = new (nothrow) EnemySettingFile();
	// �t�@�C���p�X����N���X�̃C���X�^���X���擾
	auto access = FileAccess::getInstance();
	// �ݒ�t�@�C������̓X�g���[���ɐݒ�
	stringstream file( access -> getFileStream( access -> getEnemySettingFilePath( fileName)));

	// �G���[����
	if( file.fail())
	{
		log( "Animation File Load Error");
		return nullptr;
	}

	string str;							// �t�@�C������擾������������i�[
	EnemyData* data = new EnemyData;	// �ǂݍ��񂾏����i�[����\���̂𐶐�

	// �ǂݍ��ݍs�J�E���^�Ɠǂݍ��݃u���b�N�J�E���^�����������t�@�C���I�[�܂Ń��[�v
	for( settingFile -> fileLineCount = 1, settingFile -> loadCount = 0; getline( file, str); settingFile -> fileLineCount++)
	{
		string tmp;						// �ǂݍ��񂾕�����̈ꎞ�ۑ�
		istringstream stream(str);		// �������ҏW�\��
		Vec3 vector;					// �t�@�C���ɑ��݂���x�N�g�������ꎞ�ۑ�

		if( settingFile -> loadCount >= WAVE_ENEMY_COUNT)
		{
			log( "Enemy Seting File Block Num Over");
			return settingFile;
		}

		switch( settingFile -> fileLineCount)  // ���݂̍s�ԍ��ŏ����𕪊�
		{
		case 1:							// ��s�ɂ̓f�[�^�����݂��Ȃ��t�@�C���\���Ȃ̂ŃX�L�b�v
		case 3:
		case 5:
			break;

		case 2:
			// �E�F�[�u�i���o�[���擾
			// �J���}��؂�Ńf�[�^���擾 (�ȉ����������̓R�����g�ȗ�)
			getline( stream, tmp, ',');
			// �ǂݍ��񂾕�����𐔒l�ɕϊ����ۑ� (�ȉ��ގ������̓R�����g�ȗ�)
			data -> Num = atoi( tmp.c_str());

			// �o���n�_���W���擾
			// �f�[�^�擾
			getline( stream, tmp, ',');
			// ���f�[�^�ɑ��݂��� ""(" ��2�������폜
			tmp.erase( 0, 2);
			vector.x = atof( tmp.c_str());

			getline( stream, tmp, ',');
			vector.z = -atof( tmp.c_str());

			getline( stream, tmp, ',');
			vector.y = atof( tmp.c_str());

			// �擾�����f�[�^Vec3�`���Ŋi�[
			data -> startPos = vector;

			// �ҋ@���Ԃ��擾
			getline( stream, tmp, ',');
			data -> sleepTime = TIME_CHANGER( atoi( tmp.c_str()));

			// �ڕW�n�_���W���擾 (�ő�3����)
			for( int i = 0; i < TARGET_POS_COUNT; i++)
			{
				getline( stream, tmp, ',');
				// ���f�[�^�ɑ��݂��� ""(" ��2�������폜
				tmp.erase( 0, 2);
				vector.x = atof( tmp.c_str());
				getline( stream, tmp, ',');
				vector.z = -atof( tmp.c_str());
				getline( stream, tmp, ',');
				vector.y = atof( tmp.c_str());
				data -> targetPos[i] = vector;
			}

			for( int i = 0; i < WAIT_TO_ATTACK_COUNT; i++)
			{
				// �U���Ɉڂ�܂ł̑ҋ@���Ԃ��擾
				getline( stream, tmp, ',');
				data -> waitToAttack[i] = TIME_CHANGER( atoi( tmp.c_str()));
			}
			break;

		case 4:
			// �o�����t���O��ݒ�
			getline( stream, tmp, ',');
			if( tmp == "FALSE" || tmp == "false")
			{
				data -> entryFlag = false;
			}
			else if( tmp == "TRUE" || tmp == "true")
			{
				data -> entryFlag = true;
			}
			else if( atoi( tmp.c_str()) == 0)
			{
				data -> entryFlag = false;
			}
			else if( atoi( tmp.c_str()) == 1)
			{
				data -> entryFlag = true;
			}

			// ��A�̍s���I�����玟�̍s���J�n�܂ł̑ҋ@���Ԃ��擾
			for( int i = 0; i < WAIT_TO_MOVE_COUNT; i++)
			{
				getline( stream, tmp, ',');
				data -> waitToMove[i] = TIME_CHANGER( atoi( tmp.c_str()));
			}

			// �������|���ꂽ��ɏo������G�̔z��ԍ����擾
			for( int i = 0; i < NEXT_ENEMYS_COUNT; i++)
			{
				getline( stream, tmp, ',');
				data -> nextEnemies[i] = atoi( tmp.c_str());
			}
			break;

		case 6:
			// �퓬��','�܂ł̓f�[�^�����Ȃ̂ŋ�ǂ�
			getline( stream, tmp, ',');

			// �o�����̃��[�V���������擾
			getline( stream, tmp, ',');

			// �擾��������������ɔԍ����������i�[
			data -> aiAppear = getAIAppearNumber( tmp);

			// �ړ����̃��[�V���������擾���ݒ�
			for( int i = 0; i < AI_MOVE_COUNT; i++)
			{
				// �ړ����̃��[�V���������擾
				getline( stream, tmp, ',');
				// �擾��������������ɔԍ����������i�[
				data -> aiMove[i] = getAIMoveNumber( tmp);
			}

			// �U�����̃��[�V���������擾���ݒ�
			for( int i = 0; i < AI_ATTACK_COUNT; i++)
			{
				// �U�����̃��[�V���������擾
				getline( stream, tmp, ',');
				// �擾��������������ɔԍ����������i�[
				data -> aiAtk[i] = getAIAttackNumber( tmp);
			}

			// �U�����̃��[�V���������擾
			getline( stream, tmp, ',');
			// �擾��������������ɔԍ����������i�[
			data -> aiLifeCycle = getAILifeCycleNumber( tmp);
			break;

		default:
			break;
		}

		// 1�u���b�N���̃f�[�^��ǂݍ��񂾏ꍇ
		if( settingFile -> fileLineCount >= BLOCK_LINE_COUNT)
		{
			// �z��ɓǂݍ��񂾃f�[�^��ۑ�
			settingFile -> dataList[settingFile -> loadCount] = data;
			// �V�����������̈���m��
			data = new EnemyData;
			// �ǂݍ��݃u���b�N�J�E���^�𑝉�
			settingFile -> loadCount++;
			// �ǂݍ��ݍs�ԍ���������
			settingFile -> fileLineCount = 0;
		}
	}
	return settingFile;
}

int EnemySettingFile::getAIAppearNumber( const string& dataString)
{
	// ���[�V�������ɍ��킹�����l���i�[
	if( dataString == "run")
	{
		return AI_APPEAR_RUN;
	}
	else if( dataString == "walk")
	{
		return AI_APPEAR_WALK;
	}
	else if( dataString == "sWalk")
	{
		return AI_APPEAR_SWALK;
	}
	else if( dataString == "slide")
	{
		return AI_APPEAR_SLIDE;
	}
	else if( dataString == "acro")
	{
		return AI_ATK_ACRO;
	}
	else if( dataString == "fJump")
	{
		return AI_APPEAR_FJUMP;
	}
	else
	{
		return -999;
	}
}

int EnemySettingFile::getAIMoveNumber( const string& dataString)
{
	if( dataString == "none")
	{
		return AI_MOVE_NONE;
	}
	else if( dataString == "run")
	{
		return AI_MOVE_RUN;
	}
	else if( dataString == "walk")
	{
		return AI_MOVE_WALK;
	}
	else if( dataString == "sWalk")
	{
		return AI_MOVE_SWALK;
	}
	else if( dataString == "slide")
	{
		return AI_MOVE_SLIDE;
	}
	else if( dataString == "acro")
	{
		return AI_MOVE_ACRO;
	}
	else
	{
		return -999;
	}
}

int EnemySettingFile::getAIAttackNumber( const string& dataString)
{
	if( dataString == "none")
	{
		return AI_ATK_NONE;
	}
	else if( dataString == "fake")
	{
		return AI_ATK_FAKE;
	}
	else if( dataString == "shot")
	{
		return AI_ATK_SSHOT;
	}
	else if( dataString == "slide")
	{
		return AI_ATK_SLIDE;
	}
	else if( dataString == "acro")
	{
		return AI_MOVE_ACRO;
	}
	else
	{
		return -999;
	}
}

int EnemySettingFile::getAILifeCycleNumber( const string& dataString)
{
	if( dataString == "once")
	{
		return AI_LIFE_ONCE;
	}
	else if( dataString == "switch")
	{
		return AI_LIFE_SWITCH;
	}
	else if( dataString == "stop")
	{
		return AI_LIFE_STOP;
	}
	else
	{
		return -999;
	}
}
