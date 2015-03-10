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
		*	タイトルシーンの作成
		*
		*	@author	minaka
		*	@param	なし
		*	@return 作成したシーンのポインタ
		*/
		static cocos2d::Scene* createScene( void);

		/**
		*	タイトルシーンの初期化
		*
		*	@author	minaka
		*	@param	なし
		*	@return 初期化成功時 true 失敗時 false
		*/
		bool init( void);

		/**
		*	タイトルシーンの更新
		*
		*	@author	minaka
		*	@param	delta 現在のゲームの実行時間 (ミリ秒)
		*	@return なし
		*/
		void update( float delta);

		/**
		 *	タッチ入力の受け取り (押された時)
		 *
		 *	@author	minaka
		 *	@param	cocos依存なので省略
		 *	@return cocos依存なので省略
		 */
		bool onTouchBegan( cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

		/**
		 *	タッチ入力の受け取り (動かした時)
		 *
		 *	@author	minaka
		 *	@param	cocos依存なので省略
		 *	@return なし
		 */
		void onTouchMoved( cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

		/**
		 *	タッチ入力の受け取り (離されたとき)
		 *
		 *	@author	minaka
		 *	@param	cocos依存なので省略
		 *	@return なし
		 */
		void onTouchEnded( cocos2d::Touch *pTouch, cocos2d::Event *pEvent);

		/**
		 *	GameStartのコールバック関数
		 *
		 *	@author	minaka
		 *	@param	cocos依存なので省略
		 *	@return なし
		 */
		void menuStartCallback( cocos2d::Ref* pSender);

		/**
		 *	GameEndのコールバック関数
		 *
		 *	@author	minaka
		 *	@param	cocos依存なので省略
		 *	@return なし
		 */
		void menuEndCallback( cocos2d::Ref* pSender);

		/**
		 *	クレジットのコールバック関数
		 *
		 *	@author	minaka
		 *	@param	cocos依存なので省略
		 *	@return なし
		 */
		void menuCreditCallback( cocos2d::Ref* pSender);

		// 必要な関数の自動生成
		CREATE_FUNC( TitleScene);

	private:
		enum SpriteName		// 画面に表示するスプライト一覧
		{
			BG,
			Logo,
			Frame,
			Menu,
			SpriteNum,
		};

		enum MenuFlag		// シーン全体の制御定数
		{
			TeamLogo	= ( 1 << 0),
			TitleLogoIn	= ( 1 << 1),
			TitleLogoOK	= ( 1 << 2),

			MenuIn	= ( 1 << 3),
			MenuOK	= ( 1 << 4),

			TitleEnd = ( 1 << 5),
		};

		enum TeamLogoState	// チームロゴの制御状態定数
		{
			LogoIn,
			LogoOut,
			Wait,
			WaitTime = 75,
			AlphaValue = 5,
		};

		enum MenuName		// メニューの識別定数
		{
			Start,
			End,
			Credit,
			MenuNum,
		};

		// ロゴのフェードイン・アウトの制御カウンタ
		int logoWaitCount;
		// 制御状態を保存
		TeamLogoState teamLogoState;

		// シーン内の処理遷移を制御
		MenuFlag menuFlag;
		// メニューのアニメーション状態管理フラグ
		bool menuActionFlag;
		// チームロゴのアルファ値保存
		int alphaCount;

		// タイトルロゴのフェードイン状態の制御フラグ
		bool logoAlphaFlag;
		// タイトルロゴのアルファ値を保存
		int logoAlphaCount;
		// タイトルロゴのアルファ加減算待機カウンタ
		int logoAlphaWaitCount;

		// チームロゴ用のスプライト
		cocos2d::Sprite* teamLogo;
		// メニュー以外のタイトル画面のスプライト
		cocos2d::Sprite* sprite[SpriteNum];
		// メニュー用のオブジェクト
		cocos2d::Menu* menu[MenuNum];

		/**
		 *	チームロゴの描画制御
		 *
		 *	@author	minaka
		 *	@param	なし
		 *	@return なし
		 */
		void teamLogoAction( void);

		/**
		 *	タイトルシーン用のスプライト作成
		 *
		 *	@author	minaka
		 *	@param	なし
		 *	@return なし
		 */
		void setSprite( void);

		/**
		 *	タイトルシーン用のメニューの作成
		 *
		 *	@author	minaka
		 *	@param	なし
		 *	@return なし
		 */
		void setMenu( void);

		/**
		 *	メニューの移動処理
		 *
		 *	@author	minaka
		 *	@param	なし
		 *	@return なし
		 *	@date	2/19 Ver 1.0
		 */
		void menuAction( void);

		/**
		 *	各種画像データの読み込み
		 *
		 *	@author	minaka
		 *	@param	cocos依存なので省略
		 *	@return なし
		 */
		void loadPicture( void);

		/**
		*	各種モデルデータの読み込み
		*
		*	@author	minaka
		*	@param	cocos依存なので省略
		*	@return なし
		*/
		void loadModels( void);

		/**
		 *	各種サウンドデータの読み込み
		 *
		 *	@author	minaka
		 *	@param	cocos依存なので省略
		 *	@return なし
		 *	@date	2/19 Ver 1.0
		 */
		void loadSound( void);


		/**
		 *	フラグ変数をチェック
		 *
		 *	@author	minaka
		 *	@param	flag フラグ変数
		 *	@param	number 調べる値
		 *	@return フラグon : true  フラグoff : false
		 */
		template<class P> static bool checkFlag( P* flag, const P number);

		/**
		 *	フラグ変数のフラグをonにする
		 *
		 *	@author	minaka
		 *	@param	flag フラグ変数
		 *	@param	number onにする値
		 *	@return なし
		 */
		template<class P> static void setFlag( P* flag, const P number);

		/**
		 *	フラグ変数のフラグをoffにする
		 *
		 *	@author	minaka
		 *	@param	flag フラグ変数
		 *	@param	number offにする値
		 *	@return なし
		 */
		template<class P> static void resetFlag( P* flag, const P number);
	};
}

#endif // __TITLE_SCENE_H__
