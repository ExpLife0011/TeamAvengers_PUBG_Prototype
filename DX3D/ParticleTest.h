#pragma once
#include "IDisplayObject.h"

struct Attribute//��ƼŬ �ϳ� �ϳ��� ���� �Ӽ���
{
	D3DXVECTOR3 _position;
	D3DXCOLOR _color;
};

class ParticleTest :
	public IDisplayObject
{
public:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DTEXTURE9		m_pTex;
	vector<Attribute*>		m_vecAtt;

	ParticleTest();
	~ParticleTest();

	// Inherited via IDisplayObject
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

