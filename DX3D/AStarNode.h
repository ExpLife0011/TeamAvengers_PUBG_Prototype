#pragma once
#include "BaseObject.h"

// ���� ����� �̿���� ����
struct EDGE_INFO
{
    int index;

	// �� ���κ��� �̿��������� ���(�Ÿ�)
	float edgeCost;
};

enum NODE_STATE
{
	STATE_NONE,
	STATE_OPEN,
	STATE_CLOSE,
	STATE_USING,
	STATE_WALL 
};

class AStarNode :public BaseObject
{//astarnode�� EDGE_INFO�� �迭�� ���� �־
public:
	D3DXVECTOR3 m_location;
	int			m_index;
	NODE_STATE  m_nodeState;
	float		m_g;	// ���������κ����� �ڽ�Ʈ
	float		m_h;	// ������������ �ڽ�Ʈ
	float		m_f;	// g + h
	int			m_via; //������尡 ������� �����ϴ� �κ�

	vector<EDGE_INFO*>		m_vecEdgeInfo; // �ִ� 8��
	vector<D3DXVECTOR3>		m_vecLine; // ����׿�

	AStarNode(D3DXVECTOR3 location, int index);
	virtual ~AStarNode();

	void SetValue(NODE_STATE _nodeState, float _g, float _h, float _f, int _via);
	void AddEdge(AStarNode* pNode);
	const D3DXVECTOR3& GetLocation() { return m_location; }
	const vector<D3DXVECTOR3>& GetVecLines() { return m_vecLine; }
	const vector<EDGE_INFO*>& GetEdgeInfos() { return m_vecEdgeInfo; }

};

