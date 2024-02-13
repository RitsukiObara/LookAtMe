//===========================================
//
// 標識のメイン処理[obstacle.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "input.h"
#include "signpole.h"
#include "sound.h"
#include "useful.h"

#include "block.h"
#include "debugproc.h"

//-------------------------------------------
// マクロ定義
//-------------------------------------------
#define SIGNPOLE_TOPPLE_ADDSPEED	(0.005f)		// 倒れ状態の時の倒れるスピードの加算数

//==============================
// コンストラクタ
//==============================
CSignPole::CSignPole() : CObstacle(CObject::TYPE_OBSTACLE, CObject::PRIORITY_BLOCK)
{
	// 全ての値をクリアする
	m_state = STATE_NONE;		// 状態
	m_fGravity = 0.0f;			// 重力
	ZeroMemory(&m_vtx[0], sizeof(m_vtx));		// 頂点座標
}

//==============================
// デストラクタ
//==============================
CSignPole::~CSignPole()
{

}

//==============================
//標識の初期化処理
//==============================
HRESULT CSignPole::Init(void)
{
	if (FAILED(CObstacle::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_state = STATE_NONE;		// 状態
	m_fGravity = 0.0f;			// 重力
	ZeroMemory(&m_vtx[0], sizeof(m_vtx));		// 頂点座標

	// 値を返す
	return S_OK;
}

//========================================
//標識の終了処理
//========================================
void CSignPole::Uninit(void)
{
	// 終了処理
	CObstacle::Uninit();
}

//========================================
//標識の更新処理
//========================================
void CSignPole::Update(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = GetRot();

	switch (m_state)
	{
	case STATE_NONE:			// 通常状態

		break;

	case STATE_TOPPLE:			// 倒れ状態

		// 重力を加算する
		m_fGravity += SIGNPOLE_TOPPLE_ADDSPEED;

		// 標識を倒していく
		rot.z -= m_fGravity;

		if (rot.z <= -D3DX_PI * 0.5f)
		{ // 向きが一定以上になった場合

			// 向きを補正する
			rot.z = -D3DX_PI * 0.5f;

			// 重力を初期化する
			m_fGravity = 0.0f;

			// 停止状態にする
			m_state = STATE_STOP;

			// 標識の止まる音を鳴らす
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SIGNPOLE_STOP);
		}

		// 向きを更新する
		SetRot(rot);

		if (ToppleSystem() == true)
		{ // 当たり判定が true だった場合

			// 重力を初期化する
			m_fGravity = 0.0f;

			// 停止状態にする
			m_state = STATE_STOP;

			// 標識の止まる音を鳴らす
			CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SIGNPOLE_STOP);
		}

		break;

	case STATE_STOP:			// 停止状態

		break;

	default:

		// 警告文
		MessageBox(NULL, "標識の状態で規定外の数値が検出されました！", "警告！", MB_ICONWARNING);

		break;
	}

	// 更新処理
	CObstacle::Update();
}

//=====================================
//標識の描画処理
//=====================================
void CSignPole::Draw(void)
{
	// 描画処理
	CObstacle::Draw();
}

//=====================================
// ヒット処理
//=====================================
bool CSignPole::Hit(void)
{
	// 倒れ状態にする
	m_state = STATE_TOPPLE;

	// 重力を 0.0f にする
	m_fGravity = 0.0f;

	// 当たり判定の設定処理
	SetCollision(false);

	// アクション判定の設定処理
	SetAttack(false);

	// 標識が倒れる音を鳴らす
	CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SIGNPOLE_FALL);

	// true に返す
	return true;
}

//=====================================
// 情報の設定処理
//=====================================
void CSignPole::SetData(const D3DXVECTOR3& pos, const TYPE type)
{
	// 初期化処理
	SetFileData(CXFile::TYPE_SIGNPOLE);		// データの設定処理

	// 全ての値を初期化する
	m_state = STATE_NONE;		// 状態
	m_fGravity = 0.0f;			// 重力

	// 情報の設定処理
	CObstacle::SetData(pos, type);
}

//=====================================
// 状態の設定処理
//=====================================
CSignPole::STATE CSignPole::GetState(void)
{
	// 状態を返す
	return m_state;
}

//=====================================
// 頂点の取得処理
//=====================================
D3DXVECTOR3 CSignPole::GetVtx(const int nIdx)
{
	// 頂点座標を返す
	return m_vtx[nIdx];
}

//=====================================
// 斜めの障害物の当たり判定
//=====================================
float CSignPole::ElevationCollision(const D3DXVECTOR3 pos)
{
	// ローカル変数宣言
	float fHeight = 0.0f;			// 対象の高さ

	m_vtx[0] = D3DXVECTOR3			// 上奥
	(
		GetPos().x + ((sinf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMin.x) + (cosf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMax.y)),
		((sinf(GetRot().z) * GetFileData().vtxMin.x) + (cosf(GetRot().z) * GetFileData().vtxMax.y)),
		GetPos().z + GetFileData().vtxMax.z
	);

	m_vtx[1] = D3DXVECTOR3			// 上手前
	(
		GetPos().x + ((sinf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMin.x) + (cosf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMax.y)),
		((sinf(GetRot().z) * GetFileData().vtxMin.x) + (cosf(GetRot().z) * GetFileData().vtxMax.y)),
		GetPos().z + GetFileData().vtxMin.z
	);

	m_vtx[2] = D3DXVECTOR3			// 下奥
	(
		GetPos().x + ((sinf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMin.x) + (cosf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMin.y)),
		((sinf(GetRot().z) * GetFileData().vtxMin.x) + (cosf(GetRot().z) * GetFileData().vtxMin.y)),
		GetPos().z + GetFileData().vtxMax.z
	);

	m_vtx[3] = D3DXVECTOR3			// 下手前
	(
		GetPos().x + ((sinf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMin.x) + (cosf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMin.y)),
		((sinf(GetRot().z) * GetFileData().vtxMin.x) + (cosf(GetRot().z) * GetFileData().vtxMin.y)),
		GetPos().z + GetFileData().vtxMin.z
	);

	// 斜めの当たり判定
	fHeight = ObliqueCollision(m_vtx, pos);

	// 高さを返す
	return fHeight;
}

//=======================================
// 斜めの当たり判定
//=======================================
float CSignPole::ObliqueCollision(const D3DXVECTOR3* Vtx,const D3DXVECTOR3& pos)
{
	// ローカル変数宣言
	D3DXVECTOR3 nor, vec1, vec2;	// 位置
	float fHeight = 0.0f;			// 対象の高さ

	if (LineOuterProductXZ(Vtx[0], Vtx[1], pos) >= 0 &&
		LineOuterProductXZ(Vtx[1], Vtx[2], pos) >= 0 &&
		LineOuterProductXZ(Vtx[2], Vtx[0], pos) >= 0)
	{ // 真ん中の境界線より右側に居た場合

		vec1 = Vtx[1] - Vtx[0];
		vec2 = Vtx[2] - Vtx[0];

		// 法線を求める
		D3DXVec3Cross(&nor, &vec1, &vec2);

		// 法線を正規化する
		D3DXVec3Normalize(&nor, &nor);

		if (nor.y != 0.0f)
		{ // 法線のYが0.0f以外の場合

			// 高さを設定する
			fHeight = -((((pos.x - Vtx[2].x) * nor.x + (pos.z - Vtx[2].z) * nor.z) - Vtx[2].y) / nor.y);
			fHeight += GetPos().y;
		}

		// 高さを返す
		return fHeight;
	}
	else if(LineOuterProductXZ(Vtx[1], Vtx[3], pos) >= 0 &&
		LineOuterProductXZ(Vtx[2], Vtx[1], pos) >= 0 &&
		LineOuterProductXZ(Vtx[3], Vtx[2], pos) >= 0)
	{ // 真ん中の境界線より左側に居た場合

		vec1 = Vtx[2] - Vtx[3];
		vec2 = Vtx[1] - Vtx[3];

		// 法線を求める
		D3DXVec3Cross(&nor, &vec1, &vec2);

		// 法線を正規化する
		D3DXVec3Normalize(&nor, &nor);

		if (nor.y != 0.0f)
		{ // 法線のYが0.0f以外の場合

			// 高さを設定する
			fHeight = -((((pos.x - Vtx[3].x) * nor.x + (pos.z - Vtx[3].z) * nor.z) - Vtx[3].y) / nor.y);
			fHeight += GetPos().y;
		}

		// 高さを返す
		return fHeight;
	}
	else
	{ // 上記以外

		// 高さを返す
		return fHeight;
	}
}

//=======================================
// 倒れ中の障害物の当たり判定
//=======================================
bool CSignPole::ToppleCollision(const D3DXVECTOR3 pos)
{
	// ローカル変数宣言
	D3DXVECTOR3 Vtx[2];				// 頂点情報

	Vtx[0] = D3DXVECTOR3			// 下
	(
		GetPos().x + ((sinf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMax.x) + (cosf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMin.y)),
		GetPos().y + ((sinf(GetRot().z) * GetFileData().vtxMax.x) + (cosf(GetRot().z) * GetFileData().vtxMin.y)),
		0.0f
	);

	Vtx[1] = D3DXVECTOR3			// 上
	(
		GetPos().x + ((sinf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMax.x) + (cosf(GetRot().z + (D3DX_PI * 0.5f)) * GetFileData().vtxMax.y)),
		GetPos().y + ((sinf(GetRot().z) * GetFileData().vtxMax.x) + (cosf(GetRot().z) * GetFileData().vtxMax.y)),
		0.0f
	);

	if (Vtx[0].x <= pos.x &&
		Vtx[1].x >= pos.x &&
		Vtx[0].y <= pos.y &&
		Vtx[1].y >= pos.y)
	{ // X軸が障害物の範囲内にある場合

		if (LineOuterProductXY(Vtx[0], Vtx[1], pos) >= 0)
		{ // 外積の当たり判定に入ったら

		  // ローカル変数宣言
			D3DXVECTOR3 rot = GetRot();		// 向きを取得する
			float fRot = 0.0f;				// 向き

			// 向きを算出する
			fRot = -atan2f(pos.x - Vtx[0].x, pos.y - Vtx[0].y);

			// 向きを設定する
			rot.z = fRot + 0.01f;

			// 向きを更新する
			SetRot(rot);

			// true を返す
			return true;
		}
		else
		{ // 上記以外

		  // false を返す
			return false;
		}
	}
	else
	{ // 上記以外

		// false を返す
		return false;
	}
}

//=======================================
// 倒れ中の当たり判定の大本
//=======================================
bool CSignPole::ToppleSystem(void)
{
	// ローカル変数宣言
	CObject* pObj = nullptr;
	CBlock* pBlock = nullptr;
	D3DXVECTOR3 pos;

	// オブジェクトの情報を取得する
	pObj = CObject::GetTop(PRIORITY_BLOCK);

	while(pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == TYPE_BLOCK)
		{ // オブジェクトの種類がブロックだった場合

			// モデルの情報を取得する
			pBlock = dynamic_cast<CBlock*>(pObj);

			if (pBlock != nullptr)
			{ // ブロックが NULL じゃない場合

				// 位置を算出する
				pos = D3DXVECTOR3
				(
					pBlock->GetPos().x + pBlock->GetFileData().vtxMin.x,
					pBlock->GetPos().y + pBlock->GetFileData().vtxMax.y,
					0.0f
				);

				if (ToppleCollision(pos) == true)
				{ // 当たり判定が true の場合

					// true を返す
					return true;
				}
			}
		}

		// 次のオブジェクトを設定する
		pObj = pObj->GetNext();
	}

	// false を返す
	return false;
}