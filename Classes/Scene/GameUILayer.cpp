
#include "GameUILayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "GameMaster.h"
#include "UI.h"

#else

#include "Base/GameMaster.h"
#include "Base/UI.h"

#endif


USING_NS_CC;
using namespace TapGun;

GameMaster* GameMasterL;//変数名は今後考慮する


/**
*	ゲーム本編のUIレイヤーを初期化
*
*	@author	sasebon
*	@param	なし
*	@return	初期化成功／不可のbool値
*	@date	1/8 Ver 1.0
*/
bool GameUILayer::init()
{
	if(!Layer::init())
	{
		return false;
	}
	GameMasterL = GameMaster::GetInstance();//ゲームパラメータクラスの初期化
	return true;
}


/**
*	ゲーム本編のUIレイヤーの各種数値初期化
*
*	@author	sasebon
*	@param	なし
*	@return	初期化成功／不可のbool値
*	@date	1/8 Ver 1.0
*/
void GameUILayer::InitLayer(void)
{
	InitAllUI();
//	SetUI();
	GameUI::getInstance() -> init( this);
}


/**
*	ゲーム本編のUIレイヤーの各種数値初期化
*
*	@author	sasebon
*	@param	なし
*	@return	初期化成功／不可のbool値
*	@date	1/8 Ver 1.0
*/
int GameUILayer::SerchFreeUI()
{
	for (int i = 0; i < MAX_UI; i++)
	{
		if (FALSE == Ui[i].valid)
		{
			return i;
		}
	}
	return -1;//全てのUISpriteが使用されていたら-1を返す
}


/**
*	UIスプライトの生成
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	2/5 Ver 1.0
*/
//void GameUILayer::LoadUISprite()
//{
//	std::string fileName1;
//
//	//レティクルの生成
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	fileName1 = "reticle.png";
//#else
//	fileName1 = "Graph/Pictures/reticle.png";
//#endif
////	UIBillBoard[UIKIND_RETICLE] = cocos2d::BillBoard::createWithTexture(
////		Sprite::createWithSpriteFrameName( "reticle_idle.png") -> getTexture(), BillBoard::Mode::VIEW_PLANE_ORIENTED);
//}


/**
*	UIの配置
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
//void GameUILayer::SetUI()
//{
//	auto s = Director::getInstance()->getWinSize();//スクリーンサイズを取得
//}




/**
*	UI系の数値初期化
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
void GameUILayer::InitAllUI()
{
	for(int i = 0; i < MAX_UI; i++)
	{
		Ui[i].Init();
	}
}



/**
*	UIの更新
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/20 Ver 1.0
*/
void GameUILayer::UpdateLayer( void)
{
	GameUILayer::MoveReticle();
	GameUI::getInstance() -> update();
}



/**
*	レティクルの移動
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
void GameUILayer::MoveReticle(void)
{
	static bool flag = false;
	if (TRUE == valid[UIKIND_RETICLE])//初期化チェックは不要ならば消す
	{
		if (flag == false)
		{
			GameUI::getInstance()->init(this);
			flag = true;
		}

		Vec2 tPos;
		auto size = Director::getInstance()->getWinSize();//ウィンドウサイズを取得
		//レティクルの挙動
		//プレイヤーの状態を取得して場合分け
		switch (GameMasterL->GetPlayerState())
		{
		case PSTATE_SHOT://攻撃中はレティクルを移動させる

			tPos = GameMasterL->GetTouchPos();//タッチ座標を取得
			tPos.y += size.height * GameMasterL->reticleAjust;//
			if (tPos.y >= size.height)
			{
				tPos.y = size.height;
			}

			UIBillBoard[UIKIND_RETICLE]->setPosition(tPos);

			break;
		case PSTATE_IDLE://アイドル状態

			UIBillBoard[UIKIND_RETICLE]->setVisible(true);
			break;
		case PSTATE_HIDE://隠れているa
		case PSTATE_APPEAR://隠れた状態から出る
		case PSTATE_DAMAGED://ダメージを受けた
			break;
		case PSTATE_RUN://走っている（Wait時）
			UIBillBoard[UIKIND_RETICLE]->setVisible(false);

			break;
		case PSTATE_DEAD://死亡
			//ウェイト時と死亡時はGSTATE_PLAYではないので、他のステート時は一括でUIの非表示を管理した方がよい
			//現在はここにも記述しておく
			UIBillBoard[UIKIND_RETICLE]->setVisible(false);
			break;
		}
	}
}