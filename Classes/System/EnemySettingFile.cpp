
#include <fstream>
#include "cocos2d.h"

#include "Platform.h"
#include "Stage/Enemy.h"
#include "System/EnemySettingFile.h"

USING_NS_CC;
using namespace std;
using namespace TapGun;

/**
*	敵設定ファイルの読み込みと設定用オブジェクトの生成
*
*	@author	minaka
*	@param	fileName 設定ファイル名
*	@return	正常終了 : 生成したオブジェクトへのポインタ  失敗 : nullptr
*/
EnemySettingFile* EnemySettingFile::create( const string& fileName)
{
	// オブジェクト生成
	auto settingFile = new (nothrow) EnemySettingFile();
	// ファイルパス制御クラスのインスタンスを取得
	auto access = FileAccess::getInstance();
	// 設定ファイルを入力ストリームに設定
	stringstream file( access -> getFileStream( access -> getEnemySettingFilePath( fileName)));

	// エラー判定
	if( file.fail())
	{
		log( "Animation File Load Error");
		return nullptr;
	}
	
	string str;						// ファイルから取得した文字列を格納
	string tmp;						// 読み込んだ文字列の一時保存
	istringstream stream(str);		// 文字列を編集可能に
	WaveData* data = new WaveData;	// 読み込んだ情報を格納する構造体を生成
	Vec3 vector;					// ファイルに存在するベクトル情報を一時保存

	// 読み込み行カウンタと読み込みブロックカウンタを初期化しファイル終端までループ
	for( settingFile -> fileLineCount = 1, settingFile -> loadCount = 0; getline( file, str); settingFile -> fileLineCount++)
	{
		// 1ブロック分のデータを読み込んだ場合
		if( settingFile -> fileLineCount > BLOCK_LINE_COUNT)
		{
			// 配列に読み込んだデータを保存
			settingFile -> dataList[settingFile -> loadCount] = data;
			// 読み込みブロックカウンタを増加
			settingFile -> loadCount++;
			// 読み込み行番号を初期化
			settingFile -> fileLineCount = 1;
		}

		if( settingFile -> loadCount >= WAVE_ENEMY_COUNT)
		{
			log( "Enemy Seting File Block Num Over");
			return settingFile;
		}

		switch( settingFile -> fileLineCount)			// 現在の行番号で処理を分岐
		{
		case 1:							// 奇数行にはデータが存在しないファイル構成なのでスキップ
		case 3:
		case 5:
			break;

		case 2:
			// ウェーブナンバーを取得
			// カンマ区切りでデータを取得 (以下同じ処理はコメント省略)
			getline( stream, tmp, ',');	
			// 読み込んだ文字列を数値に変換し保存 (以下類似処理はコメント省略)
			data -> Num = atoi( tmp.c_str());	

			// 出現地点座標を取得
			// データ取得
			getline( stream, tmp, ',');	
			// 元データに存在する ""(" の2文字を削除
			tmp.erase( 0, 2);
			vector.x = atoi( tmp.c_str());

			getline( stream, tmp, ',');
			vector.y = atoi( tmp.c_str());

			getline( stream, tmp, ',');
			vector.z = atoi( tmp.c_str());

			// 取得したデータVec3形式で格納
			data -> startPos = vector;

			// 待機時間を取得
			getline( stream, tmp, ',');
			data -> sleepTime = atoi( tmp.c_str());

			// 目標地点座標を取得 (最大3か所)
			for( int i = 0; i < TARGET_POS_COUNT; i++)
			{
				getline( stream, tmp, ',');
				// 元データに存在する ""(" の2文字を削除
				tmp.erase( 0, 2);
				vector.x = atoi( tmp.c_str());
				getline( stream, tmp, ',');
				vector.y = atoi( tmp.c_str());
				getline( stream, tmp, ',');
				vector.z = atoi( tmp.c_str());
				data -> targetPos[i] = vector;
			}

			// 攻撃に移るまでの待機時間を取得
			getline( stream, tmp, ',');
			data -> waitToAtack = atoi( tmp.c_str());
			break;

		case 4:
			// 戦闘の','まではデータ無しなので空読み
			getline( stream, tmp, ',');

			// 一連の行動終了から次の行動開始までの待機時間を取得
			for( int i = 0; i < WAIT_TO_MOVE_COUNT; i++)
			{
				getline( stream, tmp, ',');
				data -> waitToMove[i] = atoi( tmp.c_str());
			}

			// 自分が倒された後に出現する敵の配列番号を取得
			for( int i = 0; i < NEXT_ENEMYS_COUNT; i++)
			{
				getline( stream, tmp, ',');
				data -> nextEnemies[i] = atoi( tmp.c_str());
			}
			break;

		case 6:
			// 戦闘の','まではデータ無しなので空読み
			getline( stream, tmp, ',');

			// 出現時のモーション名を取得
			getline( stream, tmp, ',');

			// 取得した文字列を元に番号を検索し格納
			data -> aiAppear = getModelNumber( tmp);

			// 移動時のモーション名を取得し設定
			for( int i = 0; i < AI_MOVE_COUNT; i++)
			{
				// 移動時のモーション名を取得
				getline( stream, tmp, ',');
				// 取得した文字列を元に番号を検索し格納
				data -> aiMove[i] = getModelNumber( tmp);
			}

			// 攻撃時のモーション名を取得し設定
			for( int i = 0; i < AI_ATTACK_COUNT; i++)
			{
				// 攻撃時のモーション名を取得
				getline( stream, tmp, ',');
				// 取得した文字列を元に番号を検索し格納
				data -> aiAtk[i] = getModelNumber( tmp);
			}

			// 攻撃時のモーション名を取得
			getline( stream, tmp, ',');
			// 取得した文字列を元に番号を検索し格納
			data -> aiLifeCycle = getModelNumber( tmp);
			break;

		default:
			break;
		}
	}
	return settingFile;
}

/**
*	アニメーション名を指す文字列から数値を検索する
*
*	@author	minaka
*	@param	dataString 元アニメーション名
*	@return	検索結果の数値　発見できなかった場合は : -999
*/
int EnemySettingFile::getModelNumber( const string& dataString)
{
	// モーション名に合わせた数値を格納

	//ここから出現
	if( dataString == "run")
	{
		return AI_APPEAR_RUN;
	}
	else if (dataString == "walk")
	{
		return AI_APPEAR_WALK;
	}
	else if (dataString == "sWalk")
	{
		return AI_APPEAR_SWALK;
	}
	else if (dataString == "slide")
	{
		return AI_APPEAR_SLIDE;
	}
	else if (dataString == "acro")
	{
		return AI_ATK_ACRO;
	}
	else if (dataString == "fJump")
	{
		return AI_APPEAR_FJUMP;
	}

	//ここから移動
	else if (dataString == "none")
	{
		return AI_MOVE_NONE;
	}
	else if (dataString == "run")
	{
		return AI_MOVE_RUN;
	}
	else if (dataString == "walk")
	{
		return AI_MOVE_WALK;
	}
	else if (dataString == "sWalk")
	{
		return AI_MOVE_SWALK;
	}
	else if (dataString == "slide")
	{
		return AI_MOVE_SLIDE;
	}
	else if (dataString == "acro")
	{
		return AI_MOVE_ACRO;
	}

	//ここから攻撃
	else if (dataString == "none")
	{
		return AI_ATK_NONE;
	}
	else if (dataString == "fake")
	{
		return AI_ATK_FAKE;
	}
	else if (dataString == "shot")
	{
		return AI_ATK_SSHOT;
	}
	else if (dataString == "slide")
	{
		return AI_ATK_SLIDE;
	}
	else if (dataString == "acro")
	{
		return AI_MOVE_ACRO;
	}

	//ここからライフサイクル
	else if (dataString == "once")
	{
		return AI_LIFE_ONCE;
	}
	else if (dataString == "switch")
	{
		return AI_LIFE_SWITCH;
	}
	else if (dataString == "stop")
	{
		return AI_LIFE_STOP;
	}

	else
	{
		return -999;
	}
}