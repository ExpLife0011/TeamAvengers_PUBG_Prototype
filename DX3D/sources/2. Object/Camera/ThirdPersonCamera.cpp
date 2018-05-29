#include "stdafx.h"
#include "ThirdPersonCamera.h"
#include "Ray.h"


ThirdPersonCamera::ThirdPersonCamera()
    :ICamera()
    , m_pCamBackCheckingRay(nullptr)
{
    m_cameraState = CameraState::THIRDPERSON;
    m_distance = TP_DISTANCE;
    m_basePosX = TP_BASEPOSX;
    m_basePosY = TP_BASEPOSY;
    m_fovY = D3DX_PI *(80.0f / 180.0f);    //80 Degrees TP sight
    m_pCamBackCheckingRay = new Ray();
}

ThirdPersonCamera::~ThirdPersonCamera()
{
    SAFE_DELETE(m_pCamBackCheckingRay);
}

void ThirdPersonCamera::Init()
{
    
    ICamera::Init();
}

void ThirdPersonCamera::Update()
{
    //ALTkey�� ������ �ֺ��� �� �� �ִ� ���
    m_isALTbuttonStay = g_pKeyManager->IsStayKeyDown(VK_MENU);//Alt����� Icamera update�� �ѹ��� ���� ������ �̷��� �־����ϴ�.
    //if (m_isALTbuttonStay)
    //{
    //}
    //else//alt�� ������ �ʰ� �Ǹ� ĳ���Ͱ� �ٶ󺸰� �ִ� rotation ���� �ʱ�ȭ
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
        g_pCameraManager->SetCurrentCamera(CameraState::KYUNCHAK);

    //Ray ī�޶� ray�� �ڷ� ��Ƽ� ī�޶� m_distance�� �����ϴ� �κ�
    //CamBackDistResizeing();

    ICamera::Update();
}

void ThirdPersonCamera::CamBackDistResizeing()
{
    D3DXVECTOR3 camBackDir = m_eye - m_lookAt;      //������ �˾Ƴ�
    D3DXVec3Normalize(&camBackDir, &camBackDir);    //����ȭ
    //ray�� pos�� player�� distance�� ����, ��� ������ ī�޶� ��������
    m_pCamBackCheckingRay->SetRay(m_eyeBackRay, camBackDir);

    float ray2wallDist;//m_eyeRay 2(to) �������� �Ÿ�
    vector<D3DXVECTOR3> wall = g_pCameraManager->GetWall(); //���� ������ ���� �ִ� vector, ������ call by value �� �ް� ������, ���߿��� reference�� �޾Ƽ� ������ ���� ����
    
    m_distance = TP_DISTANCE; //Ray�� ���� �ɸ��� ���� ��� distance �ʱ�ȭ
    for (size_t i = 0u; i < wall.size(); i+=3)
    {
        //ray�� ���� �ɸ��� �ȴٸ�
        if (m_pCamBackCheckingRay->CalcIntersectTri(&wall[i], &ray2wallDist))
        {
            //�̹��� �ȿ� ��� �ö��� distance���� ����
            if (4.0f <= ray2wallDist && ray2wallDist <= 11)
            {
                m_distance = ray2wallDist - 2.0f;
                break; //for ���� �� ���Ƽ� �������� �ʱ�
            }
        }
    }

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
            g_pCameraManager->SetCurrentCamera(CameraState::FIRSTPERSON);
            //g_pCameraManager->SetCurrentCamera(CameraState::SCOPE2X);
            //g_pCameraManager->SetCurrentCamera(CameraState::SCOPE4X);
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