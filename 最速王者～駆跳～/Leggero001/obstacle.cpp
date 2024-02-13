//===========================================
//
// 障害物のメイン処理[obstacle.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "obstacle.h"
#include "object.h"
#include "useful.h"
#include "shadowCircle.h"

#include "hydrant.h"
#include "signpole.h"
#include "drum.h"
#include "woodbox.h"
#include "streetlamp.h"
#include "streetpole.h"

//==============================
// コンストラクタ
//==============================
CObstacle::CObstacle() : CModel(CObject::TYPE_OBSTACLE, CObject::PRIORITY_BLOCK)
{
	// コンストラクタの箱
	Box();
}

//==============================
// コンストラクタ
//==============================
CObstacle::CObstacle(CObject::TYPE type, CObject::PRIORITY priority) : CModel(type, priority)
{
	// コンストラクタの箱
	Box();
}

//==============================
// デストラクタ
//==============================
CObstacle::~CObstacle()
{

}

//==============================
// コンストラクタの箱
//==============================
void CObstacle::Box(void)
{
	// 全ての値をクリアする
	m_type = CObstacle::TYPE_HYDRANT;		// 種類
	m_nShadowIdx = INIT_SHADOW;				// 影のインデックス
	m_bCollision = true;					// 当たり判定が機能するか
	m_bAttack = false;						// 攻撃出来るかどうか
	m_bRangeWithin = false;					// 攻撃範囲内かどうか

// デバッグ処理
#ifdef _DEBUG

	m_bDelete = false;							// 消去状況
#endif 
}

//==============================
//障害物の初期化処理
//==============================
HRESULT CObstacle::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値をクリアする
	m_type = CObstacle::TYPE_HYDRANT;		// 種類
	m_nShadowIdx = INIT_SHADOW;				// 影のインデックス
	m_bCollision = true;					// 当たり判定が機能するか
	m_bAttack = false;						// 攻撃出来るかどうか
	m_bRangeWithin = false;					// 攻撃範囲内かどうか

// デバッグ処理
#ifdef _DEBUG

	m_bDelete = false;							// 消去状況
#endif

	// 値を返す
	return S_OK;
}

//========================================
//障害物の終了処理
//========================================
void CObstacle::Uninit(void)
{
	//// 影のポインタを取得する
	//CShadowCircle* pShadow = CShadowCircle::GetShadow(m_nShadowIdx);

	//if (CShadowCircle::GetShadow(m_nShadowIdx) != nullptr)
	//{ // 影のポインタがあった場合

	//	// 影の終了処理
	//	pShadow->Uninit();
	//	m_nShadowIdx = INIT_SHADOW;		// 影のインデックス
	//}

	// 終了処理
	CModel::Uninit();
}

//========================================
//障害物の更新処理
//========================================
void CObstacle::Update(void)
{
	// 影の位置設定処理
	ShadowPosSet();
}

//=====================================
//障害物の描画処理
//=====================================
void CObstacle::Draw(void)
{
	if (m_bRangeWithin == true)
	{ // 攻撃範囲内の場合

		// 描画処理
		CModel::Draw(D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f));
	}
	else
	{ // 上記以外

		// 描画処理
		CModel::Draw();
	}
}

//=======================================
// ヒット処理
//=======================================
bool CObstacle::Hit(void)
{
	// false を返す
	return false;
}

//=======================================
// 情報の設定処理
//=======================================
void CObstacle::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// デバッグ処理
#ifdef _DEBUG

	m_bDelete = false;				// 消去状況
#endif 

	m_type = type;					// 種類を設定する
	m_bCollision = true;			// 当たり判定が機能するか
	m_bAttack = true;				// 攻撃出来るかどうか
	m_bRangeWithin = false;			// 攻撃範囲内かどうか

	// 初期化処理
	SetPos(pos);								// 位置設定
	SetPosOld(pos);								// 前回の位置設定処理
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 向き設定
	SetScale(NONE_SCALE);						// 拡大率設定

	//// ローカル変数宣言
	//CShadowCircle* pShadow = nullptr;		// 影へのポインタ

	//// 影の設定処理
	//pShadow = pShadow->CShadowCircle::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), GetFileData().fRadius);

	//// 影のインデックス設定
	//m_nShadowIdx = pShadow->GetNumID();
}

//=======================================
// 種類の設定処理
//=======================================
void CObstacle::SetType(const TYPE type)
{
	// 種類を設定する
	m_type = type;
}

//=======================================
// 種類の取得処理
//=======================================
CObstacle::TYPE CObstacle::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//=======================================
// 当たり判定の設定処理
//=======================================
void CObstacle::SetCollision(const bool bCollision)
{
	// 当たり判定状況を設定する
	m_bCollision = bCollision;
}

//=======================================
// 当たり判定の取得処理
//=======================================
bool CObstacle::GetCollision(void) const
{
	// 当たり判定状況を返す
	return m_bCollision;
}

//=======================================
// アクションの設定処理
//=======================================
void CObstacle::SetAttack(const bool bAttack)
{
	// アクション判定を設定する
	m_bAttack = bAttack;
}

//=======================================
// アクションの取得処理
//=======================================
bool CObstacle::GetAttack(void) const
{
	// アクション判定を返す
	return m_bAttack;
}

//=======================================
// 攻撃範囲内かどうかの設定処理
//=======================================
void CObstacle::SetRange(const bool bRange)
{
	// 攻撃範囲内かどうかを設定する
	m_bRangeWithin = bRange;
}

//=======================================
// 攻撃範囲内かどうかの取得処理
//=======================================
bool CObstacle::GetRange(void) const
{
	// 攻撃範囲内かどうかを返す
	return m_bRangeWithin;
}

//=======================================
// 生成処理
//=======================================
CObstacle* CObstacle::Create(const D3DXVECTOR3& pos, const TYPE type)
{
	// ローカルオブジェクトを生成
	CObstacle* pObstacle = nullptr;	// プレイヤーのインスタンスを生成

	if (pObstacle == nullptr)
	{ // オブジェクトが NULL の場合

		switch (type)
		{
		case TYPE_HYDRANT:		// 消火栓

			// 消火栓のオブジェクトを生成
			pObstacle = new CHydrant;

			break;

		case TYPE_SIGNPOLE:		// 標識

			// 標識のオブジェクトを生成
			pObstacle = new CSignPole;

			break;

		case TYPE_DRUM:

			// ドラム缶のオブジェクトを生成
			pObstacle = new CDrum;

			break;

		case TYPE_WOODBOX:		// 木箱

			// 木箱のオブジェクトを生成
			pObstacle = new CWoodBox;

			break;

		case TYPE_LAMP:			// 街灯(光源)

			// 街灯(光源)のオブジェクトを生成
			pObstacle = new CStreetLamp;

			break;

		case TYPE_POLE:			// 街灯(支柱)

			// 街灯(支柱)のオブジェクトを生成
			pObstacle = new CStreetPole;

			break;

		default:				// 上記以外

			// 警告文
			MessageBox(NULL, "障害物の生成で規定外の数値が検出されました！", "警告！", MB_ICONWARNING);

			break;
		}
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pObstacle != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pObstacle->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "障害物の初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pObstacle->SetData(pos, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// プレイヤーのポインタを返す
	return pObstacle;
}

//=======================================
// 影の位置設定処理
//=======================================
void CObstacle::ShadowPosSet(void)
{
	//// ローカル変数宣言
	//CShadowCircle* pShadow = CShadowCircle::GetShadow(m_nShadowIdx);									// 影のポインタ

	//if (pShadow != nullptr)
	//{ // 影のポインタが NULL じゃない場合

	//	// 影の位置向き設定処理
	//	pShadow->SetPosRot(m_nShadowIdx, D3DXVECTOR3(GetPos().x, 0.0f, GetPos().z), GetRot());
	//}
}

// デバッグ処理
#ifdef _DEBUG

//=======================================
// 消去状況の設定処理
//=======================================
void CObstacle::SetDelete(const bool bDelete)
{
	// 消去状況を設定する
	m_bDelete = bDelete;
}

//=======================================
// 消去状況の取得処理
//=======================================
bool CObstacle::GetDelete(void) const
{
	// 消去状況を返す
	return m_bDelete;
}

#endif