// デバッグ版
#ifdef _DEBUG
//============================================
//
// エディットヘッダー[edit.h]
// Author：小原立暉
//
//============================================
#ifndef _EDIT_H_					// このマクロ定義がされていなかったら
#define _EDIT_H_					// 2重インクルード防止のマクロを定義する
//********************************************
// インクルードファイル
//********************************************
#include "model.h"
#include "enemy.h"
#include "block.h"

//--------------------------------------------
// クラス定義(エディット)
//--------------------------------------------
class CEdit : public CModel
{
public:				// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_ITEM = 0,		// アイテム
		TYPE_MACCHINA,		// マキナ草
		TYPE_ENEMY,			// 敵
		TYPE_TABLE,			// 台
		TYPE_AIRPLANE,		// 飛行機
		TYPE_NEEDLE,		// 棘
		TYPE_BLOCK,			// ブロック
		TYPE_MAX			// この列挙型の総数
	};

	CEdit();				// コンストラクタ
	~CEdit();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos);			// 情報の設定処理

	// 静的メンバ変数
	static CEdit* Create(const D3DXVECTOR3& pos);	// 生成処理

private:			// 自分だけアクセスできる

	// メンバ関数
	void ItemProcess(void);		// アイテムエディットの処理
	void MacchinaProcess(void);	// マキナ草エディットの処理
	void EnemyProcess(void);	// 敵エディットの処理
	void BlockProcess(void);	// ブロックエディットの処理

	void SetMaccina(void);		// マキナ草の設定処理

	void MoveInput(void);		// 移動のインプット処理
	void AdjustMoveInput(void);	// 微調整移動のインプット処理
	void Move(void);			// 移動処理
	void Type(void);			// 種類設定処理
	void Front(void);			// 奥行設定処理
	void Set(void);				// 設定処理
	void Delete(void);			// 消去処理

	// 消去処理
	void DeleteItem(void);		// アイテムの消去処理
	void DeleteMacchina(void);	// マキナ草の消去処理
	void DeleteEnemy(void);		// 敵の消去処理
	void DeleteTable(void);		// 台の消去処理
	void DeleteAirplane(void);	// 飛行機の消去処理
	void DeleteNeedle(void);	// 棘の消去処理
	void DeleteBlock(void);		// ブロックの消去処理

	// メンバ変数
	D3DXVECTOR3 m_move;				// 移動量
	TYPE m_type;					// 種類
	CEnemy::TYPE m_enemyType;		// 敵の種類
	CBlock::TYPE m_blockType;		// ブロックの種類
	CBlock::DROPTYPE m_dropType;	// ドロップの種類
	float m_fAddDepth;				// 追加の奥行
	bool m_bFront;					// 奥行状況
	bool m_bAirplaneFront;			// 飛行機の向きの状況
};

#endif

#endif