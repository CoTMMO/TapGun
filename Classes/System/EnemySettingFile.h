
#ifndef __ENEMY_SETTING_FILE_H__
#define __ENEMY_SETTING_FILE_H__

#include "cocos2d.h"

namespace TapGun
{
	class EnemySettingFile
	{
	public:
		/**
		*	ìGê›íËÉtÉ@ÉCÉãÇÃì«Ç›çûÇ›Ç∆ê›íËópÉIÉuÉWÉFÉNÉgÇÃê∂ê¨
		*
		*	@author	minaka
		*	@param	fileName ê›íËÉtÉ@ÉCÉãñº
		*	@return	ê≥èÌèIóπ : ê∂ê¨ÇµÇΩÉIÉuÉWÉFÉNÉgÇ÷ÇÃÉ|ÉCÉìÉ^  é∏îs : nullptr
		*/
		static EnemySettingFile* create( const std::string& fileName);

	private:
		// ä÷êîêÈåæóp (è⁄ç◊ÇÕå„ï˚Ç…ãLèq)
		struct EnemyData;

	public:
		enum _DATA_LIST_COUNT
		{
			WAVE_ENEMY_COUNT	= 20,	// 1waveíÜÇ…èoåªÇ∑ÇÈìGÇÃëçêî
		};

		unsigned int loadCount;			// ì«Ç›çûÇ›ÉfÅ[É^êî
		EnemyData* dataList[WAVE_ENEMY_COUNT];	// ì«Ç›çûÇÒÇæìGÉfÅ[É^ÇÃï€ë∂îzóÒ

	private:
		EnemySettingFile();

		/**
		*	ÉAÉjÉÅÅ[ÉVÉáÉìñºÇéwÇ∑ï∂éöóÒÇ©ÇÁêîílÇåüçıÇ∑ÇÈ
		*
		*	@author	minaka
		*	@param	dataString å≥ÉAÉjÉÅÅ[ÉVÉáÉìñº
		*	@return	åüçıåãâ ÇÃêîílÅ@î≠å©Ç≈Ç´Ç»Ç©Ç¡ÇΩèÍçáÇÕ : -999
		*/
		static int getAIAppearNumber( const std::string& dataString);

		/**
		*	ÉAÉjÉÅÅ[ÉVÉáÉìñºÇéwÇ∑ï∂éöóÒÇ©ÇÁêîílÇåüçıÇ∑ÇÈ
		*
		*	@author	minaka
		*	@param	dataString å≥ÉAÉjÉÅÅ[ÉVÉáÉìñº
		*	@return	åüçıåãâ ÇÃêîílÅ@î≠å©Ç≈Ç´Ç»Ç©Ç¡ÇΩèÍçáÇÕ : -999
		*/
		static int getAIMoveNumber( const std::string& dataString);

		/**
		*	ÉAÉjÉÅÅ[ÉVÉáÉìñºÇéwÇ∑ï∂éöóÒÇ©ÇÁêîílÇåüçıÇ∑ÇÈ
		*
		*	@author	minaka
		*	@param	dataString å≥ÉAÉjÉÅÅ[ÉVÉáÉìñº
		*	@return	åüçıåãâ ÇÃêîílÅ@î≠å©Ç≈Ç´Ç»Ç©Ç¡ÇΩèÍçáÇÕ : -999
		*/
		static int getAIAttackNumber( const std::string& dataString);

		/**
		*	ÉAÉjÉÅÅ[ÉVÉáÉìñºÇéwÇ∑ï∂éöóÒÇ©ÇÁêîílÇåüçıÇ∑ÇÈ
		*
		*	@author	minaka
		*	@param	dataString å≥ÉAÉjÉÅÅ[ÉVÉáÉìñº
		*	@return	åüçıåãâ ÇÃêîílÅ@î≠å©Ç≈Ç´Ç»Ç©Ç¡ÇΩèÍçáÇÕ : -999
		*/
		static int getAILifeCycleNumber( const std::string& dataString);

		enum _DATA_NUMBER_	// äeÉfÅ[É^ÇÃêî
		{
			BLOCK_LINE_COUNT		= 6,	// 1ÉuÉçÉbÉNÇÃçsêî

			TARGET_POS_COUNT		= 3,	// ñ⁄ïWínì_êî
			WAIT_TO_ATTACK_COUNT	= 3,	// çUåÇëOë“ã@éûä‘êî
			WAIT_TO_MOVE_COUNT		= 3,	// çUåÇå„ë“ã@éûä‘êî
			NEXT_ENEMYS_COUNT		= 3,	// éüÇ…èoåªÇ≥ÇπÇÈìGÉfÅ[É^êî
			AI_MOVE_COUNT			= 2,	// à⁄ìÆÉÇÅ[ÉVÉáÉìêî
			AI_ATTACK_COUNT			= 3,	// çUåÇÉÇÅ[ÉVÉáÉìêî
		};

		struct EnemyData
		{
			// ÉfÅ[É^î‘çÜ
			int Num;
			// èoåªínì_
			cocos2d::Vec3 startPos;
			// ë“ã@éûä‘
			int sleepTime;
			// ñ⁄ïWínì_
			cocos2d::Vec3 targetPos[TARGET_POS_COUNT];
			// à⁄ìÆå„ÇÃë“ã@éûä‘
			int waitToAttack[WAIT_TO_ATTACK_COUNT];
			// èoåªÉtÉâÉO
			bool entryFlag;
			// çUåÇå„ÇÃë“ã@éûä‘
			int waitToMove[WAIT_TO_MOVE_COUNT];
			// éüèoåªÇ≥ÇπÇÈìGÇÃî‘çÜ
			int nextEnemies[NEXT_ENEMYS_COUNT];
			// ìoèÍéûÉÇÅ[ÉVÉáÉìî‘çÜ
			int aiAppear;
			// à⁄ìÆÉÇÅ[ÉVÉáÉìî‘çÜ
			int aiMove[AI_MOVE_COUNT];
			// çUåÇÉÇÅ[ÉVÉáÉìî‘çÜ
			int aiAtk[AI_ATTACK_COUNT];
			// ÉâÉCÉtÉTÉCÉNÉãî‘çÜ
			int aiLifeCycle;
		};
		unsigned int fileLineCount;			// åªç›ÇÃçsî‘çÜ
	};
}

#endif // __ENEMY_SETTING_FILE_H__
