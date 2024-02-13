//============================================
//
// 破片ヘッダー[fraction.h]
// Author：小原立暉
//
//============================================
#ifndef _FRACTION_H_			//このマクロ定義がされていなかったら
#define _FRACTION_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "model.h"

//--------------------------------------------
// クラス(破片クラス)
//--------------------------------------------
class CFraction : public CModel
{
public:			// 誰でもアクセスできる

	CFraction();			// コンストラクタ
	~CFraction();			// デストラクタ

	// メンバ関数
	HRESULT Init();			// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3 scale, const D3DXVECTOR3& move, const int nTexIdx, const D3DXCOLOR col);			// 情報の設定処理

	static void SetObstacle(const D3DXVECTOR3& center, const int nTexIdx, const D3DXCOLOR col);			// 障害物の破片の設定処理
	static void SetWallKick(const D3DXVECTOR3& center, const int nTexIdx, const D3DXCOLOR col);			// 壁キック時の破片の設定処理
	static void SetLCliffCatch(const D3DXVECTOR3& center, const int nTexIdx, const D3DXCOLOR col);		// 左の崖掴み時の破片の設定処理
	static void SetRCliffCatch(const D3DXVECTOR3& center, const int nTexIdx, const D3DXCOLOR col);		// 右の崖掴み時の破片の設定処理

	// 静的メンバ関数
	static CFraction* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3 scale, const D3DXVECTOR3& move, const int nTexIdx, const D3DXCOLOR col);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	D3DXVECTOR3 m_move;		// 移動量
	D3DXCOLOR m_col;		// 色
};

#endif