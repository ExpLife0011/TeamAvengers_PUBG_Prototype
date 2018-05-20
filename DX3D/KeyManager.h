#pragma once
#define g_pKeyManager KeyManager::GetInstance()
#define KEYMAX 256
#include <bitset>

class KeyManager
{
private:
	std::bitset<KEYMAX> m_keyUp;
	std::bitset<KEYMAX> m_keyDown;

	KeyManager();
	~KeyManager();
public:
	//�̱���
	static KeyManager& GetInstance()
	{
		static KeyManager instance;
		return instance;
	}

	HRESULT Init();
	void Destroy();

	bool IsOnceKeyDown(int key); //Ű�� �ѹ��� ��������
	bool IsOnceKeyUp(int key);   //Ű�� �ѹ� �����ٰ� �ô���
	bool IsStayKeyDown(int key); //Ű�� ��� ������ �ִ���
	bool IsToggleKey(int key);   //���Ű(ĸ����, �ѹ���)�� On��������
};