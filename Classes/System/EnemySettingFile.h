
#ifndef __ENEMY_SETTING_FILE_H__
#define __ENEMY_SETTING_FILE_H__

#include "cocos2d.h"

namespace TapGun
{
	class EnemySettingFile
	{
	public:
		/**
		*	敵設定ファイルの読み込みと設定用オブジェクトの生成
		*
		*	@author	minaka
		*	@param	fileName 設定ファイル名
		*	@return	正常終了 : 生成したオブジェクトへのポインタ  失敗 : nullptr
		*/
		static EnemySettingFile* create( const std::string& fileName);

	private:
		// 関数宣言用 (詳細は後方に記述)
		struct EnemyData;

	public:
		enum _DATA_LIST_COUNT
		{
			WAVE_ENEMY_COUNT	= 20,	// 1wave中に出現する敵の総数
		};

		unsigned int loadCount;			// 読み込みデータ数
		EnemyData* dataList[WAVE_ENEMY_COUNT];	// 読み込んだ敵データの保存配列

	private:
		EnemySettingFile();

		/**
		*	アニメーション名を指す文字列から数値を検索する
		*
		*	@author	minaka
		*	@param	dataString 元アニメーション名
		*	@return	検索結果の数値　発見できなかった場合は : -999
		*/
		static int getAIAppearNumber( const std::string& dataString);

		/**
		*	アニメーション名を指す文字列から数値を検索する
		*
		*	@author	minaka
		*	@param	dataString 元アニメーション名
		*	@return	検索結果の数値　発見できなかった場合は : -999
		*/
		static int getAIMoveNumber( const std::string& dataString);

		/**
		*	アニメーション名を指す文字列から数値を検索する
		*
		*	@author	minaka
		*	@param	dataString 元アニメーション名
		*	@return	検索結果の数値　発見できなかった場合は : -999
		*/
		static int getAIAttackNumber( const std::string& dataString);

		/**
		*	アニメーション名を指す文字列から数値を検索する
		*
		*	@author	minaka
		*	@param	dataString 元アニメーション名
		*	@return	検索結果の数値　発見できなかった場合は : -999
		*/
		static int getAILifeCycleNumber( const std::string& dataString);

		enum _DATA_NUMBER_	// 各データの数
		{
			BLOCK_LINE_COUNT		= 6,	// 1ブロックの行数

			TARGET_POS_COUNT		= 3,	// 目標地点数
			WAIT_TO_ATTACK_COUNT	= 3,	// 攻撃前待機時間数
			WAIT_TO_MOVE_COUNT		= 3,	// 攻撃後待機時間数
			NEXT_ENEMYS_COUNT		= 3,	// 次に出現させる敵データ数
			AI_MOVE_COUNT			= 2,	// 移動モーション数
			AI_ATTACK_COUNT			= 3,	// 攻撃モーション数
		};

		struct EnemyData
		{
			// データ番号
			int Num;
			// 出現地点
			cocos2d::Vec3 startPos;
			// 待機時間
			int sleepTime;
			// 目標地点
			cocos2d::Vec3 targetPos[TARGET_POS_COUNT];
			// 移動後の待機時間
			int waitToAttack[WAIT_TO_ATTACK_COUNT];
			// 出現フラグ
			bool entryFlag;
			// 攻撃後の待機時間
			int waitToMove[WAIT_TO_MOVE_COUNT];
			// 次出現させる敵の番号
			int nextEnemies[NEXT_ENEMYS_COUNT];
			// 登場時モーション番号
			int aiAppear;
			// 移動モーション番号
			int aiMove[AI_MOVE_COUNT];
			// 攻撃モーション番号
			int aiAtk[AI_ATTACK_COUNT];
			// ライフサイクル番号
			int aiLifeCycle;
		};
		unsigned int fileLineCount;			// 現在の行番号
	};
}

#endif // __ENEMY_SETTING_FILE_H__
