#include "stdafx.h"
#include "Gun.h"
#include "Bullet.h"
#include "MuzzleFlash.h"

Gun::Gun(GUN_TAG gunTag, bool canChangeBurstMode, int bulletNum, int bulletFireCoolTime, float velocity, float scale, float rotY)
	: Item(ITEM_TAG::Gun, "Gun", "I am a Gun")
	, m_gunTag(gunTag)
	, m_canChangeBurstMode(canChangeBurstMode)
	, m_maxBullet(bulletNum)
	, m_bulletFireCoolTime(bulletFireCoolTime) //0.4f
	, m_velocity(velocity) //5.f;
	, m_scale(scale)       //0.7f
	, m_rotY(rotY)         //-D3DXToRadian(90)
	, m_pGunMesh(nullptr)
    , m_muzzleFlash(nullptr)
{
    switch (gunTag)
    {
    case GUN_TAG::Pistol:
        {
            m_name = "����";
            break;
        }
    case GUN_TAG::Rifle:
        {
            m_name = "����";
            break;
        }
    }
}

Gun::~Gun()
{
	SAFE_RELEASE(m_pGunMesh);
}

void Gun::Init()
{
	//m_pos = D3DXVECTOR3(0.f, 4.f, -9 * 2.f);        //�ϴ� �ϵ��ڵ����� ��ġ ����

	D3DXCreateTeapot(g_pDevice, &m_pGunMesh, NULL); //�� �޽� ���� (�ӽ÷� ������ ���)
	m_bulletFireCoolDown = m_bulletFireCoolTime;    //��ٿ��� ��Ÿ������ �ʱ�ȭ

	D3DXMatrixScaling(&m_matS, m_scale, m_scale, m_scale);
	D3DXMatrixRotationY(&m_matRotY, m_rotY);
	D3DXMatrixTranslation(&m_matT, m_pos.x, m_pos.y, m_pos.z);

    m_muzzleFlash = new MuzzleFlash;
    m_muzzleFlash->Init();
}

void Gun::Update()
{
    Item::Update();

    if (m_state != ITEM_STATE::Dropped)
    {
        float deltaTime = g_pTimeManager->GetDeltaTime(); //�����Ӵ� �ʴ��� �ð�����
        m_bulletFireCoolDown -= deltaTime;
        if (m_bulletFireCoolDown <= 0.f) m_bulletFireCoolDown = 0.f;

        D3DXMatrixRotationY(&m_matRotY, m_rotY);

        //��ȯ���
        D3DXMatrixTranslation(&m_matT, m_pos.x, m_pos.y, m_pos.z);
        m_matWorld = m_matS * m_matRotY * m_matT;
    }

    for (auto b : m_vecPBullet)
    {
        Debug->AddText("bullet pos : ");
        Debug->AddText(b->GetPosition());
        Debug->EndLine();
    }

    m_muzzleFlash->Update();
}

void Gun::Render()
{
	if (m_state == ITEM_STATE::InHand || m_state == ITEM_STATE::Equipped)
	{
		const auto dv = g_pDevice;
		dv->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

        dv->SetRenderState(D3DRS_LIGHTING, true);
		dv->SetTransform(D3DTS_WORLD, &m_matWorld);
        
        if(m_gunTag == GUN_TAG::Pistol)
            dv->SetMaterial(&DXUtil::BLUE_MTRL);
        else if(m_gunTag == GUN_TAG::Rifle)
            dv->SetMaterial(&DXUtil::RED_MTRL);
        else
            dv->SetMaterial(&DXUtil::YELLOW_MTRL);

		m_pGunMesh->DrawSubset(0);

		dv->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
    else if (m_state == ITEM_STATE::Dropped)
    {
        Item::Render();
    }

    m_muzzleFlash->Render();
}

size_t Gun::GetBulletNum()
{
    return m_vecPBullet.size();
}

int Gun::GetMaxNumBullet()
{
    return m_maxBullet;
}

int Gun::GetNeedBullet()
{
    return m_maxBullet - GetBulletNum();
}

GUN_TAG Gun::GetGunTag()
{
    return m_gunTag;
}

bool Gun::GetCanChangeBurstMode()
{
    return  m_canChangeBurstMode;
}

void Gun::Fire(const D3DXVECTOR3& dir)
{
	if (m_bulletFireCoolDown <= 0.f)
	{
		m_bulletFireCoolDown = m_bulletFireCoolTime;

		if (m_vecPBullet.empty() == false)        //�Ѿ��� ������
		{
            //Sound shot
            g_pSoundManager->Play(static_cast<int>(SOUND_TAG::Shot), SOUND_TAG::Shot);
            cout << "Bang Bnag~!" << endl;
			Bullet* bullet = m_vecPBullet.back(); //�Ѿ��� �ϳ� ������
			m_vecPBullet.pop_back();              //���Ϳ��� ������ (���� ������� ������� Update����)
			bullet->SetIsFire(true);
            D3DXVECTOR3 firePos = m_pos + dir;
			bullet->SetPosition(firePos);
            bullet->SetDirection(dir);
			g_pCurrentScene->AddSimpleDisplayObj(bullet);

            m_muzzleFlash->Flash(firePos);
		}
	}
}

void Gun::Load(Bullet* bullet)
{	
    g_pSoundManager->Play(static_cast<int>(SOUND_TAG::Reload), SOUND_TAG::Reload);
	m_vecPBullet.emplace_back(bullet);
}

void Gun::SyncRot(float rotY)
{
    m_rotY = rotY - D3DXToRadian(90);
}

void Gun::ShowBulletNumForDebug()
{
    Debug->EndLine();
    Debug->AddText("BulletNum: ");
    Debug->AddText(GetBulletNum());
    Debug->EndLine();
}

string Gun::GunTagToStrForDebug(GUN_TAG gunTag)
{
    switch (gunTag)
    {
    case GUN_TAG::Pistol:
        return "Pistol";
        break;
    case GUN_TAG::Rifle:
        return "Rifle";
        break;
    default:
        return "?";
        break;
    }
}