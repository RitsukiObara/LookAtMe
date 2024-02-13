//============================================
//
// リザルトランク処理[result_rank.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "result_rank.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"

//========================
// コンストラクタ
//========================
CResultRank::CResultRank() : CModel(TYPE_RESULTRANK, PRIORITY_UI)
{

}

//========================
// デストラクタ
//========================
CResultRank::~CResultRank()
{

}

//========================
// 初期化処理
//========================
HRESULT CResultRank::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//========================
// 終了処理
//========================
void CResultRank::Uninit(void)
{
	// 終了処理
	CModel::Uninit();
}

//========================
// 更新処理
//========================
void CResultRank::Update(void)
{

}

//========================
// 描画処理
//========================
void CResultRank::Draw(void)
{
	// 画面クリア(Zバッファのクリア)
	CManager::GetRenderer()->GetDevice()->Clear(0,
		NULL,
		D3DCLEAR_ZBUFFER,
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画処理
	CModel::Draw();
}

//========================
// 情報の設定処理
//========================
void CResultRank::SetData(const D3DXVECTOR3 pos, const CResult::RANK rank)
{
	// 情報を初期化する
	SetPos(pos);
	SetPosOld(pos);
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetScale(NONE_SCALE);

	switch (rank)
	{
	case CResult::RANK_SS:		// SSランク

		// モデルの設定処理
		SetFileData(CXFile::TYPE_RANK_SS);

		break;

	case CResult::RANK_S:		// Sランク

		// モデルの設定処理
		SetFileData(CXFile::TYPE_RANK_S);

		break;

	case CResult::RANK_A:		// Aランク

		// モデルの設定処理
		SetFileData(CXFile::TYPE_RANK_A);

		break;

	case CResult::RANK_B:		// Bランク

		// モデルの設定処理
		SetFileData(CXFile::TYPE_RANK_B);

		break;

	case CResult::RANK_C:		// Cランク

		// モデルの設定処理
		SetFileData(CXFile::TYPE_RANK_C);

		break;
	}
}

//========================
// 生成処理
//========================
CResultRank* CResultRank::Create(const D3DXVECTOR3 pos, const CResult::RANK rank)
{
	// ローカルオブジェクトを生成
	CResultRank* pRank = nullptr;	// プレイヤーのインスタンスを生成

	if (pRank == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pRank = new CResultRank;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pRank != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pRank->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "リザルトランクの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pRank->SetData(pos, rank);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// オブジェクト2Dのポインタを返す
	return pRank;
}