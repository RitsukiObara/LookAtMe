//===================================
//
// リストマネージャーヘッダー[list_manager.h]
// Author 小原立暉
//
//===================================
#ifndef _LIST_MANAGER_H_			//このマクロ定義がされていなかったら
#define _LIST_MANAGER_H_			//2重インクルード防止のマクロを定義する

//***********************************
// インクルードファイル
//***********************************
#include "main.h"
#include <list>			// リストマネージャー用

//-----------------------------------
// クラス定義(リストマネージャー)
//-----------------------------------
template <class T> class CListManager
{
public:			// 誰でもアクセス出来る

	CListManager();		// コンストラクタ
	~CListManager();	// デストラクタ

	// メンバ関数
	void Regist(T pThis);		// 登録処理

	T GetTop(void);				// 先頭の取得処理
	T GetEnd(void);				// 末尾の取得処理
	T GetData(int nIdx);		// 情報の取得処理
	int GetNumData(void);		// 要素数の取得処理
	bool IsEmpty(void);			// 空白状況の取得処理
	void Pull(T pThis);			// リスト構造の引き抜き処理


private:		// 自分だけアクセス出来る

	// メンバ変数
	std::list<T> m_list;						// リスト本体
};

//============================
// コンストラクタ
//============================
template <class T> CListManager<T>::CListManager()
{
	// リストを初期化する
	m_list = {};
}

//============================
// デストラクタ
//============================
template <class T> CListManager<T>::~CListManager()
{

}


//============================
// 登録処理
//============================
template <class T> void CListManager<T>::Regist(T pThis)
{
	// 末尾に要素を追加する
	m_list.push_back(pThis);
}

//===========================================
// オブジェクトの取得処理
//===========================================
template <class T> T CListManager<T>::GetTop(void)
{
	// リストの最初の要素を取得する
	typename std::list<T>::iterator itr = m_list.begin();

	// 最初の要素を取得する
	return (*itr);
}

//===========================================
// 末尾の取得処理
//===========================================
template <class T> T CListManager<T>::GetEnd(void)
{
	// リストの最後の要素を取得する
	typename std::list<T>::iterator itr = std::prev(m_list.end());

	// 最初の要素を取得する
	return (*itr);
}

//===========================================
// 情報の取得処理
//===========================================
template <class T> T CListManager<T>::GetData(int nIdx)
{
	// リストの要素を取得する
	typename std::list<T>::iterator itr = m_list.begin();

	// イテレーターを進める
	std::advance(itr, nIdx);

	// 最初の要素を取得する
	return (*itr);
}

//===========================================
// 要素数の取得処理
//===========================================
template <class T> int CListManager<T>::GetNumData(void)
{
	// 要素数を返す
	return m_list.size();
}

//===========================================
// 空白状況の取得処理
//===========================================
template <class T> bool CListManager<T>::IsEmpty(void)
{
	// 空白状況を返す
	return m_list.empty();
}

//===========================================
// リスト構造の引き抜き処理
//===========================================
template <class T> void CListManager<T>::Pull(T pThis)
{
	// リストの要素を取得する
	typename std::list<T>::iterator itr = m_list.begin();
	bool bEmpty = m_list.empty();		// 空白判定
	int nSize = m_list.size();			// 要素数を取得
	int nEraseIdx = 0;					// 消去するインデックス

	if (bEmpty == false)
	{ // 中に1つでも入っている場合

		for (int nCnt = 0; nCnt < nSize; nCnt++)
		{
			if (pThis == (*itr))
			{ // 消す対象だった場合

				// for文を抜け出す
				break;
			}

			// インデックスを加算する
			nEraseIdx++;

			// 次のイテレーターを取得する
			itr = std::next(m_list.begin(), nEraseIdx);
		}
	}

	// 情報を引き抜く
	m_list.erase(std::next(m_list.begin(), nEraseIdx));
}

#endif