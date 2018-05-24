#include "stdafx.h"
#include "Bullet.h"
#include "IScene.h"
#include "Collider.h"

Bullet::Bullet(float scale, float velocity)
	: Item(ITEM_TAG::Bullet, "Bullet", "I am a Bullet")
	, m_scale(scale)       //0.08f
	, m_velocity(velocity) //10.f
	, m_isFire(false)      //���� �߻���� �ʾҴ�
	, m_pBulletMesh(nullptr)
	, m_isDie(false)
    , m_pBoxCollider(nullptr)
    , m_pCollisionListner(nullptr)
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
        if (!m_pBoxCollider)
        {
            m_pCollisionListner = SetComponent<BulletCollisionListner>();
            m_pBoxCollider = SetComponent<BoxCollider>();
            m_pBoxCollider->SetListner(*m_pCollisionListner);
            m_pBoxCollider->Init(D3DXVECTOR3(-0.2f, -0.2f, -0.4f), D3DXVECTOR3(0.2f, 0.2f, 0.4f));
            m_pBoxCollider->Move(GetPosition());
            m_pBoxCollider->SetTag(CollisionTag::kBullet);
        }

		m_pos.z += deltaTime * m_velocity; //�̵��Ÿ� = �ӷ� * ����ð�, �� ������ ��ġ�� �̵��Ÿ��� �����־� ������ ���ư����Ѵ�

        D3DXMATRIXA16 m;
        D3DXMatrixTranslation(&m, 0.0f, 0.0f, deltaTime * m_velocity);
        m_pBoxCollider->Update(m);

		if (IsInBorderArea()) //��豸�� ���̸� �Ѿ� �̵�
			D3DXMatrixTranslation(&m_matT, m_pos.x, m_pos.y, m_pos.z);
		else                  //��豸�� ���̸� �Ѿ� ����
			g_pCurrentScene->Destroy(this);
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
	else
		m_isDie = true;
	return false;
}

BulletCollisionListner::BulletCollisionListner(BaseObject& owner)
    : ICollisionListner(owner)
{
}

void BulletCollisionListner::OnCollisionEnter(const ColliderBase& other)
{
}

void BulletCollisionListner::OnCollisionExit(const ColliderBase& other)
{
}

void BulletCollisionListner::OnCollisionStay(const ColliderBase& other)
{
}
