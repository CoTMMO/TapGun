
#ifndef __SOUND_H__
#define __SOUND_H__

#include "cocos2d.h"

namespace TapGun
{
	class Sound
	{
	public:
		Sound( const Sound &P) = delete;
		Sound& operator= ( const Sound &P) = delete;

		/**
		 *	サウンド管理クラスのインスタンス取得
		 *
		 *	@author	minaka
		 *	@param	fileName ファイル名
		 *	@return	インスタンスへのポインタ
		 */
		static Sound* getInstance( void);

		/**
		 *	BGMファイルの読み込み
		 *
		 *	@author	minaka
		 *	@param	fileName ファイル名
		 *	@return	なし
		 */
		void loadBGM( const std::string& fileName);

		/**
		 *	BGMファイルの読み込み（ボリューム調整付き）
		 *
		 *	@author	minaka
		 *	@param	fileName ファイル名
		 *	@param	Volume ボリュームレベル ( 0.0 ~ 100.0 )
		 *	@return	なし
		 */
		void loadBGM( const std::string& fileName, float Volume);

		/**
		 *	BGMの再生
		 *
		 *	@author	minaka
		 *	@return	正常終了:0　再生失敗:-1
		 */
		int playBGM( void);

		/**
		 *	BGMの再生（ボリューム調整付き）
		 *
		 *	@author	minaka
		 *	@param	Volume ボリュームレベル ( 0.0 ~ 100.0 )
		 *	@return	正常終了:0　再生失敗:-1
		 */
		int playBGM( float Volume);

		/**
		 *	BGMのループ再生
		 *
		 *	@author	minaka
		 *	@return	正常終了:0　再生失敗:-1
		 */
		int playBGMLoop( void);

		/**
		 *	BGMのループ再生（ボリューム調整付き）
		 *
		 *	@author	minaka
		 *	@param	Volume ボリュームレベル ( 0.0 ~ 100.0 )
		 *	@return	正常終了:0　再生失敗:-1
		 */
		int playBGMLoop( float Volume);

		/**
		 *	BGMのボリューム設定
		 *
		 *	@author	minaka
		 *	@param	Volume ボリュームレベル ( 0.0 ~ 100.0 )
		 *	@return	なし
		 */
		void setBGMVolume( float Volume);

		/**
		 *	BGMの停止
		 *
		 *	@author	minaka
		 *	@return	なし
		 */
		void stopBGM( void);

		/**
		 *	BGMの一時停止
		 *
		 *	@author	minaka
		 *	@return	なし
		 */
		void pauseBGM( void);

		/**
		 *	BGMの再開
		 *
		 *	@author	minaka
		 *	@return	なし
		 */
		void resumeBGM( void);

		/**
		 *	BGMのリスタート
		 *
		 *	@author	minaka
		 *	@return	なし
		 */
		void restartBGM( void);

		/**
		 *	BGMの再生中チェック
		 *
		 *	@author	minaka
		 *	@return	再生中:1 再生中でない:0
		 */
		int playCheckBGM( void);

		/**
		 *	SEファイルの読み込み
		 *
		 *	@author	minaka
		 *	@param	fileName ファイル名
		 *	@return	正常終了:0 エラー発生時:-1
		 */
		int loadSE( const std::string& fileName);

		/**
		 *	SEファイルの読み込み（ボリューム調整付き）
		 *
		 *	@author	minaka
		 *	@param	fileName ファイル名
		 *	@param	Volume ボリュームレベル ( 0.0 ~ 100.0 )
		 *	@return	正常終了:0 エラー発生時:-1
		 */
		int loadSE( const std::string& fileName, float Volume);

		/**
		 *	SEの再生
		 *
		 *	@author	minaka
		 *	@param	fileName ファイル名
		 *	@return	正常終了:0 エラー発生時:-1
		 */
		int playSE( const std::string& fileName);

		/**
		 *	SEの再生（ボリューム調整付き）
		 *
		 *	@author	minaka
		 *	@param	fileName ファイル名
		 *	@param	Volume ボリュームレベル ( 0.0 ~ 100.0 )
		 *	@return	正常終了:0 エラー発生時:-1
		 */
		int playSE( const std::string& fileName, float Volume);

		/**
		 *	SE全体のボリューム調整
		 *
		 *	@author	minaka
		 *	@param	Volume ボリュームレベル ( 0.0 ~ 100.0 )
		 *	@return	なし
		 */
		void setSEVolume( float Volume);

		/**
		 *	SEの停止
		 *
		 *	@author	minaka
		 *	@param	fileName ファイル名
		 *	@return	正常終了:0 エラー発生時:-1
		 */
		int stopSE( const std::string& fileName);

		/**
		 *	SEの再開
		 *
		 *	@author	minaka
		 *	@param	fileName ファイル名
		 *	@return	正常終了:0 エラー発生時:-1
		 */
		int pauseSE( const std::string& fileName);

		/**
		 *	SEのリスタート
		 *
		 *	@author	minaka
		 *	@param	fileName ファイル名
		 *	@return	正常終了:0 エラー発生時:-1
		 */
		int resumeSE( const std::string& fileName);

		/**
		 *	全SEの停止
		 *
		 *	@author	minaka
		 *	@return	なし
		 */
		void stopSEAll( void);

		/**
		 *	全SEの再開
		 *
		 *	@author	minaka
		 *	@return	なし
		 */
		void pauseSEAll( void);

		/**
		 *	全SEのリスタート
		 *
		 *	@author	minaka
		 *	@return	なし
		 */
		void resumeSEAll( void);

		/**
		 *	SE用のメモリ解放
		 *
		 *	@author	minaka
		 *	@param	fileName ファイル名
		 *	@return	正常終了:0 エラー発生時:-1
		 */
		int releaseSE( const std::string& fileName);

		/**
		 *	全SE用のメモリ解放
		 *
		 *	@author	minaka
		 *	@return	なし
		 */
		void releaseSEAll( void);

	private:
		struct SoundData			// SE・Voice管理用構造体
		{
			int ID;					// cocos2d?xから渡されるID
			std::string fileName;	// アクセス用ファイル名
			std::string filePath;	// 読み込み用ディレクトリパス
		};

		Sound();

		// 現在読み込まれているのBGMのファイル名
		std::string bgmFileName;
		// 読み込まれているSEのリスト
		std::vector<SoundData*> seDataList;
	};
}

#endif // __SOUND_H__
