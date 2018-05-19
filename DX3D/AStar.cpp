#include "stdafx.h"
#include "AStar.h"
#include "AStarNode.h"
#include "Ray.h"
#include "Heap.h"
AStar::AStar()
{
    //merge testing
    int a;
}


AStar::~AStar()
{
	SAFE_DELETE(m_pOpenNodeHeap);
	SAFE_RELEASE(m_pSphere);
	for (auto p : m_vecNode)
		SAFE_RELEASE(p);
}

void AStar::Init()
{
	float radius = 0.2f;
	D3DXCreateSphere(g_pDevice, radius, 10, 10, &m_pSphere, NULL);
	m_pOpenNodeHeap = new Heap;
	//InitNode(g_pMapManager->GetCurrentMap());
}

void AStar::Render()
{
	D3DXMATRIXA16 mat;
	g_pDevice->SetTexture(0, NULL);

	for (auto p : m_vecNode)
	{
		switch (p->m_nodeState)
		{
		case STATE_NONE:
			g_pDevice->SetMaterial(&DXUtil::WHITE_MTRL);
			break;
		case STATE_OPEN:
			g_pDevice->SetMaterial(&DXUtil::GREEN_MTRL);
			break;
		case STATE_CLOSE:
			g_pDevice->SetMaterial(&DXUtil::RED_MTRL);
			break;
		case STATE_USING:
			g_pDevice->SetMaterial(&DXUtil::YELLOW_MTRL);
			break;
		case STATE_WALL:
			g_pDevice->SetMaterial(&DXUtil::BLUE_MTRL);
			break;
		}
		
		D3DXMatrixTranslation(&mat,p->GetLocation().x, p->GetLocation().y, p->GetLocation().z);
		g_pDevice->SetTransform(D3DTS_WORLD, &mat);
		m_pSphere->DrawSubset(0);

		D3DXMatrixIdentity(&mat);
		g_pDevice->SetTransform(D3DTS_WORLD, &mat);
		g_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, p->GetVecLines().size() / 2, p->GetVecLines()[0], sizeof(D3DXVECTOR3));
	}
}

void AStar::InitNode(IMap* pMap)
{
	int nodeDim = 30;
	float interval = pMap->GetSize().x / (float)(nodeDim - 0.99f);//1�ָ� ������.. map size ������ �ϱ� ����

	for (int posZ = 0; posZ < nodeDim; posZ++)
	{
		for (int posX = 0; posX < nodeDim; posX++)
		{
			D3DXVECTOR3 location(posX*interval, 0, posZ*interval);
			pMap->GetHeight(location.y, location);

			AStarNode* pNode = new AStarNode(location, m_vecNode.size() /*posX + posZ * nodeDim*/);
			m_vecNode.push_back(pNode);

			if (posX == 2 && posZ < 5)
			{
				pNode->m_nodeState = STATE_WALL;
			}
		}
	}

	for (size_t i = 0; i < m_vecNode.size(); i++)
	{
		//���� ���� ���� �����ϰ� �ڽ��� ���� ���� ���� Edge ���
		if (i % nodeDim/*pos x*/ != nodeDim - 1)
		{
			m_vecNode[i]->AddEdge(m_vecNode[i + 1]);
			m_vecNode[i + 1]->AddEdge(m_vecNode[i]);
		}
		//���� �� Ⱦ�� �����ϰ� �ڽ��� �� ���� ���� Edge ���
		if (i < m_vecNode.size() - nodeDim)
		{
			m_vecNode[i]->AddEdge(m_vecNode[i + nodeDim]);
			m_vecNode[i + nodeDim]->AddEdge(m_vecNode[i]);
		}
	}

	/*
	i = x + dim * z 2�������� 1��������
	i % dim = x 1�������� 2��������
	*/
}

void AStar::FindPath(D3DXVECTOR3 startPos, D3DXVECTOR3 destPos, OUT vector<int>& vecIndex)
{
	RestNodes();
	
	int startNodeIdx = FindClosestNode(startPos);	//���۳�� ã��
	int destNodeIdx = FindClosestNode(destPos);		//������ ��� ã��

	if (startNodeIdx == -1 || destNodeIdx == -1)
		return; //������ ����

	//����� ��带 �ʱ�ȭ ���ְ� ���� ���·� �����
	float g = 0.0f;
	float h = GetManHattanDistance(startNodeIdx, destNodeIdx);
	float f = g + h;

	m_vecNode[startNodeIdx]->SetValue(STATE_OPEN, g, h, f, startNodeIdx);
	//m_pOpenNodeList.push_back(m_vecNode[startNodeIdx]);//���� open list�� �ϳ��� �ִ�. ���� ���������� �־ ���
	m_pOpenNodeHeap->Insert(m_vecNode[startNodeIdx]);
	//������ ��尡 ���� ��찡 �� ������ ����� ������ Ȯ���� ������
	//Ȯ���� �Ϸ��� ���� ���� ���·� �����
	while (m_vecNode[destNodeIdx]->m_nodeState != STATE_CLOSE)
	{
		int currIndex = GetMinFNodeIndex();
		if (currIndex == -1) return;

		Extend(currIndex, destNodeIdx);
		m_vecNode[currIndex]->m_nodeState = STATE_CLOSE;
	}
	//���� ���� �� ã�� ����

	//������ ������ ����� ��� ���� ���� ���� ������ �ε����� path ��Ͽ� �߰�
	//��Ͽ� �߰��� ������ ��� ���·� �����
	int currIndex = destNodeIdx;
	while (currIndex != startNodeIdx)
	{
		m_vecNode[currIndex]->m_nodeState = STATE_USING;
		vecIndex.push_back(currIndex);
		currIndex = m_vecNode[currIndex]->m_via;//via�� �ڱ� ���� ��忡 ���� index
	}
	//����� ����� �ε����� Path ��Ͽ� �߰��ϰ� ��� ���·� �����
	m_vecNode[currIndex]->m_nodeState = STATE_USING;
	vecIndex.push_back(currIndex);
	
	//�� ����Ʈ�� 
	//������ -> ������ .... -> ����� �ε��� ��� �ϼ�

	//m_pOpenNodeList.clear();
	m_pOpenNodeHeap->Clear();
}

void AStar::RestNodes()
{
	for (int i = 0; i < m_vecNode.size(); i++)
	{
		if (m_vecNode[i]->m_nodeState != STATE_WALL)
			m_vecNode[i]->m_nodeState = STATE_NONE;
	}
}

int AStar::FindClosestNode(const D3DXVECTOR3& pos)
{
	float minDist = FLT_MAX;
	int closestNodeIndex = -1;

	for (int i = 0; i < m_vecNode.size(); i++)
	{
		if (m_vecNode[i]->m_nodeState == STATE_WALL)//��ֹ� ���� �����ϰ�
			continue;

		D3DXVECTOR3 subtract = pos - m_vecNode[i]->GetLocation();

		float dist = D3DXVec3Length(&subtract);
		if (dist < minDist)
		{
			minDist = dist;
			closestNodeIndex = i;
		}
	}
	return closestNodeIndex;	//�ɸ��ְ� ������ -1 return�ȴ�.
}

float AStar::GetManHattanDistance(int from, int to)
{
	float offsetX = fabs(m_vecNode[from]->GetLocation().x - m_vecNode[to]->GetLocation().x);
	float offsetZ = fabs(m_vecNode[from]->GetLocation().z - m_vecNode[to]->GetLocation().z);
	return offsetX + offsetZ;
}		   

int AStar::GetMinFNodeIndex()
{
	AStarNode* node = NULL;
	
	/*
	float minValue = FLT_MAX;
	for (auto p : m_pOpenNodeList)
	{
		if (p->m_f < minValue)
		{
			minValue = p->m_f;
			node = p;
		}
	}
	if (node == NULL) 
		return -1;
	m_pOpenNodeList.remove(node);*/
	node = m_pOpenNodeHeap->ExtractMin();
	if (node == NULL) return -1;
	return node->m_index;
}

void AStar::Extend(int targetIdx, int destIdx)
{
	vector<EDGE_INFO*> vecEdge = m_vecNode[targetIdx]->GetEdgeInfos();

	for (int i = 0; i < vecEdge.size(); i++)
	{
		int index = vecEdge[i]->index;
		AStarNode* currNode = m_vecNode[index];

		if (currNode->m_nodeState == STATE_CLOSE) continue;//���� ����̰ų�
		if (currNode->m_nodeState == STATE_WALL) continue;//��ֹ��� �ÿ� �߰��� ���� �ʴ� ��

		float G = m_vecNode[targetIdx]->m_g + vecEdge[i]->edgeCost;
		float H = GetManHattanDistance(index, destIdx);
		float F = G + H;

		if (currNode->m_nodeState == STATE_OPEN && G >= currNode->m_g) continue;

		currNode->SetValue(STATE_OPEN, G, H, F, targetIdx);
		//m_pOpenNodeList.push_back(currNode);
		m_pOpenNodeHeap->Insert(currNode);
	}
}

void AStar::MakeDirectPath(const D3DXVECTOR3 & startPos, const D3DXVECTOR3 & destPos, OUT vector<int>& vecIndex)
{
	//��ֹ� ������ ��� �߶󳻱�
	if (m_vecObstacle.empty() == true)
	{
		//��ֹ��� ������ �������� �̵�
		vecIndex.clear();
	}
	else
	{
		int numNodeToErase = 0;
		//���� �������� ��ֹ� ���� ���� �������� ��� ����
		CalcEraseCount(startPos/*ĳ������ġ*/, vecIndex, true, numNodeToErase);
		if (numNodeToErase > 0)
			vecIndex.erase(vecIndex.end() - numNodeToErase, vecIndex.end());

		//������ �������� ��ֹ� ���� ���� �������� ��� ����
		CalcEraseCount(destPos/*���콺�����ͷ� ���� �������� ��ġ*/, vecIndex, false, numNodeToErase);
		if (numNodeToErase == vecIndex.size()) numNodeToErase--;
		if (numNodeToErase > 0)
			vecIndex.erase(vecIndex.begin(), vecIndex.begin() + numNodeToErase);

	}
}

void AStar::CalcEraseCount(const D3DXVECTOR3 & pos, const vector<int>& vecIndex, bool bByStartPos, OUT int & numEraseNode)
{
	numEraseNode = 0;
	Ray ray;
	ray.m_pos = pos;
	ray.m_pos.y += 0.3f;

	for (size_t index = 0; index < vecIndex.size(); index++)
	{
		if (bByStartPos == true)
		{
			int theLast = vecIndex.size() - 1;
			int last = theLast - index;
			int node_i = vecIndex[last];
			ray.m_dir = m_vecNode[node_i]->GetLocation() - pos;
		}
		else
		{
			ray.m_dir = m_vecNode[vecIndex[index]]->GetLocation() - pos;
		}
		float nodeDist = D3DXVec3Length(&ray.m_dir);
		D3DXVec3Normalize(&ray.m_dir, &ray.m_dir);
		float intersectionDist;
		bool isIntersected = false;

		for (size_t i = 0; i < m_vecObstacle.size(); i += 3)
		{
			if (ray.CalcIntersectTri(&m_vecObstacle[i], &intersectionDist))
			{
				if (intersectionDist < nodeDist)
				{
					isIntersected = true;
					break;
				}
			}
		}
		if (isIntersected == true)
		{
			numEraseNode--;
			break;
		}
		else
		{
			numEraseNode++;
		}
	}

}
