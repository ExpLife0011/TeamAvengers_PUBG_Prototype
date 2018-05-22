#pragma once
#include "IDisplayObject.h"
#include "Item.h"

//�ӽ��÷��̾�
class Pistol;
class Bullet;
class PlayerTemp : public IDisplayObject
{
private:
	LPD3DXMESH                   m_pPlayerMesh; //�ӽ��÷��̾ �׷��ֱ� ���� �޽�
	
	map<ITEM_TAG, vector<Item*>> m_mapInventory;
	Pistol*                      m_pPistol;     //�������� ��
	vector<Bullet*>              m_vecPBullet;  //�Ѿ� ������ ���� �ӽ� ����

	float                        m_velocity;
	bool                         m_isRun;

	D3DXMATRIXA16                m_matT;
	D3DXMATRIXA16                m_matRotY;

public:
	PlayerTemp();
	~PlayerTemp();

	// IDisplayObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	size_t GetInventorySize() { return m_mapInventory.size(); }
	void PutItemInInventory(Item* item);
	void ShowInventory();

	/* Ű �Է� ���� �Լ��� �и�*/
	void KeyMove();    //�̵�
	void KeyMount();   //����
	void KeyUnmount(); //��������
	void KeyLoad();    //�� ����
	void KeyFire();    //�� ���
};

