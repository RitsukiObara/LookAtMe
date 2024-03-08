//===========================================
//
// 看板のメイン処理[signboard.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "signboard.h"
#include "sound.h"
#include "texture.h"
#include "useful.h"

#include "push_timing.h"
#include "signboard_explain.h"
#include "tutorial.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const char* MODEL = "data\\MODEL\\Signboard.x";		// モデルの名前
	const float BUTTON_SHIFT = 300.0f;					// ボタンのずらす幅
	const float BUTTON_SIZE = 50.0f;					// ボタンのサイズ
	const int BUTTON_INTERVAL = 10;						// ボタンのアニメーションのインターバル
}

//-------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------
CListManager<CSignboard*> CSignboard::m_list = {};		// リスト

//==============================
// コンストラクタ
//==============================
CSignboard::CSignboard() : CModel(TYPE_SIGNBOARD, PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_pButton = nullptr;	// ボタンの情報
	m_pExplain = nullptr;	// 説明の情報
	m_type = TYPE_JUMP;		// 種類

	// リストに追加する
	m_list.Regist(this);
}

//==============================
// デストラクタ
//==============================
CSignboard::~CSignboard()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CSignboard::Init(void)
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
//ブロックの終了処理
//========================================
void CSignboard::Uninit(void)
{
	if (m_pButton != nullptr)
	{ // ボタンが NULL じゃない場合

		// ボタンの終了処理
		m_pButton = nullptr;
	}

	// 終了処理
	CModel::Uninit();

	// 引き抜き処理
	m_list.Pull(this);
}

//========================================
//ブロックの更新処理
//========================================
void CSignboard::Update(void)
{

}

//=====================================
//ブロックの描画処理
//=====================================
void CSignboard::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CSignboard::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// 情報の設定処理
	SetPos(pos);					// 位置
	SetPosOld(pos);					// 前回の位置
	SetRot(rot);					// 向き
	SetScale(NONE_SCALE);			// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// モデルの情報

	// 全ての値を設定する
	m_pButton = CPushTiming::Create(D3DXVECTOR3(pos.x, pos.y + BUTTON_SHIFT, pos.z), BUTTON_SIZE, CPushTiming::TYPE_PAD_A, BUTTON_INTERVAL);	// ボタンの情報
	m_pButton->SetEnableDisp(false);
	m_type = type;		// 種類
}

//=======================================
// 説明移行処理
//=======================================
void CSignboard::Explain(void)
{
	if (m_pExplain == nullptr)
	{ // 説明状況が NULL の場合

		// 説明を生成
		m_pExplain = CSignboardExpl::Create(m_type);
	}

	// 説明状態にする
	CTutorial::SetState(CTutorial::STATE_EXPL);

	// 看板の情報を設定する
	CTutorial::SetLookSign(this);

	// 看板音を鳴らす
	CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_SIGNBOARD);
}

//=======================================
// 生成処理
//=======================================
CSignboard* CSignboard::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// ローカルオブジェクトを生成
	CSignboard* pSignboard = nullptr;		// インスタンスを生成

	if (pSignboard == nullptr)
	{ // オブジェクトが NULL の場合

		// ブロックを生成する
		pSignboard = new CSignboard;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pSignboard != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pSignboard->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pSignboard->SetData(pos, rot, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 看板のポインタを返す
	return pSignboard;
}

//=======================================
// 説明の設定処理
//=======================================
void CSignboard::SetExplain(CSignboardExpl* pExpl)
{
	// 説明の情報を設定する
	m_pExplain = pExpl;
}

//=======================================
// ボタン表示の取得処理
//=======================================
CPushTiming* CSignboard::GetButton(void) const
{
	// ボタン表示の情報を返す
	return m_pButton;
}

//=======================================
// リストの取得処理
//=======================================
CListManager<CSignboard*> CSignboard::GetList(void)
{
	// リストの情報を返す
	return m_list;
}