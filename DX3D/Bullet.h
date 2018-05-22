#pragma once
#include "Item.h"

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