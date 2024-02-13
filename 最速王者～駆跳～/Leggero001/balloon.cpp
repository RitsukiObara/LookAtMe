//============================================
//
// 吹き出し処理[balloon.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "balloon.h"
#include "file.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include "balloonUI.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define TEXTURE				"data/TEXTURE/BalloonKari.png"		// テクスチャ(仮)
#define SMALL_SIZE			(D3DXVECTOR3(10.0f, 10.0f, 0.0f))	// 収縮状態でのサイズ
#define SIZE_CORRECT		(0.3f)								// サイズの補正倍率
#define DISP_COUNT			(20)								// 表示状態になるまでのカウント数
#define BOUND_SIZE			(3.0f)								// バウンドのサイズ差
#define BOUND_MOVE			(0.1f)								// バウンドの移動量
#define REDUCE_CHANGE_SIZE	(0.001f)							// 縮小時の変化状況の変わるサイズ

//============================
// コンストラクタ
//============================
CBalloon::CBalloon() : CBillboard(CObject::TYPE_BALLOON, PRIORITY_SHADOW)
{
	// 全ての値をクリアする
	m_state = STATE_SMALL;			// 状態
	m_type = TYPE::TYPE_NORMAL;		// 種類
	m_sizeDisp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 描画時のサイズ
	m_sizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズの移動量
	m_bChange = false;				// 変化状況
}

//============================
// デストラクタ
//============================
CBalloon::~CBalloon()
{

}

//============================
// 初期化処理
//============================
HRESULT CBalloon::Init(void)
{
	if (FAILED(CBillboard::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_state = STATE_SMALL;			// 状態
	m_type = TYPE::TYPE_NORMAL;		// 種類
	m_sizeDisp = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 描画時のサイズ
	m_sizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズの移動量
	m_bChange = false;				// 変化状況

	// 成功を返す
	return S_OK;
}

//============================
// 終了処理
//============================
void CBalloon::Uninit(void)
{
	// 終了
	CBillboard::Uninit();
}

//============================
// 更新処理
//============================
void CBalloon::Update(void)
{
	switch (m_state)
	{
	case STATE_SMALL:		// 収縮状態

		// 縮小処理
		Small();

		break;

	case STATE_DISP:		// 表示状態

		// 表示処理
		Disp();

		break;
	}

	// 頂点座標の設定処理
	SetVtxPosUnder();
}

//============================
// 描画処理
//============================
void CBalloon::Draw(void)
{
	// 描画処理
	CBillboard::DrawLightOff();
}

//============================
// 状態の設定処理
//============================
void CBalloon::SetState(const STATE state)
{
	if (m_state == state)
	{ // 状態が一定している場合

		// この先の処理を行わない
		return;
	}
	// 状態を設定する
	m_state = state;

	if (m_state == STATE_DISP)
	{ // 表示状態になったとき

		// サイズの移動量を設定する
		m_sizeMove = m_sizeDisp / DISP_COUNT;
	}

	// 変化状況を設定する
	m_bChange = true;
}

//============================
// 種類の取得処理
//============================
CBalloon::TYPE CBalloon::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//============================
// 表示状態の処理
//============================
void CBalloon::Disp(void)
{
	if (m_bChange == true)
	{ // 変化状態の場合
		
		// 拡大処理
		Expansion();
	}
	else
	{ // 上記以外

		// バウンド処理
		Bound();
	}
}

//============================
// 収縮状態の処理
//============================
void CBalloon::Small(void)
{
	if (m_bChange == true)
	{ // 変化状態の場合

		// 縮小状態
		Reduce();
	}
	else
	{ // 上記以外

	}
}

//============================
// 拡大処理
//============================
void CBalloon::Expansion(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = GetSize();		// サイズを取得する

	if (size.x < m_sizeDisp.x)
	{ // サイズがまだ表示時のサイズ未満だった場合

		// サイズを加算する
		size.x += m_sizeMove.x;

		if (size.x >= m_sizeDisp.x)
		{ // サイズが描画時のサイズ以上になった場合

			// サイズを補正する
			size.x = m_sizeDisp.x;

			// 変化状況を設定する
			m_bChange = false;
		}
	}

	if (size.y < m_sizeDisp.y)
	{ // サイズがまだ表示時のサイズ未満だった場合

		// サイズを加算する
		size.y += m_sizeMove.y;

		if (size.y >= m_sizeDisp.y)
		{ // サイズが描画時のサイズ以上になった場合

			// サイズを補正する
			size.y = m_sizeDisp.y;

			// 変化状況を設定する
			m_bChange = false;
		}
	}

	// サイズを設定する
	SetSize(size);
}

//============================
// 縮小処理
//============================
void CBalloon::Reduce(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = GetSize();		// サイズを取得する

	// サイズの補正処理
	Correct(SMALL_SIZE.x, &size.x, SIZE_CORRECT);
	Correct(SMALL_SIZE.y, &size.y, SIZE_CORRECT);

	if (size.x <= SMALL_SIZE.x + REDUCE_CHANGE_SIZE &&
		size.y <= SMALL_SIZE.y + REDUCE_CHANGE_SIZE)
	{ // サイズが一定以下になった場合

		// サイズを設定する
		size = SMALL_SIZE;

		// 変化状況を設定する
		m_bChange = false;
	}

	// サイズを設定する
	SetSize(size);
}

//============================
// バウンド処理
//============================
void CBalloon::Bound(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = GetSize();		// サイズを取得する

	// サイズを加算する
	size.y += m_sizeMove.y;

	if (size.y < m_sizeDisp.y - BOUND_SIZE)
	{ // サイズがまだ表示時のサイズ未満だった場合

		// サイズを設定する
		size.y = m_sizeDisp.y - BOUND_SIZE;

		// サイズの移動量を設定する
		m_sizeMove.y = BOUND_MOVE;
	}
	else if (size.y > m_sizeDisp.y + BOUND_SIZE)
	{ // サイズがまだ表示時のサイズ超過だった場合

		// サイズを設定する
		size.y = m_sizeDisp.y + BOUND_SIZE;

		// サイズの移動量を設定する
		m_sizeMove.y = -BOUND_MOVE;
	}

	// サイズを設定する
	SetSize(size);
}

//============================
// 情報の設定処理
//============================
void CBalloon::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// ローカル変数宣言
	CFile::SBalloonData data;		// 吹き出しの情報

	if (CManager::GetFile() != nullptr)
	{ // ファイルの情報が NULL じゃない場合

		// 吹き出しの情報を取得する
		data = CManager::GetFile()->GetBalloon(type);
	}

	// 全ての値を初期化する
	m_state = STATE_SMALL;		// 状態
	m_type = type;				// 種類
	m_sizeDisp = data.size;		// 描画時のサイズ
	m_sizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズの移動量
	m_bChange = false;			// 変化状況

	// 情報の初期化
	SetPos(pos);
	SetSize(SMALL_SIZE);

	// 頂点情報の初期化
	SetVtxPosUnder();

	// テクスチャの割り当て処理
	BindTexture(CManager::GetTexture()->Regist(data.aTextureName));
}

//============================
//生成処理
//============================
CBalloon* CBalloon::Create(const D3DXVECTOR3& pos, const TYPE type)
{
	// ローカルオブジェクトを生成
	CBalloon* pBalloon = nullptr;	// プレイヤーのインスタンスを生成

	if (pBalloon == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pBalloon = new CBalloon;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pBalloon != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pBalloon->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "吹き出しの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pBalloon->SetData(pos, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// タイトルの選択肢のポインタを返す
	return pBalloon;
}