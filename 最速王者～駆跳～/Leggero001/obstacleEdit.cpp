//===========================================
//
//オブジェクトのメイン処理[EditObject.cpp]
//Author 小原立暉
//
//===========================================
#ifdef _DEBUG	// デバッグ処理

#include "main.h"
#include "model.h"
#include "obstacleEdit.h"
#include "input.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "debugproc.h"

#include "player.h"

// マクロ定義
#define EDITOBSTACLE_DELETE_OBJECT_RANGE	(70.0f)		// オブジェクト消去の範囲
#define EDITOBSTACLE_ADJUSTMENT_MOVE		(4.0f)		// 調整用の移動量
#define EDITOBSTACLE_MOVE					(16.0f)		// 通常の移動量
#define EDITOBSTACLE_ROT_MOVE				(0.02f)		// 向きの移動量
#define EDITOBSTACLE_UPDOWN_MOVE			(6.0f)		// 上下移動の移動量
#define EDITOBSTACLE_UPDOWN_ADJUSTMENT_MOVE	(2.0f)		// 調整用の上下移動の移動量
#define EDITOBSTACLE_STREET_LAMP_POS_Z		(70.0f)		// 街灯のZ座標

//=======================================
// コンストラクタ
//=======================================
CObstacleEdit::CObstacleEdit() : CObstacle(CObject::TYPE_EDIT, CObject::PRIORITY_UI)
{

}

//=======================================
// デストラクタ
//=======================================
CObstacleEdit::~CObstacleEdit()
{

}

//=======================================
// 初期化処理
//=======================================
HRESULT CObstacleEdit::Init(void)
{
	// 初期化処理
	if (FAILED(CObstacle::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//=======================================
// 終了処理
//=======================================
void CObstacleEdit::Uninit(void)
{
	// 終了処理
	CObstacle::Uninit();
}

//=======================================
// 更新処理
//=======================================
void CObstacleEdit::Update(void)
{
	//種類変更処理
	TypeChange();

	//移動処理
	Move();

	//オブジェクトの消去
	Delete();

	//オブジェクトの設定処理
	Set();

	//オブジェクトの情報リセット処理
	Reset();

	//オブジェクトのプレイヤー位置移動
	PlayerPosSet();

	// エディットの操作デバッグ表示
	CManager::GetDebugProc()->Print("LCTRLキー：微調整キー\n1キー：種類替え\n9キー：オブジェクトの削除\n0キー：オブジェクトの設置\n"
		"W/A/S/Dキー：上下左右移動\n左ALTキー：プレイヤーの場所に移動\n");

	// エディットの情報
	CManager::GetDebugProc()->Print("位置：[%f/%f/%f]\n", GetPos().x, GetPos().y, GetPos().z);
}

//=======================================
// 描画処理
//=======================================
void CObstacleEdit::Draw(void)
{
	// エディットの描画処理
	CObstacle::Draw();
}

//=======================================
// 情報の設定処理
//=======================================
void CObstacleEdit::SetData(const D3DXVECTOR3& pos)
{
	// 情報の設定処理
	SetFileData(INIT_OBSTACLE);

	// 情報の設定処理
	CObstacle::SetData(pos,TYPE::TYPE_HYDRANT);
}

//=======================================
// 生成処理
//=======================================
CObstacleEdit* CObstacleEdit::Create(const D3DXVECTOR3& pos)
{
	// ローカルオブジェクトを生成
	CObstacleEdit* pEdit = nullptr;	// プレイヤーのインスタンスを生成

	if (pEdit == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pEdit = new CObstacleEdit;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return nullptr;
	}

	if (pEdit != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pEdit->Init()))
		{ // 初期化に失敗した場合

			// 警告文
			MessageBox(NULL, "障害物エディットの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pEdit->SetData(pos);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// エディットのポインタを返す
	return pEdit;
}

//=======================================
//オブジェクトの消去
//=======================================
void CObstacleEdit::Delete(void)
{
	// ローカル変数宣言
	CObject* pObj = nullptr;		// オブジェクトへのポインタ
	CObstacle* pObstacle = nullptr;		// ブロックへのポインタ

	// 先頭のオブジェクトの情報を取得する
	pObj = CObject::GetTop(PRIORITY_BLOCK);

	while(pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == TYPE_OBSTACLE)
		{ // オブジェクトがブロックだった場合

			// ブロックの情報を取得する
			pObstacle = dynamic_cast<CObstacle*>(pObj);

			if (pObstacle != nullptr)
			{ // ブロックの情報が NULL じゃない場合

				if (GetPos().x >= pObstacle->GetPos().x - EDITOBSTACLE_DELETE_OBJECT_RANGE &&
					GetPos().x <= pObstacle->GetPos().x + EDITOBSTACLE_DELETE_OBJECT_RANGE &&
					GetPos().y >= pObstacle->GetPos().y - EDITOBSTACLE_DELETE_OBJECT_RANGE &&
					GetPos().y <= pObstacle->GetPos().y + EDITOBSTACLE_DELETE_OBJECT_RANGE)
				{ // 範囲内に入った場合

					// 削除対象にする
					pObstacle->SetDelete(true);

					if (CManager::GetInputKeyboard()->GetTrigger(DIK_9) == true)
					{ // 9キーを押した場合

						// 削除する
						pObstacle->Uninit();
					}
				}
				else
				{ // 範囲外だった場合

					// 削除対象を false にする
					pObstacle->SetDelete(true);
				}
			}
		}

		// 次のオブジェクトを代入する
		pObj = pObj->GetNext();
	}
}

//=======================================
//種類変更処理
//=======================================
void CObstacleEdit::TypeChange(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{ // 1キーを押した場合

		// ローカル変数宣言
		CXFile::TYPE fileType;				// Xファイルの種類
		CObstacle::TYPE type = GetType();	// ブロックの種類
		CXFile::SXFile fileData;			// ファイルデータ

		// 設定オブジェクトの種類を選択する
		type = (CObstacle::TYPE)((type + 1) % CObstacle::TYPE_MAX);

		// Xファイルの種類を設定する
		fileType = (CXFile::TYPE)(type + INIT_OBSTACLE);

		// ファイルのデータを取得する
		fileData = CXFile::GetXFile(fileType);

		// 設定する
		SetFileData(fileType);

		// 種類の設定処理
		SetType(type);
	}

	if (GetType() == TYPE_DRUM)
	{ // 種類が「ドラム缶」の場合

		// 向きを設定する
		SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
	}
	else if (GetType() == TYPE_LAMP)
	{ // 種類が「街灯(光源)」の場合

		// 向きを設定する
		SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	}
	else
	{ // 上記以外

		// 向きを設定する
		SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (GetType() == TYPE::TYPE_POLE ||
		GetType() == TYPE::TYPE_LAMP)
	{ // 種類が街灯関係の場合

		// 位置を設定する
		SetPos(D3DXVECTOR3(GetPos().x, GetPos().y, EDITOBSTACLE_STREET_LAMP_POS_Z));
	}
	else
	{ // 上記以外

		// 位置を設定する
		SetPos(D3DXVECTOR3(GetPos().x, GetPos().y, 0.0f));
	}
}

//=======================================
//移動処理
//=======================================
void CObstacleEdit::Move(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置

	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{ // 左シフトキーが押されていた場合

		// 処理を抜ける
		return;
	}

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // 左コントロールキーを押していた場合

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true)
		{ // Wキーを押した場合

			// 位置を奥に進める
			pos.y += EDITOBSTACLE_ADJUSTMENT_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true)
		{ // Sキーを押した場合

			// 位置を手前に進める
			pos.y -= EDITOBSTACLE_ADJUSTMENT_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_A) == true)
		{ // Aキーを押した場合

			// 位置を左に進める
			pos.x -= EDITOBSTACLE_ADJUSTMENT_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_D) == true)
		{ // Dキーを押した場合

			// 位置を右に進める
			pos.x += EDITOBSTACLE_ADJUSTMENT_MOVE;
		}
	}
	else
	{ // 左コントロールを押していない場合

		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true)
		{ // Wキーを押した場合

			// 位置を奥に進める
			pos.y += EDITOBSTACLE_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true)
		{ // Sキーを押した場合

			// 位置を手前に進める
			pos.y -= EDITOBSTACLE_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true)
		{ // Aキーを押した場合

			// 位置を左に進める
			pos.x -= EDITOBSTACLE_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true)
		{ // Dキーを押した場合

			// 位置を右に進める
			pos.x += EDITOBSTACLE_MOVE;
		}
	}

	// 位置の設定処理
	SetPos(pos);
}

//=======================================
//オブジェクトの設定処理
//=======================================
void CObstacleEdit::Set(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置

	// 位置を切り捨てする
	pos.x = floorf(pos.x);
	pos.y = floorf(pos.y);

	// 位置の設定処理
	SetPos(pos);

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{ // 0キーを押した場合

		// ブロックの設定処理
		CObstacle::Create(GetPos(), GetType());
	}
}

//=======================================
//オブジェクトの情報リセット処理
//=======================================
void CObstacleEdit::Reset(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_2) == true)
	{ // 2キーを押した場合

		// 向きを取得する
		D3DXVECTOR3 rot = GetRot();

		// 角度を初期化する
		rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 向きの設定処理
		SetRot(rot);
	}
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_3) == true)
	{ // 3キーを押した場合

		// 拡大率を取得する
		D3DXVECTOR3 scale = GetScale();

		// 拡大率を初期化する
		scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

		// 拡大率の取得処理
		SetScale(scale);
	}
}

//=======================================
//オブジェクトのプレイヤー位置移動
//=======================================
void CObstacleEdit::PlayerPosSet(void)
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

#endif