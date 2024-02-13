//============================================
//
// カメラの範囲のメイン処理[camera_range.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "input.h"
#include "cameraEdit.h"
#include "player.h"
#include "game.h"
#include "debugproc.h"
#include "useful.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define MOVE_NORMAL		(16.0f)			// 通常移動量
#define MOVE_ADJUST		(1.0f)			// 微調整用移動量
#define SCALE_NORMAL	(8.0f)			// 通常拡縮率
#define SCALE_ADJUST	(1.0f)			// 微調整用拡縮率

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
const char* CCameraEdit::c_apTypeName[CCamera::TYPE_STARTSTAGE] =			// 種類の名前
{
	nullptr,				// 通常カメラ(例外)
	"上向きカメラ",			// 上向きカメラ
	"先行追跡カメラ",		// 先行追跡カメラ
	"ズームアウトカメラ",	// ズームアウトカメラ
	"ズームインカメラ",		// ズームインカメラ
};

//===========================================
// コンストラクタ
//===========================================
CCameraEdit::CCameraEdit() : CCameraRange(TYPE_EDIT, CObject::PRIORITY_BG)
{
	// コンストラクタの箱
	Box();
}

//===========================================
// オーバーロードコンストラクタ
//===========================================
CCameraEdit::CCameraEdit(CObject::TYPE type, PRIORITY priority) : CCameraRange(type, priority)
{
	// コンストラクタの箱
	Box();
}

//===========================================
// デストラクタ
//===========================================
CCameraEdit::~CCameraEdit()
{

}

//===========================================
// コンストラクタの箱
//===========================================
void CCameraEdit::Box(void)
{

}

//===========================================
// 初期化処理
//===========================================
HRESULT CCameraEdit::Init(void)
{
	if (FAILED(CCameraRange::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//===========================================
// 終了処理
//===========================================
void CCameraEdit::Uninit(void)
{
	// 終了
	CCameraRange::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CCameraEdit::Update(void)
{
	// ローカル変数を宣言
	D3DXVECTOR3 pos = GetPos();			// 弾の位置

	// 前回の位置の設定処理
	SetPosOld(pos);

	// 種類変更処理
	TypeChange();

	// 移動処理
	Move();

	// 拡大縮小処理(X軸)
	ScaleX();

	// 拡大縮小処理(Y軸)
	ScaleY();

	// 拡大縮小処理
	Scale();

	// 消去処理
	Delete();

	// プレイヤー位置移動処理
	PlayerPosSet();

	// 頂点の設定処理
	SetVertex();

	// 設定処理
	Set();

	// エディットの操作デバッグ表示
	CManager::GetDebugProc()->Print("LCTRLキー：微調整キー\n1キー：種類替え\n2キー：全体拡大\n3キー：全体縮小\n6キー：スタイル変更\n9キー：オブジェクトの削除\n0キー：オブジェクトの設置\n"
		"W/A/S/Dキー：上下左右移動\nU/Jキー：X軸拡大縮小\nI/Kキー：Y軸拡大縮小\n左ALTキー：プレイヤーの場所に移動\n");

	// エディットの情報
	CManager::GetDebugProc()->Print("位置：[%f/%f/%f]\nサイズ：[%f/%f/%f]\n種類：[%s]\n",
		GetPos().x, GetPos().y, GetPos().z, GetSize().x, GetSize().y, GetSize().z, c_apTypeName[GetType()]);
}

//===========================================
// 描画処理
//===========================================
void CCameraEdit::Draw(void)
{
// デバッグ用
#ifdef _DEBUG

	// 描画処理
	CCameraRange::Draw();

#endif

}

//===========================================
// 生成処理
//===========================================
CCameraEdit* CCameraEdit::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& size, CCamera::TYPE type)
{
	// ローカルオブジェクトを生成
	CCameraEdit* pCameraRange = nullptr;	// プレイヤーのインスタンスを生成

	if (pCameraRange == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pCameraRange = new CCameraEdit;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pCameraRange != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pCameraRange->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "カメラの範囲のエディットの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pCameraRange->SetData(pos, size, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// カメラの範囲のポインタを返す
	return pCameraRange;
}

//===========================================
// 種類変更処理
//===========================================
void CCameraEdit::TypeChange(void)
{
	// ローカル変数宣言
	CCamera::TYPE type;		// 種類

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{ // 1キーを押した場合

		// 種類を取得する
		type = GetType();

		// 種類を変える
		type = (CCamera::TYPE)((type + 1) % CCamera::TYPE_STARTSTAGE);

		// 種類の設定処理
		SetType(type);
	}

	if (GetType() == CCamera::TYPE_NONE)
	{ // 種類が 通常カメラ の場合

		// 種類を取得する
		type = GetType();

		// 種類を変更する
		type = (CCamera::TYPE)(type + 1);

		//　種類を更新する
		SetType(type);
	}
}

//===========================================
// 移動処理
//===========================================
void CCameraEdit::Move(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // 左コントロールキーを押していた場合

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true)
		{ // Wキーを押した場合

			// 位置を奥に進める
			pos.y += MOVE_ADJUST;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true)
		{ // Sキーを押した場合

			// 位置を手前に進める
			pos.y -= MOVE_ADJUST;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_A) == true)
		{ // Aキーを押した場合

			// 位置を左に進める
			pos.x -= MOVE_ADJUST;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_D) == true)
		{ // Dキーを押した場合

			// 位置を右に進める
			pos.x += MOVE_ADJUST;
		}
	}
	else
	{ // 左コントロールを押していない場合

		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true)
		{ // Wキーを押した場合

			// 位置を奥に進める
			pos.y += MOVE_NORMAL;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true)
		{ // Sキーを押した場合

			// 位置を手前に進める
			pos.y -= MOVE_NORMAL;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true)
		{ // Aキーを押した場合

			// 位置を左に進める
			pos.x -= MOVE_NORMAL;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true)
		{ // Dキーを押した場合

			// 位置を右に進める
			pos.x += MOVE_NORMAL;
		}
	}

	// 位置の設定処理
	SetPos(pos);
}

//===========================================
// 拡大縮小処理(X軸)
//===========================================
void CCameraEdit::ScaleX(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = GetSize();			// 拡大率を取得する

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // 左コントロールキーを押していた場合

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{ // Uキーを押した場合

			// X軸を拡大する
			size.x += SCALE_ADJUST;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_J) == true)
		{ // Jキーを押した場合

			// X軸を縮小する
			size.x -= SCALE_ADJUST;
		}
	}
	else
	{ // 左コントロールキーを押していない場合

		if (CManager::GetInputKeyboard()->GetPress(DIK_U) == true)
		{ // Uキーを押した場合

			// X軸を拡大する
			size.x += SCALE_NORMAL;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_J) == true)
		{ // Jキーを押した場合

			// X軸を縮小する
			size.x -= SCALE_NORMAL;
		}
	}

	// 拡大率の設定処理
	SetSize(size);
}

//===========================================
// 拡大縮小処理(Y軸)
//===========================================
void CCameraEdit::ScaleY(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = GetSize();			// 拡大率を取得する

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // 左コントロールキーを押していた場合

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
		{ // Iキーを押した場合

			// Y軸を拡大する
			size.y += SCALE_ADJUST;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_K) == true)
		{ // Kキーを押した場合

			// Y軸を縮小する
			size.y -= SCALE_ADJUST;
		}
	}
	else
	{ // 左コントロールキーを押していない場合

		if (CManager::GetInputKeyboard()->GetPress(DIK_I) == true)
		{ // Iキーを押した場合

			// Y軸を拡大する
			size.y += SCALE_NORMAL;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_K) == true)
		{ // Kキーを押した場合

			// Y軸を縮小する
			size.y -= SCALE_NORMAL;
		}
	}

	// 拡大率の設定処理
	SetSize(size);
}

//===========================================
// 拡大縮小処理
//===========================================
void CCameraEdit::Scale(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 size = GetSize();			// 拡大率を取得する

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // 左コントロールキーを押していた場合

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
		{ // 4キーを押した場合

			// 拡大する
			size.x += SCALE_ADJUST;
			size.y += SCALE_ADJUST;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
		{ // 5キーを押した場合

			// 縮小する
			size.x -= SCALE_ADJUST;
			size.y -= SCALE_ADJUST;
		}
	}
	else
	{ // 左コントロールキーを押していない場合

		if (CManager::GetInputKeyboard()->GetPress(DIK_2) == true)
		{ // 4キーを押した場合

			// 拡大する
			size.x += SCALE_NORMAL;
			size.y += SCALE_NORMAL;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_3) == true)
		{ // 5キーを押した場合

			// 縮小する
			size.x -= SCALE_NORMAL;
			size.y -= SCALE_NORMAL;
		}
	}

	// 拡大率の設定処理
	SetSize(size);
}

//===========================================
// オブジェクトの設定処理
//===========================================
void CCameraEdit::Set(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{ // 0キーを押した場合

		// カメラの範囲の設定処理
		CCameraRange::Create(GetPos(), GetSize(), GetType());
	}
}

//===========================================
// オブジェクトの消去
//===========================================
void CCameraEdit::Delete(void)
{
	// ローカル変数宣言
	CObject* pObj = nullptr;		// オブジェクトへのポインタ
	CCameraRange* pCamera = nullptr;		// ブロックへのポインタ

	// 先頭のオブジェクトの情報を取得する
	pObj = CObject::GetTop(PRIORITY_BG);

	while(pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == TYPE_CAMERARANGE)
		{ // オブジェクトがカメラの範囲だった場合

			// ブロックの情報を取得する
			pCamera = dynamic_cast<CCameraRange*>(pObj);

			if (pCamera != nullptr)
			{ // ブロックの情報が NULL じゃない場合

				if (RectangleCollisionXY
				(
					GetPos(),				// 位置
					pCamera->GetPos(),		// カメラの範囲の位置
					GetSize(),				// 最大値
					pCamera->GetSize(),		// カメラの範囲の最大値
					-GetSize(),				// 最小値
					-pCamera->GetSize()		// カメラの範囲の最小値
				) == true)
				{ // 矩形の当たり判定が true だった場合

					if (CManager::GetInputKeyboard()->GetTrigger(DIK_9) == true)
					{ // 9キーを押した場合

						// 削除する
						pCamera->Uninit();
					}
				}
			}
		}

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}
}

//===========================================
// オブジェクトのプレイヤー位置移動
//===========================================
void CCameraEdit::PlayerPosSet(void)
{
	// ローカル変数宣言
	CPlayer* pPlayer = nullptr;	// プレイヤーへのポインタ

	// プレイヤーの情報を取得する
	pPlayer = CScene::GetPlayer();

	if (pPlayer != nullptr &&
		CManager::GetInputKeyboard()->GetTrigger(DIK_LMENU) == true)
	{ // 左の ALTキーが押された場合

		// プレイヤーの位置に位置を移動
		SetPos(pPlayer->GetPos());
	}
}