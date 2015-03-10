#include "cocos2d.h"

#include "Object/Effect.h"
#include "System/Sound.h"

#define NULL_VECTOR ( Vec3( 0, 0 ,0))

USING_NS_CC;
using namespace std;
using namespace TapGun;

Effect::Effect()
{
	for( auto &p : playerEffect)
	{
		p = new EffectData;
		p -> flag = false;
	}
	for( auto &p : enemyEffect)
	{
		p = new EffectData;
		p -> flag = false;
	}

	playerMuzzle = new PlayerMuzzle;
	playerMuzzle -> shotFlag = false;
	for( int i = 0; i < PlayerMuzzleAnime; i++)
	{
		char buf[64];
		sprintf( buf, "P_Muzzle/p_muzzle%02d", i + 1);
		playerMuzzle -> sprite3D[i] = _Sprite3D::create( buf);
		playerMuzzle -> sprite3D[i] -> setVisible( false);
		playerMuzzle -> sprite3D[i] -> setCameraMask( (unsigned short)CameraFlag::USER1);
		playerMuzzle -> sprite3D[i] -> retain();
	}

	for( int i = 0; i < EnemyMuzzleCount; i++)
	{
		enemyMuzzle[i] = new EnemyMuzzle;
		enemyMuzzle[i] -> shotFlag = false;
		for( int j = 0; j < EnemyMuzzleAnime; j++)
		{
			char buf[64];
			sprintf( buf, "E_Muzzle/e_muzzle%02d", j + 1);
			enemyMuzzle[i] -> sprite3DR[j] = _Sprite3D::create( buf);
			enemyMuzzle[i] -> sprite3DR[j] -> setVisible( false);
			enemyMuzzle[i] -> sprite3DR[j] -> setCameraMask( (unsigned short)CameraFlag::USER1);
			enemyMuzzle[i] -> sprite3DR[j] -> retain();

			enemyMuzzle[i] -> sprite3DL[j] = _Sprite3D::create( buf);
			enemyMuzzle[i] -> sprite3DL[j] -> setVisible( false);
			enemyMuzzle[i] -> sprite3DL[j] -> setCameraMask( (unsigned short)CameraFlag::USER1);
			enemyMuzzle[i] -> sprite3DL[j] -> retain();
		}
	}
}

Effect* Effect::getInstance( void)
{
	static Effect* P;
	if( !P) P = new Effect;
	return P;
}

void Effect::createHitEffect( Sprite3D* sprite3d, Vec3 point, EffectType effectType)
{
	string fileName;
	EffectData* effect = nullptr;

	// �t���O�ݒ�����ɕ���
	switch( effectType)
	{
	case PlayerHitEffect:					// �v���C���[�̔�e�G�t�F�N�g
		for( int i = 0; i < PlayerNum; i++)
		{
			// �󂫃o�b�t�@�`�F�b�N
			if( playerEffect[i] -> flag == false)
			{
				effect = playerEffect[i];	// ���[�J���ɃR�s�[���Č�̏��������ʉ�
				fileName = "P_Hit";			// �t���O�ɉ������������ݒ�
			}
		}
		break;

	case EnemyHitEffect:					// �G�̔�e�G�t�F�N�g
		for( int i = 0; i < EnemyNum; i++)
		{
			// �󂫃o�b�t�@�`�F�b�N
			if( enemyEffect[i] -> flag == false)
			{
				effect = enemyEffect[i];	// ���[�J���ɃR�s�[���Č�̏��������ʉ�
				fileName = "E_Hit";			// �t���O�ɉ������������ݒ�
			}
		}
		break;
	}
	if( !effect) { return; } // �|�C���^�̃R�s�[�Ɏ��s�����ꍇ�͑��I��

	// �m�[�h�ƃr���{�[�h�̎��̂𐶐�
	effect -> billboard = BillBoard::create();
	effect -> billboard -> retain();
	effect -> node = Node::create();
	effect -> node -> retain();

	// �C���X�^���X�擾
	auto cache = SpriteFrameCache::getInstance();

	// ��A�j���[�V�����𐶐�
	auto animation = Animation::create();

	for( int i = 0; i < EffectGraphDataCount; i++)
	{
		char buf[64];
		sprintf( buf, "%02d.png", i + 1);
		string frameName = fileName + buf;	// ���炩���ߐݒ肵������������ɐ���
		// ��A�j���[�V�����ɉ摜�f�[�^��ǉ�
		animation -> addSpriteFrame( cache -> getSpriteFrameByName( frameName.c_str()));
	}

	// �A�j���[�V�����Đ����x��ݒ�
	animation -> setDelayPerUnit( 0.01f);
	animation -> setRestoreOriginalFrame( true);

	// �A�j���[�V���������ɃA�N�V�����𐶐�
	auto action = Animate::create( animation);

	// ��������������_���Őݒ�
	auto callfunc = CallFunc::create( [=](void)->void
										{
											effect -> flag = false;
											effect -> billboard -> removeFromParent();
											effect -> node -> removeFromParent();
										});

	// �V�[�P���X�ݒ肵���Ԃɏ��������悤�ɐݒ�
	auto sequence = Sequence::create( action, callfunc, NULL);

	// �쐬�����V�[�P���X�̎��s
	effect -> billboard -> runAction( sequence);

	// �\���ʒu��ݒ� �����ɉ����ĕύX
	if( point == NULL_VECTOR) { effect -> billboard -> setPosition3D( Vec3( 0.1, 1.5, 0.5)); }
	else { effect -> billboard -> setPosition3D( point); }

	// �\���p�x��ݒ�
	effect -> node -> setRotation3D( Vec3( 0, -sprite3d -> getRotation3D().y, 0));

	// �\���X�P�[����ݒ�
	effect -> billboard -> setScale( 0.003f);

	// �쐬������m�[�h�Ƀr���{�[�h��ڑ�
	effect -> node -> addChild( effect -> billboard);

	// �Q�[���V�[�����̃J�����ɉf��悤�ɐݒ�
	effect -> node -> setCameraMask( (unsigned short)CameraFlag::USER1);

	// �\�����������f���Ƀm�[�h��ڑ�
	sprite3d -> addChild( effect -> node);

	// �g�p���t���O��ON��
	effect -> flag = true;
	return;
}

void Effect::setPlayerHitEffect( Sprite3D* sprite3d)
{
	createHitEffect( sprite3d, NULL_VECTOR, PlayerHitEffect);
}

void Effect::setPlayerHitEffect( Sprite3D* sprite3d, Vec3 point)
{
	createHitEffect( sprite3d, point, PlayerHitEffect);
}

void Effect::setEnemyHitEffect( Sprite3D* sprite3d)
{
	createHitEffect( sprite3d, NULL_VECTOR, EnemyHitEffect);
}

void Effect::setEnemyHitEffect( Sprite3D* sprite3d, Vec3 point)
{
	createHitEffect( sprite3d, point, EnemyHitEffect);
}

void Effect::setPlayerMuzzle( Sprite3D* parentData, const string& pointName)
{
	auto sound = Sound::getInstance();
	if( playerMuzzle -> shotFlag == true) { return; }
	playerMuzzle -> shotFlag = true;
	playerMuzzle -> count = 0;
	auto point = parentData -> getAttachNode( pointName);
	if( !point) { return; }

	for( int i = 0; i < PlayerMuzzleAnime; i++)
	{
		playerMuzzle -> sprite3D[i] -> setRotation3D( Vec3( 90, 0, parentData -> getRotation3D().y));
		point -> addChild( playerMuzzle -> sprite3D[i]);
	}
	sound -> playSE( "Shot");
}

void Effect::setEnemyMuzzle( Sprite3D* parentData, const string& pointName1, const string& pointName2)
{
	auto sound = Sound::getInstance();

	for( int i = 0; i < EnemyMuzzleCount; i++)
	{
		if( enemyMuzzle[i] -> shotFlag == true) { continue; }
		enemyMuzzle[i] -> shotFlag = true;
		enemyMuzzle[i] -> count = 0;
		auto point1 = parentData -> getAttachNode( pointName1);
		if( !point1) { return; }
		auto point2 = parentData -> getAttachNode( pointName2);
		if( !point2) { return; }

		for( int j = 0; j < EnemyMuzzleAnime; j++)
		{
			// �e����������悤�ɕ␳
			enemyMuzzle[i] -> sprite3DR[j] -> setRotation3D( Vec3( 0, 270, 180));
			point1 -> addChild( enemyMuzzle[i] -> sprite3DR[j]);

			// �e����������悤�ɕ␳
			enemyMuzzle[i] -> sprite3DL[j] -> setRotation3D( Vec3( 0, 90, 180));
			point2 -> addChild( enemyMuzzle[i] -> sprite3DL[j]);
		}
		sound -> playSE( "Shot");
		return;
	}
}

void Effect::muzzleUpdate( void)
{
	if( playerMuzzle -> shotFlag)
	{
		if( playerMuzzle -> count == PlayerMuzzleAnime)
		{
			playerMuzzle -> shotFlag = false;
			playerMuzzle -> count = 0;
			for( auto &p : playerMuzzle -> sprite3D) { p -> removeFromParent(); }
			return;
		}

		switch( playerMuzzle -> count)
		{
		case 0:
			playerMuzzle -> sprite3D[0] -> setVisible( true);
			break;
		case 1:
			playerMuzzle -> sprite3D[0] -> setVisible( false);
			playerMuzzle -> sprite3D[1] -> setVisible( true);
			break;
		case 2:
			playerMuzzle -> sprite3D[1] -> setVisible( false);
			playerMuzzle -> sprite3D[2] -> setVisible( true);
			break;
		default:
			break;
		}
		playerMuzzle -> count++;
	}

	for( auto &p : enemyMuzzle)
	{
		if( p -> shotFlag)
		{
			if( p -> count == EnemyMuzzleAnime)
			{
				p -> shotFlag = false;
				p -> count = 0;
				for( auto &r : p -> sprite3DR) { r -> removeFromParent(); }
				for( auto &l : p -> sprite3DL) { l -> removeFromParent(); }
				return;
			}

			switch( p -> count)
			{
			case 0:
				p -> sprite3DR[0] -> setVisible( true);

				p -> sprite3DL[0] -> setVisible( true);
				break;
			case 1:
				p -> sprite3DR[0] -> setVisible( false);
				p -> sprite3DR[1] -> setVisible( true);

				p -> sprite3DL[0] -> setVisible( false);
				p -> sprite3DL[1] -> setVisible( true);
				break;
			default:
				break;
			}
			p -> count++;
		}
	}
}
