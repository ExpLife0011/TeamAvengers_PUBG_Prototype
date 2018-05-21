#include "stdafx.h"
#include "PlayerTemp.h"
#include "Pistol.h"

PlayerTemp::PlayerTemp()
	: m_pPlayerMesh(nullptr)
	, m_pPistol(nullptr)
	, m_velocity(5.f)
	, m_isRun(false)
{
}

PlayerTemp::~PlayerTemp()
{
	SAFE_RELEASE(m_pPlayerMesh);
	SAFE_RELEASE(m_pPistol);
}

void PlayerTemp::Init()
{
	m_pos = D3DXVECTOR3(0.f, 3.f, -20.f);                          //�ϴ� �ϵ��ڵ����� ��ġ ����
	D3DXCreateBox(g_pDevice, 2.f, 6.f, 2.f, &m_pPlayerMesh, NULL); //�ӽ� �÷��̾� �޽� ���� (�ӽ÷� �ڽ� ���)
	
	/* �� Init */
	m_pPistol = new Pistol(10, 0.4f, 5.f, 0.7f, -D3DXToRadian(90));
	m_pPistol->Init();
}

void PlayerTemp::Update()
{
	float deltaTime = g_pTimeManager->GetDeltaTime();
	float distance = deltaTime * m_velocity;

	/* �̵� ASDW */
	// �� ���� �ȿ� ���� ���� �����δ� 
	if (g_pKeyManager->IsStayKeyDown('A')) //����
	{
		if (m_pos.x - distance >= -5.f)
			m_pos.x -= distance;
	}
	if (g_pKeyManager->IsStayKeyDown('D')) //������
	{
		if (m_pos.x + distance <= 5.f)
			m_pos.x += distance;
	}
	if (g_pKeyManager->IsStayKeyDown('W')) //����
	{
		if (m_pos.z + distance <= 20.f)
			m_pos.z += distance;
	}
	if (g_pKeyManager->IsStayKeyDown('S')) //�Ʒ���
	{
		if (m_pos.z - distance >= -20.f)
			m_pos.z -= distance;
	}

	//�� ��ġ ������Ʈ
	m_pPistol->SetPosition(&D3DXVECTOR3(m_pos.x, m_pos.y + 1.f, m_pos.z + 2.f)); //�÷��̾�� ��¦ ��, ��¦ ��

	/* �ѽ�� SPACE */ //TODO: ���콺 ���� ��ư���� �ٲ����!!
	if (g_pKeyManager->IsOnceKeyDown(VK_SPACE))
	{
		m_pPistol->Fire();
	}

	/* ���� R */
	if (g_pKeyManager->IsOnceKeyDown('R'))
	{
		std::cout << "����!" << std::endl;
		m_pPistol->Load();
	}

	//��ȯ��� �缳��
	D3DXMatrixTranslation(&m_matT, m_pos.x, m_pos.y, m_pos.z);
	m_matWorld = m_matT;

	/* �� Update */
	SAFE_UPDATE(m_pPistol);
}

void PlayerTemp::Render()
{
	const auto dv = g_pDevice;
	dv->SetRenderState(D3DRS_LIGHTING, false);
	dv->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
	dv->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pPlayerMesh->DrawSubset(0);

	dv->SetRenderState(D3DRS_LIGHTING, true);
	dv->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	/* �� Render */
	SAFE_RENDER(m_pPistol);
}

