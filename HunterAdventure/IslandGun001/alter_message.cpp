//=======================================
//
// 祭壇メッセージのメイン処理[alter_message.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "alter_message.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//=======================================
// 無名名前空間
//=======================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\AlterMeta.png";		// テクスチャ
	const int EXTEND_COUNT = 50;			// 伸び状態のカウント数
	const int SHRINK_COUNT = 15;			// 縮み状態のカウント数
	const D3DXVECTOR3 SHIFT = D3DXVECTOR3(0.0f, 900.0f, 0.0f);			// ポリゴンのずらす幅
	const D3DXVECTOR3 POLYGON_SIZE = D3DXVECTOR3(400.0f, 100.0f, 0.0f);	// ポリゴンのサイズ
	const D3DXVECTOR3 EXTEND_MAGNI = D3DXVECTOR3(0.8f, 1.2f, 0.0f);		// 伸び状態の倍率
	const D3DXVECTOR3 SHRINK_MAGNI = D3DXVECTOR3(1.2f, 0.8f, 0.0f);		// 縮み状態の倍率
}

//=========================
// コンストラクタ
//=========================
CAlterMessage::CAlterMessage() : CBillboard(CObject::TYPE_EFFECT, CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	m_sizeDest = NONE_D3DXVECTOR3;	// 目的のサイズ
	m_state = STATE_EXTEND;			// 状態
	m_nStateCount = 0;				// 状態カウント
}

//=========================
// デストラクタ
//=========================
CAlterMessage::~CAlterMessage()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CAlterMessage::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CAlterMessage::Uninit(void)
{
	// 終了
	CBillboard::Uninit();
}

//=========================
// 更新処理
//=========================
void CAlterMessage::Update(void)
{
	D3DXVECTOR3 size = GetSize();

	// 状態カウントを加算する
	m_nStateCount++;

	switch (m_state)
	{
	case CAlterMessage::STATE_EXTEND:

		// サイズを設定する
		useful::FrameCorrect(m_sizeDest.x, &size.x, (m_sizeDest.x - POLYGON_SIZE.x) / EXTEND_COUNT);
		useful::FrameCorrect(m_sizeDest.y, &size.y, (m_sizeDest.y - POLYGON_SIZE.y) / EXTEND_COUNT);

		if (m_nStateCount % EXTEND_COUNT == 0)
		{ // 状態カウントが一定数になった場合

			// 状態カウントを初期化する
			m_nStateCount = 0;

			// 縮小状態にする
			m_state = STATE_SHRINK;

			// 目的のサイズを設定する
			m_sizeDest = D3DXVECTOR3(POLYGON_SIZE.x * SHRINK_MAGNI.x, POLYGON_SIZE.y * SHRINK_MAGNI.y, 0.0f);
		}

		break;

	case CAlterMessage::STATE_SHRINK:

		// サイズを設定する
		useful::FrameCorrect(m_sizeDest.x, &size.x, (m_sizeDest.x - POLYGON_SIZE.x) / SHRINK_COUNT);
		useful::FrameCorrect(m_sizeDest.y, &size.y, (m_sizeDest.y - POLYGON_SIZE.y) / SHRINK_COUNT);

		if (m_nStateCount % SHRINK_COUNT == 0)
		{ // 状態カウントが一定数になった場合

			// 状態カウントを初期化する
			m_nStateCount = 0;

			// 拡大状態にする
			m_state = STATE_EXTEND;

			// 目的のサイズを設定する
			m_sizeDest = D3DXVECTOR3(POLYGON_SIZE.x * EXTEND_MAGNI.x, POLYGON_SIZE.y * EXTEND_MAGNI.y, 0.0f);
		}

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	// サイズの設定処理
	SetSize(size);

	// 頂点座標の設定処理
	SetVertex();
}

//=========================
// 描画処理
//=========================
void CAlterMessage::Draw(void)
{
	// 描画処理
	CBillboard::DrawLightOff(false);
}

//=========================
// 情報の設定処理
//=========================
void CAlterMessage::SetData(const D3DXVECTOR3& pos)
{
	// 情報の設定処理
	SetPos(pos + SHIFT);		// 位置
	SetPosOld(pos + SHIFT);		// 前回の位置
	SetSize(POLYGON_SIZE);		// サイズ
	SetEnableLookY(true);		// Y軸状況

	// 全ての値を初期化する
	m_sizeDest = D3DXVECTOR3(POLYGON_SIZE.x * EXTEND_MAGNI.x, POLYGON_SIZE.y * EXTEND_MAGNI.y, 0.0f);	// 目的のサイズ
	m_state = STATE_EXTEND;		// 状態
	m_nStateCount = 0;			// 状態のカウント

	// 頂点情報の初期化
	SetVertex();

	// テクスチャの割り当て処理
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));
}

//=========================
// 生成処理
//=========================
CAlterMessage* CAlterMessage::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CAlterMessage* pMessage = nullptr;	// 祭壇のインスタンスを生成

	if (pMessage == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pMessage = new CAlterMessage;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pMessage != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pMessage->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pMessage->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 祭壇のメッセージのポインタを返す
	return pMessage;
}