#pragma once
#include "IDisplayObject.h"
class Ground : public IDisplayObject
{
private:
	DWORD                   m_numVertices;
	DWORD                   m_numIndices;

	LPDIRECT3DINDEXBUFFER9  m_pIB;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;

	int                     m_col; //������ ���ΰ���
	int                     m_row; //������ ���ΰ���
	float                   m_size;

public:
	Ground(int col, int row, float size);
	~Ground();

	// IDisplayObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

