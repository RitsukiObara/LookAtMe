//===================================
//
// �X�L�b�v�~�w�b�_�[[skip_circle.h]
// Author ��������
//
//===================================
#ifndef _SKIP_CIRCLE_H_
#define _SKIP_CIRCLE_H_

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "donut2D.h"

//-----------------------------------
// �N���X��`(�X�L�b�v�~)
//-----------------------------------
class CSkipCircle : public CDonut2D
{
public:			// �N�ł��A�N�Z�X�ł���

	CSkipCircle();			// �R���X�g���N�^
	~CSkipCircle();			// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const float fSize, const float fDist, const int nDivi);					// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CSkipCircle* Create(const D3DXVECTOR3& pos, const float fSize, const float fDist, const int nDivi);		// ��������

private:		// ���������A�N�Z�X�ł���

	// �����o�֐�
	void PushProcess(void);			// �����Ă��鎞�̏���
	void LeaveProcess(void);		// �����Ă��Ȃ��Ƃ��̏���
	void EndProcess(void);			// �I�����̏���
	void BigScale(void);			// �g�又��
	void SizeCorrect(void);			// �T�C�Y�̕␳����

	// �����o�ϐ�
	int m_nVtx;			// ���_��
	float m_fSize;		// �T�C�Y(�ۑ��p)
	float m_fSizeDest;	// �ړI�̃T�C�Y
	float m_fDist;		// ����(�ۑ��p)
	float m_fDistDest;	// �ړI�̋���
	bool m_bEnd;		// �I����
};

#endif