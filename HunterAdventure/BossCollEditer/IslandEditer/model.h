//============================================================
//
// モデルヘッダー [model.h]
// Author：小原立暉
//
//============================================================
#ifndef _MODEL_H_	// このマクロ定義がされていない場合
#define _MODEL_H_	// 二重インクルード防止のマクロを定義する

//************************************************************
// インクルードファイル
//************************************************************
#include "object.h"
#include "objectX.h"

//------------------------------------------------------------
// マクロ定義
//------------------------------------------------------------
#define NONE_SCALE		(D3DXVECTOR3(1.0f, 1.0f, 1.0f))

//------------------------------------------------------------
// クラス定義(モデル)
//------------------------------------------------------------
class CModel : public CObject
{
public:			// 誰でもアクセスできる

	CModel(CObject::TYPE type, PRIORITY priority = PRIORITY_ENTITY);		// オーバーロードコンストラクタ
	virtual ~CModel();	// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void) override;	// 初期化処理
	virtual void Uninit(void) override;		// 終了処理
	virtual void Update(void) override;		// 更新処理
	virtual void Draw(void) override;		// 描画処理

	void Draw(D3DXCOLOR col);		// 描画処理(色処理)
	void Draw(D3DXCOLOR* col);		// 描画処理(複数色)
	void Draw(const float fAlpha);	// 描画処理(透明度調整)
	void DrawShadow(void);			// モデル影の描画処理
	void DrawBlock(const D3DXMATERIAL* pMat);	// ブロックの描画処理
	void DrawMatrix(D3DXMATRIX mtxParent);		// 他のマトリックスとの掛け合わせ描画処理

	bool ModelCollision(D3DXVECTOR3* pos);		// モデルの当たり判定

	// セット・ゲット関数
	void SetPos(const D3DXVECTOR3& pos);	// 位置設定処理
	D3DXVECTOR3 GetPos(void) const;			// 位置取得処理

	void SetPosOld(const D3DXVECTOR3& posOld);	// 前回の位置設定処理
	D3DXVECTOR3 GetPosOld(void) const;			// 前回の位置取得処理

	void SetRot(const D3DXVECTOR3& rot);	// 向き設定処理
	D3DXVECTOR3 GetRot(void) const;			// 向き取得処理

	void SetScale(const D3DXVECTOR3& scale);	// 拡大率設定処理
	D3DXVECTOR3 GetScale(void) const;			// 拡大率取得処理

	D3DXMATRIX GetMatrix(void) const;			// マトリックスの取得処理
	D3DXMATRIX* GetMatrixPoint(void);			// マトリックスポインタの取得処理

	void SetFileData(const CXFile::SXFile filaData);				// データの設定処理(ファイルデータの設定版)

	CXFile::SXFile GetFileData(void);			// Xファイルのデータの取得処理

	D3DXMATERIAL GetMaterial(const int nCnt);	// マテリアル情報の取得処理

	void BindTexture(const int nIdx, const int nCnt);		// テクスチャの割り当て処理

	// 静的メンバ関数
	static CModel* Create(const TYPE type, const PRIORITY priority);				// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数
	D3DXVECTOR3		m_pos;			// 位置
	D3DXVECTOR3		m_posOld;		// 前回の位置
	D3DXVECTOR3		m_rot;			// 向き
	D3DXVECTOR3		m_scale;		// 拡大率
	D3DXMATRIX		m_mtxWorld;		// ワールドマトリックス
	CXFile::SXFile	m_XFileData;	// Xファイルのデータ
};

#endif