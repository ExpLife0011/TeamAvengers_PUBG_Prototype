#pragma once
#include "Item.h"
#include "Collider.h"

class BulletCollider;
class BulletCollisionListener;

class Bullet : public Item
{
private:
	GUN_TAG       m_bulletFor;    //��� ���� ���� �Ѿ�����
	LPD3DXMESH    m_pBulletMesh;  //�Ѿ��� ����ϱ� ���� �޽�
	bool          m_isFire;       //�Ѿ��� �߻�Ǿ�����
	bool          m_isDie;        //�������� ����ų� ��� �浹�� �� ����

	const float   m_velocity;     //�̵��� ���� �ӷ�
    D3DXVECTOR3   m_dir;
	const float   m_scale;

	D3DXMATRIXA16 m_matS;
	D3DXMATRIXA16 m_matT;

    BoxCollider* m_pBoxCollider;


public:
	Bullet(GUN_TAG bulletFor, float scale, float velocity);
	~Bullet();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

    bool  GetIsFire() const;
    void  SetIsFire(const bool isFire);
    float GetVelocity() const;
    bool  GetIsDie() const;
    GUN_TAG  GetBulletFor();
	bool IsBulletForThisGun(GUN_TAG gunTag);

	bool IsInBorderArea(); //��豸�� �ȿ� �ִ��� üũ

    void SetDirection(const D3DXVECTOR3& val);
};
