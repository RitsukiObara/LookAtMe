//=======================================
//
// 範囲移動のメイン処理[range_move.cpp]
// Author：小原立暉
//
//=======================================
#include "range_move.h"
#include "input.h"
#include "manager.h"
#include "model.h"
#include "debugproc.h"
#include "renderer.h"
#include "useful.h"

//---------------------------------------
// マクロ定義
//---------------------------------------
#define RANGEMOVE_SIZE		(D3DXVECTOR3(20.0f, 20.0f, 0.0f))		// 初期サイズ
#define MOVE_NORMAL			(16.0f)									// 通常の移動量
#define MOVE_ADJUSTMENT		(2.0f)									// 微調整用の移動量
#define SCALING_NORMAL		(8.0f)									// 通常のサイズの拡縮率
#define SCALING_ADJUSTMENT	(1.0f)									// 微調整用のサイズの拡縮率

//==========================================
// コンストラクタ
//==========================================
CRangeMove::CRangeMove() : CObject3D(CObject::TYPE_RANGEMOVE, CObject::PRIORITY_BG)
{
	// 全ての値をクリアする
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_state = STATE_ADJUST;						// 状態
}

//==========================================
// デストラクタ
//==========================================
CRangeMove::~CRangeMove()
{

}

//==========================================
// ゴールの初期化処理
//==========================================
HRESULT CRangeMove::Init(void)
{
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	m_state = STATE_ADJUST;						// 情報

	// 成功を返す
	return S_OK;
}

//========================================
// ゴールの終了処理
//========================================
void CRangeMove::Uninit(void)
{
	// 終了処理
	CObject3D::Uninit();
}

//========================================
// ゴールの更新処理
//========================================
void CRangeMove::Update(void)
{
	// 移動処理
	Move();

	// 状態の切り替え処理
	StateChange();

	switch (m_state)
	{
	case STATE_ADJUST:		// サイズ調整状態

		// サイズ(X軸)の設定処理
		SizeX();

		// サイズ(Y軸)の設定処理
		SizeY();

		// サイズ(全体)の設定処理
		Size();

		// 頂点カラーの設定処理
		SetVtxColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		break;

	case STATE_MOVE:		// 移動状態

		// 範囲移動処理
		RangeMove();

		// 頂点カラーの設定処理
		SetVtxColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		break;
	}

	// 頂点情報の設定処理
	SetVertex();

	// エディットの操作デバッグ表示
	CManager::GetDebugProc()->Print("LCTRLキー：微調整キー\n1キー：状態替え(白：範囲設定　赤：移動)\n4キー：全体拡大\n5キー：全体縮小\n"
		"6キー：スタイル変更\nW/A/S/Dキー：上下左右移動\nU/Jキー：X軸拡大縮小\nI/Kキー：Y軸拡大縮小\n");

	// エディットの情報
	CManager::GetDebugProc()->Print("位置：[%f/%f/%f]\nサイズ：[%f/%f/%f]\n状態：[%d](0:サイズ調整/1:移動)\n",
		GetPos().x, GetPos().y, GetPos().z, GetSize().x, GetSize().y, GetSize().z, m_state);
}

//===========================================
// ゴールの描画処理
//===========================================
void CRangeMove::Draw(void)
{
	// 描画処理
	CObject3D::Draw();
}

//===========================================
// 情報の設定処理
//===========================================
void CRangeMove::SetData(const D3DXVECTOR3& pos)
{
	// スクロールの設定処理
	SetPos(pos);								// 位置設定
	SetPosOld(pos);								// 前回の位置設定
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// 向き設定
	SetSize(RANGEMOVE_SIZE);					// サイズ設定

	// 頂点情報の設定処理
	SetVertex();
}

//===========================================
// 移動処理
//===========================================
void CRangeMove::Move(void)
{
	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // 左コントロールキーを押していた場合

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true)
		{ // Wキーを押した場合

			// 位置を奥に進める
			m_move.y = MOVE_ADJUSTMENT;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true)
		{ // Sキーを押した場合

			// 位置を手前に進める
			m_move.y = -MOVE_ADJUSTMENT;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_A) == true)
		{ // Aキーを押した場合

			// 位置を左に進める
			m_move.x = -MOVE_ADJUSTMENT;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_D) == true)
		{ // Dキーを押した場合

			// 位置を右に進める
			m_move.x = MOVE_ADJUSTMENT;
		}
		else
		{ // 上記以外

			// 移動量を設定する
			m_move.x = 0.0f;
			m_move.y = 0.0f;
		}
	}
	else
	{ // 左コントロールを押していない場合

		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true)
		{ // Wキーを押した場合

			// 位置を奥に進める
			m_move.y = MOVE_NORMAL;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true)
		{ // Sキーを押した場合

			// 位置を手前に進める
			m_move.y = -MOVE_NORMAL;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true)
		{ // Aキーを押した場合

			// 位置を左に進める
			m_move.x = -MOVE_NORMAL;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true)
		{ // Dキーを押した場合

			// 位置を右に進める
			m_move.x = MOVE_NORMAL;
		}
		else
		{ // 上記以外

			// 移動量を設定する
			m_move.x = 0.0f;
			m_move.y = 0.0f;
		}
	}

	// 位置移動処理
	Pos();
}

//===========================================
// 位置移動処理
//===========================================
void CRangeMove::Pos(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();			// 位置

	// 移動する
	pos += m_move;

	// 位置を切り捨てする
	pos.x = floorf(pos.x);
	pos.y = floorf(pos.y);

	// 位置の設定処理
	SetPos(pos);

	// 位置の設定処理
	SetPos(pos);
}

//===========================================
// 状態の切り替え処理
//===========================================
void CRangeMove::StateChange(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{ // 1キーを押した場合

		// 状態を切り替える
		m_state = (STATE)((m_state + 1) % STATE_MAX);
	}
}

//===========================================
// サイズ(X軸)の調整処理
//===========================================
void CRangeMove::SizeX(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = GetSize();			// サイズを取得する

	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{ // 左SHIFTキーを押している場合

		// この先の処理を行わない
		return;
	}

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // 左コントロールキーを押していた場合

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{ // Uキーを押した場合

			// X軸を拡大する
			size.x += SCALING_ADJUSTMENT;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_J) == true)
		{ // Jキーを押した場合

			// X軸を縮小する
			size.x -= SCALING_ADJUSTMENT;
		}
	}
	else
	{ // 左コントロールキーを押していない場合

		if (CManager::GetInputKeyboard()->GetPress(DIK_U) == true)
		{ // Uキーを押した場合

			// X軸を拡大する
			size.x += SCALING_NORMAL;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_J) == true)
		{ // Jキーを押した場合

			// X軸を縮小する
			size.x -= SCALING_NORMAL;
		}
	}

	// 拡大率の設定処理
	SetSize(size);
}

//===========================================
// サイズ(Y軸)の調整処理
//===========================================
void CRangeMove::SizeY(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = GetSize();			// 拡大率を取得する

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // 左コントロールキーを押していた場合

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
		{ // Iキーを押した場合

			// Y軸を拡大する
			size.y += SCALING_ADJUSTMENT;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_K) == true)
		{ // Kキーを押した場合

			// Y軸を縮小する
			size.y -= SCALING_ADJUSTMENT;
		}
	}
	else
	{ // 左コントロールキーを押していない場合

		if (CManager::GetInputKeyboard()->GetPress(DIK_I) == true)
		{ // Iキーを押した場合

			// Y軸を拡大する
			size.y += SCALING_NORMAL;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_K) == true)
		{ // Kキーを押した場合

			// Y軸を縮小する
			size.y -= SCALING_NORMAL;
		}
	}

	// 拡大率の設定処理
	SetSize(size);
}

//===========================================
// サイズ(全体)の調整処理
//===========================================
void CRangeMove::Size(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = GetSize();			// 拡大率を取得する

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // 左コントロールキーを押していた場合

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_4) == true)
		{ // 4キーを押した場合

			// 拡大する
			size.x += SCALING_ADJUSTMENT;
			size.y += SCALING_ADJUSTMENT;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_5) == true)
		{ // 5キーを押した場合

			// 縮小する
			size.x -= SCALING_ADJUSTMENT;
			size.y -= SCALING_ADJUSTMENT;
		}
	}
	else
	{ // 左コントロールキーを押していない場合

		if (CManager::GetInputKeyboard()->GetPress(DIK_4) == true)
		{ // 4キーを押した場合

			// 拡大する
			size.x += SCALING_NORMAL;
			size.y += SCALING_NORMAL;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_5) == true)
		{ // 5キーを押した場合

			// 縮小する
			size.x -= SCALING_NORMAL;
			size.y -= SCALING_NORMAL;
		}
	}

	// 拡大率の設定処理
	SetSize(size);
}

//===========================================
// 範囲の移動処理
//===========================================
void CRangeMove::RangeMove(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 posTarget;			// ターゲットの位置
	D3DXVECTOR3 VtxMax;				// 頂点の最大値
	D3DXVECTOR3 VtxMin;				// 頂点の最小値
	CObject* pObj = nullptr;		// 現在のオブジェクトのポインタ
	CObject* pObjNext = nullptr;	// 次のオブジェクトのポインタ
	CObject3D* pObj3D = nullptr;	// 3Dオブジェクトのポインタ
	CModel* pModel = nullptr;		// モデルのポインタ

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		// オブジェクトを代入する
		pObj = GetTop((CObject::PRIORITY)(nCntPriority));

		while (pObj != nullptr)
		{ // オブジェクトが NULL じゃない限り回す

			// 次のオブジェクトを代入する
			pObjNext = pObj->GetNext();

			if (pObj->GetType() == CObject::TYPE_BGOBJECT ||
				pObj->GetType() == CObject::TYPE_BLOCK ||
				pObj->GetType() == CObject::TYPE_OBSTACLE ||
				pObj->GetType() == CObject::TYPE_ITEM ||
				pObj->GetType() == CObject::TYPE_ITEMGENERATOR)
			{ // 一定の種類の場合

				// モデルの情報を取得する
				pModel = dynamic_cast<CModel*>(pObj);

				if (pModel != nullptr)
				{ // モデルの情報が NULL じゃない場合

					// 各情報を設定する
					posTarget = pModel->GetPos();				// ターゲットの位置
					VtxMax = pModel->GetFileData().vtxMax;		// 頂点の最大値
					VtxMin = pModel->GetFileData().vtxMin;		// 頂点の最小値

					// 当たり判定処理
					pos = Hit(posTarget, VtxMax, VtxMin);

					// 位置の設定処理
					pModel->SetPos(pos);
				}
			}
			else if(pObj->GetType() == CObject::TYPE_CAMERARANGE)
			{ // 一定の種類の場合

				// 3Dポリゴンの情報を取得する
				pObj3D = dynamic_cast<CObject3D*>(pObj);

				if (pObj3D != nullptr)
				{ // 3Dポリゴンの情報が NULL じゃない場合

					// 各情報を設定する
					posTarget = pObj3D->GetPos();		// ターゲットの位置
					VtxMax = pObj3D->GetSize();			// 頂点の最大値
					VtxMin = -pObj3D->GetSize();		// 頂点の最小値

					// 当たり判定処理
					pos = Hit(posTarget, VtxMax, VtxMin);

					// 位置の設定処理
					pObj3D->SetPos(pos);
				}
			}

			// 次のオブジェクトを代入する
			pObj = pObjNext;
		}
	}
}

//===========================================
// 当たり判定処理
//===========================================
D3DXVECTOR3 CRangeMove::Hit(D3DXVECTOR3& pos, const D3DXVECTOR3& VtxMax, const D3DXVECTOR3& VtxMin)
{
	// ローカル変数宣言
	D3DXVECTOR3 TargetPos = pos;		// 対象の位置

	if (RectangleCollisionXY
	(
		GetPos(),		// 位置
		TargetPos,		// 対象の位置
		GetSize(),		// 最大値
		VtxMax,			// 対象の最大値
		-GetSize(),		// 最小値
		VtxMin			// 対象の最小値
	) == true)
	{ // 矩形の当たり判定に当たった場合

		// 移動する
		TargetPos += m_move;
	}

	// 位置を返す
	return TargetPos;
}

//===========================================
// 生成処理
//===========================================
CRangeMove* CRangeMove::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CRangeMove* pRangeMove = nullptr;	// フィールドのインスタンスを生成

	if (pRangeMove == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pRangeMove = new CRangeMove;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pRangeMove != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pRangeMove->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "ゴールの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pRangeMove->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// ゴールのポインタを返す
	return pRangeMove;
}