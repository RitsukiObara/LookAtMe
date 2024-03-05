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

#include "model.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "enemy.h"
#include "coin.h"
#include "gold_bone.h"
#include "tree.h"
#include "rock.h"
#include "block.h"
#include "bang_flower.h"
#include "wall.h"

// 無名名前空間
namespace
{
	// 全体
	const char* STYLE_NAME[CEdit::STYLE_MAX] =		// スタイルの表示名
	{
		"Enemy",
		"Coin",
		"GoldBone",
		"Tree",
		"Rock",
		"Block",
		"BangFlower",
		"Wall",
		"Grass",
		"Lake",
	};

	// 敵関係
	const char* ENEMY_NAME[CEnemy::TYPE_MAX] =		// 敵の表示名
	{
		"Tordle",
		"Iwakari",
	};	
	const char* ENEMY_MODEL[CEnemy::TYPE_MAX] =		// 敵のモデル
	{
		"data\\MODEL\\Tordle.x",
		"data\\MODEL\\Iwakari.x",
	};

	// コイン関係
	const char* COIN_MODEL = "data\\MODEL\\Coin.x";	// コインのモデル

	// 金の骨関係
	const char* GOLDBONE_MODEL = "data\\MODEL\\GoldBone.x";		// 金の骨のモデル

	// 木関係
	const char* TREE_NAME[CTree::TYPE_MAX] =		// 木の表示名
	{
		"PalmTree",
	};
	const char* TREE_MODEL[CTree::TYPE_MAX] =		// 木のモデル
	{
		"data\\MODEL\\PalmTree.x",
	};

	// 岩関係
	const char* ROCK_NAME[CRock::TYPE_MAX] =		// 岩の表示名
	{
		"Brown",
		"Gray",
		"Break",
	};
	const char* ROCK_MODEL[CRock::TYPE_MAX] =		// 岩のモデル
	{
		"data\\MODEL\\Rock001.x",
		"data\\MODEL\\Rock002.x",
		"data\\MODEL\\Rock001.x",
	};

	// ブロック関係
	const char* BLOCK_MODEL = "data\\MODEL\\IronBlock.x";		// ブロックのモデル

	// 爆弾花関係
	const char* BANGFLOWER_MODEL = "data\\MODEL\\BombFlower.x";		// 爆弾花のモデル

	// 壁関係
	const char* WALL_MODEL[CWall::TYPE_MAX] =		// 壁のモデル
	{
		"data\\MODEL\\Wall001.x",
		"data\\MODEL\\Wall002.x",
	};

	const char* WALL_NAME[CWall::TYPE_MAX] =		// 壁の名前
	{
		"Normal",
		"Narrow"
	};

	const char* ROTTYPE_NAME[CWall::ROTTYPE_MAX] =	// 向きの名前
	{
		"Front",
		"Right",
		"Back",
		"Left",
	};

	const float MOVE = 4.0f;			// 移動量
	const float POS_MAX = 10000.0f;		// 位置の最大値
	const float POS_MIN = -10000.0f;	// 位置の最小値
	const float ROT_MOVE = 0.01f;		// 移動量
	const float SCALE_MOVE = 0.1f;		// 移動量
	const float SCALE_MAX = 99000.0f;	// 移動量
	const float SCALE_MIN = 99000.0f;	// 移動量
}

//==============================
// コンストラクタ
//==============================
CEdit::CEdit() : CObject(CObject::TYPE_EDIT, DIM_3D, CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	m_pModel = nullptr;			// モデルの情報
	m_pProcess = nullptr;		// 処理の情報
	m_style = STYLE_ENEMY;		// スタイル
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
	// モデルを生成する
	m_pModel = CModel::Create(TYPE_NONE, PRIORITY_UI);

	// 処理の情報を生成する
	m_pProcess = new CEnemyProcess();

	// 値を返す
	return S_OK;
}

//========================================
//ブロックの終了処理
//========================================
void CEdit::Uninit(void)
{
	if (m_pProcess != nullptr)
	{ // 処理が NULL じゃない場合

		// メモリを開放する
		delete m_pProcess;
		m_pProcess = nullptr;
	}

	if (m_pModel != nullptr)
	{ // モデルが NULL じゃない場合

		// 終了処理
		m_pModel->Uninit();
		m_pModel = nullptr;
	}

	// 終了処理
	Release();
}

//========================================
//ブロックの更新処理
//========================================
void CEdit::Update(void)
{
	D3DXVECTOR3 pos = m_pModel->GetPos();

	// Windowのタイトル
	ImGui::Begin("Editer");

	// リストを表示する
	if (ImGui::ListBox("STYLE", (int*)&m_style, STYLE_NAME, STYLE_MAX))
	{ // スタイルを変えたとき

		if (m_pProcess != nullptr)
		{ // 処理が NULL じゃない場合

			// 処理の情報の消去
			delete m_pProcess;
			m_pProcess = nullptr;
		}

		// 向きと拡大率を設定する
		m_pModel->SetRot(NONE_D3DXVECTOR3);
		m_pModel->SetScale(NONE_SCALE);

		switch (m_style)
		{
		case CEdit::STYLE_ENEMY:

			// 敵処理
			m_pProcess = new CEnemyProcess();

			// モデルの情報設定処理
			m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(ENEMY_MODEL[0]));

			break;

		case CEdit::STYLE_COIN:

			// コイン処理
			m_pProcess = new CCoinProcess();

			// モデルの情報設定処理
			m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(COIN_MODEL));

			break;

		case CEdit::STYLE_GOLDBONE:

			// 金の骨処理
			m_pProcess = new CGoldBoneProcess();

			// モデルの情報設定処理
			m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(GOLDBONE_MODEL));

			break;

		case CEdit::STYLE_TREE:

			// 木処理
			m_pProcess = new CTreeProcess();

			// モデルの情報設定処理
			m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(TREE_MODEL[0]));

			break;

		case CEdit::STYLE_ROCK:

			// 岩処理
			m_pProcess = new CRockProcess();

			// モデルの情報設定処理
			m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(ROCK_MODEL[0]));

			break;

		case CEdit::STYLE_BLOCK:

			// ブロック処理
			m_pProcess = new CBlockProcess();

			// モデルの情報設定処理
			m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(BLOCK_MODEL));
			
			break;

		case CEdit::STYLE_BANGFLOWER:

			// ブロック処理
			m_pProcess = new CBangFlowerProcess();

			// モデルの情報設定処理
			m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(BANGFLOWER_MODEL));

			break;

		case CEdit::STYLE_WALL:

			// 壁の処理
			m_pProcess = new CWallProcess();

			// モデルの情報設定処理
			m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(WALL_MODEL[0]));

			break;

		case CEdit::STYLE_GRASS:
			break;

		case CEdit::STYLE_LAKE:
			break;

		default:

			// 停止
			assert(false);

			break;
		}
	}

	// 位置の設定
	if (ImGui::DragFloat3("Position", (float*)&pos, MOVE, POS_MIN, POS_MAX))

	// 位置を適用する
	m_pModel->SetPos(pos);

	// 処理
	m_pProcess->Process(m_pModel);

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
	if (m_pModel != nullptr)
	{ // モデルの情報が NULL じゃない場合

		// 描画処理
		m_pModel->Draw();
	}
}

//=====================================
// 情報の設定処理
//=====================================
void CEdit::SetData(void)
{
	if (m_pModel != nullptr)
	{ // モデルの情報が NULL じゃない場合

		// 情報の設定処理
		m_pModel->SetPos(NONE_D3DXVECTOR3);		// 位置
		m_pModel->SetPosOld(NONE_D3DXVECTOR3);	// 前回の位置
		m_pModel->SetRot(NONE_D3DXVECTOR3);		// 向き
		m_pModel->SetScale(NONE_SCALE);			// 拡大率
		m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(ENEMY_MODEL[0]));		// モデルの情報
	}

	// 全ての値を設定する
	m_style = STYLE_ENEMY;		// スタイル
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

// 処理の親クラス
CProcess::CProcess()
{

}

CProcess::~CProcess()
{

}

// 敵の処理
CEnemyProcess::CEnemyProcess() : CProcess()
{
	// 全ての値をクリアする
	m_type = CEnemy::TYPE_TORDLE;		// 敵の種類
}

CEnemyProcess::~CEnemyProcess()
{

}

void CEnemyProcess::Process(CModel* m_pModel)
{
	// 向きを取得する
	D3DXVECTOR3 rot = m_pModel->GetRot();

	// 向きの設定
	ImGui::DragFloat("Rotation", (float*)&rot.y, ROT_MOVE, -D3DX_PI, D3DX_PI);

	// 向きを適用する
	m_pModel->SetRot(rot);

	// 敵の種類選択
	if (ImGui::ListBox("ENEMY TYPE", (int*)&m_type, ENEMY_NAME, CEnemy::TYPE_MAX))
	{ // 敵の種類が変わった瞬間

		// モデルを変える
		m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(ENEMY_MODEL[m_type]));
	}

	// ボタンの表示
	if (ImGui::Button("EnemySet") == true)
	{ // ボタンを押した場合

		// 敵の生成処理
		CEnemy::Create(m_pModel->GetPos(), m_pModel->GetRot(), m_type);
	}

	if (ImGui::Button("Save") == true)
	{ // ボタンを押した場合

		// 敵のセーブ処理
		CManager::Get()->GetFile()->Save(CFile::TYPE_ENEMY);
	}

	// 消去処理
	Delete(m_pModel);
}

void CEnemyProcess::Delete(CModel* m_pModel)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = m_pModel->GetPos();		// 位置
	D3DXVECTOR3 vtxMax = m_pModel->GetFileData().vtxMax;	// 最大値
	D3DXVECTOR3 vtxMin = m_pModel->GetFileData().vtxMin;	// 最小値
	D3DXVECTOR3 posObject;		// オブジェクトの位置
	D3DXVECTOR3 MaxObject;		// オブジェクトの最大値
	D3DXVECTOR3 MinObject;		// オブジェクトの最小値
	CListManager<CEnemy*> list = CEnemy::GetList();
	CEnemy* pObject = nullptr;		// 先頭の敵
	CEnemy* pObjectEnd = nullptr;	// 末尾の値
	int nIdx = 0;

	// while文処理
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pObject = list.GetTop();

		// 末尾の値を取得する
		pObjectEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 敵関係の変数を設定
			posObject = pObject->GetPos();
			MaxObject = pObject->GetCollSize();
			MinObject = D3DXVECTOR3
			(
				-pObject->GetCollSize().x,
				0.0f,
				-pObject->GetCollSize().z
			);

			if (useful::RectangleCollisionXY(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionXZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionYZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true)
			{ // 敵と重なった場合

				if (ImGui::Button("Delete") == true)
				{ // ボタンを押した場合

					// ヒット処理
					pObject->Uninit();
				}
			}

			if (pObject == pObjectEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pObject = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}
}

// コインの処理
CCoinProcess::CCoinProcess()
{

}

CCoinProcess::~CCoinProcess()
{

}

void CCoinProcess::Process(CModel* m_pModel)
{
	// ボタンの表示
	if (ImGui::Button("CoinSet") == true)
	{ // ボタンを押した場合

		// コインの生成処理
		CCoin::Create(m_pModel->GetPos());
	}

	if (ImGui::Button("Save") == true)
	{ // ボタンを押した場合

		// コインのセーブ処理
		CManager::Get()->GetFile()->Save(CFile::TYPE_COIN);
	}

	// 消去処理
	Delete(m_pModel);
}

void CCoinProcess::Delete(CModel* m_pModel)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = m_pModel->GetPos();		// 位置
	D3DXVECTOR3 vtxMax = m_pModel->GetFileData().vtxMax;	// 最大値
	D3DXVECTOR3 vtxMin = m_pModel->GetFileData().vtxMin;	// 最小値
	D3DXVECTOR3 posObject;		// オブジェクトの位置
	D3DXVECTOR3 MaxObject;		// オブジェクトの最大値
	D3DXVECTOR3 MinObject;		// オブジェクトの最小値
	CListManager<CCoin*> list = CCoin::GetList();
	CCoin* pObject = nullptr;		// 先頭の敵
	CCoin* pObjectEnd = nullptr;	// 末尾の値
	int nIdx = 0;

	// while文処理
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pObject = list.GetTop();

		// 末尾の値を取得する
		pObjectEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 敵関係の変数を設定
			posObject = pObject->GetPos();
			MaxObject = pObject->GetFileData().vtxMax;
			MinObject = pObject->GetFileData().vtxMin;

			if (useful::RectangleCollisionXY(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionXZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionYZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true)
			{ // 敵と重なった場合

				if (ImGui::Button("Delete") == true)
				{ // ボタンを押した場合

					// ヒット処理
					pObject->Uninit();
				}
			}

			if (pObject == pObjectEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pObject = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}
}

// 金の骨の処理
CGoldBoneProcess::CGoldBoneProcess()
{

}

CGoldBoneProcess::~CGoldBoneProcess()
{

}

void CGoldBoneProcess::Process(CModel* m_pModel)
{
	// ボタンの表示
	if (ImGui::Button("GoldBoneSet") == true)
	{ // ボタンを押した場合

		// コインの生成処理
		CGoldBone::Create(m_pModel->GetPos());
	}

	if (ImGui::Button("Save") == true)
	{ // ボタンを押した場合

		// 金の骨のセーブ処理
		CManager::Get()->GetFile()->Save(CFile::TYPE_GOLDBONE);
	}

	// 消去処理
	Delete(m_pModel);
}

void CGoldBoneProcess::Delete(CModel* m_pModel)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = m_pModel->GetPos();		// 位置
	D3DXVECTOR3 vtxMax = m_pModel->GetFileData().vtxMax;	// 最大値
	D3DXVECTOR3 vtxMin = m_pModel->GetFileData().vtxMin;	// 最小値
	D3DXVECTOR3 posObject;		// オブジェクトの位置
	D3DXVECTOR3 MaxObject;		// オブジェクトの最大値
	D3DXVECTOR3 MinObject;		// オブジェクトの最小値
	CListManager<CGoldBone*> list = CGoldBone::GetList();
	CGoldBone* pObject = nullptr;		// 先頭の敵
	CGoldBone* pObjectEnd = nullptr;	// 末尾の値
	int nIdx = 0;

	// while文処理
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pObject = list.GetTop();

		// 末尾の値を取得する
		pObjectEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 敵関係の変数を設定
			posObject = pObject->GetPos();
			MaxObject = pObject->GetFileData().vtxMax;
			MinObject = pObject->GetFileData().vtxMin;

			if (useful::RectangleCollisionXY(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionXZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionYZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true)
			{ // 敵と重なった場合

				if (ImGui::Button("Delete") == true)
				{ // ボタンを押した場合

					// ヒット処理
					pObject->Uninit();
				}
			}

			if (pObject == pObjectEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pObject = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}
}

// 木の処理
CTreeProcess::CTreeProcess()
{
	// 全ての値をクリアする
	m_type = CTree::TYPE_PALM;		// 木の種類
}

CTreeProcess::~CTreeProcess()
{

}

void CTreeProcess::Process(CModel* m_pModel)
{
	// 向きを取得する
	D3DXVECTOR3 rot = m_pModel->GetRot();

	// 向きの設定
	ImGui::DragFloat("Rotation", (float*)&rot.y, ROT_MOVE, -D3DX_PI, D3DX_PI);

	// 向きを適用する
	m_pModel->SetRot(rot);

	// 木の種類選択
	if (ImGui::ListBox("TREE TYPE", (int*)&m_type, TREE_NAME, CTree::TYPE_MAX))
	{ // 木の種類が変わった瞬間

		// モデルを変える
		m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(TREE_MODEL[m_type]));
	}

	// ボタンの表示
	if (ImGui::Button("TreeSet") == true)
	{ // ボタンを押した場合

		// 木の生成処理
		CTree::Create(m_pModel->GetPos(), m_pModel->GetRot(), m_type);
	}

	if (ImGui::Button("Save") == true)
	{ // ボタンを押した場合

		// 木のセーブ処理
		CManager::Get()->GetFile()->Save(CFile::TYPE_TREE);
	}

	// 消去処理
	Delete(m_pModel);
}

void CTreeProcess::Delete(CModel* m_pModel)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = m_pModel->GetPos();		// 位置
	D3DXVECTOR3 vtxMax = m_pModel->GetFileData().vtxMax;	// 最大値
	D3DXVECTOR3 vtxMin = m_pModel->GetFileData().vtxMin;	// 最小値
	D3DXVECTOR3 posObject;		// オブジェクトの位置
	D3DXVECTOR3 MaxObject;		// オブジェクトの最大値
	D3DXVECTOR3 MinObject;		// オブジェクトの最小値
	CListManager<CTree*> list = CTree::GetList();
	CTree* pObject = nullptr;		// 先頭の敵
	CTree* pObjectEnd = nullptr;	// 末尾の値
	int nIdx = 0;

	// while文処理
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pObject = list.GetTop();

		// 末尾の値を取得する
		pObjectEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 敵関係の変数を設定
			posObject = pObject->GetPos();
			MaxObject = pObject->GetFileData().vtxMax;
			MinObject = pObject->GetFileData().vtxMin;

			if (useful::RectangleCollisionXY(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionXZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionYZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true)
			{ // 敵と重なった場合

				if (ImGui::Button("Delete") == true)
				{ // ボタンを押した場合

					// ヒット処理
					pObject->Uninit();
				}
			}

			if (pObject == pObjectEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pObject = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}
}

// 岩の処理
CRockProcess::CRockProcess()
{
	// 全ての値をクリアする
	m_type = CRock::TYPE_BROWN;		// 岩の種類
}

CRockProcess::~CRockProcess()
{

}

void CRockProcess::Process(CModel* m_pModel)
{
	{ // 向きの設定

		D3DXVECTOR3 rot = m_pModel->GetRot();

		ImGui::DragFloat("Rotation", (float*)&rot.y, ROT_MOVE, -D3DX_PI, D3DX_PI);

		m_pModel->SetRot(rot);
	}

	{ // 拡大率の設定

		D3DXVECTOR3 scale = m_pModel->GetScale();

		ImGui::DragFloat3("Scaling", (float*)&scale, SCALE_MOVE, SCALE_MIN, SCALE_MAX);

		m_pModel->SetScale(scale);
	}

	// 岩の種類選択
	if (ImGui::ListBox("ROCK TYPE", (int*)&m_type, ROCK_NAME, CRock::TYPE_MAX))
	{ // 岩の種類が変わった瞬間

		// モデルを変える
		m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(ROCK_MODEL[m_type]));
	}

	// ボタンの表示
	if (ImGui::Button("RockSet") == true)
	{ // ボタンを押した場合

		// 岩の生成処理
		CRock::Create(m_pModel->GetPos(), m_pModel->GetRot(), m_pModel->GetScale(), m_type);
	}

	if (ImGui::Button("Save") == true)
	{ // ボタンを押した場合

		// 岩のセーブ処理
		CManager::Get()->GetFile()->Save(CFile::TYPE_ROCK);
	}

	// 消去処理
	Delete(m_pModel);
}

void CRockProcess::Delete(CModel* m_pModel)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = m_pModel->GetPos();		// 位置
	D3DXVECTOR3 vtxMax = m_pModel->GetFileData().vtxMax;	// 最大値
	D3DXVECTOR3 vtxMin = m_pModel->GetFileData().vtxMin;	// 最小値
	D3DXVECTOR3 posObject;		// オブジェクトの位置
	D3DXVECTOR3 MaxObject;		// オブジェクトの最大値
	D3DXVECTOR3 MinObject;		// オブジェクトの最小値
	CListManager<CRock*> list = CRock::GetList();
	CRock* pObject = nullptr;		// 先頭の敵
	CRock* pObjectEnd = nullptr;	// 末尾の値
	int nIdx = 0;

	// while文処理
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pObject = list.GetTop();

		// 末尾の値を取得する
		pObjectEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 敵関係の変数を設定
			posObject = pObject->GetPos();
			MaxObject = pObject->GetFileData().vtxMax;
			MinObject = pObject->GetFileData().vtxMin;

			if (useful::RectangleCollisionXY(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionXZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionYZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true)
			{ // 敵と重なった場合

				if (ImGui::Button("Delete") == true)
				{ // ボタンを押した場合

					// ヒット処理
					pObject->Uninit();
				}
			}

			if (pObject == pObjectEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pObject = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}
}

// ブロックの処理
CBlockProcess::CBlockProcess()
{

}

CBlockProcess::~CBlockProcess()
{

}

void CBlockProcess::Process(CModel* m_pModel)
{
	{ // 拡大率の設定

		D3DXVECTOR3 scale = m_pModel->GetScale();

		ImGui::DragFloat3("Scaling", (float*)&scale, SCALE_MOVE, SCALE_MIN, SCALE_MAX);

		m_pModel->SetScale(scale);
	}

	// ボタンの表示
	if (ImGui::Button("BlockSet") == true)
	{ // ボタンを押した場合

		// ブロックの生成処理
		CBlock::Create(m_pModel->GetPos(), m_pModel->GetScale());
	}

	if (ImGui::Button("Save") == true)
	{ // ボタンを押した場合

		// ブロックのセーブ処理
		CManager::Get()->GetFile()->Save(CFile::TYPE_BLOCK);
	}

	// 消去処理
	Delete(m_pModel);
}

void CBlockProcess::Delete(CModel* m_pModel)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = m_pModel->GetPos();		// 位置
	D3DXVECTOR3 vtxMax = m_pModel->GetFileData().vtxMax;	// 最大値
	D3DXVECTOR3 vtxMin = m_pModel->GetFileData().vtxMin;	// 最小値
	D3DXVECTOR3 posObject;		// オブジェクトの位置
	D3DXVECTOR3 MaxObject;		// オブジェクトの最大値
	D3DXVECTOR3 MinObject;		// オブジェクトの最小値
	CListManager<CBlock*> list = CBlock::GetList();
	CBlock* pObject = nullptr;		// 先頭の敵
	CBlock* pObjectEnd = nullptr;	// 末尾の値
	int nIdx = 0;

	// while文処理
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pObject = list.GetTop();

		// 末尾の値を取得する
		pObjectEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 敵関係の変数を設定
			posObject = pObject->GetPos();
			MaxObject = pObject->GetFileData().vtxMax;
			MinObject = pObject->GetFileData().vtxMin;

			if (useful::RectangleCollisionXY(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionXZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionYZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true)
			{ // 敵と重なった場合

				if (ImGui::Button("Delete") == true)
				{ // ボタンを押した場合

					// ヒット処理
					pObject->Uninit();
				}
			}

			if (pObject == pObjectEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pObject = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}
}

// 爆弾花の処理
CBangFlowerProcess::CBangFlowerProcess()
{

}

CBangFlowerProcess::~CBangFlowerProcess()
{

}

void CBangFlowerProcess::Process(CModel* m_pModel)
{
	{ // 向きの設定

		D3DXVECTOR3 rot = m_pModel->GetRot();

		ImGui::DragFloat("Rotation", (float*)&rot.y, ROT_MOVE, -D3DX_PI, D3DX_PI);

		m_pModel->SetRot(rot);
	}

	// ボタンの表示
	if (ImGui::Button("BangFlowerSet") == true)
	{ // ボタンを押した場合

		// 爆弾花の生成処理
		CBangFlower::Create(m_pModel->GetPos(), m_pModel->GetRot());
	}

	if (ImGui::Button("Save") == true)
	{ // ボタンを押した場合

		// 爆弾花のセーブ処理
		CManager::Get()->GetFile()->Save(CFile::TYPE_BANGFLOWER);
	}

	// 消去処理
	Delete(m_pModel);
}

void CBangFlowerProcess::Delete(CModel* m_pModel)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = m_pModel->GetPos();		// 位置
	D3DXVECTOR3 vtxMax = m_pModel->GetFileData().vtxMax;	// 最大値
	D3DXVECTOR3 vtxMin = m_pModel->GetFileData().vtxMin;	// 最小値
	D3DXVECTOR3 posObject;		// オブジェクトの位置
	D3DXVECTOR3 MaxObject;		// オブジェクトの最大値
	D3DXVECTOR3 MinObject;		// オブジェクトの最小値
	CListManager<CBangFlower*> list = CBangFlower::GetList();
	CBangFlower* pObject = nullptr;		// 先頭の敵
	CBangFlower* pObjectEnd = nullptr;	// 末尾の値
	int nIdx = 0;

	// while文処理
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pObject = list.GetTop();

		// 末尾の値を取得する
		pObjectEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 敵関係の変数を設定
			posObject = pObject->GetPos();
			MaxObject = pObject->GetFileData().vtxMax;
			MinObject = pObject->GetFileData().vtxMin;

			if (useful::RectangleCollisionXY(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionXZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionYZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true)
			{ // 敵と重なった場合

				if (ImGui::Button("Delete") == true)
				{ // ボタンを押した場合

					// ヒット処理
					pObject->Uninit();
				}
			}

			if (pObject == pObjectEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pObject = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}
}

// 壁の処理
CWallProcess::CWallProcess()
{
	// 全ての値をクリアする
	m_type = CWall::TYPE::TYPE_NORMAL;
	m_rottype = CWall::ROTTYPE_FRONT;
}

CWallProcess::~CWallProcess()
{
	
}

void CWallProcess::Process(CModel* m_pModel)
{
	{ // 拡大率の設定

		D3DXVECTOR3 scale = m_pModel->GetScale();

		ImGui::DragFloat3("Scaling", (float*)&scale, SCALE_MOVE, SCALE_MIN, SCALE_MAX);

		m_pModel->SetScale(scale);
	}

	// 岩の種類選択
	if (ImGui::ListBox("WALL TYPE", (int*)&m_type, WALL_NAME, CWall::TYPE_MAX))
	{ // 岩の種類が変わった瞬間

		// モデルを変える
		m_pModel->SetFileData(CManager::Get()->GetXFile()->Regist(WALL_MODEL[m_type]));
	}

	// 岩の種類選択
	if (ImGui::ListBox("ROT TYPE", (int*)&m_rottype, ROTTYPE_NAME, CWall::ROTTYPE_MAX))
	{
		switch (m_rottype)
		{
		case CWall::ROTTYPE_FRONT:		// 前向き

			// 向きを設定する
			m_pModel->SetRot(NONE_D3DXVECTOR3);

			break;

		case CWall::ROTTYPE_RIGHT:		// 右向き

			// 向きを設定する
			m_pModel->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));

			break;

		case CWall::ROTTYPE_BACK:		// 後ろ向き

			// 向きを設定する
			m_pModel->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

			break;

		case CWall::ROTTYPE_LEFT:		// 左向き

			// 向きを設定する
			m_pModel->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

			break;

		default:

			// 停止
			assert(false);

			break;
		}
	}

	// ボタンの表示
	if (ImGui::Button("WallSet") == true)
	{ // ボタンを押した場合

		// 壁の生成処理
		CWall::Create(m_pModel->GetPos(), m_pModel->GetScale(), m_type, m_rottype);
	}

	if (ImGui::Button("Save") == true)
	{ // ボタンを押した場合

		// 壁のセーブ処理
		CManager::Get()->GetFile()->Save(CFile::TYPE_WALL);
	}

	// 消去処理
	Delete(m_pModel);
}

void CWallProcess::Delete(CModel* m_pModel)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = m_pModel->GetPos();		// 位置
	D3DXVECTOR3 vtxMax = m_pModel->GetFileData().vtxMax;	// 最大値
	D3DXVECTOR3 vtxMin = m_pModel->GetFileData().vtxMin;	// 最小値
	D3DXVECTOR3 posObject;		// オブジェクトの位置
	D3DXVECTOR3 MaxObject;		// オブジェクトの最大値
	D3DXVECTOR3 MinObject;		// オブジェクトの最小値
	CListManager<CWall*> list = CWall::GetList();
	CWall* pObject = nullptr;		// 先頭の敵
	CWall* pObjectEnd = nullptr;	// 末尾の値
	int nIdx = 0;

	// while文処理
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pObject = list.GetTop();

		// 末尾の値を取得する
		pObjectEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 敵関係の変数を設定
			posObject = pObject->GetPos();
			MaxObject = pObject->GetFileData().vtxMax;
			MinObject = pObject->GetFileData().vtxMin;

			if (useful::RectangleCollisionXY(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionXZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true &&
				useful::RectangleCollisionYZ(pos, posObject, vtxMax, MaxObject, vtxMin, MinObject) == true)
			{ // 敵と重なった場合

				if (ImGui::Button("Delete") == true)
				{ // ボタンを押した場合

					// ヒット処理
					pObject->Uninit();
				}
			}

			if (pObject == pObjectEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pObject = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}
}