#pragma once
#include "IScene.h"

//�Ѿ�
struct Bullet //�Ѿ˸��� ���������� ������ �ִ� �Ӽ�
{
	int life;
	D3DXVECTOR3 pos;
	D3DXMATRIXA16 matTranslation;
};

struct BulletProperty //��� �Ѿ��� ���������� ������ �ִ� �Ӽ�
{
	float bulletVelocity;
	float scale;
	D3DXMATRIXA16 matScale;
};

//�÷��̾� -> ����
struct Player
{
	int life;
	D3DXVECTOR3 pos;
	float scale;
	float rotY;
	float velocity;

	//�Ѿ� ����
	DWORD bulletFireTime;
	DWORD oldBulletFireTime;

	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matScale;
	D3DXMATRIXA16 matRotY;
	D3DXMATRIXA16 matTranslation;
};

class Ground;
class SceneShotting : public IScene
{
private:
	Ground*           m_pGround;

	vector<VERTEX_PC> m_vecBaseline; //x, y, z ���ؼ�
	LPD3DXMESH        m_pTeapotMesh; //�÷��̾ ����ϱ� ���� �Ž�
	LPD3DXMESH        m_pBulletMesh; //�Ѿ�(�����)�� ����ϱ� ���� �Ž�

	Player            m_player;
	BulletProperty    m_bulletProperty;
	Bullet            m_bullet[10]; //�� ȭ�鿡 �̻����� ������ 10���� �����Ѵ�

public:
	SceneShotting();
	~SceneShotting();

	// IScene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

