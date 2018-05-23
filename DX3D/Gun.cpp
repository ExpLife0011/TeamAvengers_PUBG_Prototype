#include "stdafx.h"
#include "Gun.h"
#include "Bullet.h"

Gun::Gun(GUN_TAG gunTag, int bulletNum, int bulletFireCoolTime, float velocity, float scale, float rotY)
	: Item(ITEM_TAG::Gun, "Gun", "I am a Gun")
	, m_gunTag(gunTag)
	, m_maxBullet(bulletNum)
	, m_bulletFireCoolTime(bulletFireCoolTime) //0.4f
	, m_velocity(velocity) //5.f;
	, m_scale(scale)       //0.7f
	, m_rotY(rotY)         //-D3DXToRadian(90)
	, m_pGunMesh(nullptr)
{
}

Gun::~Gun()
{
	SAFE_RELEASE(m_pGunMesh);
}

void Gun::Init()
{
	m_pos = D3DXVECTOR3(0.f, 4.f, -9 * 2.f);        //�ϴ� �ϵ��ڵ����� ��ġ ����

	D3DXCreateTeapot(g_pDevice, &m_pGunMesh, NULL); //�� �޽� ���� (�ӽ÷� ������ ���)
	m_bulletFireCoolDown = m_bulletFireCoolTime;    //��ٿ��� ��Ÿ������ �ʱ�ȭ

	D3DXMatrixScaling(&m_matS, m_scale, m_scale, m_scale);
	D3DXMatrixRotationY(&m_matRotY, m_rotY);
	D3DXMatrixTranslation(&m_matT, m_pos.x, m_pos.y, m_pos.z);
}

void Gun::Update()
{
	float deltaTime = g_pTimeManager->GetDeltaTime(); //�����Ӵ� �ʴ��� �ð�����

	//�Ѿ� �߻� ��Ÿ��
	m_bulletFireCoolDown -= deltaTime;
	if (m_bulletFireCoolDown <= 0.f) m_bulletFireCoolDown = 0.f;

	//��ȯ���
	D3DXMatrixTranslation(&m_matT, m_pos.x, m_pos.y, m_pos.z);
	m_matWorld = m_matS * m_matRotY * m_matT;
}

void Gun::Render()
{
	if (m_state == ITEM_STATE::Mounting)
	{
		const auto dv = g_pDevice;
		dv->SetRenderState(D3DRS_LIGHTING, false);
		dv->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		dv->SetTransform(D3DTS_WORLD, &m_matWorld);
		m_pGunMesh->DrawSubset(0);

		dv->SetRenderState(D3DRS_LIGHTING, true);
		dv->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

void Gun::ShowBulletNumForDebug()
{
	Debug->EndLine();
	Debug->AddText("BulletNum: ");
	Debug->AddText(GetBulletNum());
	Debug->EndLine();
}

void Gun::Fire()
{
	if (m_bulletFireCoolDown <= 0.f)
	{
		m_bulletFireCoolDown = m_bulletFireCoolTime;

		if (m_vecPBullet.empty() == false)        //�Ѿ��� ������
		{
			Bullet* bullet = m_vecPBullet.back(); //�Ѿ��� �ϳ� ������
			m_vecPBullet.pop_back();              //���Ϳ��� ������ (���� ������� ������� Update����)
			bullet->SetIsFire(true);
			bullet->SetPosition(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z + 1.f));
			g_pCurrentScene->AddSimpleDisplayObj(bullet);
		}
	}
}

void Gun::Load(Bullet* bullet)
{	
	m_vecPBullet.emplace_back(bullet);
}
