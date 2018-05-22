#include "stdafx.h"
#include "Bullet.h"
#include "IScene.h"

Bullet::Bullet(float scale, float velocity)
	: Item(ITEM_TAG::Bullet, "Bullet", "I am a Bullet")
	, m_scale(scale)       //0.08f
	, m_velocity(velocity) //10.f
	, m_isFire(false)      //���� �߻���� �ʾҴ�
	, m_pBulletMesh(nullptr)
	, m_isDie(false)
    , m_pBulletCollider(nullptr)
{
}

Bullet::~Bullet()
{
	SAFE_RELEASE(m_pBulletMesh);
    SAFE_RELEASE(m_pBulletCollider);
}

void Bullet::Init()
{
	D3DXCreateCylinder(g_pDevice, 2.f, 2.f, 10.f, 15, 10, &m_pBulletMesh, NULL); //�޽� ����
	D3DXMatrixScaling(&m_matS, m_scale, m_scale, m_scale); //�Է¹��� �Ű������� ũ�� ����

    //VERTEX_PN* pV;
    //D3DXVECTOR3 min, max;
    //m_pBulletMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pV);
    //D3DXComputeBoundingBox((D3DXVECTOR3*)pV, m_pBulletMesh->GetNumVertices(), sizeof VERTEX_PN, &min, &max);
    //m_pBulletMesh->UnlockVertexBuffer();
}

void Bullet::Update()
{
	float deltaTime = g_pTimeManager->GetDeltaTime(); //�����Ӵ� �ʴ��� �ð�����

	if (m_isFire)
	{
        if (!m_pBulletCollider)
        {
            m_pBulletCollider = new BulletCollider;
            m_pBulletCollider->Init(D3DXVECTOR3(-0.2f, -0.2f, -0.4f), D3DXVECTOR3(0.2f, 0.2f, 0.4f), GetPosition());
            m_pBulletCollider->bullet = this;
        }

		m_pos.z += deltaTime * m_velocity; //�̵��Ÿ� = �ӷ� * ����ð�, �� ������ ��ġ�� �̵��Ÿ��� �����־� ������ ���ư����Ѵ�

        D3DXMATRIXA16 m;
        D3DXMatrixTranslation(&m, 0.0f, 0.0f, deltaTime * m_velocity);
        m_pBulletCollider->Update(m);

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

void BulletCollider::Init(const D3DXVECTOR3& min, const D3DXVECTOR3& max, const D3DXVECTOR3& pos)
{
    bc = &SetCollider(min, max);

    D3DXMATRIXA16 m;
    D3DXMatrixTranslation(&m, pos.x, pos.y, pos.z);
    bc->Update(m);
}

void BulletCollider::Update(const D3DXMATRIXA16& transform)
{
    bc->Update(transform);
}

void BulletCollider::OnCollision(ICollidable& other)
{
}
