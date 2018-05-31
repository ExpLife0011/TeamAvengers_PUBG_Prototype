#pragma once
#include "IScene.h"

class HeightMap;
class AseCharacter;
class Picking;

class SceneHeightmap : public IScene
{
private:
	HeightMap *		m_pHeightMap;
	AseCharacter*	m_pAseCharacter;
	Picking *		m_pPicking;
	AStar*			m_pAStar;
public:
	SceneHeightmap();
	~SceneHeightmap();

	// IScene��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};
