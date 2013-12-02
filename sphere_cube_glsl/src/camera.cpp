#include "engine.h"
        
const static float STEP_SCALE = 0.1f;
const static int MARGIN = 10;

Camera::Camera(int nWindowWidth, int nWindowHeight)
{
    m_nWindowWidth = nWindowWidth;
    m_nWindowHeight = nWindowHeight;

    m_vec3Position = Vector3f(0.0f, 0.0f, 0.0f);
    m_vec3Target = Vector3f(0.0f, 0.0f, 1.0f);
    m_vec3Target.Normalize();
    m_vec3Up = Vector3f(0.0f, 1.0f, 0.0f);

    Init();
}

Camera::Camera(int nWindowWidth, int nWindowHeight, const Vector3f& vec3Position, const Vector3f& vec3Target, const Vector3f& vec3Up)
{
    m_nWindowWidth = nWindowWidth;
    m_nWindowHeight = nWindowHeight;

    m_vec3Position = vec3Position;
    m_vec3Target = vec3Target;
    m_vec3Target.Normalize();
    m_vec3Up = vec3Up;
    m_vec3Up.Normalize();

    Init();
}

void Camera::Init()
{
    Vector3f vec3HTarget(m_vec3Target.x, 0.0f, m_vec3Target.z);
    vec3HTarget.Normalize();

    if (vec3HTarget.z >= 0.0f)
    {
        if (vec3HTarget.x >= 0.0f)
        {
            m_fAngleH = 360.0f - ToDegree(asin(vec3HTarget.z));
        }
        else
        {
            m_fAngleH = 180.0f + ToDegree(asin(vec3HTarget.z));
        }
    }
    else
    {
        if (vec3HTarget.x >= 0.0f)
        {
            m_fAngleH = ToDegree(asin(-vec3HTarget.z));
        }
        else
        {
            m_fAngleH = 90.0f  + ToDegree(asin(-vec3HTarget.z));
        }
    }

    m_fAngleV = -ToDegree(asin(m_vec3Target.y));

    m_bIsOnUpperEdge = false;
    m_bIsOnLowerEdge = false;
    m_bIsOnLeftEdge = false;
    m_bIsOnRightEdge = false;
}

void Camera::OnRender()
{
    bool bShouldUpdate = false;

    if (m_bIsOnLeftEdge)
    {
        m_fAngleH -= 0.1f;
        bShouldUpdate = true;
    }
    else if (m_bIsOnRightEdge)
    {
        m_fAngleH += 0.1f;
        bShouldUpdate = true;
    }

    if (m_bIsOnUpperEdge) 
    {
        if (m_fAngleV > -90.0f) 
        {
            m_fAngleV -= 0.1f;
            bShouldUpdate = true;
        }
    }
    else if (m_bIsOnLowerEdge) 
    {
        if (m_fAngleV < 90.0f) 
        {
           m_fAngleV += 0.1f;
           bShouldUpdate = true;
        }
    }

    if (bShouldUpdate)
    {
        Update();
    }
}

void Camera::Update()
{
    const Vector3f vec3VAxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    Vector3f vec3View(1.0f, 0.0f, 0.0f);
    vec3View.Rotate(m_fAngleH, vec3VAxis);
    vec3View.Normalize();

    // Rotate the view vector by the vertical angle around the horizontal axis
    Vector3f vec3HAxis = vec3VAxis.Cross(vec3View);
    vec3HAxis.Normalize();
    vec3View.Rotate(m_fAngleV, vec3HAxis);
       
    m_vec3Target = vec3View;
    m_vec3Target.Normalize();

    m_vec3Up = m_vec3Target.Cross(vec3HAxis);
    m_vec3Up.Normalize();
}
