#pragma once
#include "Item.h"
#include "ICollidable.h"

class BulletCollider;

class Bullet : public Item
{
private:
	LPD3DXMESH    m_pBulletMesh;  //�Ѿ��� ����ϱ� ���� �޽�
	bool          m_isFire;       //�Ѿ��� �߻�Ǿ�����
	bool          m_isDie;        //�������� ����ų� ��� �浹�� �� ����

	const float   m_velocity;     //�̵��� ���� �ӷ�
	const float   m_scale;
	//D3DXVECTOR3 m_pos;          //IDisplayObject�� ����

	D3DXMATRIXA16 m_matS;
	D3DXMATRIXA16 m_matT;

    BulletCollider* m_pBulletCollider;

public:
	Bullet(float scale, float velocity);
	~Bullet();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	bool  GetIsFire() const { return m_isFire; }
	void  SetIsFire(const bool isFire){ m_isFire = isFire; }
	float GetVelocity() const { return m_velocity; }
	bool  GetIsDie() const { return m_isDie; }

	bool IsInBorderArea(); //��豸�� �ȿ� �ִ��� üũ
};

class BulletCollider : public ICollidable
{
private: 
    BoxCollider* bc;

public:
    Bullet* bullet;

    void Init(const D3DXVECTOR3& min, const D3DXVECTOR3& max, const D3DXVECTOR3& pos);
    void Update(const D3DXMATRIXA16& transform);
    virtual void OnCollision(ICollidable & other) override;
};