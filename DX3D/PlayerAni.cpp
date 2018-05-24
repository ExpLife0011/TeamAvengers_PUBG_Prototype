#include "stdafx.h"
#include "PlayerAni.h"
#include "PlayerParts.h"
#include "Gun.h"
#include "Bullet.h"
#include "Collider.h"

enum enumParts
{
    head,
    left,
    right,
    leftleg,
    rightleg
};

PlayerAni::PlayerAni()
    : m_pGun(nullptr)
	, m_pCollisionListner(nullptr)
	, m_pBoxCollider(nullptr)
{
    m_pRootParts = NULL;

    m_isMoving = false;
    m_moveSpeed = 0.2f;
    m_currMoveSpeedRate = 1.0f;
    m_rotationSpeed = 0.1f;

    m_isJumping = false;
    m_jumpPower = 1.0f;
    m_gravity = 0.05f;
    m_currGravity = 0.0f;

    m_maxStepHeight = 2.0f;

    m_isRunnig = false;
    m_isLive = true;
}


PlayerAni::~PlayerAni()
{
    //m_pRootParts->ReleaseAll();
	
	SAFE_DELETE(m_pBoxCollider);
	SAFE_DELETE(m_pCollisionListner);
}

void PlayerAni::Init()
{
	m_pos = D3DXVECTOR3(0.f, 0.f, -20.f); //���� ��ġ �ڱ�

	g_pObjMgr->AddToTagList(TAG_PLAYER, this);

    g_pCameraManager->SetTarget(m_pos, m_pos);
    CreateAllParts();

	/* collider init */
	m_pCollisionListner = new PlayerAniCollisionListner(*this);
	
	m_pBoxCollider = new BoxCollider(*this);
	m_pBoxCollider->SetListner(*m_pCollisionListner);
	m_pBoxCollider->Init(D3DXVECTOR3(-2.0f, -3.0f, -0.7f), D3DXVECTOR3(2.0f, 3.0f, 0.7f));
	D3DXMATRIXA16 m;
	D3DXMatrixTranslation(&m, 0.0f, 3.0f, 0.0f);
	m_pBoxCollider->Update(m);
	/* end collider init */
}

void PlayerAni::Update()
{
	//�̵� ASDW
	KeyMove();	
    //���� 1, 2
	if (g_pKeyManager->IsOnceKeyDown('1'))
		KeyChangeGun(GUN_TAG::Pistol);
	else if (g_pKeyManager->IsOnceKeyDown('2'))
		KeyChangeGun(GUN_TAG::Rifle);
	//�������� X
	if (g_pKeyManager->IsOnceKeyDown('X'))
		KeyUnmount();
	//�� ���� R
	if (g_pKeyManager->IsOnceKeyDown('R'))
		KeyLoad();
	//�� ���� Enter
	if (g_pKeyManager->IsOnceKeyDown(VK_RETURN))
		KeyFire();
	//���� Space
    if(g_pKeyManager->IsOnceKeyDown(VK_SPACE))
        m_isJumping = true;
	//�״� �ִϸ��̼� �׽�Ʈ�� G
    if (g_pKeyManager->IsOnceKeyDown('G'))
        DiedAni();
	//�ٰ� �ȱ� LShift
	RunAndWalk();

	D3DXMATRIXA16 prevM = m_matWorld;
    if (m_isLive)
        UpdatePosition();
	D3DXMATRIXA16 currM = m_matWorld;
	D3DXMatrixInverse(&prevM, nullptr, &prevM);

    m_pRootParts->SetMovingState(m_isMoving);
    m_pRootParts->Update();

	/* TM = prevM^(-1) * currM */
	m_pBoxCollider->Update(prevM * currM);

	/* ����� */
	//�κ��丮 ����׿�
	ShowInventoryForDebug();

	//�Ѿ� ���� ����׿�
	if (m_pGun)
		m_pGun->ShowBulletNumForDebug();
}

void PlayerAni::Render()
{
    g_pDevice->SetRenderState(D3DRS_LIGHTING, false);
    m_pRootParts->Render();
}

void PlayerAni::UpdatePosition()
{
    m_rot += m_deltaRot * m_rotationSpeed;
    D3DXMATRIXA16 matRotY;

    if (m_isLive)
    {
        D3DXMatrixRotationY(&matRotY, m_rot.y);
        D3DXVec3TransformNormal(&m_forward, &D3DXVECTOR3(0, 0, 1), &matRotY);
    }
    else
    {
        D3DXMatrixRotationX(&matRotY, m_rot.x);
        D3DXVec3TransformNormal(&m_forward, &D3DXVECTOR3(0, 0, 1), &matRotY);
    }

    D3DXVECTOR3 targetPos;
    float basePosY = 0;
    bool	isIntersected = true;
    float	height = 0;

    if (m_isJumping == true)
    {
        m_currMoveSpeedRate = 0.7f;
        targetPos = m_pos + m_forward * m_deltaPos.z * m_moveSpeed * m_currMoveSpeedRate;

        targetPos.y += m_jumpPower - m_currGravity;
        m_currGravity += m_gravity;

        if (g_pCurrentMap != NULL)
        {
            isIntersected = g_pCurrentMap->GetHeight(height, targetPos);
        }

        if (isIntersected == false)
        {
            if (g_pCurrentMap != NULL)
            {
                isIntersected = g_pCurrentMap->GetHeight(height, m_pos);
            }
            m_pos.y = targetPos.y;
        }
        else
        {
            m_pos = targetPos;
        }

        if (m_pos.y <= height && m_jumpPower < m_currGravity)
        {
            m_pos.y = height;
            m_isJumping = false;
            m_currGravity = 0;
            m_currMoveSpeedRate = 1.0f;
        }
        //m_pos = targetPos;
    }
    else //m_isJumping == false
    {
        targetPos = m_pos + m_forward * m_deltaPos.z
            * m_moveSpeed * m_currMoveSpeedRate;

        if (g_pCurrentMap != NULL)
        {
            isIntersected = g_pCurrentMap->GetHeight(height, targetPos);
            int tmp = 0;
            if (isIntersected == false || fabs(height - m_pos.y) > m_maxStepHeight)
            {
				/* nothing */
            }
            else
            {
                targetPos.y = height;
                m_pos = targetPos;
            }
        }
        else
        {
            m_pos = targetPos;
        }

        //m_pos = targetPos;
    }

    D3DXMATRIXA16 matT;
    D3DXMatrixTranslation(&matT, m_pos.x, m_pos.y, m_pos.z);
    m_matWorld = matRotY * matT;

    if (D3DXVec3LengthSq(&m_deltaRot) > D3DX_16F_EPSILON ||
        D3DXVec3LengthSq(&m_deltaPos) > D3DX_16F_EPSILON)
        m_isMoving = true;
    else
        m_isMoving = false;
}

void PlayerAni::CreateAllParts()
{
    PlayerParts* pParts;
    //����
    m_pRootParts = new PlayerParts();
    CreateParts(m_pRootParts, this, D3DXVECTOR3(0.0f, 3.0f, 0.0f),
        D3DXVECTOR3(1.0f, 1.0f, 0.5f), D3DXVECTOR3(0, 0, 0), uvBody, tEmpty);
    //�Ӹ�
    pParts = new PlayerParts();
    CreateParts(pParts, m_pRootParts, D3DXVECTOR3(0.0f, 1.6f, 0.0f),
        D3DXVECTOR3(0.8f, 0.8f, 0.8f), D3DXVECTOR3(0, 0, 0), uvHead, tEmpty);
    //����
    pParts = new PlayerParts(0.1f);
    CreateParts(pParts, m_pRootParts, D3DXVECTOR3(-1.5f, 1.0f, 0.0f),
        D3DXVECTOR3(0.5f, 1.0f, 0.5f), D3DXVECTOR3(0, -1.0f, 0), uvLArm, tEmpty);
    //������
    pParts = new PlayerParts(-0.1f);
    CreateParts(pParts, m_pRootParts, D3DXVECTOR3(1.5f, 1.0f, 0.0f),
        D3DXVECTOR3(0.5f, 1.0f, 0.5f), D3DXVECTOR3(0, -1.0f, 0), uvRArm, tEmpty);
    //�޴ٸ�
    pParts = new PlayerParts(-0.1f);
    CreateParts(pParts, m_pRootParts, D3DXVECTOR3(-0.5f, -1.0f, 0.0f),
        D3DXVECTOR3(0.5f, 1.0f, 0.5f), D3DXVECTOR3(0, -1.0f, 0), uvLLeg, tEmpty);
    //�����ٸ�
    pParts = new PlayerParts(0.1f);
    CreateParts(pParts, m_pRootParts, D3DXVECTOR3(0.5f, -1.0f, 0.0f),
        D3DXVECTOR3(0.5f, 1.0f, 0.5f), D3DXVECTOR3(0, -1.0f, 0), uvRLeg, tEmpty);
}

void PlayerAni::CreateParts(PlayerParts *& pParts, IDisplayObject * pParent, D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 trans, vector<vector<int>>& vecUV, PartTag tag)
{
    D3DXMATRIXA16 matS, matT, mat;
    D3DXMatrixScaling(&matS, scale.x, scale.y, scale.z);
    D3DXMatrixTranslation(&matT, trans.x, trans.y, trans.z);
    mat = matS * matT;
    pParts->Init(&mat, vecUV);
    pParts->SetPosition(&pos);
    pParts->SetPartTag(tag);
    pParent->AddChild(*pParts);
}

void PlayerAni::DrawGunInOut()
{
    PartTag tag = GetChild(right)->GetPartTag();
    float tem_rotX = GetChild(left)->GetRotXspeed();

    if (tag != tEmpty)
    {
        GetChild(right)->SetPartTag(tEmpty);
        GetChild(right)->SetRotXspeed(-1 * tem_rotX);
        GetChild(right)->SyncMotion();
    }
    else
    {
        GetChild(right)->SetPartTag(tGun);
    }
}

void PlayerAni::RunAndWalk()
{
    if (g_pKeyManager->IsStayKeyDown(VK_LSHIFT))
    {
        if (m_isRunnig == false)
        {
            m_moveSpeed = 0.35;
            for (int i = 1; i < m_pRootParts->GetChildVec().size(); i++)
            {
                GetChild(i)->SetRotXspeed(GetChild(i)->GetRotXspeed() * 1.5f);
            }
            m_isRunnig = true;
        }
    }
    else
    {
        if (m_isRunnig == true)
        {
            m_moveSpeed = 0.2f;
            for (int i = 1; i < m_pRootParts->GetChildVec().size(); i++)
            {
                GetChild(i)->SetRotXspeed(GetChild(i)->GetRotXspeed()/ 1.5f);
            }
            m_isRunnig = false;
        }
    }
}

void PlayerAni::DiedAni()
{
    if (m_isLive == true)
    {
        m_isLive = false;
        m_rot.x = D3DX_PI / 2.1f;
        UpdatePosition();
        m_isMoving = false;
    }
}

/* �츮 �߰� */
void PlayerAni::PutItemInInventory(Item* item)
{
	item->SetItemState(ITEM_STATE::InInventory);
	m_mapInventory[item->GetItemTag()].push_back(item);
}

void PlayerAni::PutGuns(Gun * gun)
{
	gun->SetItemState(ITEM_STATE::InInventory);
	m_mapGuns[gun->GetGunTag()] = gun;
}

void PlayerAni::ShowInventoryForDebug()
{
	Debug->AddText("<Inventory>");
	Debug->EndLine();
	Debug->AddText("the number of Items: ");
	Debug->AddText(GetInventorySize() + GetGunsNum());
	Debug->EndLine();

	for (auto item : m_mapGuns)
	{
		auto itemTag = item.first;
		switch (itemTag)
		{
		case GUN_TAG::Pistol:
			Debug->AddText("- Pistol: ");
			Debug->EndLine();

			switch (item.second->GetItemState())
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
			break;
		case GUN_TAG::Rifle:
			Debug->AddText("- Rifle: ");
			Debug->EndLine();

			switch (item.second->GetItemState())
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
			break;
		} 
	} //for m_mapGuns

	for (auto item : m_mapInventory)
	{
		auto itemTag = item.first;
		switch (itemTag)
		{
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

				switch (static_cast<Bullet*>(i)->GetBulletFor())
				{
				case GUN_TAG::Pistol:
					Debug->AddText("Pistol");
					break;
				case GUN_TAG::Rifle:
					Debug->AddText("Rifle");
					break;
				}
				Debug->EndLine();
			} // for item.second()
			break;
		} //switch itemTag
	} //for m_mapInventory
}

/* Ű �Է� ���� �Լ��� �и� */
void PlayerAni::KeyMove()
{
	//float deltaTime = g_pTimeManager->GetDeltaTime();
	//float distance = deltaTime * m_velocity;

	/* �̵� ASDW */ // �� ���� �ȿ� ���� ���� �����δ� 
	if (g_pKeyManager->IsStayKeyDown('A')) //����
	{
		//if (m_pos.x - distance >= -5.f)
		//	m_pos.x -= distance;
		m_deltaRot = D3DXVECTOR3(0, -1, 0);
	}
	else if (g_pKeyManager->IsStayKeyDown('D')) //������
	{
		//if (m_pos.x + distance <= 5.f)
		//	m_pos.x += distance;
		m_deltaRot = D3DXVECTOR3(0,  1, 0);
	}
	else
	{
		m_deltaRot = D3DXVECTOR3(0, 0, 0);
	}

	if (g_pKeyManager->IsStayKeyDown('W')) //����
	{
		//if (m_pos.z + distance <= 20.f)
		//	m_pos.z += distance;
		m_deltaPos.z = 1;
	}
	else if (g_pKeyManager->IsStayKeyDown('S')) //�Ʒ���
	{
		//if (m_pos.z - distance >= -20.f)
		//	m_pos.z -= distance;
		m_deltaPos.z = -1;
	}
	else
	{
		m_deltaPos.z = 0;
	}

	/* �� ������ �� ��ġ ������Ʈ */
	if (m_pGun)
	{
		m_pGun->SetPosition(D3DXVECTOR3(m_pos.x + 1.5f, m_pos.y + 5.f, m_pos.z + 2.f)); //�÷��̾�� ��¦ ��, ��¦ ��
	}
}

void PlayerAni::KeyMount(GUN_TAG gunTag)
{
	/* ���� ���� */
	for (auto gun : m_mapGuns)
	{
		if (gun.first == gunTag)
		{
			DrawGunInOut();
			m_pGun = static_cast<Gun*>(gun.second);
			m_pGun->SetItemState(ITEM_STATE::Mounting); //������
			std::cout << m_pGun->GunTagToStrForDebug(gunTag) << " ���� �Ϸ�" << std::endl;
			break; //���Ѱ��� ã����
		}
	}
}

void PlayerAni::KeyUnmount()
{
	/* ���� ���� ���� */
	if (m_pGun)
	{
		DrawGunInOut();
		m_pGun->SetItemState(ITEM_STATE::InInventory); //����������
		m_pGun = nullptr;
		std::cout << "���� ����" << std::endl;
	}
}

void PlayerAni::KeyLoad()
{
	/* �Ѿ� ���� */
	if (m_pGun) //�����ϰ� �ִ� ���� ������
	{
		//�Ѿ� ã��
		for (auto& item : m_mapInventory)
		{
			if (item.first == ITEM_TAG::Bullet)
			{
				int need = m_pGun->GetNeedBullet(); //������ �ʿ��� �Ѿ� ��
				auto& bullets = item.second;
				vector<Bullet*> vecSpecificBullets; //Ư�� �Ѿ� ����Ʈ�� �����
				for (auto bullet : bullets)
				{
					auto pBullet = static_cast<Bullet*>(bullet);
					if (pBullet->GetBulletFor() == m_pGun->GetGunTag())
						vecSpecificBullets.emplace_back(pBullet);
				}

				for (int i = 0; i < need; ++i)
				{
					if(vecSpecificBullets.empty() == false) //Ư�� �Ѿ� ����Ʈ�� ���� ����
					{
						auto pLastBullet = static_cast<Bullet*>(vecSpecificBullets.back());
						if(pLastBullet->IsBulletForThisGun(m_pGun->GetGunTag()))
						{
							pLastBullet->SetItemState(ITEM_STATE::Mounting);
							m_pGun->Load(pLastBullet);
							for (auto it = bullets.begin(); it != bullets.end(); )
							{
								if (*it == static_cast<Item*>(vecSpecificBullets.back()))
									it = bullets.erase(it);
								else
									++it;
							}
							vecSpecificBullets.pop_back();
							std::cout << "�����Ϸ�" << std::endl;
						}
						else
						{
							std::cout << "�ѿ� �����ʴ� �Ѿ��Դϴ�" << std::endl;
						}
					}
				}//for need
			}//if ITEM_TAG::Bullet
		}//for m_mapInventory
	}//if m_pGun
	else //m_pPistpol == nullptr
	{
		std::cout << "���� �����ϰ� ��������~!" << std::endl;
	}
}

void PlayerAni::KeyFire()
{
	if (m_pGun) //���� �����Ǿ����� ��
	{
		m_pGun->Fire();
		if (m_pGun->GetBulletNum() > 0)
			std::cout << "���߻���~!" << std::endl;
		else
			std::cout << "�Ѿ��� ���� �ФФФ�" << std::endl;
	}
	else
	{
		std::cout << "���� ��������!" << std::endl;
	}
}

void PlayerAni::KeyChangeGun(GUN_TAG gunTag)
{
	if (m_pGun == nullptr) //�ƹ��͵� �����Ǿ����� ���� ��
	{
		KeyMount(gunTag);
	}
	else if (m_pGun && m_pGun->GetGunTag() != gunTag) //������ �Ǿ������鼭, �̹� �����ϰ� �ִ� ���� �ƴ� ��
	{
		//���� ���� �� ����
		KeyUnmount(); 
		KeyMount(gunTag);
	}
}

/* �浹 ���� */
PlayerAniCollisionListner::PlayerAniCollisionListner(BaseObject & owner)
	: ICollisionListner(owner)
{
}

void PlayerAniCollisionListner::OnCollisionEnter(const ColliderBase & other)
{
}

void PlayerAniCollisionListner::OnCollisionExit(const ColliderBase & other)
{
}

void PlayerAniCollisionListner::OnCollisionStay(const ColliderBase & other)
{
}
