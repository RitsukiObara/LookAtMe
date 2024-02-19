//===========================================
//
// プロペラのメイン処理[propeller.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "propeller.h"
#include "useful.h"

//-------------------------------------------
// 定数定義
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\Propeller.x";		// モデルのパス名
	const D3DXVECTOR3 OFFSET = D3DXVECTOR3(0.0f, 180.0f, -1570.0f);		// オフセット座標
	const float ROT_MOVE = 1.3f;		// 向きの移動量
}

//==============================
// コンストラクタ
//==============================
CPropeller::CPropeller() : CModel(CObject::TYPE_NONE, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pMtxParent = nullptr;		// 親のマトリックス
}

//==============================
// デストラクタ
//==============================
CPropeller::~CPropeller()
{

}

//==============================
// 破片の初期化処理
//==============================
HRESULT CPropeller::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 値を返す
	return S_OK;
}

//========================================
// 破片の終了処理
//========================================
void CPropeller::Uninit(void)
{
	// 終了処理
	CModel::Uninit();
}

//=====================================
// 破片の更新処理
//=====================================
void CPropeller::Update(void)
{
	// 向きを取得
	D3DXVECTOR3 rot = GetRot();

	// 向きを加算する
	rot.z += ROT_MOVE;

	// 向きの正規化
	useful::RotNormalize(&rot.z);

	// 向きを適用
	SetRot(rot);
}

//=====================================
// 破片の描画処理
//=====================================
void CPropeller::Draw(void)
{
	// 描画処理
	CModel::DrawMatrix(*m_pMtxParent);
}

//=====================================
// 情報の設定処理
//=====================================
void CPropeller::SetData(D3DXMATRIX* pMtx)
{
	// 情報の設定処理
	SetPos(OFFSET);						// 位置
	SetPosOld(OFFSET);					// 前回の位置
	SetRot(NONE_D3DXVECTOR3);			// 向き
	SetScale(NONE_SCALE);				// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// モデル情報

	// 全ての値を設定する
	m_pMtxParent = pMtx;		// 親のマトリックス
}

//=======================================
// 生成処理
//=======================================
CPropeller* CPropeller::Create(D3DXMATRIX* pMtx)
{
	// ローカルオブジェクトを生成
	CPropeller* pPro = nullptr;		// インスタンスを生成

	if (pPro == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pPro = new CPropeller;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pPro != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pPro->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pPro->SetData(pMtx);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// プロペラのポインタを返す
	return pPro;
}