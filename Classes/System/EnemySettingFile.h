
#ifndef __ENEMY_SETTING_FILE_H__
#define __ENEMY_SETTING_FILE_H__

#include "cocos2d.h"

namespace TapGun
{
	class EnemySettingFile
	{
	public:
		/**
		*	ìGê›íËÉtÉ@ÉCÉãÁE�ì«ÁE��çûÁE��ÁE�Eê›íËópÉIÉuÉWÉFÉNÉgÁE�ê∂ê¨
		*
		*	@author	minaka
		*	@param	fileName ê›íËÉtÉ@ÉCÉãñº
		*	@return	ê≥èÌèIóπ : ê∂ê¨ÁE�ÁE�ÉIÉuÉWÉFÉNÉgÁE�ÁE�É|ÉCÉìÉ^  é∏îs : nullptr
		*/
		static EnemySettingFile* create( const std::string& fileName);

	private:
		// ä÷êîêÈåæóp (è⁁E�◊ÁE�å„ï˚ÁE��ãLèq)
		struct EnemyData;

	public:
		enum _DATA_LIST_COUNT
		{
			WAVE_ENEMY_COUNT	= 20,	// 1waveíÜÁE��èoåªÁE�EÁE�ìGÁE�ëçêî
		};

		unsigned int loadCount;			// ì«ÁE��çûÁE��ÉfÁEÉ^êî
		EnemyData* dataList[WAVE_ENEMY_COUNT];	// ì«ÁE��çûÁE�ÁE�ìGÉfÁEÉ^ÁE�ï€ë∂îzóÁE

	private:
		EnemySettingFile();

		/**
		*	ÉAÉjÉÁE�EÉVÉáÉìñºÁE��éwÁE�Eï∂éöóÒÁE�ÁE�E�îílÁE��åüçıÁE�EÁE�E
		*
		*	@author	minaka
		*	@param	dataString å≥ÉAÉjÉÁE�EÉVÉáÉìñº
		*	@return	åüçıåãâ ÁE�êîílÁEî≠å©ÁE��ÁE�ÁE�ÁE�ÁE�ÁE�èÍçáÁE�E: -999
		*/
		static int getAIAppearNumber( const std::string& dataString);

		/**
		*	ÉAÉjÉÁE�EÉVÉáÉìñºÁE��éwÁE�Eï∂éöóÒÁE�ÁE�E�îílÁE��åüçıÁE�EÁE�E
		*
		*	@author	minaka
		*	@param	dataString å≥ÉAÉjÉÁE�EÉVÉáÉìñº
		*	@return	åüçıåãâ ÁE�êîílÁEî≠å©ÁE��ÁE�ÁE�ÁE�ÁE�ÁE�èÍçáÁE�E: -999
		*/
		static int getAIMoveNumber( const std::string& dataString);

		/**
		*	ÉAÉjÉÁE�EÉVÉáÉìñºÁE��éwÁE�Eï∂éöóÒÁE�ÁE�E�îílÁE��åüçıÁE�EÁE�E
		*
		*	@author	minaka
		*	@param	dataString å≥ÉAÉjÉÁE�EÉVÉáÉìñº
		*	@return	åüçıåãâ ÁE�êîílÁEî≠å©ÁE��ÁE�ÁE�ÁE�ÁE�ÁE�èÍçáÁE�E: -999
		*/
		static int getAIAttackNumber( const std::string& dataString);

		/**
		*	ÉAÉjÉÁE�EÉVÉáÉìñºÁE��éwÁE�Eï∂éöóÒÁE�ÁE�E�îílÁE��åüçıÁE�EÁE�E
		*
		*	@author	minaka
		*	@param	dataString å≥ÉAÉjÉÁE�EÉVÉáÉìñº
		*	@return	åüçıåãâ ÁE�êîílÁEî≠å©ÁE��ÁE�ÁE�ÁE�ÁE�ÁE�èÍçáÁE�E: -999
		*/
		static int getAILifeCycleNumber( const std::string& dataString);

		enum _DATA_NUMBER_	// äeÉfÁEÉ^ÁE�êî
		{
			BLOCK_LINE_COUNT		= 6,	// 1ÉuÉçÉbÉNÁE�çsêî

			TARGET_POS_COUNT		= 3,	// ñ⁁E�Wínì_êî
			WAIT_TO_ATTACK_COUNT	= 3,	// çUåÁE�Oë“ã@éûä‘êî
			WAIT_TO_MOVE_COUNT		= 3,	// çUåÁE�„ë“ã@éûä‘êî
			NEXT_ENEMYS_COUNT		= 3,	// éüÁE��èoåªÁE��ÁE�ÁE�ìGÉfÁEÉ^êî
			AI_MOVE_COUNT			= 2,	// à⁁E�ÁE�ÁE�EÉVÉáÉìêî
			AI_ATTACK_COUNT			= 3,	// çUåÁE�ÁE�EÉVÉáÉìêî
		};

		struct EnemyData
		{
			// ÉfÁEÉ^î‘çÁE
			int Num;
			// èoåªínì_
			cocos2d::Vec3 startPos;
			// ë“ã@éûä E
			int sleepTime;
			// ñ⁁E�Wínì_
			cocos2d::Vec3 targetPos[TARGET_POS_COUNT];
			// à⁁E�ÁE�„ÁE�ë“ã@éûä E
			int waitToAttack[WAIT_TO_ATTACK_COUNT];
			// èoåªÉtÉâÉO
			bool entryFlag;
			// çUåÁE�„ÁE�ë“ã@éûä E
			int waitToMove[WAIT_TO_MOVE_COUNT];
			// éüèoåªÁE��ÁE�ÁE�ìGÁE�î‘çÁE
			int nextEnemies[NEXT_ENEMYS_COUNT];
			// ìoèÍéûÉÁE�EÉVÉáÉìî‘çÁE
			int aiAppear;
			// à⁁E�ÁE�ÁE�EÉVÉáÉìî‘çÁE
			int aiMove[AI_MOVE_COUNT];
			// çUåÁE�ÁE�EÉVÉáÉìî‘çÁE
			int aiAtk[AI_ATTACK_COUNT];
			// ÉâÉCÉtÉTÉCÉNÉãî‘çÁE
			int aiLifeCycle;
		};
		unsigned int fileLineCount;			// åªç›ÁE�çsî‘çÁE
	};
}

#endif // __ENEMY_SETTING_FILE_H__
