#include "stdafx.h"
#include "KeyManager.h"

KeyManager::KeyManager()
{
}

KeyManager::~KeyManager()
{
}

HRESULT KeyManager::Init()
{
	//Ű ���� ���� ������ ���� ���� ���·� �ʱ�ȭ
	for (int i = 0; i < KEYMAX; ++i)
	{
		m_keyUp.set(i, false);
		m_keyDown.set(i, false);
	}
	return S_OK;
}

void KeyManager::Update()
{
    m_prevMousePos = m_currMousePos;
    GetCursorPos(&m_currMousePos);
    ScreenToClient(g_hWnd, &m_currMousePos);

    m_prevMouseKeyDown = m_mouseKeyDown;
    m_mouseKeyDown[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
    m_mouseKeyDown[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
}

void KeyManager::Destroy()
{
	//���� ���x
}

bool KeyManager::IsOnceKeyDown(int key)  //Ű�� �ѹ��� ��������
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (m_keyDown[key])
		{
			/* Do nothing */
		}
		else //m_keyDown[key] == false
		{
			m_keyDown.set(key, true);
			return true;
		}
	}
	else
	{
		m_keyDown.set(key, false);
	}
	return false;
}

bool KeyManager::IsOnceKeyUp(int key)    //Ű�� �ѹ� �����ٰ� �ô���
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		m_keyUp.set(key, true);
	}
	else
	{
		if (m_keyUp[key])
		{
			m_keyUp.set(key, false);
			return true;
		}
		else
		{
			/* Do nothing */
		}
	}
	return false;
}

bool KeyManager::IsStayKeyDown(int key)  //Ű�� ��� ������ �ִ���
{
	if (GetAsyncKeyState(key) & 0x8000)
		return true;
	return false;
}

bool KeyManager::IsToggleKey(int key)    //���Ű(ĸ����, �ѹ���)�� On��������
{
	if (GetKeyState(key) & 0x0001)
		return true;
	return false;
}

const POINT& KeyManager::GetCurrentMousePos() const
{
    return m_currMousePos;
}

const POINT& KeyManager::GetPreviousMousePos() const
{
    return m_prevMousePos;
}

bool KeyManager::IsKeyDownMouseL() const
{
    return m_mouseKeyDown[0];
}

bool KeyManager::GetPrevIsKeyDownMouseL() const
{
    return m_prevMouseKeyDown[0];
}

bool KeyManager::IsKeyDownMouseR() const
{
    return m_mouseKeyDown[1];
}

bool KeyManager::GetPrevIsKeyDownMouseR() const
{
    return m_prevMouseKeyDown[1];
}
