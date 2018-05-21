#include "stdafx.h"
#include "Bullet.h"
#include "IScene.h"

Bullet::Bullet(float scale, float velocity)
	: m_scale(scale)       //0.08f
	, m_velocity(velocity) //10.f
	, m_isFire(false)      //���� �߻���� �ʾҴ�
	, m_pBulletMesh(nullptr)
{
}

Bullet::~Bullet()
{
	SAFE_RELEASE(m_pBulletMesh);
}

void Bullet::Init()
{
	D3DXCreateCylinder(g_pDevice, 2.f, 2.f, 10.f, 15, 10, &m_pBulletMesh, NULL); //�޽� ����
	D3DXMatrixScaling(&m_matS, m_scale, m_scale, m_scale); //�Է¹��� �Ű������� ũ�� ����
}

void Bullet::Update()
{
	float deltaTime = g_pTimeManager->GetDeltaTime(); //�����Ӵ� �ʴ��� �ð�����

	if (m_isFire)
	{
		m_pos.z += deltaTime * m_velocity; //�̵��Ÿ� = �ӷ� * ����ð�, �� ������ ��ġ�� �̵��Ÿ��� �����־� ������ ���ư����Ѵ�

		if (IsInBorderArea()) //��豸�� ���̸� �Ѿ� �̵�
			D3DXMatrixTranslation(&m_matT, m_pos.x, m_pos.y, m_pos.z);
		else                  //��豸���� �Ѿ�� �Ѿ��� ����
			m_isFire = false;
	}

	//��ȯ���
	m_matWorld = m_matS * m_matT;
}

void Bullet::Render()
{
	const auto dv = g_pDevice;
	dv->SetRenderState(D3DRS_LIGHTING, false);
	dv->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	if (m_isFire) //�߻�� �Ѿ˸� �׷��ֱ�
	{
		dv->SetTransform(D3DTS_WORLD, &m_matWorld);
		m_pBulletMesh->DrawSubset(0);
	}

	dv->SetRenderState(D3DRS_LIGHTING, true);
	dv->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

bool Bullet::IsInBorderArea()
{
	if (m_pos.z < 20.f) //����� Ground�� ���� 20�̹Ƿ�
		return true;
	return false;
}
