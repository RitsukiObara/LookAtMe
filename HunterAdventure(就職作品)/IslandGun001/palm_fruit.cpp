//===========================================
//
// ヤシの実のメイン処理[palm_fruit.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "palm_fruit.h"
#include "renderer.h"
#include "useful.h"

#include "objectElevation.h"

//-------------------------------------------
// 無名名前空間
//-------------------------------------------
namespace
{
	const char* FRUIT_MODEL = "data\\MODEL\\PalmFruit.x";		// ヤシの実のモデル
	const char* ITEM_MODEL = "data\\MODEL\\PalmItem.x";			// ヤシの実(アイテム体)のモデル
	const float GRAVITY = 0.5f;			// 重力
	const float BOUND_GRAVITY = 10.0f;	// バウンドしたときのY軸の移動量
	const float BOUND_MOVE = 4.4f;		// バウンドしたときの移動量
	const float BOUND_MOVE_ROT = 0.6f;	// バウンドしたときの移動量の向き
}

//-------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------
CListManager<CPalmFruit*> CPalmFruit::m_list = {};		// リスト

//==============================
// コンストラクタ
//==============================
CPalmFruit::CPalmFruit() : CModel(CObject::TYPE_PALMFRUIT, CObject::PRIORITY_ENTITY)
{
	// 全ての値をクリアする
	m_move = NONE_D3DXVECTOR3;	// 移動量
	m_state = STATE_NONE;		// 状態

	// リストに追加する
	m_list.Regist(this);
}

//==============================
// デストラクタ
//==============================
CPalmFruit::~CPalmFruit()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CPalmFruit::Init(void)
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
void CPalmFruit::Uninit(void)
{
	// 終了処理
	CModel::Uninit();

	// 引き抜き処理
	m_list.Pull(this);
}

//========================================
//ブロックの更新処理
//========================================
void CPalmFruit::Update(void)
{
	switch (m_state)
	{
	case CPalmFruit::STATE_NONE:

		// 起伏地面の当たり判定
		ElevationCollision();

		break;

	case CPalmFruit::STATE_FALL:

		// 重力処理
		Gravity();

		if (ElevationCollision() == true)
		{ // 起伏の地面に付いた場合

			// バウンド状態にする
			m_state = STATE_BOUND;

			// 移動量を設定する
			m_move.x = sinf(GetRot().y + BOUND_MOVE_ROT) * BOUND_MOVE;
			m_move.y = BOUND_GRAVITY;
			m_move.z = cosf(GetRot().y + BOUND_MOVE_ROT) * BOUND_MOVE;
		}

		break;

	case CPalmFruit::STATE_BOUND:

		// 移動処理
		Move();

		// 重力処理
		Gravity();

		if (ElevationCollision() == true)
		{ // 起伏の地面に付いた場合

			// 停止状態にする
			m_state = STATE_STOP;

			// 重力を0.0fにする
			m_move.y = 0.0f;
		}

		break;

	case CPalmFruit::STATE_STOP:



		break;

	case CPalmFruit::STATE_GET:

		break;

	default:

		// 停止
		assert(false);

		break;
	}
}

//=====================================
//ブロックの描画処理
//=====================================
void CPalmFruit::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CPalmFruit::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// 情報の設定処理
	SetPos(pos);							// 位置
	SetPosOld(pos);							// 前回の位置
	SetRot(rot);							// 向き
	SetScale(NONE_SCALE);					// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(FRUIT_MODEL));	// モデルの情報

	// 全ての値を設定する
	m_move = NONE_D3DXVECTOR3;	// 移動量
	m_state = STATE_NONE;		// 状態
}

//=======================================
// 生成処理
//=======================================
CPalmFruit* CPalmFruit::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
	// ローカルオブジェクトを生成
	CPalmFruit* pFruit = nullptr;	// インスタンスを生成

	if (pFruit == nullptr)
	{ // オブジェクトが NULL の場合

		// インスタンスを生成
		pFruit = new CPalmFruit;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pFruit != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pFruit->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pFruit->SetData(pos, rot);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// ヤシの実のポインタを返す
	return pFruit;
}

//=======================================
// リストの取得処理
//=======================================
CListManager<CPalmFruit*> CPalmFruit::GetList(void)
{
	// リストマネージャーを返す
	return m_list;
}

//=======================================
// 移動量の設定処理
//=======================================
void CPalmFruit::SetMove(const D3DXVECTOR3& move)
{
	// 移動量を設定する
	m_move = move;
}

//=======================================
// 移動量の取得処理
//=======================================
D3DXVECTOR3 CPalmFruit::GetMove(void) const
{
	// 移動量を返す
	return m_move;
}

//=======================================
// 状態の設定処理
//=======================================
void CPalmFruit::SetState(const STATE state)
{
	// 状態を設定する
	m_state = state;
}

//=======================================
// 状態の取得処理
//=======================================
CPalmFruit::STATE CPalmFruit::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//=======================================
// ヒット処理
//=======================================
void CPalmFruit::Hit(void)
{
	// モデルをヤシの実(アイテム型)にする
	SetFileData(CManager::Get()->GetXFile()->Regist(ITEM_MODEL));
}

//=======================================
// 移動処理
//=======================================
void CPalmFruit::Move(void)
{
	// 位置を取得する
	D3DXVECTOR3 pos = GetPos();

	// 移動させる
	pos.x += m_move.x;
	pos.z += m_move.z;

	// 位置を適用する
	SetPos(pos);
}

//=======================================
// 重力処理
//=======================================
void CPalmFruit::Gravity(void)
{
	// 位置を取得する
	D3DXVECTOR3 pos = GetPos();

	// 重力処理
	useful::Gravity(&m_move.y, &pos.y, GRAVITY);

	// 位置を適用する
	SetPos(pos);
}

//=======================================
// 起伏地面の当たり判定処理
//=======================================
bool CPalmFruit::ElevationCollision(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	float fHeight = 0.0f;			// 高さ
	CListManager<CElevation*> list = CElevation::GetList();
	CElevation* pElev = nullptr;			// 先頭の小判
	CElevation* pElevEnd = nullptr;		// 末尾の値
	int nIdx = 0;

	// while文処理
	if (list.IsEmpty() == false)
	{ // 空白じゃない場合

		// 先頭の値を取得する
		pElev = list.GetTop();

		// 末尾の値を取得する
		pElevEnd = list.GetEnd();

		while (true)
		{ // 無限ループ

			// 当たり判定を取る
			fHeight = pElev->ElevationCollision(pos);

			if (pos.y < fHeight)
			{ // 当たり判定の位置が高かった場合

				// 高さを設定する
				pos.y = fHeight;

				// 重力を設定する
				m_move.y = 0.0f;

				// 位置を更新する
				SetPos(pos);

				// true を返す
				return true;
			}

			if (pElev == pElevEnd)
			{ // 末尾に達した場合

				// while文を抜け出す
				break;
			}

			// 次のオブジェクトを代入する
			pElev = list.GetData(nIdx + 1);

			// インデックスを加算する
			nIdx++;
		}
	}

	// false を返す
	return false;
}