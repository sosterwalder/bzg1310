#ifndef __CAMERA_H
#define __CAMERA_H

class Camera
{
    public:
        Camera(int nWindowWidth, int nWindowHeight);
        Camera(int nWindowWidth, int nWindowHeight, const Vector3f& vec3Position, const Vector3f& vec3Target, const Vector3f& vec3Up);
        ~Camera() {}

        void OnRender();
        const Vector3f& GetPosition() const
        {
            return m_vec3Position;
        }
        const Vector3f& GetTarget() const
        {
            return m_vec3Target;
        }
        const Vector3f& GetUp() const
        {
            return m_vec3Up;
        }

    private:
        void Init();
        void Update();

        Vector3f m_vec3Position;
        Vector3f m_vec3Target;
        Vector3f m_vec3Up;

        int m_nWindowWidth;
        int m_nWindowHeight;

        float m_fAngleH;
        float m_fAngleV;

        bool m_bIsOnUpperEdge;
        bool m_bIsOnLowerEdge;
        bool m_bIsOnLeftEdge;
        bool m_bIsOnRightEdge;
};

#endif
