//===================================
//
// �����蔻��w�b�_�[[collision.h]
// Author ��������
//
//===================================
#ifndef _COLLISION_H_			//���̃}�N����`������Ă��Ȃ�������
#define _COLLISION_H_			//2�d�C���N���[�h�h�~�̃}�N�����`����

//***********************************
// �C���N���[�h�t�@�C��
//***********************************
#include "main.h"

//-----------------------------------
// �O���錾
//-----------------------------------
class CPlayer;			// �v���C���[
class CBullet;			// �e
class CHierarchy;		// �K�w���f��
class CShadowCircle;	// �ۉe
class CObstacle;		// ��Q��
class CEnemy;			// �G
class CCrawlEnemy;		// ����G
class CGoal;			// �S�[��

// ���O���(�����蔻��)
namespace collision
{
	// �v���C���[�̓����蔻��
	bool PlayerBlockCollision(bool* pRight, bool* pLeft, CPlayer *player, const float fWidth, const float fHeight);		// �v���C���[�ƃu���b�N�̓����蔻��
	bool PlayerObstacleCollision(CPlayer *player, const float fWidth, const float fHeight);								// �v���C���[�Ə�Q���̓����蔻��
	void PlayerObstcleAttack(CPlayer *player);																			// �v���C���[�̏�Q���ɑ΂���U�����菈��
	bool PlayerCollPolygonCollision(CPlayer *player, const float fWidth, const float fHeight);							// �v���C���[�Ɠ����蔻��|���S���̓����蔻��
	void PlayerItemCollision(CPlayer &player);			// �v���C���[�ƃA�C�e���̓����蔻��
	void PlayerBlockPenetration(CPlayer *player, const float fWidth, const float fHeight);			// �v���C���[�ƃu���b�N�̂߂荞�ݔ���
	void PlayerObstaclePenetration(CPlayer *player, const float fWidth, const float fHeight);		// �v���C���[�Ə�Q���̂߂荞�ݔ���
	void PlayerCollPolygonPenetration(CPlayer *player, const float fWidth, const float fHeight);	// �v���C���[�Ɠ����蔻��|���S���̂߂荞�ݔ���
	void PlayerCameraRangeCollision(CPlayer& player, const float fWidth, const float fHeight);		// �v���C���[�ƃJ�����͈̔͂̓����蔻�菈��
	void PlayerBalloonHit(CPlayer* pPlayer);			// �v���C���[�Ɛ����o���̓����蔻��

	// �e�̓����蔻��
	bool BulletBlockCollision(CBullet bullet);			// �e�ƃu���b�N�̓����蔻��
	bool BulletPlayerCollision(CBullet bullet);			// �e�ƃv���C���[�̓����蔻��
	bool BulletEnemyCollision(CBullet bullet);			// �e�ƓG�̓����蔻�菈��

	// �ۉe�̓����蔻��
	void ShadowCollision(const D3DXVECTOR3& pos, int nIdx);		// �ۉe�̓����蔻��

	// ��Q���̓����蔻��
	bool ObstacleBlockCollision(CObstacle *pTarget);	// ��Q���ƃu���b�N�̓����蔻��(���n����)
	bool ObstacleBlockCrush(CObstacle *pTarget);		// ��Q���ƃu���b�N�̓����蔻��(�q�b�g����)
	bool ObstacleMutualCollision(CObstacle* pTarget);	// ��Q�����m�̓����蔻��
	bool ObstacleWoodBoxCollision(CObstacle* pTarget);	// ��Q���Ɩؔ��̓����蔻��

	// �S�[���̓����蔻��
	bool GoalPlayerCollision(CGoal* pGoal);				// �S�[���ƃv���C���[�̓����蔻��

	// �G�̓����蔻��
	bool EnemyBlockCollision(CEnemy* pEnemy);			// �G�ƃu���b�N�̓����蔻��
}

#endif