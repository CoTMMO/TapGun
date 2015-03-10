
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
		struct EnemyData;								// 螳滓?九?ｮ螳｣險�縺ｫ繧｢繧ｯ繧ｻ繧ｹ繝ｬ繝吶Ν繧貞粋繧上○繧?

	public:
		enum _DATA_LIST_COUNT
		{
			WAVE_ENEMY_COUNT	= 20,
		};

		unsigned int loadCount;
		EnemyData* dataList[WAVE_ENEMY_COUNT];			// ??ｿｽ?ｿｽd??ｿｽ?ｿｽl??ｿｽ?ｿｽ??ｿｽ?ｿｽ10??ｿｽ?ｿｽﾌ以擾ｿｽ??ｿｽ?ｿｽﾌ敵??ｿｽ?ｿｽﾍ出??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾈゑｿｽ??ｿｽ?ｿｽﾌゑｿｽ 10

	private:
		EnemySettingFile() {}

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

		enum _DATA_NUMBER_
		{
			BLOCK_LINE_COUNT		= 6,

			TARGET_POS_COUNT		= 3,
			WAIT_TO_ATTACK_COUNT	= 3,
			WAIT_TO_MOVE_COUNT		= 3,
			NEXT_ENEMYS_COUNT		= 3,
			AI_MOVE_COUNT			= 2,
			AI_ATTACK_COUNT			= 3,
		};

		struct EnemyData
		{
			int Num;					// ??ｿｽ?ｿｽG??ｿｽ?ｿｽﾌ趣ｿｽ??ｿｽ?ｿｽﾊ番搾ｿｽ??ｿｽ?ｿｽﾅゑｿｽ??ｿｽ?ｿｽBWave??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾆゑｿｽ0??ｿｽ?ｿｽﾔゑｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽn??ｿｽ?ｿｽﾟてゑｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ
			cocos2d::Vec3 startPos;		// ??ｿｽ?ｿｽG??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽB??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾄゑｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽW??ｿｽ?ｿｽﾅゑｿｽ
			int sleepTime;				// ??ｿｽ?ｿｽO??ｿｽ?ｿｽﾌ敵??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ|??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾄゑｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽA??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾌ敵??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽo??ｿｽ?ｿｽﾄゑｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾜでの待ゑｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾔでゑｿｽ
			cocos2d::Vec3 targetPos[TARGET_POS_COUNT];	// ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾌ敵??ｿｽ?ｿｽﾌ目標??ｿｽ?ｿｽn??ｿｽ?ｿｽ_??ｿｽ?ｿｽﾅゑｿｽ??ｿｽ?ｿｽB??ｿｽ?ｿｽS??ｿｽ?ｿｽﾄゑｿｽ??ｿｽ?ｿｽg??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽK??ｿｽ?ｿｽv??ｿｽ?ｿｽﾍゑｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾜゑｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽB
			int waitToAttack[WAIT_TO_ATTACK_COUNT];			// ??ｿｽ?ｿｽﾚ標??ｿｽ?ｿｽn??ｿｽ?ｿｽ_??ｿｽ?ｿｽﾉ難ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾄゑｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽU??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾜでの待ゑｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾔでゑｿｽ

			bool entryFlag;
			int waitToMove[WAIT_TO_MOVE_COUNT];			// ??ｿｽ?ｿｽU??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽI??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾄゑｿｽ??ｿｽ?ｿｽ邇滂ｿｽﾌ行??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽJ??ｿｽ?ｿｽn??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾜでの待ゑｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾔ（??ｿｽ?ｿｽA??ｿｽ?ｿｽC??ｿｽ?ｿｽh??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾔ）??ｿｽ?ｿｽﾅゑｿｽ??ｿｽ?ｿｽB??ｿｽ?ｿｽ~??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽb??ｿｽ?ｿｽﾅゑｿｽ??ｿｽ?ｿｽB
			int nextEnemies[NEXT_ENEMYS_COUNT];			// ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾌ敵??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ|??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾄゑｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽA??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾉ出??ｿｽ?ｿｽﾄゑｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽG??ｿｽ?ｿｽﾅゑｿｽ??ｿｽ?ｿｽB3??ｿｽ?ｿｽﾌまで可能??ｿｽ?ｿｽﾅゑｿｽ??ｿｽ?ｿｽB

			int aiAppear;				// ??ｿｽ?ｿｽo??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ?ｿｽ???ｿｽ?ｿｽ??ｿｽ?ｿｽ[??ｿｽ?ｿｽV??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾅゑｿｽ
			int aiMove[AI_MOVE_COUNT];				// ??ｿｽ?ｿｽﾚ難ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ?ｿｽ???ｿｽ?ｿｽ??ｿｽ?ｿｽ[??ｿｽ?ｿｽV??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾅゑｿｽ
			int aiAtk[AI_ATTACK_COUNT];				// ??ｿｽ?ｿｽU??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ?ｿｽ???ｿｽ?ｿｽ??ｿｽ?ｿｽ[??ｿｽ?ｿｽV??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽ??ｿｽ?ｿｽﾅゑｿｽ
			int aiLifeCycle;			//
		};

		unsigned int fileLineCount;
	};
}

#endif // __ENEMY_SETTING_FILE_H__
