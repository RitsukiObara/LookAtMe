//===========================================
//
//オブジェクトのメイン処理[EditObject.cpp]
//Author 小原立暉
//
//===========================================
#ifdef _DEBUG	// デバッグ処理

#include "main.h"
#include "model.h"
#include "bgEdit.h"
#include "bgmodel.h"
#include "input.h"
#include "manager.h"
#include "debugproc.h"
#include "renderer.h"

// マクロ定義
#define EDITBG_SELECT_MATERIAL_ALPHA	(1.0f)		// 選択中のマテリアルの透明度
#define EDITBG_NORMAL_MATERIAL_ALPHA	(0.5f)		// 選択していないマテリアルの透明度
#define EDITBG_DELETE_OBJECT_RANGE		(70.0f)		// オブジェクト消去の範囲
#define EDITBG_ADJUSTMENT_MOVE			(4.0f)		// 調整用の移動量
#define EDITBG_MOVE						(16.0f)		// 通常の移動量
#define EDITBG_ROT_MOVE					(0.02f)		// 向きの移動量
#define EDITBG_SCALING					(0.02f)		// 拡大縮小率
#define EDITBG_COL_CHANGE_CNT			(5)			// 色が変わるカウント
#define EDITBG_COL_CONVERSION			(0.01f)		// マテリアルの変化量
#define EDITBG_UPDOWN_MOVE				(6.0f)		// 上下移動の移動量
#define EDITBG_UPDOWN_ADJUSTMENT_MOVE	(2.0f)		// 調整用の上下移動の移動量
#define EDITBG_ADJUSTMENT_ROT_MOVE		(15)		// 調整用の向きの移動量
#define EDITBG_POS_Z					(500.0f)	// 背景オブジェクトのZ位置

//=======================================
// コンストラクタ
//=======================================
CBgEdit::CBgEdit() : CModel(CObject::TYPE_EDIT, CObject::PRIORITY_UI)
{
	// 全ての値をクリアする
	for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
	{
		m_MatCopy[nCntMat];						// マテリアルのコピー
		m_EditMaterial[nCntMat];				// カスタム用のマテリアル情報
	}

	m_nCntMaterial = 0;;						// マテリアルの番号
	m_nColorCount = 0;							// 色を変えるときのカウント
	m_type = CXFile::TYPE_ENERGYITEM;			// 種類
}

//=======================================
// デストラクタ
//=======================================
CBgEdit::~CBgEdit()
{

}

//=======================================
// 初期化処理
//=======================================
HRESULT CBgEdit::Init(void)
{
	// 初期化処理
	if (FAILED(CModel::Init()))
	{ // 初期化処理に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値をクリアする
	for (int nCntMat = 0; nCntMat < MAX_MATERIAL; nCntMat++)
	{
		m_MatCopy[nCntMat];						// マテリアルのコピー
		m_EditMaterial[nCntMat];				// カスタム用のマテリアル情報
	}

	m_nCntMaterial = 0;;						// マテリアルの番号
	m_nColorCount = 0;							// 色を変えるときのカウント
	m_type = CXFile::TYPE_ENERGYITEM;			// 種類

	// 成功を返す
	return S_OK;
}

//=======================================
// 終了処理
//=======================================
void CBgEdit::Uninit(void)
{
	// 終了処理
	CModel::Uninit();
}

//=======================================
// 更新処理
//=======================================
void CBgEdit::Update(void)
{
	//種類変更処理
	TypeChange();

	//移動処理
	Move();

	//回転処理
	Rotation();

	//オブジェクトの直角処理
	RightAngle();

	// 向きを取得する
	D3DXVECTOR3 rot = GetRot();

	if (rot.y > D3DX_PI)
	{//3.14fより大きくなった場合

		//-3.14fに補正する
		rot.y -= D3DX_PI * 2;
	}
	else if (rot.y < -D3DX_PI)
	{//-3.14fより小さくなった場合

		//3.14fに補正する
		rot.y += D3DX_PI * 2;
	}

	// 向きを設定する
	SetRot(rot);

	//オブジェクトの消去
	Delete();

	//オブジェクトの設定処理
	Set();

	//オブジェクトの拡大縮小処理(X軸)
	ScaleX();

	//オブジェクトの拡大縮小処理(Y軸)
	ScaleY();

	//オブジェクトの拡大縮小処理
	Scale();

	//オブジェクトの情報リセット処理
	Reset();

	//マテリアルのエディット処理
	MaterialCustom();

	// エディットの操作デバッグ表示
	CManager::GetDebugProc()->Print("LCTRLキー：微調整キー\n1キー：種類替え\n2キー：向きのリセット\n3キー：拡大率のリセット\n4キー：全体拡大\n5キー：全体縮小\n"
		"9キー：オブジェクトの削除\n0キー：オブジェクトの設置\nW/A/S/Dキー：上下左右移動\n左SHIFT+W/Sキー：縦移動\nQ/Eキー：回転\n左SHIFT+Q/Eキー：15度回転\n"
		"U/Jキー：X軸拡大縮小\nI/Kキー：Y軸拡大縮小\n左SHIFT+R/Fキー：R値変更\n左SHIFTキー+T/Gキー：G値変更\n左SHIFTキー+Y/Hキー：B値変更\n"
		"左SHIFTキー+V/B/Nキー：R/G/B値のリセット\nSPACEキー：マテリアル変更\n左ALTキー：プレイヤーの場所に移動\n");

	// マテリアルの情報を入れる
	D3DXCOLOR MatCol = m_EditMaterial[m_nCntMaterial].MatD3D.Diffuse;

	// エディットの情報
	CManager::GetDebugProc()->Print("位置：[%f/%f/%f]\n向き：[%f/%f/%f]\n拡大率：[%f/%f/%f]\nマテリアルの色：[%f/%f/%f]\n",
		GetPos().x, GetPos().y, GetPos().z, GetRot().x, GetRot().y, GetRot().z, GetScale().x, GetScale().y, GetScale().z, MatCol.r, MatCol.g, MatCol.b);
}

//=======================================
// 描画処理
//=======================================
void CBgEdit::Draw(void)
{
	// エディットの描画処理
	CModel::DrawBlock(m_EditMaterial);
}

//=======================================
// 情報の設定処理
//=======================================
void CBgEdit::SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const CXFile::SXFile fileData)
{
	// ローカル変数宣言
	D3DXMATERIAL* pMat;		// マテリアルの変数

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)fileData.pBuffMat->GetBufferPointer();

	// 全ての値をクリアする
	for (int nCntMat = 0; nCntMat < (int)fileData.dwNumMat; nCntMat++)
	{
		m_MatCopy[nCntMat] = pMat[nCntMat];			// マテリアルのコピー
		m_EditMaterial[nCntMat] = pMat[nCntMat];	// カスタム用のマテリアル情報
	}

	m_nCntMaterial = 0;;						// マテリアルの番号
	m_nColorCount = 0;							// 色を変えるときのカウント
	m_type = CXFile::TYPE_ENERGYITEM;			// 種類

	// 情報の設定
	SetPos(D3DXVECTOR3(pos.x, pos.y, EDITBG_POS_Z));			// 位置
	SetPosOld(pos);			// 前回の位置
	SetRot(rot);			// 向き
	SetScale(scale);		// 拡大率
	SetFileData(fileData);	// モデルのデータ
}

//=======================================
// 生成処理
//=======================================
CBgEdit* CBgEdit::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const D3DXVECTOR3& scale, const CXFile::SXFile fileData)
{
	// ローカルオブジェクトを生成
	CBgEdit* pEdit = nullptr;	// プレイヤーのインスタンスを生成

	if (pEdit == nullptr)
	{ // オブジェクトが NULL の場合

		// オブジェクトを生成
		pEdit = new CBgEdit;
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
			MessageBox(NULL, "背景エディットの初期化に失敗！", "警告！", MB_ICONWARNING);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pEdit->SetData(pos, rot, scale, fileData);
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
void CBgEdit::Delete(void)
{
	// ローカル変数宣言
	CObject* pObj = nullptr;		// オブジェクトへのポインタ
	CBgModel* pBgModel = nullptr;		// ブロックへのポインタ

	// 先頭のオブジェクトの情報を取得する
	pObj = GetTop(PRIORITY_BG);

	while (pObj != nullptr)
	{ // オブジェクトが NULL じゃない限り回す

		if (pObj->GetType() == TYPE_BGOBJECT)
		{ // オブジェクトが背景オブジェクトだった場合

			// ブロックの情報を取得する
			pBgModel = dynamic_cast<CBgModel*>(pObj);

			if (pBgModel != nullptr)
			{ // ブロックの情報が NULL じゃない場合

				if (GetPos().x >= pBgModel->GetPos().x - EDITBG_DELETE_OBJECT_RANGE &&
					GetPos().x <= pBgModel->GetPos().x + EDITBG_DELETE_OBJECT_RANGE &&
					GetPos().y >= pBgModel->GetPos().y - EDITBG_DELETE_OBJECT_RANGE &&
					GetPos().y <= pBgModel->GetPos().y + EDITBG_DELETE_OBJECT_RANGE)
				{ // 範囲内に入った場合

					if (CManager::GetInputKeyboard()->GetTrigger(DIK_9) == true)
					{ // 9キーを押した場合

						// 削除する
						pBgModel->Uninit();
					}
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
void CBgEdit::TypeChange(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{ // 1キーを押した場合

		// ローカル変数宣言
		D3DXMATERIAL* pMat;					// マテリアルの変数
		CXFile::SXFile fileData;			// ファイルデータ

		// 設定オブジェクトの種類を選択する
		m_type = (CXFile::TYPE)((m_type + 1) % CXFile::TYPE_MAX);

		// ファイルのデータを取得する
		fileData = CXFile::GetXFile(m_type);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)fileData.pBuffMat->GetBufferPointer();

		// 全ての値をクリアする
		for (int nCntMat = 0; nCntMat < (int)fileData.dwNumMat; nCntMat++)
		{
			m_MatCopy[nCntMat] = pMat[nCntMat];			// マテリアルのコピー
			m_EditMaterial[nCntMat] = pMat[nCntMat];	// カスタム用のマテリアル情報
		}

		// 設定する
		SetFileData(m_type);


	}
}

//=======================================
//移動処理
//=======================================
void CBgEdit::Move(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 pos = GetPos();		// 位置

	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{ // 左シフトキーが押されていた場合

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true)
		{ // Wキーを押した場合

			// 位置を奥に進める
			pos.y += EDITBG_ADJUSTMENT_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true)
		{ // Sキーを押した場合

			// 位置を手前に進める
			pos.y -= EDITBG_ADJUSTMENT_MOVE;
		}
	}

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // 左コントロールキーを押していた場合

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true)
		{ // Wキーを押した場合

			// 位置を奥に進める
			pos.z += EDITBG_ADJUSTMENT_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true)
		{ // Sキーを押した場合

			// 位置を手前に進める
			pos.z -= EDITBG_ADJUSTMENT_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_A) == true)
		{ // Aキーを押した場合

			// 位置を左に進める
			pos.x -= EDITBG_ADJUSTMENT_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_D) == true)
		{ // Dキーを押した場合

			// 位置を右に進める
			pos.x += EDITBG_ADJUSTMENT_MOVE;
		}
	}
	else
	{ // 左コントロールを押していない場合

		if (CManager::GetInputKeyboard()->GetPress(DIK_W) == true)
		{ // Wキーを押した場合

			// 位置を奥に進める
			pos.z += EDITBG_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_S) == true)
		{ // Sキーを押した場合

			// 位置を手前に進める
			pos.z -= EDITBG_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_A) == true)
		{ // Aキーを押した場合

			// 位置を左に進める
			pos.x -= EDITBG_MOVE;
		}

		if (CManager::GetInputKeyboard()->GetPress(DIK_D) == true)
		{ // Dキーを押した場合

			// 位置を右に進める
			pos.x += EDITBG_MOVE;
		}
	}

	// 位置の設定処理
	SetPos(pos);
}

//=======================================
//回転処理
//=======================================
void CBgEdit::Rotation(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = GetRot();			// 向きを取得する

	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{ // 左CTRLキーを押している場合

		// 処理を抜ける
		return;
	}

	if (CManager::GetInputKeyboard()->GetPress(DIK_Q) == true)
	{ // Qキーを押した場合

		// 向きを変える
		rot.y += EDITBG_ROT_MOVE;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_E) == true)
	{ // Eキーを押した場合

		// 向きを変える
		rot.y -= EDITBG_ROT_MOVE;
	}

	// 向きの設定処理
	SetRot(rot);
}

//=======================================
//オブジェクトの設定処理
//=======================================
void CBgEdit::Set(void)
{
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_0) == true)
	{ // 0キーを押した場合

		for (int nCount = 0; nCount < (int)GetFileData().dwNumMat; nCount++)
		{
			// 透明度を元に戻す
			m_EditMaterial[nCount].MatD3D.Ambient.a = m_MatCopy[nCount].MatD3D.Ambient.a;
			m_EditMaterial[nCount].MatD3D.Diffuse.a = m_MatCopy[nCount].MatD3D.Diffuse.a;
		}

		// ブロックの設定処理
		CBgModel::Create(GetPos(), GetRot(), GetScale(), &m_EditMaterial[0], m_type);
	}
}

//=======================================
//オブジェクトの拡大縮小処理(X軸)
//=======================================
void CBgEdit::ScaleX(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 scale = GetScale();			// 拡大率を取得する

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // 左コントロールキーを押していた場合

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_U) == true)
		{ // Uキーを押した場合

			// X軸を拡大する
			scale.x += EDITBG_SCALING;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_J) == true)
		{ // Jキーを押した場合

			// X軸を縮小する
			scale.x -= EDITBG_SCALING;
		}
	}
	else
	{ // 左コントロールキーを押していない場合

		if (CManager::GetInputKeyboard()->GetPress(DIK_U) == true)
		{ // Uキーを押した場合

			// X軸を拡大する
			scale.x += EDITBG_SCALING;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_J) == true)
		{ // Jキーを押した場合

			// X軸を縮小する
			scale.x -= EDITBG_SCALING;
		}
	}

	// 拡大率の設定処理
	SetScale(scale);
}

//=======================================
//オブジェクトの拡大縮小処理(Y軸)
//=======================================
void CBgEdit::ScaleY(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 scale = GetScale();			// 拡大率を取得する

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // 左コントロールキーを押していた場合

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_I) == true)
		{ // Iキーを押した場合

			// Y軸を拡大する
			scale.y += EDITBG_SCALING;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_K) == true)
		{ // Kキーを押した場合

			// Y軸を縮小する
			scale.y -= EDITBG_SCALING;
		}
	}
	else
	{ // 左コントロールキーを押していない場合

		if (CManager::GetInputKeyboard()->GetPress(DIK_I) == true)
		{ // Iキーを押した場合

			// Y軸を拡大する
			scale.y += EDITBG_SCALING;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_K) == true)
		{ // Kキーを押した場合

			// Y軸を縮小する
			scale.y -= EDITBG_SCALING;
		}
	}

	// 拡大率の設定処理
	SetScale(scale);
}

//=======================================
//オブジェクトの拡大縮小処理
//=======================================
void CBgEdit::Scale(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 scale = GetScale();			// 拡大率を取得する

	if (CManager::GetInputKeyboard()->GetPress(DIK_LCONTROL) == true)
	{ // 左コントロールキーを押していた場合

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_4) == true)
		{ // 4キーを押した場合

			// 拡大する
			scale.x += EDITBG_SCALING;
			scale.y += EDITBG_SCALING;
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_5) == true)
		{ // 5キーを押した場合

			// 縮小する
			scale.x -= EDITBG_SCALING;
			scale.y -= EDITBG_SCALING;
		}
	}
	else
	{ // 左コントロールキーを押していない場合
		if (CManager::GetInputKeyboard()->GetPress(DIK_4) == true)
		{ // 4キーを押した場合

			// 拡大する
			scale.x += EDITBG_SCALING;
			scale.y += EDITBG_SCALING;
		}
		else if (CManager::GetInputKeyboard()->GetPress(DIK_5) == true)
		{ // 5キーを押した場合

			// 縮小する
			scale.x -= EDITBG_SCALING;
			scale.y -= EDITBG_SCALING;
		}
	}

	// 拡大率の設定処理
	SetScale(scale);
}

//=======================================
//オブジェクトの情報リセット処理
//=======================================
void CBgEdit::Reset(void)
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
//マテリアルのエディット処理
//=======================================
void CBgEdit::MaterialCustom(void)
{
	D3DMATERIAL9 *pMatEdit;		//マテリアルデータへのポインタ

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true)
	{ // SPACEキーを押した場合

		// マテリアルを変更する
		m_nCntMaterial = (m_nCntMaterial + 1) % (int)(GetFileData().dwNumMat);
	}

	for (int nCntMat = 0; nCntMat < (int)GetFileData().dwNumMat; nCntMat++)
	{
		// マテリアルデータへのポインタを取得
		pMatEdit = &m_EditMaterial[nCntMat].MatD3D;

		if (nCntMat == m_nCntMaterial)
		{ // 選択中のマテリアルだった場合

			// 透明度を 1.0f に設定する
			pMatEdit->Diffuse.a = 1.0f;

			if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
			{ // 左シフトキーを押した場合

				if (CManager::GetInputKeyboard()->GetPress(DIK_R) == true)
				{ // Rキーを押した場合

					// カウントを加算する
					m_nColorCount++;

					if (m_nColorCount % EDITBG_COL_CHANGE_CNT == 0)
					{//20Fごとに

						//R値を増やす
						pMatEdit->Ambient.r += EDITBG_COL_CONVERSION;

						if (pMatEdit->Ambient.r > 1.0f)
						{//R値が限界を超えたら

							//R値を0に戻す
							pMatEdit->Ambient.r = 0.0f;
						}

						pMatEdit->Diffuse.r += EDITBG_COL_CONVERSION;

						if (pMatEdit->Diffuse.r > 1.0f)
						{//R値が限界を超えたら

							//R値を0に戻す
							pMatEdit->Diffuse.r = 0.0f;
						}
					}
				}
				else if (CManager::GetInputKeyboard()->GetPress(DIK_F) == true)
				{//Fキーを押した場合

					//カウントを加算する
					m_nColorCount++;

					if (m_nColorCount % EDITBG_COL_CHANGE_CNT == 0)
					{//20Fごとに

						//R値を増やす
						pMatEdit->Ambient.r -= EDITBG_COL_CONVERSION;

						if (pMatEdit->Ambient.r < 0.0f)
						{//R値が限界を超えたら

							//R値を0に戻す
							pMatEdit->Ambient.r = 1.0f;
						}

						pMatEdit->Diffuse.r -= EDITBG_COL_CONVERSION;

						if (pMatEdit->Diffuse.r < 0.0f)
						{//R値が限界を超えたら

							//R値を0に戻す
							pMatEdit->Diffuse.r = 1.0f;
						}
					}
				}

				if (CManager::GetInputKeyboard()->GetPress(DIK_T) == true)
				{//Tキーを押した場合

					//カウントを加算する
					m_nColorCount++;

					if (m_nColorCount % EDITBG_COL_CHANGE_CNT == 0)
					{//20Fごとに

						//G値を増やす
						pMatEdit->Ambient.g += EDITBG_COL_CONVERSION;

						if (pMatEdit->Ambient.g > 1.0f)
						{//G値が限界を超えたら

							//G値を0に戻す
							pMatEdit->Ambient.g = 0.0f;
						}

						//G値を増やす
						pMatEdit->Diffuse.g += EDITBG_COL_CONVERSION;

						if (pMatEdit->Diffuse.g > 1.0f)
						{//G値が限界を超えたら

							//G値を0に戻す
							pMatEdit->Diffuse.g = 0.0f;
						}
					}
				}
				else if (CManager::GetInputKeyboard()->GetPress(DIK_G) == true)
				{//Gキーを押した場合

					//カウントを加算する
					m_nColorCount++;

					if (m_nColorCount % EDITBG_COL_CHANGE_CNT == 0)
					{//20Fごとに

						//R値を増やす
						pMatEdit->Ambient.g -= EDITBG_COL_CONVERSION;

						if (pMatEdit->Ambient.g < 0.0f)
						{//R値が限界を超えたら

							//R値を0に戻す
							pMatEdit->Ambient.g = 1.0f;
						}

						//G値を増やす
						pMatEdit->Diffuse.g -= EDITBG_COL_CONVERSION;

						if (pMatEdit->Diffuse.g < 0.0f)
						{//R値が限界を超えたら

							//R値を0に戻す
							pMatEdit->Diffuse.g = 1.0f;
						}
					}
				}

				if (CManager::GetInputKeyboard()->GetPress(DIK_Y) == true)
				{//Yキーを押した場合

					//カウントを加算する
					m_nColorCount++;

					if (m_nColorCount % EDITBG_COL_CHANGE_CNT == 0)
					{//20Fごとに

						//B値を増やす
						pMatEdit->Ambient.b += EDITBG_COL_CONVERSION;

						if (pMatEdit->Ambient.b > 1.0f)
						{//B値が限界を超えたら

							//B値を0に戻す
							pMatEdit->Ambient.b = 0.0f;
						}

						//B値を増やす
						pMatEdit->Diffuse.b += EDITBG_COL_CONVERSION;

						if (pMatEdit->Diffuse.b > 1.0f)
						{//B値が限界を超えたら

							//B値を0に戻す
							pMatEdit->Diffuse.b = 0.0f;
						}
					}
				}
				else if (CManager::GetInputKeyboard()->GetPress(DIK_H) == true)
				{//Hキーを押した場合

					//カウントを加算する
					m_nColorCount++;

					if (m_nColorCount % EDITBG_COL_CHANGE_CNT == 0)
					{//20Fごとに

						//R値を増やす
						pMatEdit->Ambient.b -= EDITBG_COL_CONVERSION;

						if (pMatEdit->Ambient.b < 0.0f)
						{//R値が限界を超えたら

							//R値を0に戻す
							pMatEdit->Ambient.b = 1.0f;
						}

						pMatEdit->Diffuse.b -= EDITBG_COL_CONVERSION;

						if (pMatEdit->Diffuse.b < 0.0f)
						{//R値が限界を超えたら

							//R値を0に戻す
							pMatEdit->Diffuse.b = 1.0f;
						}
					}
				}

				//アドレスを入れる
				D3DMATERIAL9 *pMatOriginal = &m_MatCopy[nCntMat].MatD3D;

				if (CManager::GetInputKeyboard()->GetPress(DIK_V) == true)
				{//Vキーを押した場合

					//R値のリセット
					pMatEdit->Ambient.r = pMatOriginal->Ambient.r;
					pMatEdit->Diffuse.r = pMatOriginal->Diffuse.r;
				}

				if (CManager::GetInputKeyboard()->GetPress(DIK_B) == true)
				{//Bキーを押した場合

					//G値のリセット
					pMatEdit->Ambient.g = pMatOriginal->Ambient.g;
					pMatEdit->Diffuse.g = pMatOriginal->Diffuse.g;
				}

				if (CManager::GetInputKeyboard()->GetPress(DIK_N) == true)
				{//Nキーを押した場合

					//B値のリセット
					pMatEdit->Ambient.b = pMatOriginal->Ambient.b;
					pMatEdit->Diffuse.b = pMatOriginal->Diffuse.b;
				}
			}
		}
		else
		{ // 選択しているマテリアル以外

			// 透明度を 0.5f に設定する
			pMatEdit->Diffuse.a = 0.5f;
		}
	}
}

//=======================================
//オブジェクトの直角処理
//=======================================
void CBgEdit::RightAngle(void)
{
	// ローカル変数宣言
	D3DXVECTOR3 rot = GetRot();		// 向き

	if (CManager::GetInputKeyboard()->GetPress(DIK_LSHIFT) == true)
	{ // 左CTRLキーを押している場合

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_Q) == true)
		{ // Qキーを押した場合

			// 向きを変える
			rot.y += D3DXToRadian(EDITBG_ADJUSTMENT_ROT_MOVE);
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_E) == true)
		{ // Eキーを押した場合

			// 向きを変える
			rot.y -= D3DXToRadian(EDITBG_ADJUSTMENT_ROT_MOVE);
		}
	}

	// 向きの設定処理
	SetRot(rot);
}

#endif