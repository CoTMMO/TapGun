
#include <fstream>
#include "cocos2d.h"

#include "Platform.h"
#include "Stage/Enemy.h"
#include "System/EnemySettingFile.h"

USING_NS_CC;
using namespace std;
using namespace TapGun;

/**
*	�G�ݒ�t�@�C���̓ǂݍ��݂Ɛݒ�p�I�u�W�F�N�g�̐���
*
*	@author	minaka
*	@param	fileName �ݒ�t�@�C����
*	@return	����I�� : ���������I�u�W�F�N�g�ւ̃|�C���^  ���s : nullptr
*/
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
	
	string str;						// �t�@�C������擾������������i�[
	string tmp;						// �ǂݍ��񂾕�����̈ꎞ�ۑ�
	istringstream stream(str);		// �������ҏW�\��
	WaveData* data = new WaveData;	// �ǂݍ��񂾏����i�[����\���̂𐶐�
	Vec3 vector;					// �t�@�C���ɑ��݂���x�N�g�������ꎞ�ۑ�

	// �ǂݍ��ݍs�J�E���^�Ɠǂݍ��݃u���b�N�J�E���^�����������t�@�C���I�[�܂Ń��[�v
	for( settingFile -> fileLineCount = 1, settingFile -> loadCount = 0; getline( file, str); settingFile -> fileLineCount++)
	{
		// 1�u���b�N���̃f�[�^��ǂݍ��񂾏ꍇ
		if( settingFile -> fileLineCount > BLOCK_LINE_COUNT)
		{
			// �z��ɓǂݍ��񂾃f�[�^��ۑ�
			settingFile -> dataList[settingFile -> loadCount] = data;
			// �ǂݍ��݃u���b�N�J�E���^�𑝉�
			settingFile -> loadCount++;
			// �ǂݍ��ݍs�ԍ���������
			settingFile -> fileLineCount = 1;
		}

		if( settingFile -> loadCount >= WAVE_ENEMY_COUNT)
		{
			log( "Enemy Seting File Block Num Over");
			return settingFile;
		}

		switch( settingFile -> fileLineCount)			// ���݂̍s�ԍ��ŏ����𕪊�
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
			vector.x = atoi( tmp.c_str());

			getline( stream, tmp, ',');
			vector.y = atoi( tmp.c_str());

			getline( stream, tmp, ',');
			vector.z = atoi( tmp.c_str());

			// �擾�����f�[�^Vec3�`���Ŋi�[
			data -> startPos = vector;

			// �ҋ@���Ԃ��擾
			getline( stream, tmp, ',');
			data -> sleepTime = atoi( tmp.c_str());

			// �ڕW�n�_���W���擾 (�ő�3����)
			for( int i = 0; i < TARGET_POS_COUNT; i++)
			{
				getline( stream, tmp, ',');
				// ���f�[�^�ɑ��݂��� ""(" ��2�������폜
				tmp.erase( 0, 2);
				vector.x = atoi( tmp.c_str());
				getline( stream, tmp, ',');
				vector.y = atoi( tmp.c_str());
				getline( stream, tmp, ',');
				vector.z = atoi( tmp.c_str());
				data -> targetPos[i] = vector;
			}

			// �U���Ɉڂ�܂ł̑ҋ@���Ԃ��擾
			getline( stream, tmp, ',');
			data -> waitToAtack = atoi( tmp.c_str());
			break;

		case 4:
			// �퓬��','�܂ł̓f�[�^�����Ȃ̂ŋ�ǂ�
			getline( stream, tmp, ',');

			// ��A�̍s���I�����玟�̍s���J�n�܂ł̑ҋ@���Ԃ��擾
			for( int i = 0; i < WAIT_TO_MOVE_COUNT; i++)
			{
				getline( stream, tmp, ',');
				data -> waitToMove[i] = atoi( tmp.c_str());
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
			data -> aiAppear = getModelNumber( tmp);

			// �ړ����̃��[�V���������擾���ݒ�
			for( int i = 0; i < AI_MOVE_COUNT; i++)
			{
				// �ړ����̃��[�V���������擾
				getline( stream, tmp, ',');
				// �擾��������������ɔԍ����������i�[
				data -> aiMove[i] = getModelNumber( tmp);
			}

			// �U�����̃��[�V���������擾���ݒ�
			for( int i = 0; i < AI_ATTACK_COUNT; i++)
			{
				// �U�����̃��[�V���������擾
				getline( stream, tmp, ',');
				// �擾��������������ɔԍ����������i�[
				data -> aiAtk[i] = getModelNumber( tmp);
			}

			// �U�����̃��[�V���������擾
			getline( stream, tmp, ',');
			// �擾��������������ɔԍ����������i�[
			data -> aiLifeCycle = getModelNumber( tmp);
			break;

		default:
			break;
		}
	}
	return settingFile;
}

/**
*	�A�j���[�V���������w�������񂩂琔�l����������
*
*	@author	minaka
*	@param	dataString ���A�j���[�V������
*	@return	�������ʂ̐��l�@�����ł��Ȃ������ꍇ�� : -999
*/
int EnemySettingFile::getModelNumber( const string& dataString)
{
	// ���[�V�������ɍ��킹�����l���i�[

	//��������o��
	if( dataString == "run")
	{
		return AI_APPEAR_RUN;
	}
	else if (dataString == "walk")
	{
		return AI_APPEAR_WALK;
	}
	else if (dataString == "sWalk")
	{
		return AI_APPEAR_SWALK;
	}
	else if (dataString == "slide")
	{
		return AI_APPEAR_SLIDE;
	}
	else if (dataString == "acro")
	{
		return AI_ATK_ACRO;
	}
	else if (dataString == "fJump")
	{
		return AI_APPEAR_FJUMP;
	}

	//��������ړ�
	else if (dataString == "none")
	{
		return AI_MOVE_NONE;
	}
	else if (dataString == "run")
	{
		return AI_MOVE_RUN;
	}
	else if (dataString == "walk")
	{
		return AI_MOVE_WALK;
	}
	else if (dataString == "sWalk")
	{
		return AI_MOVE_SWALK;
	}
	else if (dataString == "slide")
	{
		return AI_MOVE_SLIDE;
	}
	else if (dataString == "acro")
	{
		return AI_MOVE_ACRO;
	}

	//��������U��
	else if (dataString == "none")
	{
		return AI_ATK_NONE;
	}
	else if (dataString == "fake")
	{
		return AI_ATK_FAKE;
	}
	else if (dataString == "shot")
	{
		return AI_ATK_SSHOT;
	}
	else if (dataString == "slide")
	{
		return AI_ATK_SLIDE;
	}
	else if (dataString == "acro")
	{
		return AI_MOVE_ACRO;
	}

	//�������烉�C�t�T�C�N��
	else if (dataString == "once")
	{
		return AI_LIFE_ONCE;
	}
	else if (dataString == "switch")
	{
		return AI_LIFE_SWITCH;
	}
	else if (dataString == "stop")
	{
		return AI_LIFE_STOP;
	}

	else
	{
		return -999;
	}
}