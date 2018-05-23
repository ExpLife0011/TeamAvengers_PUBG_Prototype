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
    //키 값을 전부 눌려져 있지 않은 상태로 초기화
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
    //아직 사용x
}

bool KeyManager::IsOnceKeyDown(int key)  //키를 한번만 눌렀는지
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

bool KeyManager::IsOnceKeyUp(int key)    //키를 한번 눌렀다가 뗐는지
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

bool KeyManager::IsStayKeyDown(int key)  //키가 계속 눌려져 있는지
{
    if (GetAsyncKeyState(key) & 0x8000)
        return true;
    return false;
}

bool KeyManager::IsToggleKey(int key)    //토글키(캡스락, 넘버락)가 On상태인지
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
