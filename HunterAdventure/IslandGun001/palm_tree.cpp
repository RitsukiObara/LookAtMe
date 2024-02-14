//===========================================
//
// ヤシの木のメイン処理[palm_tree.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "palm_tree.h"
#include "renderer.h"
#include "useful.h"

#include "palm_fruit.h"
#include "texture.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const float FRUIT_SHIFT = 100.0f;		// ヤシの実のずらす幅
	const float FRUIT_HEIGHT = 630.0f;		// ヤシの実の高さ
	const float FRUIT_ROT = -0.5f;			// ヤシの実の向き
}

//==============================
// コンストラクタ
//==============================
CPalmTree::CPalmTree() : CTree()
{
	// 全ての値をクリアする
	m_pPalmFruit = nullptr;		// ヤシの実の情報
}

//==============================
// デストラクタ
//==============================
CPalmTree::~CPalmTree()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CPalmTree::Init(void)
{
	if (FAILED(CTree::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 値を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CPalmTree::Uninit(void)
{
	// ヤシの実を NULL にする
	m_pPalmFruit = nullptr;

	// 終了処理
	CTree::Uninit();
}

//========================================
//ブロックの更新処理
//========================================
void CPalmTree::Update(void)
{
	// 更新処理
	CTree::Update();
}

//=====================================
//ブロックの描画処理
//=====================================
void CPalmTree::Draw(void)
{
	// 描画処理
	CTree::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CPalmTree::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// 情報の設定処理
	CTree::SetData(pos, rot, type);

	D3DXVECTOR3 posFruit;		// ヤシの実の位置
	D3DXVECTOR3 rotFruit;		// ヤシの実の向き

	// ヤシの実の位置と向きを設定する
	posFruit.x = pos.x + sinf(rot.y) * FRUIT_SHIFT;
	posFruit.y = pos.y + FRUIT_HEIGHT;
	posFruit.z = pos.z + cosf(rot.y) * FRUIT_SHIFT;
	rotFruit.x = FRUIT_ROT;
	rotFruit.y = rot.y;
	rotFruit.z = 0.0f;

	// 全ての値を設定する
	m_pPalmFruit = CPalmFruit::Create(posFruit, rotFruit);		// ヤシの実の情報
}

//=====================================
// ヒット処理
//=====================================
void CPalmTree::Hit(void)
{
	// ヒット処理
	CTree::Hit();

	if (m_pPalmFruit != nullptr)
	{ //ヤシの実が NULL じゃない場合

		// 落下状態にする
		m_pPalmFruit->SetState(CPalmFruit::STATE_FALL);

		// ヤシの実を NULL にする
		m_pPalmFruit = nullptr;
	}
}