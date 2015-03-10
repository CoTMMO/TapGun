
#ifndef __RESOURCE_LOADER_SCENE_H__
#define __RESOURCE_LOADER_SCENE_H__

#include "cocos2d.h"

#include "Base/Sprite3D.h"

namespace TapGun
{
	class ResourceLoader
	{
	public:
		enum ModelNumber		// �񓯊��ǂݍ��ݔz��̋�؂�ԍ�
		{
			Map = 0,
			EnemyStart = 1,
			EnemyEnd = 20,
			BulletStart = 21,
			BulletEnd = 70,
			Player = 71,
			ModelMax,
		};

		ResourceLoader( const ResourceLoader& p) = delete;
		ResourceLoader& operator=( const ResourceLoader& P) = delete;

		/**
		 *	3D���f���̔񓯊��ǂݍ��݃I�u�W�F�N�g�̃C���X�^���X�̐���
		 *
		 *	@author	minaka
		 *	@param	�Ȃ�
		 *	@return	���������I�u�W�F�N�g�ւ̃|�C���^
		 */
		static ResourceLoader* getInstance( void);

		/**
		 *	3D���f���̔񓯊��ǂݍ���
		 *
		 *	@author	minaka
		 *	@param	fileName �ǂݍ��݂������f���̃t�@�C���p�X
		 *	@return	�Ȃ�
		 */
		void loadModel( const std::string& fileName);

		/**
		 *	3D���f���̔񓯊��ǂݍ���
		 *
		 *	@author	minaka
		 *	@param	fileName �ǂݍ��݂������f���̃t�@�C���p�X
		 *	@param	texturePath �ǂݍ��݂����e�N�X�`���̃t�@�C���p�X
		 *	@return	�Ȃ�
		 */
		void loadModel( const std::string& modelPath, const std::string& texturePath);

		/**
		 *	�ǂݍ��ݍς�3D���f���̃|�C���^�擾
		 *
		 *	@author	minaka
		 *	@param	count �A�N�Z�X���������f���̔ԍ� ����̓I�Ȕԍ��̓w�b�_�[��ModelNumber���Q��
		 *	@return	�ǂݍ��ݍς� : ���̂ւ̃|�C���^�@�ǂݍ��܂�Ă��Ȃ� : nullptr
		 */
		_Sprite3D* getSprite3D( int count);

	private:
		enum ModelTypeCount
		{
			TypeMap,
			TypeEnemy,
			TypeBullet,
			TypePlayer,
			TypeCountNum,
		};

		/**
		 *	�����o�ϐ��̏�����
		 *
		 *	@author	minaka
		 *	@param	�Ȃ�
		 */
		ResourceLoader();

		/**
		 *	�񓯊��ǂݍ��݊������f����z��Ɋi�[
		 *
		 *	@author	minaka
		 *	@param	sprite3D �ǂݍ��񂾃��f���f�[�^�ւ̃|�C���^
		 *	@param	param �f�[�^�̐e�|�C���^
		 *	@param	fileName �ǂݍ��񂾃��f���̃t�@�C����
		 *	@return	�ǂݍ��ݍς� : ���̂ւ̃|�C���^�@�ǂݍ��܂�Ă��Ȃ� : nullptr
		 */
		void sprite3DLoadCallback( _Sprite3D* sprite3D, void* param, const std::string& fileName);

		bool loadFlag[ModelMax];		// �ǂݍ��݊�������t���O
		int typeCount[TypeCountNum];	// ���f���ʓǂݍ��ݐ��J�E���g
		_Sprite3D* models[ModelMax];	// �ǂݍ��񂾃��f���ւ̃|�C���^
	};
}

#endif // __RESOURCE_LOADER_SCENE_H__
