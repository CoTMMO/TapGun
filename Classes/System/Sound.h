
#ifndef __SOUND_H__
#define __SOUND_H__

#include "cocos2d.h"

namespace TapGun
{
	class Sound
	{
	public:
		Sound( const Sound &P) = delete;
		Sound& operator= ( const Sound &P) = delete;

		/**
		 *	�T�E���h�Ǘ��N���X�̃C���X�^���X�擾
		 *
		 *	@author	minaka
		 *	@param	fileName �t�@�C����
		 *	@return	�C���X�^���X�ւ̃|�C���^
		 */
		static Sound* getInstance( void);

		/**
		 *	BGM�t�@�C���̓ǂݍ���
		 *
		 *	@author	minaka
		 *	@param	fileName �t�@�C����
		 *	@return	�Ȃ�
		 */
		void loadBGM( const std::string& fileName);

		/**
		 *	BGM�t�@�C���̓ǂݍ��݁i�{�����[�������t���j
		 *
		 *	@author	minaka
		 *	@param	fileName �t�@�C����
		 *	@param	Volume �{�����[�����x�� ( 0.0 ~ 100.0 )
		 *	@return	�Ȃ�
		 */
		void loadBGM( const std::string& fileName, float Volume);

		/**
		 *	BGM�̍Đ�
		 *
		 *	@author	minaka
		 *	@return	����I��:0�@�Đ����s:-1
		 */
		int playBGM( void);

		/**
		 *	BGM�̍Đ��i�{�����[�������t���j
		 *
		 *	@author	minaka
		 *	@param	Volume �{�����[�����x�� ( 0.0 ~ 100.0 )
		 *	@return	����I��:0�@�Đ����s:-1
		 */
		int playBGM( float Volume);

		/**
		 *	BGM�̃��[�v�Đ�
		 *
		 *	@author	minaka
		 *	@return	����I��:0�@�Đ����s:-1
		 */
		int playBGMLoop( void);

		/**
		 *	BGM�̃��[�v�Đ��i�{�����[�������t���j
		 *
		 *	@author	minaka
		 *	@param	Volume �{�����[�����x�� ( 0.0 ~ 100.0 )
		 *	@return	����I��:0�@�Đ����s:-1
		 */
		int playBGMLoop( float Volume);

		/**
		 *	BGM�̃{�����[���ݒ�
		 *
		 *	@author	minaka
		 *	@param	Volume �{�����[�����x�� ( 0.0 ~ 100.0 )
		 *	@return	�Ȃ�
		 */
		void setBGMVolume( float Volume);

		/**
		 *	BGM�̒�~
		 *
		 *	@author	minaka
		 *	@return	�Ȃ�
		 */
		void stopBGM( void);

		/**
		 *	BGM�̈ꎞ��~
		 *
		 *	@author	minaka
		 *	@return	�Ȃ�
		 */
		void pauseBGM( void);

		/**
		 *	BGM�̍ĊJ
		 *
		 *	@author	minaka
		 *	@return	�Ȃ�
		 */
		void resumeBGM( void);

		/**
		 *	BGM�̃��X�^�[�g
		 *
		 *	@author	minaka
		 *	@return	�Ȃ�
		 */
		void restartBGM( void);

		/**
		 *	BGM�̍Đ����`�F�b�N
		 *
		 *	@author	minaka
		 *	@return	�Đ���:1 �Đ����łȂ�:0
		 */
		int playCheckBGM( void);

		/**
		 *	SE�t�@�C���̓ǂݍ���
		 *
		 *	@author	minaka
		 *	@param	fileName �t�@�C����
		 *	@return	����I��:0 �G���[������:-1
		 */
		int loadSE( const std::string& fileName);

		/**
		 *	SE�t�@�C���̓ǂݍ��݁i�{�����[�������t���j
		 *
		 *	@author	minaka
		 *	@param	fileName �t�@�C����
		 *	@param	Volume �{�����[�����x�� ( 0.0 ~ 100.0 )
		 *	@return	����I��:0 �G���[������:-1
		 */
		int loadSE( const std::string& fileName, float Volume);

		/**
		 *	SE�̍Đ�
		 *
		 *	@author	minaka
		 *	@param	fileName �t�@�C����
		 *	@return	����I��:0 �G���[������:-1
		 */
		int playSE( const std::string& fileName);

		/**
		 *	SE�̍Đ��i�{�����[�������t���j
		 *
		 *	@author	minaka
		 *	@param	fileName �t�@�C����
		 *	@param	Volume �{�����[�����x�� ( 0.0 ~ 100.0 )
		 *	@return	����I��:0 �G���[������:-1
		 */
		int playSE( const std::string& fileName, float Volume);

		/**
		 *	SE�S�̂̃{�����[������
		 *
		 *	@author	minaka
		 *	@param	Volume �{�����[�����x�� ( 0.0 ~ 100.0 )
		 *	@return	�Ȃ�
		 */
		void setSEVolume( float Volume);

		/**
		 *	SE�̒�~
		 *
		 *	@author	minaka
		 *	@param	fileName �t�@�C����
		 *	@return	����I��:0 �G���[������:-1
		 */
		int stopSE( const std::string& fileName);

		/**
		 *	SE�̍ĊJ
		 *
		 *	@author	minaka
		 *	@param	fileName �t�@�C����
		 *	@return	����I��:0 �G���[������:-1
		 */
		int pauseSE( const std::string& fileName);

		/**
		 *	SE�̃��X�^�[�g
		 *
		 *	@author	minaka
		 *	@param	fileName �t�@�C����
		 *	@return	����I��:0 �G���[������:-1
		 */
		int resumeSE( const std::string& fileName);

		/**
		 *	�SSE�̒�~
		 *
		 *	@author	minaka
		 *	@return	�Ȃ�
		 */
		void stopSEAll( void);

		/**
		 *	�SSE�̍ĊJ
		 *
		 *	@author	minaka
		 *	@return	�Ȃ�
		 */
		void pauseSEAll( void);

		/**
		 *	�SSE�̃��X�^�[�g
		 *
		 *	@author	minaka
		 *	@return	�Ȃ�
		 */
		void resumeSEAll( void);

		/**
		 *	SE�p�̃��������
		 *
		 *	@author	minaka
		 *	@param	fileName �t�@�C����
		 *	@return	����I��:0 �G���[������:-1
		 */
		int releaseSE( const std::string& fileName);

		/**
		 *	�SSE�p�̃��������
		 *
		 *	@author	minaka
		 *	@return	�Ȃ�
		 */
		void releaseSEAll( void);

	private:
		struct SoundData			// SE�EVoice�Ǘ��p�\����
		{
			int ID;					// cocos2d?x����n�����ID
			std::string fileName;	// �A�N�Z�X�p�t�@�C����
			std::string filePath;	// �ǂݍ��ݗp�f�B���N�g���p�X
		};

		Sound();

		// ���ݓǂݍ��܂�Ă����BGM�̃t�@�C����
		std::string bgmFileName;
		// �ǂݍ��܂�Ă���SE�̃��X�g
		std::vector<SoundData*> seDataList;
	};
}

#endif // __SOUND_H__
