//===========================================
//
// 木のメイン処理[tree.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "tree.h"
#include "useful.h"

#include "palm_tree.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const char* FILE_NAME[CTree::TYPE_MAX] =	// Xファイルの名前
	{
		"data\\MODEL\\PalmTree.x",		// ヤシの木
	};
}

//-------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------
CListManager<CTree*> CTree::m_list = {};		// リスト

//==============================
// コンストラクタ
//==============================
CTree::CTree(CObject::TYPE type, PRIORITY priority) : CModel(type, priority)
{
	// 全ての値をクリアする
	m_type = TYPE_PALM;		// 種類

	// リストに追加する
	m_list.Regist(this);
}

//==============================
// デストラクタ
//==============================
CTree::~CTree()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CTree::Init(void)
{
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 値を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CTree::Uninit(void)
{
	// 終了処理
	CModel::Uninit();

	// 引き抜き処理
	m_list.Pull(this);
}

//========================================
//ブロックの更新処理
//========================================
void CTree::Update(void)
{

}

//=====================================
//ブロックの描画処理
//=====================================
void CTree::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CTree::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// 情報の設定処理
	SetPos(pos);					// 位置
	SetPosOld(pos);					// 前回の位置
	SetRot(rot);					// 向き
	SetScale(NONE_SCALE);			// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(FILE_NAME[type]));	// モデルの情報

	// 全ての値を設定する
	m_type = type;		// 種類
}

//=====================================
// ヒット処理
//=====================================
void CTree::Hit(void)
{

}

//=======================================
// 生成処理
//=======================================
CTree* CTree::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type)
{
	// ローカルオブジェクトを生成
	CTree* pTree = nullptr;	// インスタンスを生成

	if (pTree == nullptr)
	{ // オブジェクトが NULL の場合

		switch (type)
		{
		case CTree::TYPE_PALM:		// ヤシの木

			// ヤシの木を生成する
			pTree = new CPalmTree;

			break;

		default:

			// 停止
			assert(false);

			break;
		}
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pTree != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pTree->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pTree->SetData(pos, rot, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 木のポインタを返す
	return pTree;
}

//=======================================
// リストの取得処理
//=======================================
CListManager<CTree*> CTree::GetList(void)
{
	// リストマネージャーを返す
	return m_list;
}

//=======================================
// 種類の取得処理
//=======================================
CTree::TYPE CTree::GetType(void) const
{
	// 種類を返す
	return m_type;
}