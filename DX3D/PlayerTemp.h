#pragma once
#include "IDisplayObject.h"

//�ӽ��÷��̾�
class Pistol;
class PlayerTemp : public IDisplayObject
{
private:
	LPD3DXMESH              m_pPlayerMesh; //�ӽ��÷��̾ �׷��ֱ� ���� �޽�
	Pistol*                 m_pPistol;        //���� �����ϰ� �ִ�

	float                   m_velocity;
	bool                    m_isRun;

	D3DXMATRIXA16           m_matT;
	D3DXMATRIXA16           m_matRotY;

public:
	PlayerTemp();
	~PlayerTemp();

	// IDisplayObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

