#include "stdafx.h"
#include "ParticleTest.h"


ParticleTest::ParticleTest()
{
}


ParticleTest::~ParticleTest()
{
	SAFE_RELEASE(m_pVB);
}

void ParticleTest::Init()
{
	//�̹��� ���� �־��־ �����̴�.
	m_pTex = g_pTextureManager->GetTexture(string("resources/particle/im.png")/*"resources/particle/sf.jpg"*/);
	if (m_pTex == NULL)
	{
		MessageBox(NULL, TEXT("�ؽ��� ����\n"), 0, 0);
		return;
	}
	size_t numParticle = 5000;
	m_vecAtt.resize(numParticle);
	for (size_t i = 0; i < numParticle; i++)
	{
		Attribute* att = new Attribute();
		att->_position = D3DXVECTOR3(
			GetRandomFloat(-30, 30), 
			GetRandomFloat(-30, 30), 
			GetRandomFloat(-30, 30));
		att->_color = 0xffffffff;//* GetRandomFloat(0, 1);
		m_vecAtt[i] = att;
	}

	g_pDevice->CreateVertexBuffer(numParticle * sizeof(VERTEX_PC),
		D3DUSAGE_POINTS/*����Ʈ������ ���۷� ���ڴ�*/
		| D3DUSAGE_DYNAMIC/*���� ��� �ٲپ� �ٰ��ε�, dynamic���� �� ������ �ӵ��� ������*/
		| D3DUSAGE_WRITEONLY/*���� ��������*/,
		VERTEX_PC::FVF,
		D3DPOOL_DEFAULT,/*dynamic�� ����Ҷ����� Managed�� ������. ���� ���� �ٲپ� �־�� �ϴµ� GPU����
									  ������ �ϴ� ���� ���� ������, ��� GPU�� �ȱ��*/
		&m_pVB,0);
}

void ParticleTest::Update()
{
	if (GetAsyncKeyState('2') & 0x8000)
	{
		for (size_t i = 0; i < m_vecAtt.size(); i++)
		{
			m_vecAtt[i]->_position.x += GetRandomFloat(-0.001f, 0.01f);
			m_vecAtt[i]->_position.y += GetRandomFloat(0.01f, 0.03f);
			m_vecAtt[i]->_color.a += 0.003f;
		}
	}
	else
	{
		for (size_t i = 0; i < m_vecAtt.size(); i++)
		{
			m_vecAtt[i]->_position.x -= GetRandomFloat(-0.001f, 0.01f);
			m_vecAtt[i]->_position.y -= GetRandomFloat(0.01f, 0.03f);
			m_vecAtt[i]->_color.a -= 0.003f;
		}
	}


	VERTEX_PC* v;
	m_pVB->Lock(0, 0, (LPVOID*)&v, D3DLOCK_DISCARD/*������ �־��� ������ �Ű澲�� �ʴ´ٴ� ��*/);
	for (auto p : m_vecAtt)
	{
		v->p = p->_position;
		v->c = p->_color;
		v++;
	}
	m_pVB->Unlock();
}

void ParticleTest::Render()
{
	g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);// ���۸� �׸��� z ���� ���� �ʴ´�//texture�� �ܰ� �κ��� �߶� �� �ִ�.
	g_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	g_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true); //���� ũ�⸦ �����ϰڴ�.
	g_pDevice->SetRenderState(D3DRS_POINTSIZE, FtoDw(0.4f));
	g_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));

	g_pDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	g_pDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	g_pDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	//g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);//���İ��� �ƱԸ�Ʈ1���� ���ͼ� �����̴�.//D3DTOP_SELECTARG1 �ؽ�ó�� alpha�� �����̴�.
	//g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);//D3DTOP_MODULATE ���� D3DTA_TEXTURE*D3DTA_DIFFUSE �ؼ� ���İ��� �����̴�. so alpha���� ���� �� �ִ�.
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//g_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	g_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	g_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	g_pDevice->SetTexture(0, m_pTex);
	g_pDevice->SetFVF(VERTEX_PC::FVF);
	g_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_PC));
	g_pDevice->DrawPrimitive(D3DPT_POINTLIST, 0, m_vecAtt.size());

	g_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	g_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	g_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	g_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
