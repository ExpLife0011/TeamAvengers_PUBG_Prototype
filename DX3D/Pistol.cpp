#include "stdafx.h"
#include "Pistol.h"
#include "Bullet.h"
#include "IScene.h"

Pistol::Pistol(int bulletNum, int bulletFireCoolTime, float velocity, float scale, float rotY)
	: m_bulletNum(bulletNum)
	, m_bulletFireCoolTime(bulletFireCoolTime) //0.4f
	, m_velocity(velocity) //5.f;
	, m_scale(scale)       //0.7f
	, m_rotY(rotY)         //-D3DXToRadian(90)
	, m_pGunMesh(nullptr)
{
}

Pistol::~Pistol()
{
	/* �Ѿ� Release */
	for (auto bullet : m_vecBullet)
	{
		SAFE_RELEASE(bullet);
	}

	SAFE_RELEASE(m_pGunMesh);
}

void Pistol::Init()
{
	m_pos = D3DXVECTOR3(0.f, 4.f, -9 * 2.f);        //�ϴ� �ϵ��ڵ����� ��ġ ����

	D3DXCreateTeapot(g_pDevice, &m_pGunMesh, NULL); //�� �޽� ���� (�ӽ÷� ������ ���)
	m_bulletFireCoolDown = m_bulletFireCoolTime;    //��ٿ��� ��Ÿ������ �ʱ�ȭ

	D3DXMatrixScaling(&m_matS, m_scale, m_scale, m_scale);
	D3DXMatrixRotationY(&m_matRotY, m_rotY);
	D3DXMatrixTranslation(&m_matT, m_pos.x, m_pos.y, m_pos.z);

	//�ִ� ���� �������� ���� �ִ� �Ѿ� ������ ���ظ�ŭ �������ش�
	for (int i = 0; i < m_bulletNum; ++i)
	{
		Bullet* bullet = new Bullet(0.08f, 10.f);
		bullet->Init(); /* �Ѿ� Init */
		m_vecBullet.emplace_back(bullet);
	}
}

void Pistol::Update()
{
	float deltaTime = g_pTimeManager->GetDeltaTime(); //�����Ӵ� �ʴ��� �ð�����

	//�Ѿ� �߻� ��Ÿ��
	m_bulletFireCoolDown -= deltaTime;
	if (m_bulletFireCoolDown <= 0.f) m_bulletFireCoolDown = 0.f;

	//��ȯ���
	D3DXMatrixTranslation(&m_matT, m_pos.x, m_pos.y, m_pos.z);
	m_matWorld = m_matS * m_matRotY * m_matT;

	/* �Ѿ� Update */
	for (auto bullet : m_vecBullet)
	{
		SAFE_UPDATE(bullet);
	}
}

void Pistol::Render()
{
	const auto dv = g_pDevice;
	dv->SetRenderState(D3DRS_LIGHTING, false);
	dv->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	dv->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGunMesh->DrawSubset(0);

	dv->SetRenderState(D3DRS_LIGHTING, true);
	dv->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	/* �Ѿ� Render */
	for (auto bullet : m_vecBullet)
	{
		SAFE_RENDER(bullet);
	}

	Debug->AddText("The number of Bullets: ");
	Debug->AddText(m_vecBullet.size());
	Debug->EndLine();
}

void Pistol::Fire()
{
	if (m_bulletFireCoolDown <= 0.f)
	{
		m_bulletFireCoolDown = m_bulletFireCoolTime;

		for (auto bullet : m_vecBullet)
		{
			if (bullet->GetIsFire() == false) //�Ҹ�� �̻��� �߿��� �ϳ��� �츰��
			{
				bullet->SetIsFire(true);
				bullet->SetPosition(&D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z + 1.f)); //�÷��̾� ��ġ���� �߻��ϵ�, �÷��̾�� �ణ ���ʿ��� �߻�
				break; //�̻��� �ϳ��� ������ �ݺ��� Ż��
			}
		}
	}
}

void Pistol::Load()
{	
}
