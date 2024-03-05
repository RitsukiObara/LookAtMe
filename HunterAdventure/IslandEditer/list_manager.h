//===================================
//
// ���X�g�}�l�[�W���[�w�b�_�[[list_manager.h]
// Author ��������
//
//===================================
#ifndef _LIST_MANAGER_H_			//���̃}�N����`������Ă��Ȃ�������
#define _LIST_MANAGER_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "main.h"
#include <list>			// ���X�g�}�l�[�W���[�p

//-----------------------------------
// �N���X��`(���X�g�}�l�[�W���[)
//-----------------------------------
template <class T> class CListManager
{
public:			// �N�ł��A�N�Z�X�o����

	CListManager();		// �R���X�g���N�^
	~CListManager();	// �f�X�g���N�^

	// �����o�֐�
	void Regist(T pThis);		// �o�^����

	T GetTop(void);				// �擪�̎擾����
	T GetEnd(void);				// �����̎擾����
	T GetData(int nIdx);		// ���̎擾����
	int GetNumData(void);		// �v�f���̎擾����
	bool IsEmpty(void);			// �󔒏󋵂̎擾����
	void Pull(T pThis);			// ���X�g�\���̈�����������


private:		// ���������A�N�Z�X�o����

	// �����o�ϐ�
	std::list<T> m_list;						// ���X�g�{��
};

//============================
// �R���X�g���N�^
//============================
template <class T> CListManager<T>::CListManager()
{
	// ���X�g������������
	m_list = {};
}

//============================
// �f�X�g���N�^
//============================
template <class T> CListManager<T>::~CListManager()
{

}


//============================
// �o�^����
//============================
template <class T> void CListManager<T>::Regist(T pThis)
{
	// �����ɗv�f��ǉ�����
	m_list.push_back(pThis);
}

//===========================================
// �I�u�W�F�N�g�̎擾����
//===========================================
template <class T> T CListManager<T>::GetTop(void)
{
	// ���X�g�̍ŏ��̗v�f���擾����
	typename std::list<T>::iterator itr = m_list.begin();

	// �ŏ��̗v�f���擾����
	return (*itr);
}

//===========================================
// �����̎擾����
//===========================================
template <class T> T CListManager<T>::GetEnd(void)
{
	// ���X�g�̍Ō�̗v�f���擾����
	typename std::list<T>::iterator itr = std::prev(m_list.end());

	// �ŏ��̗v�f���擾����
	return (*itr);
}

//===========================================
// ���̎擾����
//===========================================
template <class T> T CListManager<T>::GetData(int nIdx)
{
	// ���X�g�̗v�f���擾����
	typename std::list<T>::iterator itr = m_list.begin();

	// �C�e���[�^�[��i�߂�
	std::advance(itr, nIdx);

	// �ŏ��̗v�f���擾����
	return (*itr);
}

//===========================================
// �v�f���̎擾����
//===========================================
template <class T> int CListManager<T>::GetNumData(void)
{
	// �v�f����Ԃ�
	return m_list.size();
}

//===========================================
// �󔒏󋵂̎擾����
//===========================================
template <class T> bool CListManager<T>::IsEmpty(void)
{
	// �󔒏󋵂�Ԃ�
	return m_list.empty();
}

//===========================================
// ���X�g�\���̈�����������
//===========================================
template <class T> void CListManager<T>::Pull(T pThis)
{
	// ���X�g�̗v�f���擾����
	typename std::list<T>::iterator itr = m_list.begin();
	bool bEmpty = m_list.empty();		// �󔒔���
	int nSize = m_list.size();			// �v�f�����擾
	int nEraseIdx = 0;					// ��������C���f�b�N�X

	if (bEmpty == false)
	{ // ����1�ł������Ă���ꍇ

		for (int nCnt = 0; nCnt < nSize; nCnt++)
		{
			if (pThis == (*itr))
			{ // �����Ώۂ������ꍇ

				// for���𔲂��o��
				break;
			}

			// �C���f�b�N�X�����Z����
			nEraseIdx++;

			// ���̃C�e���[�^�[���擾����
			itr = std::next(m_list.begin(), nEraseIdx);
		}
	}

	// ������������
	m_list.erase(std::next(m_list.begin(), nEraseIdx));
}

#endif