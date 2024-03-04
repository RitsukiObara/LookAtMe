//============================================
//
// 看板の説明処理[signboard_explain.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "signboard_explain.h"
#include "object2D.h"
#include "input.h"
#include "texture.h"
#include "sound.h"
#include "useful.h"

#include "tutorial.h"
#include "signboard.h"

//--------------------------------------------
// 定数定義
//--------------------------------------------
namespace
{
	const D3DXVECTOR3 SCREEN_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);		// 背景の位置
	const D3DXVECTOR3 SCREEN_SIZE = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);		// 背景のサイズ
	const D3DXVECTOR3 EXPLAIN_POS = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, -200.0f, 0.0f);					// 説明の位置
	const D3DXVECTOR3 EXPLAIN_SIZE = D3DXVECTOR3(500.0f, 250.0f, 0.0f);									// 説明のサイズ
	const char* EXPLAIN_TEXTURE[CSignboard::TYPE_MAX] = 	// 説明のテクスチャ
	{
		"data\\TEXTURE\\Signboard001.png",
		"data\\TEXTURE\\Signboard002.png",
		"data\\TEXTURE\\Signboard003.png",
		"data\\TEXTURE\\Signboard004.png",
		"data\\TEXTURE\\Signboard005.png",
		"data\\TEXTURE\\Signboard006.png",
		"data\\TEXTURE\\Signboard007.png",
		"data\\TEXTURE\\Signboard008.png",
		"data\\TEXTURE\\Signboard009.png",
	};
	const D3DXCOLOR SCREEN_COL = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// 背景の色
	const float SCREEN_ADD_ALPHA = 0.01f;			// 背景の透明度の加算数
	const float SCREEN_MAX_ALPHA = 0.6f;			// 背景の透明度の最大数

	const float APPEAR_ADD_MOVE = 1.2f;				// 出現状態の移動量の追加量
	const float LEAVE_ADD_MOVE = 1.2f;				// 退場状態の移動量の追加量
	const float LEAVE_DEST_EXPLAIN_POSY = -300.0f;	// 退場状態の説明文の目的の高さ
	const float SCREEN_LEAVE_ALPHA = 0.0f;			// 退場状態の背景の透明度
}

//============================
// コンストラクタ
//============================
CSignboardExpl::CSignboardExpl() : CObject(CObject::TYPE_SIGNEXPLAIN, DIM_2D, PRIORITY_UI)
{
	// 全ての値をクリアする
	m_screen.pScreen = nullptr;			// 画面の情報
	m_screen.fAlpha = 0.0f;				// 透明度
	m_explain.pExplain = nullptr;		// 説明のポリゴン
	m_explain.fMoveY = 0.0f;			// Y軸の移動量
	m_state = STATE_APPEAR;				// 状態
}

//============================
// デストラクタ
//============================
CSignboardExpl::~CSignboardExpl()
{

}

//============================
// 初期化処理
//============================
HRESULT CSignboardExpl::Init(void)
{
	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CSignboardExpl::Uninit(void)
{
	if (m_screen.pScreen != nullptr)
	{ // 背景が NULL じゃない場合

		// 背景の終了処理
		m_screen.pScreen->Uninit();
		m_screen.pScreen = nullptr;
	}

	if (m_explain.pExplain != nullptr)
	{ // 説明が NULL じゃない場合

		// 説明の終了処理
		m_explain.pExplain->Uninit();
		m_explain.pExplain = nullptr;
	}

	// 破棄処理
	Release();
}

//============================
// 更新処理
//============================
void CSignboardExpl::Update(void)
{
	switch (m_state)
	{
	case CSignboardExpl::STATE_APPEAR:		// 出現状態

		// 出現状態処理
		Appear();

		break;

	case CSignboardExpl::STATE_STOP:		// 停止状態

		if (CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true ||
			CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_SPACE) == true)
		{ // Aキーを押した場合

			// 退場状態にする
			m_state = STATE_LEAVE;

			// 看板音を鳴らす
			CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_SIGNBOARD);
		}

		break;

	case CSignboardExpl::STATE_LEAVE:		// 退場状態

		if (Leave() == true)
		{ // 退場しきった場合

			// 通常状態にする
			CTutorial::SetState(CTutorial::STATE_NONE);

			// 看板の説明を NULL にする
			CTutorial::GetLookSign()->SetExplain(nullptr);

			// 終了処理
			Uninit();

			// この先の処理を行わない
			return;
		}

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// 頂点座標の設定処理
	m_explain.pExplain->SetVertex();

	// 頂点カラーの設定処理
	m_screen.pScreen->SetVtxColor(D3DXCOLOR(SCREEN_COL.r, SCREEN_COL.g, SCREEN_COL.b, m_screen.fAlpha));
}

//============================
// 描画処理
//============================
void CSignboardExpl::Draw(void)
{
	if (m_screen.pScreen != nullptr)
	{ // 背景が NULL じゃない場合

		// 背景の描画処理
		m_screen.pScreen->Draw();
	}

	if (m_explain.pExplain != nullptr)
	{ // 説明が NULL じゃない場合

		// 説明の描画処理
		m_explain.pExplain->Draw();
	}
}

//============================
// 情報の設定処理
//============================
void CSignboardExpl::SetData(const CSignboard::TYPE type)
{
	if (m_screen.pScreen == nullptr)
	{ // 背景が NULL の場合

		// 背景を生成
		m_screen.pScreen = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);

		// 情報の設定処理
		m_screen.pScreen->SetPos(SCREEN_POS);			// 位置
		m_screen.pScreen->SetPosOld(SCREEN_POS);		// 前回の位置
		m_screen.pScreen->SetRot(NONE_D3DXVECTOR3);		// 向き
		m_screen.pScreen->SetSize(SCREEN_SIZE);			// サイズ
		m_screen.pScreen->SetAngle();					// 方向
		m_screen.pScreen->SetLength();					// 長さ

		// 頂点座標の設定処理
		m_screen.pScreen->SetVertex();

		// 頂点カラーの設定処理
		m_screen.pScreen->SetVtxColor(D3DXCOLOR(SCREEN_COL.r, SCREEN_COL.g, SCREEN_COL.b, m_screen.fAlpha));
	}
	m_screen.fAlpha = 0.0f;

	if (m_explain.pExplain == nullptr)
	{ // 説明が NULL の場合

		// 説明を生成
		m_explain.pExplain = CObject2D::Create(CObject2D::TYPE_NONE, TYPE_NONE, PRIORITY_UI);

		// 情報の設定処理
		m_explain.pExplain->SetPos(EXPLAIN_POS);		// 位置
		m_explain.pExplain->SetPosOld(EXPLAIN_POS);		// 前回の位置
		m_explain.pExplain->SetRot(NONE_D3DXVECTOR3);	// 向き
		m_explain.pExplain->SetSize(EXPLAIN_SIZE);		// サイズ
		m_explain.pExplain->SetAngle();					// 方向
		m_explain.pExplain->SetLength();				// 長さ

		// テクスチャの設定処理
		m_explain.pExplain->BindTexture(CManager::Get()->GetTexture()->Regist(EXPLAIN_TEXTURE[type]));

		// 頂点座標の設定処理
		m_explain.pExplain->SetVertex();
	}
	m_explain.fMoveY = 0.0f;

	m_state = STATE_APPEAR;			// 状態
}

//============================
//生成処理
//============================
CSignboardExpl* CSignboardExpl::Create(const CSignboard::TYPE type)
{
	// ローカルオブジェクトを生成
	CSignboardExpl* pExplain = nullptr;			// 説明のインスタンスを生成

	if (pExplain == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pExplain = new CSignboardExpl;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pExplain != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pExplain->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pExplain->SetData(type);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 説明のポインタを返す
	return pExplain;
}

//============================
// 出現状態処理
//============================
void CSignboardExpl::Appear(void)
{
	// 背景を黒に近づける
	useful::FrameCorrect(SCREEN_MAX_ALPHA, &m_screen.fAlpha, SCREEN_ADD_ALPHA);

	// 移動量を加算する
	m_explain.fMoveY += APPEAR_ADD_MOVE;

	// 位置を取得する
	D3DXVECTOR3 pos = m_explain.pExplain->GetPos();

	// 移動する
	pos.y += m_explain.fMoveY;

	if (pos.y >= SCREEN_HEIGHT * 0.5f)
	{ // 中心を超えた場合

		// 位置を補正する
		pos.y = SCREEN_HEIGHT * 0.5f;

		// 停止状態にする
		m_state = STATE_STOP;

		// 移動量をリセットする
		m_explain.fMoveY = 0.0f;
	}

	// 位置を適用
	m_explain.pExplain->SetPos(pos);
}

//============================
// 退場状態処理
//============================
bool CSignboardExpl::Leave(void)
{
	// 終了状況
	bool bEnd = false;

	// 背景を黒に近づける
	useful::FrameCorrect(SCREEN_LEAVE_ALPHA, &m_screen.fAlpha, SCREEN_ADD_ALPHA);

	// 移動量を加算する
	m_explain.fMoveY -= LEAVE_ADD_MOVE;

	// 位置を取得する
	D3DXVECTOR3 pos = m_explain.pExplain->GetPos();

	// 移動する
	pos.y += m_explain.fMoveY;

	if (pos.y <= LEAVE_DEST_EXPLAIN_POSY)
	{ // 中心を超えた場合

		// 終了する
		bEnd = true;
	}

	// 位置を適用
	m_explain.pExplain->SetPos(pos);

	// 終了状況を返す
	return bEnd;
}