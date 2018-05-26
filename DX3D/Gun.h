#pragma once
#include "Item.h"

class Bullet;
class MuzzleFlash;

class Gun : public Item
{
private:
	GUN_TAG          m_gunTag;
    LPD3DXMESH       m_pGunMesh;            //���� �׷��ֱ� ���� �޽�
	vector<Bullet*>  m_vecPBullet;          //�Ѿ� ���� ����
	const int        m_maxBullet;           //�ִ� ���� ����

	const int        m_bulletFireCoolTime; 
	int              m_bulletFireCoolDown; //�Ѿ� �߻�� ������ ��Ÿ�� �ð��� ������ �ٽ� �߻簡��

    bool             m_canChangeBurstMode; //������ ������ ������

	const float      m_velocity;           //�̵��� ���� �ӷ�
	const float      m_scale;
	float            m_rotY;
	
	D3DXMATRIXA16    m_matS;
	D3DXMATRIXA16    m_matRotY;
	D3DXMATRIXA16    m_matT;

    MuzzleFlash* m_muzzleFlash;

public:
	Gun(GUN_TAG gunTag, bool canChangeBurstMode, int bulletNum, int bulletFireCoolTime, float velocity, float scale, float rotY);
	virtual ~Gun();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

    size_t GetBulletNum();
    int GetMaxNumBullet();
    int GetNeedBullet();
    GUN_TAG GetGunTag();
    bool GetCanChangeBurstMode();

	void Fire(const D3DXVECTOR3& dir); //�ѽ��
	void Load(Bullet* bullet); //����
    void SyncRot(float rotY);   //ĳ���� ��ġ�� �°� ����. (�ش� ������ ������ǥ ������ -D3DXToRadian(90)����)

    /* ����� */
    void ShowBulletNumForDebug();
    string GunTagToStrForDebug(GUN_TAG gunTag);
};

