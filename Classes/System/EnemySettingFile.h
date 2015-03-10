
#ifndef __ENEMY_SETTING_FILE_H__
#define __ENEMY_SETTING_FILE_H__

#include "cocos2d.h"

namespace TapGun
{
	class EnemySettingFile
	{
	public:
		/**
		*	�G�ݒ�t�@�C���̓ǂݍ��݂Ɛݒ�p�I�u�W�F�N�g�̐���
		*
		*	@author	minaka
		*	@param	fileName �ݒ�t�@�C����
		*	@return	����I�� : ���������I�u�W�F�N�g�ւ̃|�C���^  ���s : nullptr
		*/
		static EnemySettingFile* create( const std::string& fileName);

	private:
		// �֐��錾�p (�ڍׂ͌���ɋL�q)
		struct EnemyData;

	public:
		enum _DATA_LIST_COUNT
		{
			WAVE_ENEMY_COUNT	= 20,	// 1wave���ɏo������G�̑���
		};

		unsigned int loadCount;			// �ǂݍ��݃f�[�^��
		EnemyData* dataList[WAVE_ENEMY_COUNT];	// �ǂݍ��񂾓G�f�[�^�̕ۑ��z��

	private:
		EnemySettingFile();

		/**
		*	�A�j���[�V���������w�������񂩂琔�l����������
		*
		*	@author	minaka
		*	@param	dataString ���A�j���[�V������
		*	@return	�������ʂ̐��l�@�����ł��Ȃ������ꍇ�� : -999
		*/
		static int getAIAppearNumber( const std::string& dataString);

		/**
		*	�A�j���[�V���������w�������񂩂琔�l����������
		*
		*	@author	minaka
		*	@param	dataString ���A�j���[�V������
		*	@return	�������ʂ̐��l�@�����ł��Ȃ������ꍇ�� : -999
		*/
		static int getAIMoveNumber( const std::string& dataString);

		/**
		*	�A�j���[�V���������w�������񂩂琔�l����������
		*
		*	@author	minaka
		*	@param	dataString ���A�j���[�V������
		*	@return	�������ʂ̐��l�@�����ł��Ȃ������ꍇ�� : -999
		*/
		static int getAIAttackNumber( const std::string& dataString);

		/**
		*	�A�j���[�V���������w�������񂩂琔�l����������
		*
		*	@author	minaka
		*	@param	dataString ���A�j���[�V������
		*	@return	�������ʂ̐��l�@�����ł��Ȃ������ꍇ�� : -999
		*/
		static int getAILifeCycleNumber( const std::string& dataString);

		enum _DATA_NUMBER_	// �e�f�[�^�̐�
		{
			BLOCK_LINE_COUNT		= 6,	// 1�u���b�N�̍s��

			TARGET_POS_COUNT		= 3,	// �ڕW�n�_��
			WAIT_TO_ATTACK_COUNT	= 3,	// �U���O�ҋ@���Ԑ�
			WAIT_TO_MOVE_COUNT		= 3,	// �U����ҋ@���Ԑ�
			NEXT_ENEMYS_COUNT		= 3,	// ���ɏo��������G�f�[�^��
			AI_MOVE_COUNT			= 2,	// �ړ����[�V������
			AI_ATTACK_COUNT			= 3,	// �U�����[�V������
		};

		struct EnemyData
		{
			// �f�[�^�ԍ�
			int Num;
			// �o���n�_
			cocos2d::Vec3 startPos;
			// �ҋ@����
			int sleepTime;
			// �ڕW�n�_
			cocos2d::Vec3 targetPos[TARGET_POS_COUNT];
			// �ړ���̑ҋ@����
			int waitToAttack[WAIT_TO_ATTACK_COUNT];
			// �o���t���O
			bool entryFlag;
			// �U����̑ҋ@����
			int waitToMove[WAIT_TO_MOVE_COUNT];
			// ���o��������G�̔ԍ�
			int nextEnemies[NEXT_ENEMYS_COUNT];
			// �o�ꎞ���[�V�����ԍ�
			int aiAppear;
			// �ړ����[�V�����ԍ�
			int aiMove[AI_MOVE_COUNT];
			// �U�����[�V�����ԍ�
			int aiAtk[AI_ATTACK_COUNT];
			// ���C�t�T�C�N���ԍ�
			int aiLifeCycle;
		};
		unsigned int fileLineCount;			// ���݂̍s�ԍ�
	};
}

#endif // __ENEMY_SETTING_FILE_H__
