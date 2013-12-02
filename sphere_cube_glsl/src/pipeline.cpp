#include "engine.h"

const Matrix4f& Pipeline::GetWorldTransformation()
{
    Matrices matrices = PrepareMatrices();

    m_mat4WorldTransformation = 
        matrices.mat4TranslationTransformation *
        matrices.mat4RotationTransformation; 

    return m_mat4WorldTransformation;
}

const Matrix4f& Pipeline::GetWorldTransformation(const Matrix4f& mat4Matrix)
{
    Matrices matrices = PrepareMatrices();

    m_mat4WorldTransformation = 
        mat4Matrix *
        matrices.mat4TranslationTransformation *
        matrices.mat4RotationTransformation;

    return m_mat4WorldTransformation;
}

const Matrix4f& Pipeline::GetWorldTransformationAroundPivot()
{
    Matrices matrices = PrepareMatrices();

    m_mat4WorldTransformation = 
        matrices.mat4RotationTransformation *
        matrices.mat4TranslationTransformation;

    return m_mat4WorldTransformation;
}


const Matrix4f& Pipeline::GetWorldTransformationAroundPivot(const Matrix4f& mat4Matrix)
{
    Matrices matrices = PrepareMatrices();

    m_mat4WorldTransformation = 
        mat4Matrix *
        matrices.mat4RotationTransformation *
        matrices.mat4TranslationTransformation; 

    return m_mat4WorldTransformation;
}

const Matrix4f& Pipeline::GetWorldPerspectiveTransformation(const Matrix4f& mat4WorldTransformation)
{
    Matrices matrices = PrepareWorldMatrices();


    m_mat4WorldPerspectiveTransformation =
        matrices.mat4ScaleTransformation *
        matrices.mat4TranslationTransformation *
        mat4WorldTransformation *
        matrices.mat4RotationTransformation;

    return m_mat4WorldPerspectiveTransformation;
}

Pipeline::Matrices Pipeline::PrepareMatrices()
{
    Matrices matrices;
    Matrix4f mat4ScaleTransformation, mat4RotationTransformation, mat4TranslationTransformation;

    mat4ScaleTransformation.InitScaleTransform(
        m_vec3Scale.x, 
        m_vec3Scale.y, 
        m_vec3Scale.z
    );

    mat4RotationTransformation.InitRotateTransform(
        m_vec3RotationInfo.x,
        m_vec3RotationInfo.y,
        m_vec3RotationInfo.z
    );

    mat4TranslationTransformation.InitTranslationTransform(
        m_vec3WorldPosition.x,
        m_vec3WorldPosition.y,
        m_vec3WorldPosition.z
    );

    matrices.mat4ScaleTransformation        = mat4ScaleTransformation;
    matrices.mat4RotationTransformation     = mat4RotationTransformation;
    matrices.mat4TranslationTransformation  = mat4TranslationTransformation;

    return matrices;
}

Pipeline::Matrices Pipeline::PrepareWorldMatrices()
{
    Matrices matrices;
    Matrix4f mat4CameraTranslationTransformation;
    Matrix4f mat4CameraRotationTransformation;
    Matrix4f mat4PerspectiveProjectionTransformation;

    mat4CameraTranslationTransformation.InitTranslationTransform(
        -m_Camera.vec3Position.x,
        -m_Camera.vec3Position.y,
        -m_Camera.vec3Position.z
    );

    mat4CameraRotationTransformation.InitCameraTransform(
        m_Camera.vec3Target,
        m_Camera.vec3Up
    );
    
    mat4PerspectiveProjectionTransformation.InitPersProjTransform(
        m_PerspectiveProjection.fFov,
        m_PerspectiveProjection.fWidth,
        m_PerspectiveProjection.fHeight,
        m_PerspectiveProjection.fZNear,
        m_PerspectiveProjection.fZFar
    );

    matrices.mat4TranslationTransformation  = mat4CameraTranslationTransformation;
    matrices.mat4ScaleTransformation        = mat4PerspectiveProjectionTransformation;
    matrices.mat4RotationTransformation     = mat4CameraRotationTransformation;

    return matrices;
}
