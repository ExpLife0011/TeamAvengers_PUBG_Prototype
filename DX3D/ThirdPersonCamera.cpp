#include "stdafx.h"
#include "ThirdPersonCamera.h"


ThirdPersonCamera::ThirdPersonCamera()
    :ICamera()
{
    m_cameraState = CameraState::THIRDPERSON;
    m_distance = TP_DISTANCE;
    m_basePosX = TP_BASEPOSX;
    m_basePosY = TP_BASEPOSY;
    m_fovY = D3DX_PI *(80.0f / 180.0f);    //80 Degrees TP sight
}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

void ThirdPersonCamera::Init()
{
    ICamera::Init();
}

void ThirdPersonCamera::Update()
{
    const float dt = g_pTimeManager->GetDeltaTime();
    
    //ALTkey�� ������ �ֺ��� �� �� �ִ� ���
    m_isALTbuttonStay = g_pKeyManager->IsStayKeyDown(VK_MENU);//Alt����� Icamera update�� �ѹ��� ���� ������ �̷��� �־����ϴ�.
    if (m_isALTbuttonStay)
    {
        POINT currPoint;
        m_ptPrevMouse = g_pKeyManager->GetPreviousMousePos();
        currPoint = g_pKeyManager->GetCurrentMousePos();
        POINT diff;
        diff.x = currPoint.x - m_ptPrevMouse.x;
        diff.y = currPoint.y - m_ptPrevMouse.y;
        const float factorX = 0.1f;
        const float factorY = 0.1f;
        m_rotX += diff.y * factorX * dt;
        m_rotY += diff.x * factorY * dt;
    }
    else//alt�� ������ �ʰ� �Ǹ� ĳ���Ͱ� �ٶ󺸰� �ִ� rotation ���� �ʱ�ȭ
    {
        if (m_pTargetRot)
        {
            m_rotX = m_pTargetRot->x;
            m_rotY = m_pTargetRot->y;
        }
        else
        {
            m_rotX = m_rotY = 0.0f;
        }
    }

    //�����ϴ� �κ��� 3��Ī������ �ֱ⿡
    if (g_pKeyManager->IsOnceKeyDown(VK_RBUTTON))
    {
        g_pCameraManager->SetCurrentCamera(CameraState::KYUNCHAK);
    }
    ICamera::Update();
}

//--------------------------------------------


CameraTPToFP::CameraTPToFP()
    : ThirdPersonCamera()
    , m_vel(0.0f)
{
    m_cameraState = CameraState::TP2FP;
}

CameraTPToFP::~CameraTPToFP()
{
}

void CameraTPToFP::Init()
{
    m_distance = TP_DISTANCE;
    m_vel = 0.0f;
    //m_fovY = D3DX_PI * (80.0f / 180.0f);    //80 Degrees TP sight
    ThirdPersonCamera::Init();
}

void CameraTPToFP::Update()
{
    const auto dt = g_pTimeManager->GetDeltaTime();
    m_vel += dt * 5.0f;
    m_distance -= m_vel * dt;
    //m_fovY-= m_vel * dt*0.3f;
    //D3DXMatrixPerspectiveFovLH(&m_matProj, m_fovY, m_aspect, 1, 1000);
    //g_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
    if(m_distance <= TP_DISTANCE-1.0f)
        g_pCameraManager->SetCurrentCamera(CameraState::FIRSTPERSON);
    ThirdPersonCamera::Update();
}
CameraFPToTP::CameraFPToTP()
    : ThirdPersonCamera()
    , m_vel(0.0f)
{
    m_cameraState = CameraState::FP2TP;
}

CameraFPToTP::~CameraFPToTP()
{
}

void CameraFPToTP::Init()
{
    m_distance = TP_DISTANCE-1.0f;
    m_vel = 4.0f;
    //m_fovY = ?;    //Unknown sight
    ThirdPersonCamera::Init();
}

void CameraFPToTP::Update()
{
    const auto dt = g_pTimeManager->GetDeltaTime();
    m_vel -= dt * 5.0f;
    m_distance += m_vel * dt;
    //m_fovY += m_vel * dt*0.3f;
    //D3DXMatrixPerspectiveFovLH(&m_matProj, m_fovY, m_aspect, 1, 1000);
    //g_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
    if (m_distance>= TP_DISTANCE-0.01f)
        g_pCameraManager->SetCurrentCamera(CameraState::THIRDPERSON);
    ThirdPersonCamera::Update();
}

//--------------------------------------------

CameraKyunChak::CameraKyunChak()
    :ThirdPersonCamera()
    ,m_vel(0.0f)
{
    m_cameraState = CameraState::KYUNCHAK;
}

CameraKyunChak::~CameraKyunChak()
{
}

void CameraKyunChak::Init()
{
    m_distance = TP_DISTANCE;
    m_basePosY = TP_BASEPOSY;
    ThirdPersonCamera::Init();
}

void CameraKyunChak::Update()
{
    const auto dt = g_pTimeManager->GetDeltaTime();
    const float factor = 5.0f;
    bool bR_buttonStay = g_pKeyManager->IsStayKeyDown(VK_RBUTTON);
    bool bR_buttonUp = g_pKeyManager->IsOnceKeyUp(VK_RBUTTON);
    
    if (bR_buttonStay)//R_button�� ������ ������
    {
        if (m_distance >= TP_DISTANCE - factor)//�������
        {
            m_vel += dt * factor;
            m_distance -= m_vel * dt;
            m_basePosY -= m_vel * dt * 0.5f;
        }
    }
    else//R_button�� ����������
    {
        //bR_buttonUp true, distance�� �ణ ������/ �� (���� Ŭ���� ��� ��������)(�������� �Ѿ)
        if (bR_buttonUp&&m_distance >= TP_DISTANCE-1.0f)
        {
            //!!! ������ �̰����� ĳ���Ͱ� ��� �ִ� �����ۿ� ����(2����,4���� no ���� ��) �ٲپ� �ִ� �ڵ带 ������ �Ѵ�.
            //g_pCameraManager->SetCurrentCamera(CameraState::FIRSTPERSON);
            //g_pCameraManager->SetCurrentCamera(CameraState::SCOPE2X);
            g_pCameraManager->SetCurrentCamera(CameraState::SCOPE4X);
        }
        else//�ƴѰ�� ��� �ٿ��ִٰ� ���� �ٴް� �Ǹ� TP�� �ٲ�
        {
            if (m_distance <= TP_DISTANCE - 0.15f)
            {
                m_vel -= dt * factor;
                m_distance += m_vel * dt;
                m_basePosY += m_vel * dt * 0.5f;
            }
            else
            {
                g_pCameraManager->SetCurrentCamera(CameraState::THIRDPERSON);
            }
        }

    }
    //if (temp2)
    //{
    //    g_pCameraManager->SetCurrentCamera(CameraState::FIRSTPERSON);
    //}

    //Debug->AddText("m_vel : ");
    //Debug->AddText(m_vel);
    Debug->AddText("   m_distance : ");
    Debug->AddText(m_distance);
    //Debug->AddText("   m_basePosY : ");
    //Debug->AddText(m_basePosY);
    Debug->EndLine();
    ThirdPersonCamera::Update();
}

//Debug->AddText(dt);
//Debug->EndLine();
//m_currTime += dt;
//D3DXVec2Lerp(&m_accelarate, &D3DXVECTOR2(1.0f, 0.0f), &D3DXVECTOR2(0.0f, 0.0f), m_currTime / m_totalTime);
//if (m_accelarate.x <= 0.0f)
//    g_pCameraManager->SetCurrentCamera(CameraState::THIRDPERSON);
//    
//const float factor = 0.1f;
//m_vel += m_accelarate.x * factor;
//m_distance += m_vel * dt;