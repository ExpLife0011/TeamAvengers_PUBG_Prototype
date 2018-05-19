#pragma once
#include "BaseObject.h"
class AStarNode;
class Heap;
class AStar :public BaseObject
{
private:
	vector<AStarNode*> m_vecNode;
	list<AStarNode*>   m_pOpenNodeList;	//heap �뵵�� ������/ �̰����� open�� �͵� ����
	Heap*			   m_pOpenNodeHeap;
	LPD3DXMESH		   m_pSphere;
	vector<D3DXVECTOR3> m_vecObstacle; //��ֹ��� position

public:
	AStar();
	virtual ~AStar();

	void Init();
	void Render(); //���� �����ϰ�, �� �׷��ִ� ���� �ϴ� �κ�
	void InitNode(IMap* pMap); //map�� ũ�⸦ ������ �ͼ� setting ���ִ� �κ�
	const vector<AStarNode*>& GetNodes() { return m_vecNode; }
	void SetObstacle(const vector<D3DXVECTOR3> &vecObstacle)
	{
		m_vecObstacle = vecObstacle;
	}
	void FindPath(D3DXVECTOR3 startPos, D3DXVECTOR3 destPos, OUT vector<int>& vecIndex);//���� ����� �̰��� ������ �ִ� �κ�

private:
	void RestNodes();	//���۽� ������ �ʱ�ȭ ���ִ� �κ�
	int FindClosestNode(const D3DXVECTOR3 &pos);	//position���� ���� ����� ���� ã��
	float GetManHattanDistance(int from, int to);	//from ���� to ���� ����ź distance
	int GetMinFNodeIndex();							//���� ���� F ����� Index�� ��ȯ 
	void Extend(int targetIdx, int destIdx);		//Ȯ��

public:
	void MakeDirectPath(const D3DXVECTOR3 &startPos, const D3DXVECTOR3 &destPos, OUT vector<int>& vecIndex);
private:
	void CalcEraseCount(const D3DXVECTOR3& pos, const vector<int>& vecIndex, bool bByStartPos, OUT int &numEraseNode);

};

