//===========================================
//
// 矢印看板のメイン処理[arrowsign.cpp]
// Author：小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "manager.h"
#include "arrowsign.h"
#include "model.h"

//==============================
// コンストラクタ
//==============================
CArrowSign::CArrowSign() : CObject(CObject::TYPE_ARROWSIGN, CObject::PRIORITY_BLOCK)
{
	// コンストラクタの箱
	Box();
}

//==============================
// コンストラクタ
//==============================
CArrowSign::CArrowSign(CObject::TYPE type, CObject::PRIORITY priority) : CObject(type, priority)
{
	// コンストラクタの箱
	Box();
}

//==============================
// デストラクタ
//==============================
CArrowSign::~CArrowSign()
{

}

//==============================
// コンストラクタの箱
//==============================
void CArrowSign::Box(void)
{
	// 全ての値をクリアする
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apArrow[nCnt] = nullptr;		// モデルの情報
	}
}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CArrowSign::Init(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apArrow[nCnt] == nullptr)
		{ // モデルの情報が NULL だった場合

			// モデルのオブジェクトを生成
			m_apArrow[nCnt] = new CModel(TYPE_NONE, PRIORITY_BLOCK);

			if (m_apArrow[nCnt] != nullptr)
			{ // モデルの情報が NULL じゃない場合

				if (FAILED(m_apArrow[nCnt]->Init()))
				{ // 初期化に失敗した場合

					// 失敗を返す
					return E_FAIL;
				}
			}
			else
			{ // 上記以外

				// 失敗を返す
				return E_FAIL;
			}
		}
		else
		{ // 上記以外

			// 失敗を返す
			return E_FAIL;
		}
	}

	// 成功を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CArrowSign::Uninit(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// 終了処理
		m_apArrow[nCnt]->Uninit();
		m_apArrow[nCnt] = nullptr;
	}

	// 破棄処理
	Release();
}

//========================================
//ブロックの更新処理
//========================================
void CArrowSign::Update(void)
{

}

//=====================================
//ブロックの描画処理
//=====================================
void CArrowSign::Draw(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apArrow[nCnt] != nullptr)
		{ // モデルの情報が NULL じゃない場合

			// 描画処理
			m_apArrow[nCnt]->Draw();
		}
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CArrowSign::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rotPole, const D3DXVECTOR3& rotSign)
{
	// 柱の情報設定処理
	SetDataPole(pos, rotPole);

	// 矢印の情報設定処理
	SetDataArrow(rotSign);
}

//=======================================
// 生成処理
//=======================================
CArrowSign* CArrowSign::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rotPole, const D3DXVECTOR3& rotSign)
{
	// ローカルオブジェクトを生成
	CArrowSign* pArrow = nullptr;	// プレイヤーのインスタンスを生成

	if (pArrow == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pArrow = new CArrowSign;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pArrow != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pArrow->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "矢印看板の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pArrow->SetData(pos, rotPole, rotSign);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// プレイヤーのポインタを返す
	return pArrow;
}

//=======================================
// 柱の情報の設定処理
//=======================================
void CArrowSign::SetDataPole(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	if (m_apArrow[TYPE_POLE] != nullptr)
	{ // 柱のポインタが NULL じゃない場合

		// 情報を設定する
		m_apArrow[TYPE_POLE]->SetPos(pos);								// 位置
		m_apArrow[TYPE_POLE]->SetRot(rot);								// 向き
		m_apArrow[TYPE_POLE]->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));	// 拡大率
		m_apArrow[TYPE_POLE]->SetFileData(CXFile::TYPE_ARROWPOLE);		// モデルの情報
	}
}

//=======================================
// 矢印の情報の設定処理
//=======================================
void CArrowSign::SetDataArrow(const D3DXVECTOR3& rot)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;		// 位置

	if (m_apArrow[TYPE_POLE] != nullptr &&
		m_apArrow[TYPE_ARROW] != nullptr)
	{ // 矢印と柱のポインタが NULL じゃない場合

		// 位置を設定する
		pos.x = m_apArrow[TYPE_POLE]->GetPos().x;
		pos.y = m_apArrow[TYPE_POLE]->GetPos().y + cosf(m_apArrow[TYPE_POLE]->GetRot().z) * m_apArrow[TYPE_POLE]->GetFileData().vtxMax.y;
		pos.z = m_apArrow[TYPE_POLE]->GetPos().z + m_apArrow[TYPE_POLE]->GetFileData().vtxMin.z;

		// 情報を設定する
		m_apArrow[TYPE_ARROW]->SetPos(pos);				// 位置
		m_apArrow[TYPE_ARROW]->SetRot(rot);				// 向き
		m_apArrow[TYPE_ARROW]->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));	// 拡大率
		m_apArrow[TYPE_ARROW]->SetFileData(CXFile::TYPE_ARROWSIGN);		// モデルの情報
	}
}