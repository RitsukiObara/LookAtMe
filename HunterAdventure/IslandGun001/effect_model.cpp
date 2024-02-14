//=======================================
//
// エフェクトモデルのメイン処理[effect_model.cpp]
// Author 小原立暉
//
//=======================================
#include "manager.h"
#include "effect_model.h"
#include "useful.h"

//=======================================
// 無名名前空間
//=======================================
namespace
{
	const char* MODEL = "data\\MODEL\\Platform\\FracScrew.x";		// モデルの名前
}

//=========================
// コンストラクタ
//=========================
CEffectModel::CEffectModel() : CModel(CObject::TYPE_EFFECTMODEL, CObject::PRIORITY_EFFECT)
{
	// 全ての値をクリアする
	m_move = NONE_D3DXVECTOR3;		// 移動量
	m_rotMove = NONE_D3DXVECTOR3;	// 向きの移動量
	m_reduce = NONE_D3DXVECTOR3;	// 拡大率の縮む間隔
	m_nLife = 0;					// 寿命
	m_fAlpha = 1.0f;				// 透明度
	m_fSub = 0.0f;					// 透明になる間隔
}

//=========================
// デストラクタ
//=========================
CEffectModel::~CEffectModel()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CEffectModel::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_move = NONE_D3DXVECTOR3;		// 移動量
	m_rotMove = NONE_D3DXVECTOR3;	// 向きの移動量
	m_reduce = NONE_D3DXVECTOR3;	// 拡大率の縮む間隔
	m_nLife = 0;					// 寿命
	m_fAlpha = 1.0f;				// 透明度
	m_fSub = 0.0f;					// 透明になる間隔

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CEffectModel::Uninit(void)
{
	// 終了
	CModel::Uninit();
}

//=========================
// 更新処理
//=========================
void CEffectModel::Update(void)
{
	// 移動処理
	Move();

	// 向きの移動処理
	RotMove();

	// 透明度の減算処理
	SubAlpha();

	// 拡大率の減算処理
	Reduce();

	// 寿命を減らしていく
	m_nLife--;

	if (m_nLife <= 0)
	{ // エフェクトの寿命が尽きた時

		// 破棄処理
		Uninit();

		// この先の処理を行わない
		return;
	}
}

//=========================
// 描画処理
//=========================
void CEffectModel::Draw(void)
{
	// 描画処理
	CModel::Draw(m_fAlpha);
}

//=========================
// 情報の設定処理
//=========================
void CEffectModel::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const D3DXVECTOR3& scale, const int nLife, const D3DXVECTOR3& rotMove)
{
	// 情報の設定処理
	SetPos(pos);							// 位置
	SetPosOld(pos);							// 前回の位置
	SetRot(NONE_D3DXVECTOR3);				// 向き
	SetScale(scale);						// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL));	// モデルの情報

	// 全ての値を設定する
	m_move = move;				// 移動量
	m_rotMove = rotMove;		// 向きの移動量
	m_nLife = nLife;			// 寿命
	m_fAlpha = 1.0f;			// 透明度
	m_fSub = m_fAlpha * (1.0f / m_nLife);		// 透明になる間隔
	m_reduce.x = scale.x * (1.0f / m_nLife);	// 拡大率の縮む間隔(X軸)
	m_reduce.y = scale.y * (1.0f / m_nLife);	// 拡大率の縮む間隔(Y軸)
	m_reduce.z = scale.z * (1.0f / m_nLife);	// 拡大率の縮む間隔(Z軸)
}

//=========================
// 生成処理
//=========================
CEffectModel* CEffectModel::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, const D3DXVECTOR3& scale, const int nLife, const D3DXVECTOR3& rotMove)
{
	// ローカルオブジェクトを生成
	CEffectModel* pEffect = nullptr;	// プレイヤーのインスタンスを生成

	if (pEffect == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pEffect = new CEffectModel;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pEffect != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pEffect->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pEffect->SetData(pos, move, scale, nLife, rotMove);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pEffect;
}

//=========================
// 移動処理
//=========================
void CEffectModel::Move(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する

	// 移動する
	pos += m_move;

	// 位置を更新する
	SetPos(pos);
}

//=========================
// 向きの移動処理
//=========================
void CEffectModel::RotMove(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = GetRot();		// 向きを取得する

	// 向きを移動する
	rot += m_rotMove;

	// 向きの正規化
	useful::RotNormalize(&rot.x);
	useful::RotNormalize(&rot.y);
	useful::RotNormalize(&rot.z);

	// 位置を更新する
	SetRot(rot);
}

//=========================
// 透明度の減算処理
//=========================
void CEffectModel::SubAlpha(void)
{
	// 透明度を減算させる
	m_fAlpha -= m_fSub;

	if (m_fAlpha <= 0.0f)
	{ // 透明度が0.0f以下の場合

		// 透明度を補正する
		m_fAlpha = 0.0f;
	}
}

//=========================
// 拡大率の減算処理
//=========================
void CEffectModel::Reduce(void)
{
	// ローカル変数を宣言
	D3DXVECTOR3 scale = GetScale();		// 拡大率を取得する

	// 拡大率を縮める
	scale -= m_reduce;

	if (scale.x <= 0.0f)
	{ // 拡大率が0.0f以下の場合

		// 拡大率を補正する
		scale.x = 0.0f;
	}

	if (scale.y <= 0.0f)
	{ // 拡大率が0.0f以下の場合

		// 拡大率を補正する
		scale.y = 0.0f;
	}

	if (scale.z <= 0.0f)
	{ // 拡大率が0.0f以下の場合

		// 拡大率を補正する
		scale.z = 0.0f;
	}

	// 拡大率を適用する
	SetScale(scale);
}