
#include "GameUILayer.h"

#include "Base/GameMaster.h"
#include "System/Sound.h"

USING_NS_CC;
using namespace TapGun;

/**
 *	�Q�[���{�҂�UI���C���[��������
 *
 *	@author	minaka
 *	@param	�Ȃ�
 *	@return	������������ true ���s�� false
 */
bool GameUILayer::init()
{
	if( !Layer::init()) { return false; }
	return true;
}

/**
 *	�e��UI��������
 *
 *	@author	minaka
 *	@param	�Ȃ�
 *	@return	�Ȃ�
 */
void GameUILayer::InitLayer( void)
{
	initHPGauge();
	initBulletCounter();
	initOtherUI();
}

/**
 *	UI�̍X�V
 *
 *	@author	minaka
 *	@param	�Ȃ�
 *	@return	�Ȃ�
 */
void GameUILayer::UpdateLayer( void)
{
	auto master = GameMaster::GetInstance();

	setLogo();
	setHP( master -> GetPlayerHP());
	setGameTime( master -> GetGameTime());
	setBulletCount( master -> GetPlayerBullets());
	setReticlePoint();
}

/**
*	�e���S�̕\������
*
*	@author	minaka
*	@param	�Ȃ�
*	@return	�Ȃ�
*	@date	2/26 Ver 1.0
*/
void GameUILayer::setLogo( void)
{
	auto master = GameMaster::GetInstance();
	static bool waitFlag = false;
	static int waitTime = 0;

	auto sound = Sound::getInstance();
	static bool bgmFlag = false;

	if( master -> GetGameState() == GSTATE_WAIT)
	{
		logo[ReloadLogo] -> setVisible( false);
		if( bgmFlag == false && waitTime == 70)
		{
			// �K���ɑ҂��Ă���BGM���Đ�
			sound -> playBGMLoop();
			bgmFlag = true;
		}
		if( waitTime % 30 == 0 || waitTime == 0)
		{
			waitFlag = !waitFlag;
			logo[WaitLogo] -> setVisible( waitFlag);
		}
		escape[0] -> setOpacity( 25);
		escape[1] -> setOpacity( 25);
		waitTime++;
	}
	else if( master -> GetGameState() == GSTATE_PLAY_SET)
	{
		waitFlag = false;
		logo[WaitLogo] -> setVisible( false);
	}
	else if( master -> GetGameState() == GSTATE_PLAY_ACTION)
	{
		logo[ActionLogo] -> setVisible( true);
		if( master -> playerSide == PSIDE_LEFT)
		{
			escape[0] -> setOpacity( 255);
		}
		else
		{
			escape[1] -> setOpacity( 255);
		}
	}
	else if( master -> GetGameState() == GSTATE_PLAY)
	{
		logo[ActionLogo] -> setVisible( false);
	}
	else if(master->GetGameState() == GSTATE_CLEAR)
	{
		logo[WaitLogo]->setVisible(false);
	}

	//�ǉ��F�E�F�[�u�����c�e�؂�̎��̂݃����[�hUI��\��
	if(master->GetGameState() == GSTATE_PLAY && master->GetPlayerBullets() == 0)
	{
		logo[ReloadLogo] -> setVisible( true);
	}
	else
	{
		logo[ReloadLogo] -> setVisible( false);
	}
}

/**
*	HP�Q�[�W�̍X�V����
*
*	@author	minaka
*	@param	�Ȃ�
*	@return	�Ȃ�
*	@date	2/26 Ver 1.0
*/
void GameUILayer::setHP( int count)
{
	for( int i = 0; i < 6; i++)
	{
		hp[i + 2] -> setVisible( false);
	}
	for( int i = count; i > 0; i--)
	{
		hp[i + 1] -> setVisible( true);
	}
}

/**
*	HP�Q�[�W���̃^�C�}�[�X�V����
*
*	@author	minaka
*	@param	�Ȃ�
*	@return	�Ȃ�
*	@date	2/26 Ver 1.0
*/
void GameUILayer::setGameTime( float time)
{
	auto master = GameMaster::GetInstance();

	// 1/100�b�̐���t���O
	static bool timeFlag = false;
	// 1/10�b�ȉ��̐���J�E���^
	static int frame = 0;

	int timer = time * 0.001f;
	int count100 = timer / 100;
	int count10 = ((int)timer % 100);
	int count1 = count10 - ((count10 / 10) * 10);
	static int count01 = 0;
	static int count001 = 0;

	for( int i = 0; i < 10; i++)
	{
		if( timer / 100 == i) { continue; }
		timeNumber[0][i] -> setVisible( false);
	}
	timeNumber[0][(int)timer / 100] -> setVisible( true);

	for( int i = 0; i < 10; i++)
	{
		if( count10 / 10 == i) { continue; }
		timeNumber[1][i] -> setVisible( false);
	}
	timeNumber[1][count10 / 10] -> setVisible( true);

	for( int i = 0; i < 10; i++)
	{
		if( count1 / 10 == i) { continue; }
		timeNumber[2][i] -> setVisible( false);
	}
	timeNumber[2][count1] -> setVisible( true);

	if( master -> GetGameState() == GSTATE_PLAY)
	{
		if( frame % 6 == 0)
		{
			for( int i = 0; i < 10; i++)
			{
				if( count01 / 10 == i) { continue; }
				timeNumber[3][i] -> setVisible( false);
			}
			timeNumber[3][count01] -> setVisible( true);
			if( count01 == 0) { count01 = 10; }
			count01--;
		}
		if( timeFlag)
		{
			for( int i = 0; i < 10; i++)
			{
				if( count001 / 10 == i) { continue; }
				timeNumber[4][i] -> setVisible( false);
			}
			timeNumber[4][count001] -> setVisible( true);
			if( count001 == 1) { count001 = 10; timeFlag = !timeFlag; }
			count001 -= 2;
		}
		else
		{
			for( int i = 0; i < 10; i++)
			{
				if( count001 / 10 == i) { continue; }
				timeNumber[4][i] -> setVisible( false);
			}
			timeNumber[4][count001] -> setVisible( true);
			if( count001 == 0) { count001 = 11; timeFlag = !timeFlag; }
			count001 -= 2;
		}
		frame++;
	}
	else if(master->GetGameState() == GSTATE_TIMEOVER)//�ǉ�
	{
		//�^�C���I�[�o�[���̓R���}�ȉ��̐�����0�ɂ���
		for(int i = 0; i < 10; i++)
		{
			if(count01 / 10 == i) { continue; }
			timeNumber[3][i]->setVisible(false);
		}
		timeNumber[3][0]->setVisible(true);

		for(int i = 0; i < 10; i++)
		{
			if(count001 / 10 == i) { continue; }
			timeNumber[4][i]->setVisible(false);
		}
		timeNumber[4][0]->setVisible(true);
	}
}

/**
*	�c�e���l�̍X�V����
*
*	@author	minaka
*	@param	�Ȃ�
*	@return	�Ȃ�
*	@date	2/26 Ver 1.0
*/
void GameUILayer::setBulletCount( int count)
{
	int Bcount = count - ((count / 10) * 10);

	for( int i = 0; i < 10; i++)
	{
		if( count / 10 == i) { continue; }
		bulletNumber[0][i] -> setVisible( false);
	}
	bulletNumber[0][count / 10] -> setVisible( true);

	for( int i = 0; i < 10; i++)
	{
		if( Bcount == i) { continue; }
		bulletNumber[1][i] -> setVisible( false);
	}
	bulletNumber[1][Bcount] -> setVisible( true);
}


void GameUILayer::initHPGauge( void)
{
	// HP�Q�[�W�̐e�ƂȂ�m�[�h���쐬
	hpParent = Node::create();
	// HP�Q�[�W�̘g��ǂݍ���
	hp[HPFrame] = Sprite::createWithSpriteFrameName( "HPFrame.png");
	// HP�Q�[�W�̔w�i��ǂݍ���
	hp[HPBG] = Sprite::createWithSpriteFrameName( "HPGaugeBG.png");

	// �Q�[�W�{�̕�����ǂݍ��ݔz��Ɋ��蓖��
	for( int i = 2, k = HPGaugeCount; i < HPGaugeCount; i++, k--)
	{
		// �t�@�C�����p�o�b�t�@
		char buf[64];
		// �t�@�C�����𐶐�
		sprintf( buf, "HPGauge_%02d.png", i - 1);
		// �z��̍Ō�̗v�f���珇�Ԃɉ摜�f�[�^��ۑ��@(��̑����̏����ȗ����̂���)
		hp[k - 1] = Sprite::createWithSpriteFrameName( buf);
	}

	// �S�Ẳ摜�f�[�^�ɃA�N�Z�X
	for( auto &p : hp)
	{
		// �`��ʒu�ݒ�
		p -> setPosition( Vec2( HPPositionX, HPPositionY));
		// �\���T�C�Y��ݒ�
		p -> setScale( 0.8);

		p -> retain();
		// �e�m�[�h�ɐڑ�
		hpParent -> addChild( p, 2);
	}

	hpParent -> retain();

	// �e�m�[�h�����C���[�ɐڑ�
	addChild( hpParent);

	// �Q�[�W���̃^�C�}�[�̐e�ƂȂ�m�[�h���쐬
//	timeParent = Node::create();

	// �`�悷�鐔����������
	for( int i = 0; i < TimeCount; i++)
	{
		// 0 ~ 9�܂ł̐��l��������
		for( int k = 0; k < 10; k++)
		{
			// �t�@�C�����p�o�b�t�@
			char buf[64];
			// �t�@�C�����𐶐�
			sprintf( buf, "%d.png", k);
			// �������Ƃ�0 ~ 9�܂ł̃X�v���C�g�𐶐�
			timeNumber[i][k] = Sprite::createWithSpriteFrameName( buf);
			// �\������T�C�Y��ݒ�
			timeNumber[i][k] -> setScale( 0.16f);
			// �ŏ��͑S�Ĕ�\����
			timeNumber[i][k] -> setVisible( false);

			timeNumber[i][k] -> retain();
			// �^�C�}�[�̐e�ɐڑ�
			
			addChild( timeNumber[i][k]);
		}
	}
		
	for( int i = 0; i < 10; i++)
	{
		timeNumber[0][i] -> setPosition( Vec2( 560, 700));
		timeNumber[1][i] -> setPosition( Vec2( 600, 700));
		timeNumber[2][i] -> setPosition( Vec2( 640, 700));
		timeNumber[3][i] -> setPosition( Vec2( 690, 695));
		timeNumber[4][i] -> setPosition( Vec2( 720, 695));
		timeNumber[3][i] -> setScale( 0.12f);
		timeNumber[4][i] -> setScale( 0.12f);
	}

	timeNumber[0][3] -> setVisible( true);
	timeNumber[1][0] -> setVisible( true);
	timeNumber[2][0] -> setVisible( true);
	timeNumber[3][0] -> setVisible( true);
	timeNumber[4][0] -> setVisible( true);


//	timeParent -> retain();

	// �e�m�[�h�����C���[�ɐڑ�
//	addChild( timeParent);
}

void GameUILayer::initBulletCounter( void)
{
	// �c�e�A�C�R���̐e�ƂȂ�m�[�h�𐶐�
	bulletParent = Node::create();
	// �c�e�A�C�R����ǂݍ���
	bullet = Sprite::createWithSpriteFrameName( "BulletFrame.png");
	// �\���ʒu��ݒ�
	bullet -> setPosition( Vec2( BulletFrameX, BulletFrameY));
	// �\���T�C�Y��ݒ�
	bullet -> setScale( 0.15f);
	// �e�m�[�h�ɐڑ�
	bulletParent -> addChild( bullet, 1);

	// �`�悷�鐔����������
	for( int i = 0; i < BulletCount; i++)
	{
		// 0 ~ 9�܂ł̐��l��������
		for( int k = 0; k < 10; k++)
		{
			// �t�@�C�����p�o�b�t�@
			char buf[64];
			// �t�@�C�����𐶐�
			sprintf( buf, "%d_.png", k);
			// �������Ƃ�0 ~ 9�܂ł̃X�v���C�g�𐶐�
			bulletNumber[i][k] = Sprite::createWithSpriteFrameName( buf);
			// �\������T�C�Y��ݒ�
			bulletNumber[i][k] -> setScale( 0.15f);
			// �ŏ��͑S�Ĕ�\����
			bulletNumber[i][k] -> setVisible( false);
			// �c�e�A�C�R���̐e�m�[�h�ɐڑ�
			bulletParent -> addChild( bulletNumber[i][k], 3);
		}
	}

	// 0 ~ 9�܂ł̐��l���̐ݒ�
	for( int i = 0; i < 10; i++)
	{
		bulletNumber[0][i] -> setPosition( Vec2( 580, 55));
		bulletNumber[1][i] -> setPosition( Vec2( 615, 55));

		// �ȉ��̓}�K�W���̑��e���p�̂��ߏ������\������悤�ɐݒ�
		bulletNumber[2][i] -> setScale( 0.12f);
		bulletNumber[2][i] -> setPosition( Vec2( 670, 50));
		bulletNumber[3][i] -> setScale( 0.12f);
		bulletNumber[3][i] -> setPosition( Vec2( 700, 50));
	}

	// �����ݒ�̑��e���� 30 �Ȃ̂� 3 �� 0 �̂ݕ\��
	bulletNumber[2][3] -> setVisible( true);
	bulletNumber[3][0] -> setVisible( true);

	bulletParent -> retain();

	// �e�m�[�h�����C���[�ɐڑ�
	addChild( bulletParent);
}


void GameUILayer::initOtherUI( void)
{
	// ��ʃT�C�Y���擾
	auto visibleSize = Director::getInstance() -> getVisibleSize();

	// ����A�C�R����������
	for( int i = 0; i < EscapeCount; i++)
	{
		// �X�v���C�g�𐶐�
		escape[i] = Sprite::createWithSpriteFrameName( "escape.png");
		// �\���T�C�Y��ύX
		escape[i] -> setScale( 0.4f);
		// ���C���[�ɐڑ�
		addChild( escape[i]);
	}
	// ����A�C�R���̕\���ʒu�ݒ� (���A�C�R��)
	escape[EscapeLeft] -> setPosition( Vec2( 130, 130));
	// ����A�C�R���̕\���ʒu�ݒ� (�E�A�C�R��)
	escape[EscapeRight] -> setPosition( Vec2( 1150, 130));
	// �E���̃A�C�R���p�ɍ��E���]�A�N�V�����𐶐�
	auto flip = FlipX::create(true);
	// �E���̃A�C�R���̂ݍ��E���]
	escape[EscapeRight] -> runAction(flip);

	// �d�l�ύX�̂��߃J�b�g
/*	pause = Sprite::create();

	for( int i = 0; i < EnemyAttackCount; i++)
	{
		for( int k = 0; k < 3; k++)
		{
			char buf[64];
			sprintf( buf, "enemy_attack_%02d.png", k + 1);
			enemyAttack[i][k] = Sprite::createWithSpriteFrameName( buf);
			enemyAttack[i][k] -> setVisible( false);
			addChild( enemyAttack[i][k]);
		}
	}

	for( int i = 0; i < DeleteCount; i++)
	{
		deleteLogo[i] = Sprite::createWithSpriteFrameName( "derete.png");
		deleteLogo[i] -> setVisible( false);
		addChild( deleteLogo[i]);
	}
*/

	// �Q�[�����Ŏg�p���郍�S�𐶐�
	logo[ActionLogo] = Sprite::createWithSpriteFrameName( "action.png");
	logo[WaitLogo] = Sprite::createWithSpriteFrameName( "wait.png");
	logo[ReloadLogo] = Sprite::createWithSpriteFrameName( "rerode.png");

	// �e���S�̏�����
	for( auto &p : logo)
	{
		// �\���ʒu��ݒ�
		p -> setPosition( 1280 / 2, 250);
		// �\���T�C�Y��ݒ�
		p -> setScale( 0.3f);
		// ��\����Ԃɐݒ�
		p -> setVisible( false);
		// ���C���[�ɐڑ�
		addChild( p);
	}

	// ���e�B�N���̐���t���O��������
	// �E�F�[�u���ł̏��ˌ��Ǘ��t���O
	firstShotFlag = false;
	// ��ˌ���Ԃ���̕��A�t���O
	returnEscapeFlag = false;
	// ���A��̏��ˌ��Ǘ��t���O
	returnEscapeNoShotFlag = false;

	// �A�C�h����ԃ��e�B�N����ǂݍ���
	reticle[ReticleIdle] = Sprite::createWithSpriteFrameName( "reticle_idle.png");
	// �ˌ���ԃ��e�B�N����ǂݍ���
	reticle[ReticleShot] = Sprite::createWithSpriteFrameName( "reticle_shot.png");
	// �A�C�h����Ԏ��̓����x��ݒ� (��20%)
	reticle[ReticleIdle] -> setOpacity( 50);

	// �e���e�B�N���̏�����
	for( auto &p : reticle)
	{
		// �\���ʒu�̐ݒ�
		p -> setPosition( Vec2( visibleSize.width / 2, visibleSize.height / 2));
		// �\���T�C�Y�̐ݒ�
		p -> setScale( 0.5f);
		// ��\����Ԃɐݒ�
		p -> setVisible( false);
		// ���C���[�ɐڑ�
		addChild( p);
	}
}



void GameUILayer::setReticlePoint( void)
{
	// �^�b�`���ꂽ���W��ۑ�
	static Vec2 pos;
	// ��ʃT�C�Y���擾
	auto visibleSize = Director::getInstance() -> getVisibleSize();
	// �Q�[���}�X�^�[�̃C���X�^���X���擾
	auto master = GameMaster::GetInstance();

	// �E�F�[�u���ȊO�̓��e�B�N����\��
	if( master -> GetGameState() != GSTATE_PLAY && master -> GetGameState() != GSTATE_PLAY_ACTION)
	{
		// �S�Ĕ�\����Ԃɐݒ�
		for( auto &p : reticle) { p -> setVisible( false); }
		// �����I��
		return;
	}

	// ��ʂ��^�b�`����Ă��邩��ʏ���Ȃ����Ă���ꍇ
	if( master -> GetTouchState() == TSTATE_ON || master -> GetTouchState() == TSTATE_MOVE)
	{
		// ���ˌ��t���O�𗧂Ă�
		firstShotFlag = true;
		// �^�b�`���W�����[�J���ɕۑ�
		pos = master -> GetTouchPos();
	}

	// �v���C���[���ˌ����܂��̓A�C�h����Ԃł��鎞�A1�x�ł��ˌ����Ă����ꍇ���e�B�N�����ˌ���ԂɕύX
	if( ( master -> GetPlayerState() == PSTATE_SHOT || master -> GetPlayerState() == PSTATE_IDLE)
		&& firstShotFlag)// && !returnEscapeFlag)
	{
		// �����Ԃ���̕��A���ˌ��O
		if( returnEscapeNoShotFlag)
		{
			// �\�����e�B�N�����A�C�h����ԂɕύX
			reticle[ReticleShot] -> setVisible( false);
			reticle[ReticleIdle] -> setVisible( true);
			// ��ʂ��^�b�`����Ă��邩��ʏ���Ȃ����Ă���ꍇ
			if( master -> GetTouchState() == TSTATE_ON || master -> GetTouchState() == TSTATE_MOVE)
			{
				// ���A��ˌ��O�t���O��|��
				returnEscapeNoShotFlag = false;
			}
		}
		else
		{
			// �ˌ����̏ꍇ

			// �\�����e�B�N�����V���b�g��ԂɕύX
			reticle[ReticleIdle] -> setVisible( false);
			reticle[ReticleShot] -> setVisible( true);

			// ���e�B�N���\���ʒu���^�b�`�ʒu�̏�ɐݒ�
			reticle[ReticleShot] -> setPosition( pos.x, pos.y + visibleSize.height * master -> reticleAjust);

			// ���e�B�N������ʊO�֏o��ꍇ
			if( reticle[ReticleShot] -> getPositionY() > visibleSize.height)
			{
				// ��ʓ��ɕ\�������悤�ɕ␳
				reticle[ReticleShot] -> setPositionY( reticle[ReticleShot] -> getPositionY()
												- ( reticle[ReticleShot] -> getPositionY() - visibleSize.height));
			}
		}
	}
	else
	{
		// �\�����e�B�N�����A�C�h����ԂɕύX
		reticle[ReticleShot] -> setVisible( false);
		reticle[ReticleIdle] -> setVisible( true);

		// ��ʂ��^�b�`����Ă��Ȃ���
		if( master -> GetTouchState() == TSTATE_OFF)// && returnEscapeFlag)
		{
		//	returnEscapeFlag = false;
			firstShotFlag = true;
			returnEscapeNoShotFlag = true;
			pos = Vec2( visibleSize.width / 2, visibleSize.height / 2 - visibleSize.height * master -> reticleAjust);
		}
		else
		{
		//	returnEscapeFlag = true;
		}
	}
}
