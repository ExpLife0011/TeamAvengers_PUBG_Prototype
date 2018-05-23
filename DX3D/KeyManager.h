#pragma once
#define g_pKeyManager KeyManager::GetInstance()
#define KEYMAX 256

class KeyManager
{
private:
    bitset<KEYMAX> m_keyUp;
    bitset<KEYMAX> m_keyDown;

    POINT m_prevMousePos;
    POINT m_currMousePos;
    static const int m_kMouseKeySize = 2;
    bitset<m_kMouseKeySize> m_mouseKeyDown;
    bitset<m_kMouseKeySize> m_prevMouseKeyDown;

    KeyManager();
    ~KeyManager();

public:
    //�̱���
    static KeyManager* GetInstance()
    {
        static KeyManager instance;
        return &instance;
    }

    HRESULT Init();
    void Update();
    void Destroy();

    bool IsOnceKeyDown(int key); //Ű�� �ѹ��� ��������
    bool IsOnceKeyUp(int key);   //Ű�� �ѹ� �����ٰ� �ô���
    bool IsStayKeyDown(int key); //Ű�� ��� ������ �ִ���
    bool IsToggleKey(int key);   //���Ű(ĸ����, �ѹ���)�� On��������

    const POINT& GetCurrentMousePos() const;
    const POINT& GetPreviousMousePos() const;
    bool IsKeyDownMouseL() const;
    bool IsKeyDownMouseR() const;
    bool GetPrevIsKeyDownMouseL() const;
    bool GetPrevIsKeyDownMouseR() const;
};