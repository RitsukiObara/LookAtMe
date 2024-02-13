//=======================================
//
// ゴールのメイン処理[Goal.cpp]
// Author：小原立暉
//
//=======================================
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "goal.h"
#include "useful.h"

#include "game_goal.h"
#include "tutorial_goal.h"

//---------------------------------------
// マクロ定義
//---------------------------------------
#define GOAL_TEXTURE		"data/TEXTURE/Goal.png"					// ゴールのテクスチャ
#define GOAL_SIZE			(D3DXVECTOR3(30.0f, 5000.0f, 0.0f))		// ゴールのサイズ
#define GOAL_INIT_COLOR		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f))		// ゴールの初期色
#define GOAL_ADD_ALPHA		(0.01f)									// ゴールの透明度の追加量
#define GOAL_ALPHA_MAX		(0.7f)									// ゴールの透明度の最大値
#define GOAL_ALPHA_MIN		(0.3f)									// ゴールの透明度の最小値

//==========================================
// コンストラクタ
//==========================================
CGoal::CGoal() : CObject3D(CObject::TYPE_GOAL, CObject::PRIORITY_PLAYER)
{
	// コンストラクタの箱
	Box();
}

//==========================================
// オーバーロードコンストラクタ
//==========================================
CGoal::CGoal(CObject::TYPE type, CObject::PRIORITY priority) : CObject3D(type, priority)
{
	// コンストラクタの箱
	Box();
}

//==========================================
// デストラクタ
//==========================================
CGoal::~CGoal()
{

}

//==========================================
// コンストラクタの箱
//==========================================
void CGoal::Box(void)
{
	// 全ての値をクリアする
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// 色
	m_fSubCol = 0.0f;								// 透明度の減算量
}

//==========================================
// ゴールの初期化処理
//==========================================
HRESULT CGoal::Init(void)
{
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値をクリアする
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		// 色
	m_fSubCol = 0.0f;								// 透明度の減算量

	// 成功を返す
	return S_OK;
}

//========================================
// ゴールの終了処理
//========================================
void CGoal::Uninit(void)
{
	// 終了処理
	CObject3D::Uninit();
}

//===========================================
// ゴールの描画処理
//===========================================
void CGoal::Draw(void)
{
	// 描画処理
	CObject3D::DrawLightOff();
}

//===========================================
// 情報の設定処理
//===========================================
void CGoal::SetData(const D3DXVECTOR3& pos)
{
	// 全ての値を初期化する
	m_col = GOAL_INIT_COLOR;					// 色
	m_fSubCol = GOAL_ADD_ALPHA;					// 透明度の追加量

	// スクロールの設定処理
	SetPos(pos);								// 位置設定
	SetPosOld(pos);								// 前回の位置設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 向き設定
	SetSize(GOAL_SIZE);							// サイズ設定

	// 頂点情報の設定処理
	SetVertex();

	// 頂点カラーの設定処理
	SetVtxColor(m_col);

	// テクスチャ座標の設定処理(縦の一定間隔テクスチャ)
	SetVtxTextureHeight(D3DXVECTOR2(GetSize().x, GetSize().y));

	// テクスチャの読み込み処理
	BindTexture(CManager::GetTexture()->Regist(GOAL_TEXTURE));
}

//===========================================
// 透明度の変化
//===========================================
void CGoal::AlphaChange(void)
{
	// 透明度を減算する
	m_col.a -= m_fSubCol;

	if (m_col.a >= GOAL_ALPHA_MAX)
	{ // 最大値を下回った場合

		// 透明度を補正する
		m_col.a = GOAL_ALPHA_MAX;

		// 透明度の減算量に -1 をかける
		m_fSubCol *= -1;
	}
	else if (m_col.a <= GOAL_ALPHA_MIN)
	{ // 最小値を下回った場合

		// 透明度を補正する
		m_col.a = GOAL_ALPHA_MIN;

		// 透明度の減算量に -1 をかける
		m_fSubCol *= -1;
	}

	// 頂点カラーの設定処理
	SetVtxColor(m_col);
}

//===========================================
// 生成処理
//===========================================
CGoal* CGoal::Create(const D3DXVECTOR3& pos, const CScene::MODE mode)
{
	// ローカルオブジェクトを生成
	CGoal* pGoal = nullptr;	// フィールドのインスタンスを生成

	if (pGoal == nullptr)
	{ // オブジェクトが NULL の場合

		switch (mode)
		{
		case CScene::MODE_TUTORIAL:	// チュートリアルの場合

			// オブジェクトを生成
			pGoal = new CTutorialGoal;

			break;

		case CScene::MODE_GAME:		// ゲームモードの場合

			// オブジェクトを生成
			pGoal = new CGameGoal;

			break;

		default:					// 上記以外

			// NULL を返す
			return nullptr;

			break;
		}
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pGoal != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pGoal->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "ゴールの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pGoal->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// ゴールのポインタを返す
	return pGoal;
}