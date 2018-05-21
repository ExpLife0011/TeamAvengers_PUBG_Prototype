#pragma once

#define g_pCameraManager CameraManager::GetInstance()
#define g_pCurrentCamera CameraManager::GetInstance()->GetCurrentMap()

class ICamera;

class CameraManager
{
    SINGLETON(CameraManager)

private:
    ICamera * m_pCurrentCamera;
    map<string, ICamera*> m_mapList;

public:
    //�̷��� ���� �� �ʿ䰡 ������? ���� First�� Third�� ����� ���̱� ������?
    //�׷� enum�� ��𿡴ٰ� �־�� �ϴ°�?
    void AddCamera(string cameraName, ICamera* pCamera)
    {
        if (m_mapList.count(cameraName) == 1)
            return;
        m_mapList[cameraName] = pCamera;
    }
    void SetCurrentCamera(string cameraName)
    { m_pCurrentCamera = m_mapList[cameraName]; }
    ICamera* GetCurrentCamera() { return m_pCurrentCamera; }
};

