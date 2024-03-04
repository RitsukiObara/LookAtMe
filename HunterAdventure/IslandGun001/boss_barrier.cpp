//=======================================
//
// ボスのバリアのメイン処理[boss_barrier.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "boss_barrier.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

//=======================================
// 無名名前空間
//=======================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\BossBarrier.png";		// 弾のテクスチャ
	const D3DXVECTOR3 INIT_RADIUS = D3DXVECTOR3(100.0f, 100.0f, 0.0f);	// 初期半径
	const int MAX_LIFE = 65;				// 寿命
}

//=========================
// コンストラクタ
//=========================
CBossBarrier::CBossBarrier() : CBillboard(CObject::TYPE_BOSSBARRIER, CObject::PRIORITY_EFFECT)
{
	// 全ての値をクリアする
	m_nLife = MAX_LIFE;		// 寿命
}

//=========================
// デストラクタ
//=========================
CBossBarrier::~CBossBarrier()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CBossBarrier::Init(void)
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
void CBossBarrier::Uninit(void)
{
	// 終了
	CBillboard::Uninit();
}

//=========================
// 更新処理
//=========================
void CBossBarrier::Update(void)
{
	// 寿命を減算する
	m_nLife--;

	if (m_nLife <= 0)
	{ // 寿命が無くなった場合

		// 終了処理
		Uninit();

		// この先の処理を行わない
		return;
	}

	// 頂点座標の設定処理
	SetVertex();
}

//=========================
// 描画処理
//=========================
void CBossBarrier::Draw(void)
{
	// 描画処理
	CBillboard::DrawLightOff(true);
}

//=========================
// 情報の設定処理
//=========================
void CBossBarrier::SetData(const D3DXVECTOR3& pos)
{
	// 情報の設定処理
	SetPos(pos);			// 位置
	SetPosOld(pos);			// 前回の位置
	SetRot(0.0f);			// 向き
	SetSize(INIT_RADIUS);	// サイズ
	SetAngle();				// 方向
	SetLength();			// 長さ
	SetEnableLookY(true);	// Y軸状況

	// 頂点情報の初期化
	SetVertex();

	// テクスチャの読み込み処理
	BindTexture(CManager::Get()->GetTexture()->Regist(TEXTURE));

	// 全ての値を設定する
	m_nLife = MAX_LIFE;		// 寿命
}

//=========================
// 生成処理
//=========================
CBossBarrier* CBossBarrier::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CBossBarrier* pBarrier = nullptr;	// ボスのインスタンスを生成

	if (pBarrier == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pBarrier = new CBossBarrier;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pBarrier != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pBarrier->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pBarrier->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// ボスのポインタを返す
	return pBarrier;
}