#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"

#include "Base/Sprite3D.h"

namespace TapGun
{
	class TitleScene : public cocos2d::Layer
	{
	public:
		/**
		*	�^�C�g���V�[���̍쐬
		*
		*	@author	minaka
		*	@param	�Ȃ�
		*	@return �쐬�����V�[���̃|�C���^
		*/
		static cocos2d::Scene* createScene( void);

		/**
		*	�^�C�g���V�[���̏�����
		*
		*	@author	minaka
		*	@param	�Ȃ�
		*	@return ������������ true ���s�� false
		*/
		bool init( void);

		/**
		*	�^�C�g���V�[���̍X�V
		*
		*	@author	minaka
		*	@param	delta ���݂̃Q�[���̎��s���� (�~���b)
		*	@return �Ȃ�
		*/
		void update( float delta);

		/**
		 *	�^�b�`���͂̎󂯎�� (�����ꂽ�Ƃ�)
		 *
		 *	@author	minaka
		 *	@param	cocos�ˑ��Ȃ̂ŏȗ�
		 *	@return �Ȃ�
		 */
		void onTouchEnded( cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

		/**
		 *	GameStart�̃R�[���o�b�N�֐�
		 *
		 *	@author	minaka
		 *	@param	cocos�ˑ��Ȃ̂ŏȗ�
		 *	@return �Ȃ�
		 */
		void menuStartCallback( cocos2d::Ref* pSender);

		/**
		 *	GameEnd�̃R�[���o�b�N�֐�
		 *
		 *	@author	minaka
		 *	@param	cocos�ˑ��Ȃ̂ŏȗ�
		 *	@return �Ȃ�
		 */
		void menuEndCallback( cocos2d::Ref* pSender);

		/**
		 *	�N���W�b�g�̃R�[���o�b�N�֐�
		 *
		 *	@author	minaka
		 *	@param	cocos�ˑ��Ȃ̂ŏȗ�
		 *	@return �Ȃ�
		 */
		void menuCreditCallback( cocos2d::Ref* pSender);

		CREATE_FUNC( TitleScene);

	private:
		enum SpriteName
		{
			BG,
			Logo,
			Frame,
			Menu,
			SpriteNum,
		};

		enum MenuFlag
		{
			TeamLogo	= ( 1 << 0),
			TitleLogoIn	= ( 1 << 1),
			TitleLogoOK	= ( 1 << 2),

			MenuIn	= ( 1 << 3),
			MenuOK	= ( 1 << 4),

			TitleEnd = ( 1 << 5),
		};

		enum TeamLogoState
		{
			LogoIn,
			LogoOut,
			Wait,
			WaitTime = 75,
			AlphaValue = 5,
		};

		enum MenuName
		{
			Start,
			End,
			Credit,
			MenuNum,
		};


		int logoWaitCount;
		TeamLogoState teamLogoState;

		MenuFlag menuFlag;
		bool menuActionFlag;
		int alphaCount;

		bool logoAlphaFlag;
		int logoAlphaCount;
		int logoAlphaWaitCount;

		cocos2d::Sprite* teamLogo;
		cocos2d::Sprite* sprite[SpriteNum];
		cocos2d::Menu* menu[MenuNum];

		/**
		 *	�`�[�����S�̕`�搧��
		 *
		 *	@author	minaka
		 *	@param	�Ȃ�
		 *	@return �Ȃ�
		 */
		void teamLogoAction( void);

		/**
		 *	�^�C�g���V�[���p�̃X�v���C�g�쐬
		 *
		 *	@author	minaka
		 *	@param	�Ȃ�
		 *	@return �Ȃ�
		 */
		void setSprite( void);

		/**
		 *	�^�C�g���V�[���p�̃��j���[�̍쐬
		 *
		 *	@author	minaka
		 *	@param	�Ȃ�
		 *	@return �Ȃ�
		 */
		void setMenu( void);

		/**
		 *	���j���[�̈ړ�����
		 *
		 *	@author	minaka
		 *	@param	�Ȃ�
		 *	@return �Ȃ�
		 *	@date	2/19 Ver 1.0
		 */
		void menuAction( void);

		/**
		 *	�e��摜�f�[�^�̓ǂݍ���
		 *
		 *	@author	minaka
		 *	@param	cocos�ˑ��Ȃ̂ŏȗ�
		 *	@return �Ȃ�
		 */
		void loadPicture( void);

		/**
		*	�e�탂�f���f�[�^�̓ǂݍ���
		*
		*	@author	minaka
		*	@param	cocos�ˑ��Ȃ̂ŏȗ�
		*	@return �Ȃ�
		*/
		void loadModels( void);

		/**
		 *	�e��T�E���h�f�[�^�̓ǂݍ���
		 *
		 *	@author	minaka
		 *	@param	cocos�ˑ��Ȃ̂ŏȗ�
		 *	@return �Ȃ�
		 *	@date	2/19 Ver 1.0
		 */
		void loadSound( void);


		/**
		 *	�t���O�ϐ����`�F�b�N
		 *
		 *	@author	minaka
		 *	@param	flag �t���O�ϐ�
		 *	@param	number ���ׂ�l
		 *	@return �t���Oon : true  �t���Ooff : false
		 */
		template<class P> static bool checkFlag( P* flag, const P number);

		/**
		 *	�t���O�ϐ��̃t���O��on�ɂ���
		 *
		 *	@author	minaka
		 *	@param	flag �t���O�ϐ�
		 *	@param	number on�ɂ���l
		 *	@return �Ȃ�
		 */
		template<class P> static void setFlag( P* flag, const P number);

		/**
		 *	�t���O�ϐ��̃t���O��off�ɂ���
		 *
		 *	@author	minaka
		 *	@param	flag �t���O�ϐ�
		 *	@param	number off�ɂ���l
		 *	@return �Ȃ�
		 */
		template<class P> static void resetFlag( P* flag, const P number);
	};
}

#endif // __TITLE_SCENE_H__
