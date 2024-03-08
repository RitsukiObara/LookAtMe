//===========================================
//
// 岩のメイン処理[rock.cpp]
// Author 小原立暉
//
//===========================================
//*******************************************
// インクルードファイル
//*******************************************
#include "main.h"
#include "manager.h"
#include "rock.h"
#include "texture.h"
#include "sound.h"
#include "useful.h"

#include "fraction.h"

//-------------------------------------------
// 定数定義
//-------------------------------------------
namespace
{
	const char* MODEL[CRock::TYPE_MAX] =		// モデルの名前
	{
		"data\\MODEL\\Rock001.x",				// 茶色岩
		"data\\MODEL\\Rock002.x",				// 灰色岩
		"data\\MODEL\\Rock001.x",				// 壊れる岩
	};
	const char* BREAKTEXTURE = "data\\TEXTURE\\Rock002.png";	// 破壊できる岩のテクスチャ
	const int TEXTURE_IDX = 0;					// 変わるテクスチャのインデックス
	const int NUM_FRACTION = 10;				// 破壊の時の破片の数
	const int FRACTION_MOVE_WIDTH = 20;			// 破片の幅
	const int FRACTION_MOVE_HEIGHT = 10;		// 破片の高さ
	const int FRACTION_LIFE = 60;				// 破片の寿命
}

//-------------------------------------------
// 静的メンバ変数宣言
//-------------------------------------------
CListManager<CRock*> CRock::m_aList[area::NUM_AREA] = {};		// リスト

//==============================
// コンストラクタ
//==============================
CRock::CRock() : CModel(TYPE_ROCK, PRIORITY_BLOCK)
{
	// 全ての値をクリアする
	m_type = TYPE_BROWN;		// 種類
	m_fRadius = 0.0f;			// 半径
	m_fTop = 0.0f;				// 上の高さ
	m_fBottom = 0.0f;			// 下の高さ
	m_nFieldIdx = 0;			// 区分の番号
}

//==============================
// デストラクタ
//==============================
CRock::~CRock()
{

}

//==============================
//ブロックの初期化処理
//==============================
HRESULT CRock::Init(void)
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
void CRock::Uninit(void)
{
	// 引き抜き処理
	m_aList[m_nFieldIdx].Pull(this);

	// 終了処理
	CModel::Uninit();
}

//========================================
//ブロックの更新処理
//========================================
void CRock::Update(void)
{

}

//=====================================
//ブロックの描画処理
//=====================================
void CRock::Draw(void)
{
	// 描画処理
	CModel::Draw();
}

//=====================================
// 情報の設定処理
//=====================================
void CRock::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const TYPE type)
{
	// 情報の設定処理
	SetPos(pos);					// 位置
	SetPosOld(pos);					// 前回の位置
	SetRot(rot);					// 向き
	SetScale(scale);				// 拡大率
	SetFileData(CManager::Get()->GetXFile()->Regist(MODEL[type]));	// モデルの情報

	// 全ての値を設定する
	m_type = type;				// 種類

	if (m_type == TYPE_BREAK)
	{ // 壊れる岩の場合

		// テクスチャの割り当て処理
		BindTexture(CManager::Get()->GetTexture()->Regist(BREAKTEXTURE), TEXTURE_IDX);
	}

	// 半径を設定する
	m_fRadius = GetFileData().fRadius * ((scale.x + scale.z) * 0.5f);

	// 高さを設定する
	m_fTop = GetFileData().vtxMax.y * scale.y;
	m_fBottom = GetFileData().vtxMin.y * scale.y;

	// 区分の設定処理
	m_nFieldIdx = area::SetFieldIdx(GetPos());

	// リストに追加する
	m_aList[m_nFieldIdx].Regist(this);
}

//=====================================
// 破壊処理
//=====================================
void CRock::Break(void)
{
	// 終了処理
	Uninit();

	D3DXVECTOR3 pos = GetPos();		// 岩の位置
	D3DXVECTOR3 posFrac;			// 破片の位置
	int nRadius = (int)(m_fRadius * 0.5f);	// 出る範囲(XZ面)
	int nHeight = (int)(m_fTop * 0.5f);		// 出る範囲(Y軸)

	for (int nCnt = 0; nCnt < NUM_FRACTION; nCnt++)
	{
		// 破片の位置を設定する
		posFrac.x = pos.x + (float)(rand() % nRadius - (nRadius / 2));
		posFrac.y = pos.y + (float)(rand() % nHeight - (nHeight / 2));
		posFrac.z = pos.z + (float)(rand() % nRadius - (nRadius / 2));

		// 破片を生成
		CFraction::Create(posFrac, CFraction::TYPE_ROCK, FRACTION_LIFE, FRACTION_MOVE_WIDTH, FRACTION_MOVE_HEIGHT);
	}

	// 岩の破壊音を鳴らす
	CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_ROCKBREAK);
}

//=======================================
// 生成処理
//=======================================
CRock* CRock::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const TYPE type)
{
	// ローカルオブジェクトを生成
	CRock* pRock = nullptr;		// インスタンスを生成

	if (pRock == nullptr)
	{ // オブジェクトが NULL の場合

		// 岩を生成する
		pRock = new CRock;
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	if (pRock != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pRock->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pRock->SetData(pos, rot, scale, type);
	}
	else
	{ // オブジェクトが NULL の場合

		// 停止
		assert(false);

		// NULL を返す
		return nullptr;
	}

	// 岩のポインタを返す
	return pRock;
}

//=======================================
// リストの取得処理
//=======================================
CListManager<CRock*> CRock::GetList(const int nIdx)
{
	// リストマネージャーを返す
	return m_aList[nIdx];
}

//=======================================
// 種類の取得処理
//=======================================
CRock::TYPE CRock::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//=======================================
// 半径の取得処理
//=======================================
float CRock::GetRadius(void) const
{
	// 半径を返す
	return m_fRadius;
}

//=======================================
// 上の高さの取得処理
//=======================================
float CRock::GetTopHeight(void) const
{
	// 上の高さを返す
	return m_fTop;
}

//=======================================
// 下の高さの取得処理
//=======================================
float CRock::GetBottomHeight(void) const
{
	// 下の高さを返す
	return m_fBottom;
}