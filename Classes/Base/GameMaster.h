#ifndef __GAMEMASTER_H__
#define __GAMEMASTER_H__

#include "cocos2d.h"


namespace TapGun
{

	//この定義は今後異なるヘッダファイルに移し替えます
#define FALSE -1
#define TRUE 1

//プレイヤーのパラメータ定義
#define STS_MAXPLAYERHP 6//プレイヤーの最大HP
#define STS_RAPIDSPEED 8//プレイヤーの連射速度（フレーム）
#define STS_MAXBULLETS 30//プレイヤーの最大弾数
#define STS_MUTEKITIME 2000 //無敵時間(ミリ秒)

#define STS_RUNSPEED 0.005f//プレイヤーのウェイト時の速度
#define STS_HIDEWAIT 25//回避モーションが終了するまでの全体フレーム
#define STS_HIDESTART 1//回避ボタンを押してから回避モーションが始まるまでの時間
#define STS_MUTEKISTART 0//回避モーションが始まってから無敵時間に移行するまでの時間
#define STS_APPEARSTART 1//回避ボタンを離してから突撃モーションが始まるまでの時間
#define STS_MUTEKIEND (STS_HIDEWAIT - STS_MUTEKISTART)//突撃モーションが始まってから無敵時間が終了するまでの時間
#define STS_RELOADSTART 12//回避モーションが始まってからリロードが行われるまでの時間



//敵のパラメータ定義
#define STS_ENEMY_HP 6//敵のHP
#define STS_ENEMY_MAXSHOT 5//敵の連射数

#define STS_ENEMY_RUNSPEED 0.005f//敵の走り速度
#define STS_ENEMY_WALKSPEED 0.002f//敵の歩き速度
#define STS_ENEMY_SWALKSPEED 0.0005f//敵の横歩き速度
#define STS_ENEMY_SLIDESPEED 0.005f//スライドジャンプ速度
#define STS_ENEMY_ACROSPEED 0.003f//アクロバティック速度
#define STS_ENEMY_FJUMPSPEED 0.003f//前ジャンプ速度

#define POS_ENEMY_LPOX -0.3f//敵の左の銃のポイント
#define POS_ENEMY_LPOY 1.25f//敵の左の銃のポイント
#define POS_ENEMY_LPOZ 0.1f//敵の左の銃のポイント
#define POS_ENEMY_RPOX 0.3f//敵の右の銃のポイント
#define POS_ENEMY_RPOY 1.25f//敵の右の銃のポイント
#define POS_ENEMY_RPOZ 0.1f//敵の右の銃のポイント

#define STS_EBULLET_SPEED 0.019f//敵の弾の速度
#define STS_EBULLET_VANISH_TIME 3000//敵の弾の速度
#define STS_SSHOT_START 1000//SShot時の弾の発射開始までの待ち時間
#define STS_SSHOT_SPAN 68//SShot時の弾の発射開始までの待ち時間

#define BATTLE_FEILD_X 0.8//タッチで攻撃可能な画面割合


//当たり判定系定義
#define PLAYER_CENTER_Y 1.2f//プレイヤーの当たり判定高さ（敵弾が目標とする高さ）


//回避ボタンの当たり判定定義（ボタンのスプライトと当たり判定を分けて実装します）
#define HIDE_UI_RECT_X 0.18f//当たり判定矩形
#define HIDE_UI_RECT_Y 0.18f//当たり判定矩形
#define HIDE_UI_POS_X 0.09f//当たり判定中心
#define HIDE_UI_POS_Y 0.2f//当たり判定中心


//以上のステータスをもとに座標を計算する
#define HIDE_UI_X0 (HIDE_UI_RECT_X * 0.5f - HIDE_UI_POS_X)
#define HIDE_UI_Y0 (HIDE_UI_RECT_Y * 0.5f - HIDE_UI_POS_Y)
#define HIDE_UI_X1 (HIDE_UI_RECT_X * 0.5f + HIDE_UI_POS_X)
#define HIDE_UI_Y1 (HIDE_UI_RECT_Y * 0.5f + HIDE_UI_POS_Y)


#define PERSE 22.296f//カメラ視野角

//Action時のカメラの設定(右)
#define C_SETX_R -0.62f
#define C_SETY_R 1.51f
#define C_SETZ_R 5.3f
#define C_ROTX_R -4.0f
#define C_ROTY_R 10.0f
#define C_ROTZ_R 0.0f


//Action時のカメラの設定(左)
#define C_SETX_L 0.0f
#define C_SETY_L 1.61f
#define C_SETZ_L 4.8f
#define C_ROTX_L 0.0f
#define C_ROTY_L -13.0f
#define C_ROTZ_L 0.0f


//Wait時のカメラの設定
#define W_PERSE 45//カメラ視野角
#define W_SETX 0.0f
#define W_SETY 1.61f
#define W_SETZ 4.0f

#define W_ROTX 0.0f
#define W_ROTY 0.0f
#define W_ROTZ 0.0f


//死亡時のカメラの初期設定
#define KE_POSX 0.0f//Kill Eye Pos
#define KE_POSY 2.8f
#define KE_POSZ -6.0f

#define KT_POSX 0.0f//Kill Target Pos
#define KT_POSY 0.0f
#define KT_POSZ 0.0f


//3DSMAXのカメラ角度を通常角度に変換するためのマクロ
#define MACRO_CROT_X(x) (x - 90)
#define MACRO_CROT_Y(y) (y + 180)

	//ミリ秒をフレームに変換/フレームをミリ秒に変換
#define MACRO_StoF(second) (second * 60.0f * 0.001f)
#define MACRO_FtoMS(frame) (frame * 0.01666f * 1000)
#define MACRO_FtoS(frame) (frame * 0.01666f)


	//プレイヤーが回避する時の軸の座標（プレイヤーから見た相対座標、左側時）
	//プレイヤーは0度基準で配置しているので、X軸Y軸がカメラとそれぞれ反転しています
#define HIDEPOINT_X 0.35f
#define HIDEPOINT_Y 0.35f

	//回避した時のカメラの移動後の座標（カメラから見た相対座標、左側時）
#define HIDECAMERA_X 1.1f
#define HIDECAMERA_Y -1.1f

//ゲーム関連時間
#define TIME_MAXTIME 180000//ゲーム本編の最大時間（ミリ秒）
#define TIME_ACTION_UI 1800//ActionのUIを表示する時間（ミリ秒）
#define TIME_DEAD_UI 3000//死亡時のカメラ表示する時間（ミリ秒）
#define TIME_OP 0//OPの時間（ミリ秒）・・・現在はOPがないので0ミリ秒

#define TIME_WROT 300//


	enum _CAMERA_FLAG_
	{
		//CAMFLAG_DEFAULT = CameraFlag::DEFAULT,//
		//CAMFLAG_3D = CameraFlag::USER1
		CAMFLAG_DEFAULT = 1,//
		CAMFLAG_3D = 2
	};

	enum _GAME_STATE_
	{
		GSTATE_CREATE,//レイヤー生成時に呼ばれる
		GSTATE_INIT,//
		GSTATE_OP,//OP状態
		GSTATE_WAIT,//ウェイト時
		GSTATE_PLAY_SET,//戦闘開始前の待ち時間（敵の配置にのみ使用する）
		GSTATE_PLAY_ACTION,//戦闘開始前の待ち時間（ActionのUIを描画するときに使用する）
		GSTATE_PLAY,
		GSTATE_PAUSE,
		GSTATE_TIMEOVER,//タイムオーバーによる演出
		GSTATE_DEADOVER,//ダメージ死亡による演出

		GSTATE_CONTINUE_INIT,//コンティニュー画面前の準備
		GSTATE_CONTINUE,//コンティニューするかどうかの操作はここで行う
		GSTATE_CONTINUE_END,//コンティニュー終了後の初期化

		GSTATE_GAMEOVER,//
		GSTATE_CLEAR,//
		GSTATE_END,//ゲームシーンを終了してタイトルへ戻る

		GSTATE_EVENT,//ムービーイベントなどを進行させるときに使用する？（現在未使用）
		GSTATE_NUM
	};

	enum _PLAYER_STATE_
	{
		PSTATE_IDLE,
		PSTATE_SHOT,
		//		PSTATE_PLAY_SET,//アクションの準備中
		PSTATE_DODGE,//隠れ中
		PSTATE_HIDE,//隠れている
		PSTATE_APPEAR,//隠れた状態から出る
		PSTATE_DAMAGED,//攻撃を食らった
		PSTATE_RECOVER,//被弾からの回復
		PSTATE_RUN,
		PSTATE_DEAD,//プレイヤーの死亡
		PSTATE_CONTINUE,//プレイヤー死亡後の操作待ち
		PSTATE_NUM
	};

	enum _PLAYERSIDE_
	{
		PSIDE_RIGHT,
		PSIDE_LEFT
	};

	enum _TOUCH_STATE_
	{
		TSTATE_OFF,
		TSTATE_ON,
		TSTATE_MOVE,
		TSTATE_RELEASE
	};

	enum _TOUCH_FLAG_
	{
		TFLAG_OFF,
		TFLAG_ON,
		TFLAG_MOVE,
		TFLAG_RELEASE,
		TFLAG_CANCEL,
		TFLAG_NUM
	};

	//
	enum _STAGE_POINT_DEF_
	{
		PO_NONE,//使用しない
		PO_START,//スタート地点
		PO_BATTLE,//バトルマス
		PO_CHANGE,//方向転換
		PO_FINISH,
	};

	enum _STAGE_POINT_
	{
		//W = ウェーブ
		//L = ルート

		POINT_START,
		//POINT_L1_0,は使用しない
		POINT_W1,

		POINT_L2a,
		POINT_L2b,
		POINT_L2c,
		POINT_L2d,

		//POINT_W2,

		//POINT_L3_1,
		//POINT_W3,
		//POINT_W3_2,
		//POINT_W4,
		POINT_END,
	};


	//プレイヤーの進行座標を定義する構造体
	typedef struct
	{
		cocos2d::Vec3 pPos;//プレイヤーの位置
		cocos2d::Vec3 pRot;//プレイヤーの角度
		cocos2d::Vec3 cPos;//カメラの位置
		cocos2d::Vec3 cRot;//カメラの角度

		int pointType;
		int playerSide;
		cocos2d::Point hidePoint;//回避動作の軸となる座標
	}StagePoint;

	class GameMaster
	{
	public:
		//各種public変数は今後privateに置き換えていきます

		//タイム
		int gameActionTime;//ウェーブの残り時間（ミリ秒）

		/*
		timeval* nowTV;//現在時刻(timeval)
		timeval* preTV;//前フレームまでの時刻(timeval)
		int nowTime;//現在時刻（ミリ秒）
		int preTime;//1ループ前の時刻（ミリ秒）
		*/
		int loopTime;//ループに要した時間（ミリ秒）

		int shotFlag;//プレイヤーの攻撃アニメーション用フラグ(5フレーム以上タッチしているかどうか)

		//各種フラグ（後でprivateに修正する）
		int waitFlag;//ウェイトモードを進行させるフラグ
		int sPoint;//現在のステージポイント
		float rapidFrame;//連射待ち時間
		int wave;//現在ウェーブ
		StagePoint stagePoint[100];//プレイヤーの進行座標を定義する構造体

		int playerSide;//プレイヤーが左右どちらに立っているかのフラグ
		int playerHitFlag;//プレイヤーの無敵状態のフラグ（TRUE/FALSE）

		int flgPlayerATK;//プレイヤーの攻撃処理判定を行うか（TRUE/FALSE）
		int mutekiTime;//無敵時間(ミリ秒)

		float reticleAjust;//指の位置とレティクルの位置の差（画面に対する割合）


		//関数
		GameMaster(const GameMaster &P) = delete;
		GameMaster& operator= (const GameMaster &P) = delete;
		static GameMaster* GetInstance(void);

		static cocos2d::Camera* Get2DCamInstance(void);
		static cocos2d::Camera* Get3DCamInstance(void);
		static cocos2d::Node* GetCamNodeInstance(void);

		void InitScreenSize(void);

		void InitParam(void);
		void InitParam(int wave);

		void InitCamera2D(void);
		void InitCamera3D(void);

		//2Dカメラ用
		void SetCamera2DPos(cocos2d::Vec3 pos);//2Dカメラの位置を変更することはあまりないので必要ない？
		cocos2d::Camera* GetCamera2D(void);//2D用カメラのクラスポインタを返す

		//3Dカメラ用
		void SetCamera3DPos(cocos2d::Vec3 pos);
		void SetCamera3DRot(cocos2d::Vec3 rot);
		void AddCamera3DPos(cocos2d::Vec3 pos);
		void AddCamera3DRot(cocos2d::Vec3 rot);
		cocos2d::Vec3 GetCamera3DPos(void);
		cocos2d::Vec3 GetCamera3DRot(void);
		//3Dカメラのノード
		void SetCameraNodePos(cocos2d::Vec3 pos);
		void SetCameraNodeRot(cocos2d::Vec3 rot);
		void AddCameraNodePos(cocos2d::Vec3 pos);
		void AddCameraNodeRot(cocos2d::Vec3 rot);
		cocos2d::Vec3 GetCameraNodePos(void);
		cocos2d::Vec3 GetCameraNodeRot(void);
		//3Dカメラの注視点
		void SetCameraTarget(cocos2d::Vec3 pos);
		cocos2d::Vec3 GetCameraTarget(void);

		cocos2d::Camera* GetCamera3D(void);
		const cocos2d::Node* GetCameraNode(void);

		void SetPlayerState(int state);

		//タッチ関連
		void UpdateTouchManager(void);//タッチ管理クラスを更新
		void SetTouchPos(cocos2d::Touch* tch);//タッチ座標を取得
		int SetTouchFlag(int state);//タッチフラグを取得
		cocos2d::Vec2 GetTouchPosInView(void);//タッチ座標を返す
		cocos2d::Vec2 GetTouchPos(void);//タッチ座標を返す

		int GetTouchFlag(void);//タッチフラグを返す
		int GetTouchState(void);//タッチ状態を返す

		int GetPlayerState(void);

		//ゲーム状態のセットと取得
		int GetGameState(void);
		void SetGameState(int gState);

		//プレイヤーHPのゲッターとセッター
		int GetPlayerHP(void);
		int SetPlayerHP(int value);
		int AddPlayerHP(int value);

		//プレイヤー弾数のゲッターとセッター
		int GetPlayerBullets(void);
		int SetPlayerBullets(int value);
		int AddPlayerBullets(int value);

		void CalcCameraRot(void);

		//現在ゲーム時間のゲッターとセッター
		int GetGameTime(void);
		int setGameTime(int time);
		int AddGameTime(int time);

	private:
		int playerState;//プレイヤーの状態
		int gameState;//ゲームの状態

		cocos2d::Touch* touch;//タッチ管理
		int touchState;//タッチの状態
		int touchFlag;//

		//プレイヤーのステータス
		int playerHP;//プレイヤーの体力
		int nowBullets;//プレイヤーの現在弾数

		cocos2d::Size screenSize;

		static cocos2d::Camera* camera2D;
		static cocos2d::Camera* camera3D;
		static cocos2d::Node* CamNode;

		//3Dカメラのターゲット位置を取得できないので、いったんGameMaster.hに書きます
		cocos2d::Vec3 camTarget;//3Dカメラの注視点
		//
		cocos2d::Vec2 setHidePoint(StagePoint stagePoint);

		GameMaster() {}
	};
}

#endif // __GAMEMASTER_H__
