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

	// フラグ設定を元に分岐
	switch( effectType)
	{
	case PlayerHitEffect:					// プレイヤーの被弾エフェクト
		for( int i = 0; i < PlayerNum; i++)
		{
			// 空きバッファチェック
			if( playerEffect[i] -> flag == false)
			{
				effect = playerEffect[i];	// ローカルにコピーして後の処理を共通化
				fileName = "P_Hit";			// フラグに応じた文字列を設定
			}
		}
		break;

	case EnemyHitEffect:					// 敵の被弾エフェクト
		for( int i = 0; i < EnemyNum; i++)
		{
			// 空きバッファチェック
			if( enemyEffect[i] -> flag == false)
			{
				effect = enemyEffect[i];	// ローカルにコピーして後の処理を共通化
				fileName = "E_Hit";			// フラグに応じた文字列を設定
			}
		}
		break;
	}
	if( !effect) { return; } // ポインタのコピーに失敗した場合は即終了

	// ノードとビルボードの実体を生成
	effect -> billboard = BillBoard::create();
	effect -> billboard -> retain();
	effect -> node = Node::create();
	effect -> node -> retain();

	// インスタンス取得
	auto cache = SpriteFrameCache::getInstance();

	// 空アニメーションを生成
	auto animation = Animation::create();

	for( int i = 0; i < EffectGraphDataCount; i++)
	{
		char buf[64];
		sprintf( buf, "%02d.png", i + 1);
		string frameName = fileName + buf;	// あらかじめ設定した文字列を元に生成
		// 空アニメーションに画像データを追加
		animation -> addSpriteFrame( cache -> getSpriteFrameByName( frameName.c_str()));
	}

	// アニメーション再生速度を設定
	animation -> setDelayPerUnit( 0.01f);
	animation -> setRestoreOriginalFrame( true);

	// アニメーションを元にアクションを生成
	auto action = Animate::create( animation);

	// 解放処理をラムダ式で設定
	auto callfunc = CallFunc::create( [=](void)->void
										{
											effect -> flag = false;
											effect -> billboard -> removeFromParent();
											effect -> node -> removeFromParent();
										});

	// シーケンス設定し順番に処理されるように設定
	auto sequence = Sequence::create( action, callfunc, NULL);

	// 作成したシーケンスの実行
	effect -> billboard -> runAction( sequence);

	// 表示位置を設定 引数に応じて変更
	if( point == NULL_VECTOR) { effect -> billboard -> setPosition3D( Vec3( 0.1, 1.5, 0.5)); }
	else { effect -> billboard -> setPosition3D( point); }

	// 表示角度を設定
	effect -> node -> setRotation3D( Vec3( 0, -sprite3d -> getRotation3D().y, 0));

	// 表示スケールを設定
	effect -> billboard -> setScale( 0.003f);

	// 作成した空ノードにビルボードを接続
	effect -> node -> addChild( effect -> billboard);

	// ゲームシーン内のカメラに映るように設定
	effect -> node -> setCameraMask( (unsigned short)CameraFlag::USER1);

	// 表示したいモデルにノードを接続
	sprite3d -> addChild( effect -> node);

	// 使用中フラグをONに
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
			// 銃口先を向くように補正
			enemyMuzzle[i] -> sprite3DR[j] -> setRotation3D( Vec3( 0, 270, 180));
			point1 -> addChild( enemyMuzzle[i] -> sprite3DR[j]);

			// 銃口先を向くように補正
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
