//===========================================
//
// エディットのメイン処理[edit.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "edit.h"
#include "renderer.h"
#include "useful.h"
#include "file.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "boss.h"
#include "boss_collision.h"
#include "game.h"

// 無名名前空間
namespace
{
	
}

//==============================
// コンストラクタ
//==============================
CEdit::CEdit() : CObject(CObject::TYPE_EDIT, DIM_3D, CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	m_pBoss = nullptr;
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		m_apColl[nCnt] = nullptr;
	}
	m_nSetPartNumber = 0;
	m_nSetCollNumber = 0;
}

//==============================
// デストラクタ
//==============================
CEdit::~CEdit()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CEdit::Init(void)
{
	m_pBoss = CBoss::Create(NONE_D3DXVECTOR3, NONE_D3DXVECTOR3);

	// 値を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CEdit::Uninit(void)
{
	if (m_pBoss != nullptr)
	{ // モデルが NULL じゃない場合

		// 終了処理
		m_pBoss = nullptr;
	}

	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		if (m_apColl[nCnt] != nullptr)
		{
			m_apColl[nCnt]->Uninit();
			m_apColl[nCnt] = nullptr;
		}
	}

	// 終了処理
	Release();

	// エディットの消去処理
	CGame::DeleteEdit();
}

//========================================
//ブロックの更新処理
//========================================
void CEdit::Update(void)
{
	// Windowのタイトル
	ImGui::Begin("Editer");

	// 番号の設定処理
	SetIdx();

	// 番号の補正処理
	CorrectIdx();

	// 位置設定処理
	Positioning();

	// 半径設定処理
	SetRadius();

	// 弱点状況の設定処理
	SetWeakness();

	// 当たり判定の追加処理
	AddCollision();
	
	// 当たり判定の総数表示処理
	NumCollDisp();

	// 当たり判定のマトリックス掛け合わせ
	CollMatrix();

	if (ImGui::Button("Save") == true)
	{ // 当たり判定の追加を押した場合

		CManager::Get()->GetFile()->Save(CFile::TYPE_BOSSCOLL);
	}

	//// テキストの表示
	//ImGui::Text("Obara is Stupid");

	//// チェックボックスの表示
	//ImGui::Checkbox("Y/N", &bObaraNew);

	//// テキストの表示
	//ImGui::Text("DonkuraiKusa?");

	//// INT型のスライダー
	//if (ImGui::SliderInt("Level", &nGlassLevel, 0, 6000) == true)
	//{ // 動かした場合

	//}

	//// float型3つのスライダー
	//if (ImGui::SliderFloat3("position", (float*)&f, -4000.0f, 4000.0f))
	//{
	//	int n = 0;
	//}

	//// ボタンの表示
	//if (ImGui::Button("KusaHayashi") == true)
	//{ // ボタンを押した場合

	//	nGlassLevel++;
	//}
	//// 後に続くテキストを同じ行に表示する
	//ImGui::SameLine();

	//// テキストを表示する
	//ImGui::Text("KusaLevel = %d", nGlassLevel);

	// ImGuiの表示の終了
	ImGui::End();
}

//=====================================
//ブロックの描画処理
//=====================================
void CEdit::Draw(void)
{
	if (m_pBoss != nullptr)
	{ // モデルの情報が NULL じゃない場合

		// 描画処理
		m_pBoss->Draw();
	}

	for (int nCnt = 0; nCnt < m_pBoss->GetNumModel(); nCnt++)
	{
		if (m_apColl[nCnt] != nullptr)
		{ // モデルが NULL じゃない場合

			// 描画処理
			m_apColl[nCnt]->Draw(m_pBoss, nCnt);
		}
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CEdit::SetData(void)
{
	CManager::Get()->GetFile()->SetBossColl(&m_apColl[0]);
}

//=======================================
// 生成処理
//=======================================
CEdit* CEdit::Create(void)
{
	// ローカルオブジェクトを生成
	CEdit* pEdit = nullptr;		// エディットを生成

	if (pEdit == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pEdit = new CEdit;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pEdit != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pEdit->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pEdit->SetData();
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// エディットのポインタを返す
	return pEdit;
}

//===========================================
// 当たり判定のマトリックス
//===========================================
void CEdit::CollMatrix(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 posPart = NONE_D3DXVECTOR3;
	CBossCollision* coll = nullptr;

	// 変数を宣言
	D3DXMATRIX   mtxScale, mtxRot, mtxTrans, mtx, mtxColl;	// 計算用マトリックス
	D3DXMATRIX mtxWorld = m_pBoss->GetMatrix();		// マトリックスを取得する

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_pBoss->GetScale().x, m_pBoss->GetScale().y, m_pBoss->GetScale().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_pBoss->GetRot().y + D3DX_PI, m_pBoss->GetRot().x, m_pBoss->GetRot().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pBoss->GetPos().x, m_pBoss->GetPos().y, m_pBoss->GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	for (int nCntPart = 0; nCntPart < m_pBoss->GetNumModel(); nCntPart++)
	{
		if (m_apColl[nCntPart] != nullptr)
		{
			// マトリックスの計算処理
			m_pBoss->GetHierarchy(nCntPart)->MatrixCalc(&mtx, mtxWorld);

			for (int nCntColl = 0; nCntColl < m_apColl[nCntPart]->GetNumColl(); nCntColl++)
			{
				coll = m_apColl[nCntPart];

				// マトリックスの初期化
				D3DXMatrixIdentity(&mtxColl);

				// 位置を反映
				D3DXMatrixTranslation(&mtxTrans, coll->GetCollOffset(nCntColl).x, coll->GetCollOffset(nCntColl).y, coll->GetCollOffset(nCntColl).z);
				D3DXMatrixMultiply(&mtxColl, &mtxColl, &mtxTrans);

				// 算出した「パーツのワールドマトリックス」と「親のマトリックス」を掛け合わせる
				D3DXMatrixMultiply
				(
					&mtxColl,
					&mtxColl,
					&mtx
				);

				// 位置を設定する
				posPart.x = mtxColl._41;
				posPart.y = mtxColl._42;
				posPart.z = mtxColl._43;
			}
		}
	}
}

//===========================================
// ボスの取得処理
//===========================================
CBoss* CEdit::GetBoss(void)
{
	// ボスの情報を返す
	return m_pBoss;
}

//===========================================
// 当たり判定の情報の取得処理
//===========================================
CBossCollision* CEdit::GetColl(const int nIdx)
{
	// 当たり判定の情報を返す
	return m_apColl[nIdx];
}

//===========================================
// 位置設定処理
//===========================================
void CEdit::Positioning(void)
{
	if (m_apColl[m_nSetPartNumber] != nullptr)
	{ // 当たり判定が NULL じゃない場合

		D3DXVECTOR3 pos = m_apColl[m_nSetPartNumber]->GetCollOffset(m_nSetCollNumber);

		// 位置の設定
		ImGui::DragFloat3("Position", (float*)&pos, 1.0f, -10000.0f, 10000.0f);

		// 位置を適用
		m_apColl[m_nSetPartNumber]->SetCollOffset(pos, m_nSetCollNumber);
	}
}

//===========================================
// 番号の設定処理
//===========================================
void CEdit::SetIdx(void)
{
	ImGui::DragInt("SetPartNumber", &m_nSetPartNumber, 1, 0, m_pBoss->GetNumModel() - 1);

	if (m_apColl[m_nSetPartNumber] != nullptr)
	{ // 当たり判定が NULL じゃない場合

		ImGui::DragInt("SetCollNumber", &m_nSetCollNumber, 1, 0, m_apColl[m_nSetPartNumber]->GetNumColl() - 1);
	}
}

//===========================================
// 番号の補正処理
//===========================================
void CEdit::CorrectIdx(void)
{
	// パーツ番号の補正
	if (m_nSetPartNumber >= m_pBoss->GetNumModel())
	{
		m_nSetPartNumber = m_pBoss->GetNumModel() - 1;
	}
	if (m_nSetPartNumber < 0)
	{
		m_nSetPartNumber = 0;
	}

	// 当たり判定番号の補正
	if (m_apColl[m_nSetPartNumber] != nullptr &&
		m_nSetCollNumber >= m_apColl[m_nSetPartNumber]->GetNumColl())
	{
		m_nSetCollNumber = m_apColl[m_nSetPartNumber]->GetNumColl() - 1;
	}
	if (m_nSetCollNumber < 0)
	{
		m_nSetCollNumber = 0;
	}
}

//===========================================
// 当たり判定の追加処理
//===========================================
void CEdit::AddCollision(void)
{
	if (ImGui::Button("Add Collision") == true)
	{ // 当たり判定の追加を押した場合

		if (m_apColl[m_nSetPartNumber] == nullptr)
		{ // 当たり判定の球が NULL の場合

			// 当たり判定を生成
			m_apColl[m_nSetPartNumber] = CBossCollision::Create(0);
		}

		if (m_apColl[m_nSetPartNumber]->GetNumColl() < CBossCollision::MAX_COLLISION)
		{ // 最大値じゃない場合

			int nNumColl = m_apColl[m_nSetPartNumber]->GetNumColl();

			// モデルを生成
			m_apColl[m_nSetPartNumber]->SetModel(nNumColl);

			nNumColl++;

			// 数を設定する
			m_apColl[m_nSetPartNumber]->SetNumColl(nNumColl);
		}
	}
}

//===========================================
// 当たり判定の総数表示処理
//===========================================
void CEdit::NumCollDisp(void)
{
	if (m_apColl[m_nSetPartNumber] != nullptr)
	{ // 当たり判定が NULL じゃない場合

		if (m_apColl[m_nSetPartNumber]->GetNumColl() >= CBossCollision::MAX_COLLISION)
		{ // 当たり判定が最大の場合

			// テキストを表示する
			ImGui::Text("NumColl = MAX");
		}
		else
		{ // 上記以外

			// テキストを表示する
			ImGui::Text("NumColl = %d", m_apColl[m_nSetPartNumber]->GetNumColl());
		}
	}
	else
	{ // 上記以外

		// テキストを表示する
		ImGui::Text("404Collision");
	}
}

//===========================================
// 当たり判定の半径の設定処理
//===========================================
void CEdit::SetRadius(void)
{
	if (m_apColl[m_nSetPartNumber] != nullptr)
	{ // 当たり判定が NULL じゃない場合

		float fRadius = m_apColl[m_nSetPartNumber]->GetRadius(m_nSetCollNumber);

		// 位置の設定
		ImGui::DragFloat("Radius", &fRadius, 1.0f, 1.0f, 10000.0f);

		// 位置を適用
		m_apColl[m_nSetPartNumber]->SetRadius(fRadius, m_nSetCollNumber);
	}
}

//===========================================
// 当たり判定の弱点状況の設定処理
//===========================================
void CEdit::SetWeakness(void)
{
	if (m_apColl[m_nSetPartNumber] != nullptr)
	{ // 当たり判定が NULL じゃない場合

		if (ImGui::Button("Weakness") == true)
		{ // 弱点状況のボタンを押した場合

			// 弱点状況を取得
			bool bWeak = m_apColl[m_nSetPartNumber]->GetWeakness(m_nSetCollNumber);

			// 弱点状況を切り替える
			bWeak = !bWeak;

			// 弱点状況を適用
			m_apColl[m_nSetPartNumber]->SetWeakness(bWeak, m_nSetCollNumber);
		}

		// 後に続くテキストを同じ行に表示する
		ImGui::SameLine();

		if (m_apColl[m_nSetPartNumber]->GetWeakness(m_nSetCollNumber) == true)
		{ // 弱点の場合

			ImGui::Text("TRUE");
		}
		else
		{ // 上記以外

			ImGui::Text("FALSE");
		}
	}
}