
#ifndef __ENEMY_SETTING_FILE_H__
#define __ENEMY_SETTING_FILE_H__

#include "cocos2d.h"

namespace TapGun
{
class EnemySettingFile
{
public:
	EnemySettingFile( const EnemySettingFile &P) = delete;
	EnemySettingFile& operator= ( const EnemySettingFile &P) = delete;
	static EnemySettingFile* getInstance( void);

	void loadTableFile( const std::string& fileName);

private:
	EnemySettingFile() {}

	struct WaveData
	{
		int Num;					// 敵の識別番号です。Waveごとに0番から始めてください
		cocos2d::Vec3 startPos;		// 敵が隠れている座標です
		float sleeptime;			// 前の敵が倒れてから、この敵が出てくるまでの待ち時間です
		cocos2d::Vec3 targetPos[3];	// この敵の目標地点です。全てを使う必要はありません。
		int waitToAtack;			// 目標地点に到着してから攻撃するまでの待ち時間です
		int waitToMove[3];			// 攻撃終了してから次の行動を開始するまでの待ち時間（アイドル時間）です。ミリ秒です。
		int NextEnemies[3];			// この敵が倒れてから、次に出てくる敵です。3体まで可能です。

		int AIAppear;				// 出現時のモーションです
		int AIMove[3];				// 移動時のモーションです
		int AIAtk[3];				// 攻撃時のモーションです
		int AILifeCycle;			//
	};

	WaveData* dataList[10];			// 仕様上10体以上の敵は出現しないので 10
};

}

#endif // __ENEMY_SETTING_FILE_H__
