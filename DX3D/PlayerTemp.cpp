#include "stdafx.h"
#include "PlayerTemp.h"
#include "Pistol.h"
#include "Bullet.h"

PlayerTemp::PlayerTemp()
	: m_pPlayerMesh(nullptr)
	, m_velocity(5.f)
	, m_isRun(false)
	, m_pPistol(nullptr)
{
}

PlayerTemp::~PlayerTemp()
{
	SAFE_RELEASE(m_pPlayerMesh);
}

void PlayerTemp::Init()
{
	m_pos = D3DXVECTOR3(0.f, 3.f, -20.f);                          //�ϴ� �ϵ��ڵ����� ��ġ ����
	D3DXCreateBox(g_pDevice, 2.f, 6.f, 2.f, &m_pPlayerMesh, NULL); //�ӽ� �÷��̾� �޽� ���� (�ӽ÷� �ڽ� ���)
}

void PlayerTemp::Update()
{
	KeyMove();    //�̵�
	KeyMount();   //����
	KeyUnmount(); //��������
	KeyLoad();    //�� ����
	KeyFire();    //�� ���

	//��ȯ��� �缳��
	D3DXMatrixTranslation(&m_matT, m_pos.x, m_pos.y, m_pos.z);
	m_matWorld = m_matT;

	/* ����� */
	//�κ��丮 ����׿�
	ShowInventoryForDebug();

	//�Ѿ� ���� ����׿�
	if (m_pPistol)
		m_pPistol->ShowBulletNumForDebug();
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
}

void PlayerTemp::PutItemInInventory(Item* item)
{
	item->SetItemState(ITEM_STATE::InInventory);
	m_mapInventory[item->GetItemTag()].push_back(item);
}
void PlayerTemp::ShowInventoryForDebug()
{
	Debug->AddText("<Inventory>");
	Debug->EndLine();
	Debug->AddText("the number of Items: ");
	Debug->AddText(GetInventorySize());
	Debug->EndLine();

	for (auto item : m_mapInventory)
	{
		auto itemTag = item.first;
		switch (itemTag)
		{
		case ITEM_TAG::Pistol:
			Debug->AddText("- Pistol: ");
			Debug->AddText(item.second.size());
			Debug->EndLine();

			for (auto i : item.second)
			{
				switch (i->GetItemState())
				{
				case ITEM_STATE::Dropped:
					Debug->AddText("Dropped");
					break;
				case ITEM_STATE::InInventory:
					Debug->AddText("InInventory");
					break;
				case ITEM_STATE::Mounting:
					Debug->AddText("Mounting");
					break;
				} //swtich ItemState
				Debug->EndLine();
			} // for item.second()

			break;
		case ITEM_TAG::Bullet:
			Debug->AddText("- Bullet: ");
			Debug->AddText(item.second.size());
			Debug->EndLine();

			for (auto i : item.second)
			{
				switch (i->GetItemState())
				{
				case ITEM_STATE::Dropped:
					Debug->AddText("Dropped");
					break;
				case ITEM_STATE::InInventory:
					Debug->AddText("InInventory");
					break;
				case ITEM_STATE::Mounting:
					Debug->AddText("Mounting");
					break;
				} //swtich ItemState
				Debug->EndLine();
			} // for item.second()
			break;
		} //switch itemTag
	} //for m_mapInventory
}


/* Ű �Է� ���� �Լ��� �и� */
void PlayerTemp::KeyMove()
{
	float deltaTime = g_pTimeManager->GetDeltaTime();
	float distance = deltaTime * m_velocity;

	/* �̵� ASDW */ // �� ���� �ȿ� ���� ���� �����δ� 
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

	/* �� ������ �� ��ġ ������Ʈ */
	if (m_pPistol)
	{
		m_pPistol->SetPosition(&D3DXVECTOR3(m_pos.x, m_pos.y + 1.f, m_pos.z + 2.f)); //�÷��̾�� ��¦ ��, ��¦ ��
	}
}

void PlayerTemp::KeyMount()
{
	/* ���� ���� */
	if (g_pKeyManager->IsOnceKeyDown('1'))
	{
		if (m_pPistol == nullptr) //�ƹ��͵� �����Ǿ����� ���� ��
		{
			for (auto item : m_mapInventory) //�κ��丮���� �� ã�� ���� �տ� �ִ� �� �ٶ�
			{
				auto itemTag = item.first;
				if (itemTag == ITEM_TAG::Pistol)
				{
					m_pPistol = static_cast<Pistol*>(item.second.front());
					m_pPistol->SetItemState(ITEM_STATE::Mounting); //������
					std::cout << "���� �Ϸ�" << std::endl;
					break; //���Ѱ��� ã����
				}
			}
		}
	}
}

void PlayerTemp::KeyUnmount()
{
	/* ���� ���� ���� */
	if (g_pKeyManager->IsOnceKeyDown('X'))
	{
		if (m_pPistol)
		{
			m_pPistol->SetItemState(ITEM_STATE::InInventory); //����������
			m_pPistol = nullptr;
			std::cout << "���� ����" << std::endl;
		}
	}

}

void PlayerTemp::KeyLoad()
{
	/* �Ѿ� ���� */
	if (g_pKeyManager->IsOnceKeyDown('R'))
	{
		if (m_pPistol) //�����ϰ� �ִ� ���� ������
		{
			//�Ѿ� ã��
			for (auto item : m_mapInventory)
			{
				if (item.first == ITEM_TAG::Bullet)
				{
					int need = m_pPistol->GetNeedBullet(); //������ �ʿ��� �Ѿ� ��
					auto& bullets = item.second; 
					for (int i = 0; i < need; ++i)
					{
						if (bullets.empty() == false)
						{
							auto bullet = bullets.back();
							bullet->SetItemState(ITEM_STATE::Mounting);
							m_pPistol->Load(static_cast<Bullet*>(bullet));
							bullets.pop_back();
							std::cout << "�����Ϸ�" << std::endl;
						}
					}
				}//if ITEM_TAG::Bullet
			}//for m_mapInventory
		}//if m_pPistol
		else //m_pPistpol == nullptr
		{
			std::cout << "���� �����ϰ� ��������~!" << std::endl;
		}
	}
}

void PlayerTemp::KeyFire()
{
	/* �ѽ�� SPACE */ //TODO: ���콺 ���� ��ư���� �ٲ����!!
	if (g_pKeyManager->IsOnceKeyDown(VK_SPACE))
	{
		if (m_pPistol) //���� �����Ǿ����� ��
		{
			m_pPistol->Fire();
			if (m_pPistol->GetBulletNum() > 0)
				std::cout << "���߻���~!" << std::endl;
			else
				std::cout << "�Ѿ��� ���� �ФФФ�" << std::endl;
		}
		else
		{
			std::cout << "���� ��������!" << std::endl;
		}
	}
}
