//=======================================
//
// 斬撃の波紋のメイン処理[slash_ripple.cpp]
// Author：小原立暉
//
//=======================================
#include "manager.h"
#include "slash_ripple.h"
#include "renderer.h"
#include "collision.h"
#include "texture.h"
#include "useful.h"

//---------------------------------------
// 無名名前空間
namespace
{
	const int LIFE = 140;				// 寿命
	const float ADD_SCALE = 0.4f;		// 拡大率の加算数
	const float SUB_ALPHA = 0.05f;		// 透明度の減算量
}

//==========================================
// 静的メンバ変数
//==========================================
CListManager<CSlashRipple*> CSlashRipple::m_list = {};		// リスト

//==========================================
// コンストラクタ
//==========================================
CSlashRipple::CSlashRipple() : CRipple(TYPE_SLASHRIPPLE, PRIORITY_PLAYER)
{
	// 全ての値をクリアする
	m_nLife = LIFE;		// 寿命

	// リストに追加する
	m_list.Regist(this);
}

//==========================================
// デストラクタ
//==========================================
CSlashRipple::~CSlashRipple()
{

}

//==========================================
// 波紋の初期化処理
//==========================================
HRESULT CSlashRipple::Init(void)
{
	if (FAILED(CRipple::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//========================================
// 波紋の終了処理
//========================================
void CSlashRipple::Uninit(void)
{
	// 終了処理
	CRipple::Uninit();

	// 引き抜き処理
	m_list.Pull(this);
}

//========================================
// 波紋の更新処理
//========================================
void CSlashRipple::Update(void)
{
	// 拡大率を取得
	D3DXVECTOR3 scale = GetScale();

	// 拡大率を変化させる
	scale.x += ADD_SCALE;
	scale.z += ADD_SCALE;

	// 情報を更新する
	SetScale(scale);			// 拡大率

	// 爆弾との当たり判定
	collision::BombHitToSlashRipple(GetPos(), GetFileData().vtxMax.x * scale.x, GetFileData().vtxMax.y);

	// 爆弾花との当たり判定
	collision::BangFlowerHit(GetPos(), GetFileData().vtxMax.x * scale.x, GetFileData().vtxMax.y);

	// 的(風船)との当たり判定
	collision::TargetHit(GetPos(), GetFileData().vtxMax.x * scale.x);

	if (m_nLife < 0)
	{ // 寿命が 0 未満の場合

		// 透明度を取得
		float fAlpha = GetAlpha();

		// 透明度を減らす
		fAlpha -= SUB_ALPHA;

		// 透明度を適用する
		SetAlpha(fAlpha);

		if (fAlpha <= 0.0f)
		{ // 透明度が 0.0f 以下の場合

			// 終了処理
			Uninit();

			// この先の処理を行わない
			return;
		}
	}
	else
	{ // 上記以外

		// 寿命を減らす
		m_nLife--;
	}
}

//===========================================
// 波紋の描画処理
//===========================================
void CSlashRipple::Draw(void)
{
	// 描画処理
	CRipple::Draw();
}

//===========================================
// 情報の設定処理
//===========================================
void CSlashRipple::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// 情報の設定処理
	CRipple::SetData(pos, rot, NONE_SCALE);

	// 全ての値を設定する
	m_nLife = LIFE;		// 寿命
}

//===========================================
// 生成処理
//===========================================
CSlashRipple* CSlashRipple::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ローカルオブジェクトを生成
	CSlashRipple* pRipple = nullptr;	// 波紋のインスタンスを生成

	if (pRipple == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pRipple = new CSlashRipple;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pRipple != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pRipple->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pRipple->SetData(pos, rot);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// 波紋のポインタを返す
	return pRipple;
}

//===========================================
// リストの取得処理
//===========================================
CListManager<CSlashRipple*> CSlashRipple::GetList(void)
{
	// リストの情報を返す
	return m_list;
}