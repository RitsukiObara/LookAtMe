//=======================================
//
// シルエットのメイン処理[silhouette.cpp]
// Author 小原立暉
//
//=======================================
#include "silhouette.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "useful.h"

#include"locus2D.h"

//---------------------------------------
// マクロ定義
//---------------------------------------
#define SILHOUETTE_SIZE			(D3DXVECTOR3(150.0f, 150.0f, 0.0f))						// シルエットのサイズ
#define SILHOUETTE_RIGHT_POS	(D3DXVECTOR3(SCREEN_WIDTH - 200.0f, 600.0f, 0.0f))		// シルエットの右の位置
#define SILHOUETTE_LEFT_POS		(D3DXVECTOR3(200.0f, 600.0f, 0.0f))						// シルエットの左の位置
#define SILHOUETTE_DISTANCE		(400.0f)												// シルエットの距離
#define SILHOUETTE_APPEAR_COUNT	(40)													// 出現するカウント数
#define SILHOUETTE_LOCUS_COUNT	(4)														// 残像を出すカウント数

//=======================================
// 静的メンバ変数宣言
//=======================================
const char* CSilhouette::m_apTextureFile[TYPE_MAX] = 			// テクスチャの名前
{
	"data/TEXTURE/Silhouette000.png",			// 走る人型
	"data/TEXTURE/Silhouette001.png",			// 乗り越える人型
	"data/TEXTURE/Silhouette002.png",			// 掴む人型
	"data/TEXTURE/Silhouette003.png",			// 飛び越える人型
};

//=========================
// コンストラクタ
//=========================
CSilhouette::CSilhouette() : CObject2D(CObject2D::TYPE_SILHOUETTE, CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	m_state = STATE_APPEAR;		// 状態
	m_type = TYPE_RUN;			// 種類
	m_nStateCount = 0;			// 状態カウント
	m_fCircumRot = 0.0f;		// 円周上の向き
	m_fRotMove = 0.0f;			// 向きの移動量
	m_bRight = true;			// 右側かどうか
}

//=========================
// デストラクタ
//=========================
CSilhouette::~CSilhouette()
{

}

//=========================
// 初期化処理
//=========================
HRESULT CSilhouette::Init(void)
{
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_state = STATE_APPEAR;		// 状態
	m_type = TYPE_RUN;			// 種類
	m_nStateCount = 0;			// 状態カウント
	m_fCircumRot = 0.0f;		// 円周上の向き
	m_fRotMove = 0.0f;			// 向きの移動量
	m_bRight = true;			// 右側かどうか

	// 成功を返す
	return S_OK;
}

//=========================
// 終了処理
//=========================
void CSilhouette::Uninit(void)
{
	// 終了
	CObject2D::Uninit();
}

//=========================
// 更新処理
//=========================
void CSilhouette::Update(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置

	switch (m_state)
	{
	case STATE_APPEAR:		// 出現状態

		// 状態カウントを加算する
		m_nStateCount++;

		if (m_nStateCount <= SILHOUETTE_APPEAR_COUNT)
		{ // 移動カウントが一定数以上の場合

			// 円周上の向きを減算する
			m_fCircumRot += m_fRotMove;
		}
		else
		{ // 上記以外

			// 停止状態にする
			m_state = STATE_STOP;
		}

		// 向きの正規化
		RotNormalize(&m_fCircumRot);

		if (m_bRight == true)
		{ // 右側の場合

			// 位置を設定する
			pos.x = SILHOUETTE_RIGHT_POS.x + sinf(m_fCircumRot)* SILHOUETTE_DISTANCE;
			pos.y = SILHOUETTE_RIGHT_POS.y + cosf(m_fCircumRot)* SILHOUETTE_DISTANCE;
		}
		else
		{ // 左側の場合

			// 位置を設定する
			pos.x = SILHOUETTE_LEFT_POS.x + sinf(m_fCircumRot)* SILHOUETTE_DISTANCE;
			pos.y = SILHOUETTE_LEFT_POS.y + cosf(m_fCircumRot)* SILHOUETTE_DISTANCE;
		}

		if (m_nStateCount % SILHOUETTE_LOCUS_COUNT == 0)
		{ // 一定間隔で

			// 残像の生成
			CLocus2D::Create(pos, GetSize(), 0.4f, 13, GetTexIdx());
		}

		break;

	case STATE_STOP:		// 停止状態

		// 特に処理無し

		break;
	}

	// 情報を設定する
	SetPos(pos);				// 位置
	SetLength();				// 長さ
	SetAngle();					// 方向

	// 頂点情報の初期化
	SetVertex();
}

//=========================
// 描画処理
//=========================
void CSilhouette::Draw(void)
{
	// 描画処理
	CObject2D::Draw();
}

//=========================
// 情報の設定処理
//=========================
void CSilhouette::SetData(const TYPE type, const bool bRight)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;		// 位置

	// スクロールの設定処理
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));			// 向き設定
	SetSize(SILHOUETTE_SIZE);	// サイズ設定

	// 全ての値を初期化する
	m_state = STATE_APPEAR;		// 状態
	m_type = type;				// 種類
	m_nStateCount = 0;			// 状態カウント
	m_bRight = bRight;			// 右側かどうか

	if (m_bRight == true)
	{ // 右側の場合

		// 円周上の向きを設定する
		m_fCircumRot = D3DX_PI * 0.5f;

		// 位置を設定する
		pos.x = SILHOUETTE_RIGHT_POS.x + sinf(m_fCircumRot) * SILHOUETTE_DISTANCE;
		pos.y = SILHOUETTE_RIGHT_POS.y + cosf(m_fCircumRot) * SILHOUETTE_DISTANCE;
		pos.z = 0.0f;

		// 位置設定
		SetPos(pos);
	}
	else
	{ // 左側の場合

		// 円周上の向きを設定する
		m_fCircumRot = D3DX_PI * -0.5f;

		// 位置を設定する
		pos.x = SILHOUETTE_LEFT_POS.x + sinf(m_fCircumRot) * SILHOUETTE_DISTANCE;
		pos.y = SILHOUETTE_LEFT_POS.y + cosf(m_fCircumRot) * SILHOUETTE_DISTANCE;
		pos.z = 0.0f;

		// 位置設定
		SetPos(pos);
	}

	// 向きの移動量を設定する
	m_fRotMove = D3DX_PI - m_fCircumRot;

	// 向きの正規化
	RotNormalize(&m_fRotMove);

	// フレーム毎の移動量にする
	m_fRotMove /= SILHOUETTE_APPEAR_COUNT;

	SetLength();				// 長さ設定
	SetAngle();					// 方向設定

	// 頂点情報の初期化
	SetVertex();

	// テクスチャの設定処理
	SetVtxTexture();

	// テクスチャの読み込み処理
	BindTexture(CManager::GetTexture()->Regist(m_apTextureFile[m_type]));
}

//=========================
// 生成処理
//=========================
CSilhouette* CSilhouette::Create(const TYPE type, const bool bRight)
{
	// ローカルオブジェクトを生成
	CSilhouette* pEffect = nullptr;	// プレイヤーのインスタンスを生成

	if (pEffect == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pEffect = new CSilhouette;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pEffect != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pEffect->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "シルエットの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pEffect->SetData(type, bRight);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pEffect;
}