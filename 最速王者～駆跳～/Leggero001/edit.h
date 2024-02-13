#ifdef _DEBUG	// デバッグ処理
//===================================
//
// エディットヘッダー[EditObject.h]
// Author 小原立暉
//
//===================================
#ifndef _EDITOBJECT_H_
#define _EDITOBJECT_H_

#include "main.h"
#include "block.h"

//------------------------------------------
// クラス定義(エディット)
//------------------------------------------
class CEdit : public CBlock
{
public:			// 誰でもアクセスできる

	CEdit();			// コンストラクタ
	~CEdit();			// デストラクタ

	//プロトタイプ宣言
	HRESULT Init(void);		// オブジェクトの初期化処理
	void Uninit(void);		// オブジェクトの終了処理
	void Update(void);		// オブジェクトの更新処理
	void Draw(void);		// オブジェクトの描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);		// 情報の

	// 静的メンバ関数
	static CEdit* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void TypeChange(void);		// 種類変更処理
	void Move(void);			// 移動処理
	void Rotation(void);		// 回転処理
	void Set(void);				// オブジェクトの設定処理
	void Delete(void);			// オブジェクトの消去
	void ScaleX(void);			// オブジェクトの拡大縮小処理(X軸)
	void ScaleY(void);			// オブジェクトの拡大縮小処理(Y軸)
	void Scale(void);			// オブジェクトの拡大縮小処理
	void Reset(void);			// オブジェクトの情報リセット処理
	void MaterialCustom(void);	// マテリアルのエディット処理
	void RightAngle(void);		// オブジェクトの直角処理
	void PlayerPosSet(void);	// オブジェクトのプレイヤー位置移動
	void CliffChange(void);		// 崖捕まりの変化処理

	// メンバ変数
	D3DXMATERIAL m_MatCopy[MAX_MATERIAL];					// マテリアルのコピー
	D3DXMATERIAL m_EditMaterial[MAX_MATERIAL];				// カスタム用のマテリアル情報
	int m_nCntMaterial;										// マテリアルの番号
	int m_nColorCount;										// 色を変えるときのカウント
};

#endif

#endif