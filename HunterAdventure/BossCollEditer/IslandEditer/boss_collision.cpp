//=========================================================
//
// ボスの当たり判定処理 [boss_collision.cpp]
// Author：小原立暉
//
//=========================================================
//*********************************************************
//	インクルードファイル
//*********************************************************
#include "boss_collision.h"
#include "boss.h"

#include "manager.h"
#include "objectX.h"

//===============================
// コンストラクタ
//===============================
CBossCollision::CBossCollision()
{
	// 全ての値をクリアする
	memset(m_coll, 0, sizeof(m_coll));	// 当たり判定の情報
	m_nNumColl = 0;						// 当たり判定の総数
}

//===============================
// デストラクタ
//===============================
CBossCollision::~CBossCollision()
{

}

//===============================
// 終了処理
//===============================
void CBossCollision::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_COLLISION; nCnt++)
	{
		if (m_coll[nCnt].pModel != nullptr)
		{ // モデルが NULL じゃない場合

			// モデルの終了処理
			m_coll[nCnt].pModel->Uninit();
			m_coll[nCnt].pModel = nullptr;
		}
	}

	// 自身を削除する
	delete this;
}

//===============================
// 更新処理
//===============================
void CBossCollision::Update(void)
{

}

//===============================
// 描画処理
//===============================
void CBossCollision::Draw(CBoss* pBoss, const int nIdx)
{
	// ローカル変数宣言
	D3DXVECTOR3 posPart = NONE_D3DXVECTOR3;

	// 変数を宣言
	D3DXMATRIX mtxScale, mtxRot, mtxTrans, mtx, mtxColl;	// 計算用マトリックス
	D3DXMATRIX mtxWorld = pBoss->GetMatrix();		// マトリックスを取得する

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, pBoss->GetScale().x, pBoss->GetScale().y, pBoss->GetScale().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, pBoss->GetRot().y + D3DX_PI, pBoss->GetRot().x, pBoss->GetRot().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pBoss->GetPos().x, pBoss->GetPos().y, pBoss->GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// マトリックスの計算処理
	pBoss->GetHierarchy(nIdx)->MatrixCalc(&mtx, mtxWorld);

	for (int nCnt = 0; nCnt < m_nNumColl; nCnt++)
	{
		m_coll[nCnt].pModel->DrawMatrix(mtx);
	}
}

//===============================
// 情報の設定処理
//===============================
void CBossCollision::SetData(const int nNum)
{
	m_nNumColl = nNum;

	for (int nCnt = 0; nCnt < m_nNumColl; nCnt++)
	{
		m_coll[nCnt].pModel = CModel::Create(CObject::TYPE_NONE, CObject::PRIORITY_BG);

		m_coll[nCnt].pModel->SetFileData(CManager::Get()->GetXFile()->Regist("data\\MODEL\\CollPoint.x"));
	}
}

//===============================
// 生成処理
//===============================
CBossCollision* CBossCollision::Create(const int nNum)
{
	// ボスのインスタンス
	CBossCollision* pColl = nullptr;

	if (pColl == nullptr)
	{ // オブジェクトが NULL の場合

		// ボスを生成する
		pColl = new CBossCollision;

		// 情報の設定処理
		pColl->SetData(nNum);
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return pColl;
	}

	// ボスのポインタを返す
	return pColl;
}

//===============================
// 当たり判定の情報の設定処理
//===============================
void CBossCollision::SetColl(const SCollision& coll, const int nIdx)
{
	// 当たり判定の情報を設定する
	m_coll[nIdx] = coll;
}

//===============================
// 当たり判定の情報の取得処理
//===============================
CBossCollision::SCollision CBossCollision::GetColl(const int nIdx) const
{
	// 当たり判定の情報を返す
	return m_coll[nIdx];
}

//===============================
// オフセット座標の設定処理
//===============================
void CBossCollision::SetCollOffset(const D3DXVECTOR3& offset, const int nIdx)
{
	// オフセット座標を設定する
	m_coll[nIdx].offset = offset;

	m_coll[nIdx].pModel->SetPos(offset);
}

//===============================
// オフセット座標の取得処理
//===============================
D3DXVECTOR3 CBossCollision::GetCollOffset(const int nIdx) const
{
	// オフセット座標を返す
	return m_coll[nIdx].offset;
}

//===============================
// 半径の設定処理
//===============================
void CBossCollision::SetRadius(const float fRadius, const int nIdx)
{
	// 半径を設定する
	m_coll[nIdx].fRadius = fRadius;

	float f = fRadius * 0.01f;

	m_coll[nIdx].pModel->SetScale(D3DXVECTOR3(f, f, f));
}

//===============================
// 半径の取得処理
//===============================
float CBossCollision::GetRadius(const int nIdx) const
{
	// 半径を返す
	return m_coll[nIdx].fRadius;
}

//===============================
// 弱点状況の設定処理
//===============================
void CBossCollision::SetWeakness(const bool bWeak, const int nIdx)
{
	// 弱点状況を設定する
	m_coll[nIdx].bWeakness = bWeak;
}

//===============================
// 弱点状況の取得処理
//===============================
bool CBossCollision::GetWeakness(const int nIdx) const
{
	// 弱点状況を返す
	return m_coll[nIdx].bWeakness;
}

//===============================
// 当たり判定の総数の設定処理
//===============================
void CBossCollision::SetNumColl(const int nNum)
{
	// 総数を設定する
	m_nNumColl = nNum;
}

//===============================
// 当たり判定の総数の取得処理
//===============================
int CBossCollision::GetNumColl(void) const
{
	// 総数を返す
	return m_nNumColl;
}

//===============================
// モデルの設定処理
//===============================
void CBossCollision::SetModel(const int nIdx)
{
	if (m_coll[nIdx].pModel == nullptr)
	{
		m_coll[nIdx].pModel = CModel::Create(CObject::TYPE_NONE, CObject::PRIORITY_BG);

		m_coll[nIdx].pModel->SetFileData(CManager::Get()->GetXFile()->Regist("data\\MODEL\\CollPoint.x"));
	}
}

//===============================
// モデルの消去処理
//===============================
void CBossCollision::DeleteModel(const int nIdx)
{
	if (m_coll[nIdx].pModel != nullptr)
	{ // モデルが NULL じゃない場合

		// 終了処理
		m_coll[nIdx].pModel->Uninit();
		m_coll[nIdx].pModel = nullptr;
	}
}