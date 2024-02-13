//=============================================================================
//
// サウンド処理 [sound.h]
// Author 小原立暉
//
//=============================================================================
#ifndef _SOUND_H_					//このマクロ定義がされていなかったら
#define _SOUND_H_					//2重インクルード防止のマクロを定義する

#include "main.h"

// クラス定義(CSound)
class CSound
{
public:			// 誰でもアクセス出来る

	//-------------------------------------------------------------------------
	// サウンド一覧
	//-------------------------------------------------------------------------
	typedef enum
	{
		SOUND_LABEL_BGM_TITLE = 0,		// タイトルBGM
		SOUND_LABEL_BGM_TUTORIAL,		// チュートリアルBGM
		SOUND_LABEL_BGM_GAME,			// ゲームBGM
		SOUND_LABEL_BGM_RESULT,			// リザルトBGM
		SOUND_LABEL_BGM_RANKING,		// ランキングBGM
		SOUND_LABEL_SE_RUNSTART,		// 走り始め音
		SOUND_LABEL_SE_JUMP,			// ジャンプ音
		SOUND_LABEL_SE_BIGJUMP,			// 大ジャンプ音
		SOUND_LABEL_SE_CLIFFCATCH,		// 崖捕まり
		SOUND_LABEL_SE_WALLKICK,		// 壁キック
		SOUND_LABEL_SE_TURBO,			// ターボ
		SOUND_LABEL_SE_LANDINGMISS,		// 着地ミス
		SOUND_LABEL_SE_CRUSH,			// 破壊音
		SOUND_LABEL_SE_METALCRUSH,		// 金属物破壊音
		SOUND_LABEL_SE_LANDING,			// 着地音
		SOUND_LABEL_SE_BIGLANDING,		// 高いところからの着地音
		SOUND_LABEL_SE_GOALCHEERS,		// ゴール時の歓声
		SOUND_LABEL_SE_DASH,			// 走行音
		SOUND_LABEL_SE_DRINKGET,		// ドリンクゲット
		SOUND_LABEL_SE_COUNTDOWN,		// カウントダウン
		SOUND_LABEL_SE_LAMPCREAK,		// 街灯の軋む音
		SOUND_LABEL_SE_LAMPJUMP,		// 街灯ジャンプ
		SOUND_LABEL_SE_CLIFFSUCCESS,	// 崖着地成功
		SOUND_LABEL_SE_CLIFFFAILED,		// 崖着地失敗
		SOUND_LABEL_SE_PRESSVOICE,		// 潰れたときの声
		SOUND_LABEL_SE_RUBBING,			// 体を擦る音
		SOUND_LABEL_SE_SELECT,			// 選択音
		SOUND_LABEL_SE_DECIDE,			// 決定音
		SOUND_LABEL_SE_START,			// スタート音
		SOUND_LABEL_SE_DRUMROLL,		// ドラム缶の転がる音
		SOUND_LABEL_SE_SIGNPOLE_FALL,	// 標識の倒れる音
		SOUND_LABEL_SE_SIGNPOLE_STOP,	// 標識の止まる音
		SOUND_LABEL_SE_SUPERDASH_START,	// スーパーブーストスタート
		SOUND_LABEL_SE_SUPERDASH,		// スーパーブースト音
		SOUND_LABEL_SE_CLIFFLAND,		// 崖着地の音
		SOUND_LABEL_SE_LAMPCATCH,		// 街灯掴む音
		SOUND_LABEL_SE_FALLING,			// 落下音
		SOUND_LABEL_SE_ATTACK,			// 攻撃音
		SOUND_LABEL_SE_JUMPCHARGE,		// 超ジャンプチャージ音
		SOUND_LABEL_SE_VENDING_SWING,	// 自販機の揺れる音
		SOUND_LABEL_SE_ITEM_APPEAR,		// アイテムの出現音
		SOUND_LABEL_SE_SKIPCIRCLE,		// スキップ円の出現音
		SOUND_LABEL_SE_RANKAPPEAR,		// ランクの出現音
		SOUND_LABEL_SE_NEWRECORD,		// 新記録の音
		SOUND_LABEL_MAX					// この列挙型の総数
	} SOUND_LABEL;

	//-----------------------------------------------------------------------------
	// サウンド情報の構造体定義
	//-----------------------------------------------------------------------------
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDINFO;

	CSound();			// コンストラクタ
	~CSound();			// デストラクタ

	// メンバ関数
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT Play(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	void Stop(void);

private:		// 自分だけアクセスできる

	// メンバ関数
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	// メンバ変数
	IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ
	static SOUNDINFO m_aSoundInfo[CSound::SOUND_LABEL_MAX];	// サウンドの情報
};

#endif
