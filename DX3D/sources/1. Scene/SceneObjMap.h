#pragma once
#include "IScene.h"

class ObjMap;
class Cubeman;
class AseCharacter;
class PlayerAni;

class SceneObjMap : public IScene
{
private:
	ObjMap *		m_pObjMap;
    PlayerAni *		m_pPlayerAni;
	//AseCharacter*	m_pAseCharacter;

public:
	SceneObjMap();
	~SceneObjMap();

	// IScene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};
