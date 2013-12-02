#ifndef __PIPELINE_H
#define __PIPELINE_H

class Pipeline
{
    public:
        Pipeline()
        {
            m_vec3Scale = Vector3f(1.0f, 1.0f, 1.0f);
            m_vec3WorldPosition = Vector3f(0.0f, 0.0f, 0.0f);
            m_vec3RotationInfo = Vector3f(0.0f, 0.0f, 0.0f);
        }
        ~Pipeline() {}

        void SetScale(float fScaleX, float fScaleY, float fScaleZ)
        {
            m_vec3Scale.x = fScaleX;
            m_vec3Scale.y = fScaleY;
            m_vec3Scale.z = fScaleZ;
        }

        void SetWorldPosition(float fX, float fY, float fZ)
        {
            m_vec3WorldPosition.x = fX;
            m_vec3WorldPosition.y = fY;
            m_vec3WorldPosition.z = fZ;
        }
        
        void SetRotation(float fRotationX, float fRotationY, float fRotationZ)
        {
            m_vec3RotationInfo.x = fRotationX;
            m_vec3RotationInfo.y = fRotationY;
            m_vec3RotationInfo.z = fRotationZ;
        }

        void SetPerspectiveProjection(float fFov, float fWidth, float fHeight, float fZNear, float fZFar)
        {
            m_PerspectiveProjection.fFov = fFov;
            m_PerspectiveProjection.fWidth = fWidth;
            m_PerspectiveProjection.fHeight = fHeight;
            m_PerspectiveProjection.fZNear = fZNear;
            m_PerspectiveProjection.fZFar = fZFar;
        }

        void SetCamera(const Vector3f& vec3Position, const Vector3f& vec3Target, const Vector3f& vec3Up)
        {
            m_Camera.vec3Position = vec3Position;
            m_Camera.vec3Target = vec3Target;
            m_Camera.vec3Up = vec3Up;
        }

        const Matrix4f& GetWorldTransformation();
        const Matrix4f& GetWorldTransformation(const Matrix4f& mat4Matrix);
        const Matrix4f& GetWorldTransformationAroundPivot();
        const Matrix4f& GetWorldTransformationAroundPivot(const Matrix4f& mat4Matrix);

        const Matrix4f& GetWorldPerspectiveTransformation(const Matrix4f& mat4WorldTransformation);

    private:
        Vector3f m_vec3Scale;
        Vector3f m_vec3WorldPosition;
        Vector3f m_vec3RotationInfo;

        struct Matrices {
            Matrix4f mat4ScaleTransformation;
            Matrix4f mat4TranslationTransformation;
            Matrix4f mat4RotationTransformation;
        };

        Matrices PrepareMatrices();
        Matrices PrepareWorldMatrices();

        struct {
            float fFov;
            float fWidth;
            float fHeight;
            float fZNear;
            float fZFar;
        } m_PerspectiveProjection;

        struct {
            Vector3f vec3Position;
            Vector3f vec3Target;
            Vector3f vec3Up;
        } m_Camera;

        Matrix4f m_mat4WorldPerspectiveTransformation;
        Matrix4f m_mat4WorldTransformation;
};

#endif
