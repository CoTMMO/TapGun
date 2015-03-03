
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
		int Num;					// �G�̎��ʔԍ��ł��BWave���Ƃ�0�Ԃ���n�߂Ă�������
		cocos2d::Vec3 startPos;		// �G���B��Ă�����W�ł�
		float sleeptime;			// �O�̓G���|��Ă���A���̓G���o�Ă���܂ł̑҂����Ԃł�
		cocos2d::Vec3 targetPos[3];	// ���̓G�̖ڕW�n�_�ł��B�S�Ă��g���K�v�͂���܂���B
		int waitToAtack;			// �ڕW�n�_�ɓ������Ă���U������܂ł̑҂����Ԃł�
		int waitToMove[3];			// �U���I�����Ă��玟�̍s�����J�n����܂ł̑҂����ԁi�A�C�h�����ԁj�ł��B�~���b�ł��B
		int NextEnemies[3];			// ���̓G���|��Ă���A���ɏo�Ă���G�ł��B3�̂܂ŉ\�ł��B

		int AIAppear;				// �o�����̃��[�V�����ł�
		int AIMove[3];				// �ړ����̃��[�V�����ł�
		int AIAtk[3];				// �U�����̃��[�V�����ł�
		int AILifeCycle;			//
	};

	WaveData* dataList[10];			// �d�l��10�̈ȏ�̓G�͏o�����Ȃ��̂� 10
};

}

#endif // __ENEMY_SETTING_FILE_H__
