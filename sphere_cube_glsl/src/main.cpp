#include "engine.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

class Framework
{
    public:
        Framework()
        {
            m_pEffect                               = nullptr;
            m_pPlane                                = nullptr;
            m_pCube                                 = nullptr;
            m_pSphere                               = nullptr;
            m_pCamera                               = nullptr;
            m_pWindow                               = nullptr;
       
            m_directionalLight.Color                = Vector3f(0.2f, 0.2f, 0.4f);
            m_directionalLight.AmbientIntensity     = 0.7f;
            m_directionalLight.DiffuseIntensity     = 0.21f;
            m_directionalLight.Direction            = Vector3f(1.0f, -1.0, 0.0);
            
            m_bIsRunning                            = false;
        }

        ~Framework()
        {
            SAFE_DELETE(m_pPlane);
            SAFE_DELETE(m_pCube);
            SAFE_DELETE(m_pSphere);
            SAFE_DELETE(m_pCamera);
        }

        bool Init()
        {
            glfwSetErrorCallback(Framework::ErrorCallback);
            
            Vector3f vec3Position(-1.5f, 2.0f, -10.0f);
            Vector3f vec3Target(0.0f, 0.0f, 1.0f);
            Vector3f vec3Up(0.0f, 1.0f, 0.0f);

            /* Initialize the library */
            if (!glfwInit())
            {
                printf("ERROR: Could not initialize GLFW\n");
                exit(EXIT_FAILURE);
            }

            printf("INFO: Initialized GLFW\n");

            //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
            //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

            /* Create a windowed mode window and its OpenGL context */
            m_pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Sneaker system", NULL, NULL);
            if (!m_pWindow)
            {
                printf("ERROR: Could not create window\n");
                glfwTerminate();
                exit(EXIT_FAILURE);
            }
            printf("INFO: Created window\n");

            /* UGLY! But we need framework in here */
            glfwSetWindowUserPointer(m_pWindow, this);
            
            glfwSetKeyCallback(m_pWindow, Framework::KeyCallback);

            /* Make the window's context current */
            glfwMakeContextCurrent(m_pWindow);
            printf("INFO: Created context\n");
            
            /* Initialize GLEW */
            GLenum eError = glewInit();
            if (eError != GLEW_OK)
            {
                printf("ERROR: Could not initialize GLEW: '%s'\n", glewGetErrorString(eError));
                glfwTerminate();
                exit(EXIT_FAILURE);
            }
            printf("INFO: Initialized GLEW\n");
            printf("INFO: OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
            
            m_pCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, vec3Position, vec3Target, vec3Up);
            
            m_pEffect = new LightingTechnique();
            if (!m_pEffect->Init()) {
                printf("ERROR: Could not initialize lighting technique\n");
                glfwTerminate();
                exit(EXIT_FAILURE);
            }
            m_pEffect->Enable();
            m_pEffect->SetTextureUnit(0);

            // Load meshes
            m_pPlane = new Mesh();
            m_pPlane->LoadMesh("data/models/plane.dae");
            m_pCube = new Mesh();
            m_pCube->LoadMesh("data/models/cube.dae");
            m_pSphere = new Mesh();
            m_pSphere->LoadMesh("data/models/sphere.dae");

            return true;
        }

        void Run()
        {
            PointLight pl[2];
            SpotLight sl[2];

            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glFrontFace(GL_CW);
            glCullFace(GL_BACK);
            glEnable(GL_CULL_FACE);
            glEnable(GL_DEPTH_TEST);
            glDisable(GL_LIGHTING);
            m_bIsRunning = true;


            while (m_bIsRunning) {
                float fRotation = (float)glfwGetTime() * 50.0f;

                m_pCamera->OnRender();

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // First point light 
                pl[0].DiffuseIntensity = 0.25f;
                pl[0].Color = Vector3f(1.0f, 0.5f, 0.0f);
                pl[0].Position = Vector3f(3.0f, 1.0f, 18.0f);
                pl[0].Attenuation.Linear = 0.1f;

                // Second point light
                pl[1].DiffuseIntensity = 0.25f;
                pl[1].Color = Vector3f(0.0f, 0.5f, 1.0f);
                pl[1].Position = Vector3f(7.0f, 1.0f, -4.0f);
                pl[1].Attenuation.Linear = 0.1f;
                
                // Add point lights
                m_pEffect->SetPointLights(2, pl);

                // First spot light
                sl[0].DiffuseIntensity = 0.9f;
                sl[0].Color = Vector3f(0.0f, 1.0f, 2.0f);
                sl[0].Position = m_pCamera->GetPosition();
                sl[0].Direction = m_pCamera->GetTarget();
                sl[0].Attenuation.Linear = 0.1f;
                sl[0].Cutoff = 10.0f;

                // Second spot light
                sl[1].DiffuseIntensity = 0.75f;
                sl[1].Color = Vector3f(0.0f, 0.5f, 1.0f);
                sl[1].Position = Vector3f(7.0f, 1.0f, -2.0f);
                sl[1].Attenuation.Linear = 0.1f;
                
                // Add spot lights
                m_pEffect->SetSpotLights(1, sl);

                // Effects
                m_pEffect->SetDirectionalLight(m_directionalLight);
                m_pEffect->SetEyeWorldPos(m_pCamera->GetPosition());
                m_pEffect->SetMatSpecularIntensity(0.9f);
                m_pEffect->SetMatSpecularPower(0.8f);

                // Position of objects
                Vector3f vec3Pos;
                // Rotation of objects
                Vector3f vec3Rot;
                // Scale of objects
                Matrix4f mat4Scale;

                // Pipieline
                Pipeline p;
                p.SetPerspectiveProjection(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);
                p.SetCamera(m_pCamera->GetPosition(), m_pCamera->GetTarget(), m_pCamera->GetUp());
                
                // Plane
                vec3Pos = Vector3f(0.0f, -2.0f, 0.0f);
                vec3Rot = Vector3f(0.0f, 0.0f, 0.0f);
                mat4Scale.InitScaleTransform(
                    100.0f,
                    0.1f,
                    100.0f
                );
                this->RenderMesh(p, m_pPlane, vec3Pos, vec3Rot, mat4Scale);

                // Cuboid
                vec3Pos = Vector3f(0.0f, 0.0f, 0.0f);
                vec3Rot = Vector3f(0.0f, 0.0f, 0.0f);
                mat4Scale.InitScaleTransform(
                    1.0f,
                    1.0f,
                    1.0f
                );
                this->RenderMesh(p, m_pCube, vec3Pos, vec3Rot, mat4Scale);

                // Re-set scaling for spheres
                mat4Scale.InitScaleTransform(
                    0.4f,
                    0.4f,
                    0.4f
                ); 

                // First sphere
                vec3Pos = Vector3f(-1.0f, 1.0f, -1.0f);
                vec3Rot = Vector3f(0.0f, (fRotation * 3.0f) * (-1), 0.0f);
                this->RenderMesh(p, m_pSphere, vec3Pos, vec3Rot, mat4Scale);
                
                // Second sphere
                vec3Pos = Vector3f(1.0f, 1.0f, -1.0f);
                vec3Rot = Vector3f(0.0f, (fRotation * 3.0f) * (-1), 0.0f);
                this->RenderMesh(p, m_pSphere, vec3Pos, vec3Rot, mat4Scale);

                // Third sphere
                vec3Pos = Vector3f(1.0f, 1.0f, 1.0f);
                vec3Rot = Vector3f(0.0f, (fRotation * 3.0f) * (-1), 0.0f);
                this->RenderMesh(p, m_pSphere, vec3Pos, vec3Rot, mat4Scale);

                // Fourth sphere
                vec3Pos = Vector3f(-1.0f, 1.0f, 1.0f);
                vec3Rot = Vector3f(0.0f, (fRotation * 3.0f) * (-1), 0.0f);
                this->RenderMesh(p, m_pSphere, vec3Pos, vec3Rot, mat4Scale);

                // Fifth sphere
                vec3Pos = Vector3f(-1.0f, -1.0f, -1.0f);
                vec3Rot = Vector3f(0.0f, (fRotation * 3.0f) * (-1), 0.0f);
                this->RenderMesh(p, m_pSphere, vec3Pos, vec3Rot, mat4Scale);

                // Sixth sphere
                vec3Pos = Vector3f(1.0f, -1.0f, -1.0f);
                vec3Rot = Vector3f(0.0f, (fRotation * 3.0f) * (-1), 0.0f);
                this->RenderMesh(p, m_pSphere, vec3Pos, vec3Rot, mat4Scale);
                
                // Seventh sphere
                vec3Pos = Vector3f(1.0f, -1.0f, 1.0f);
                vec3Rot = Vector3f(0.0f, (fRotation * 3.0f) * (-1), 0.0f);
                this->RenderMesh(p, m_pSphere, vec3Pos, vec3Rot, mat4Scale);
                
                // Eighth sphere
                vec3Pos = Vector3f(-1.0f, -1.0f, 1.0f);
                vec3Rot = Vector3f(0.0f, (fRotation * 3.0f) * (-1), 0.0f);
                this->RenderMesh(p, m_pSphere, vec3Pos, vec3Rot, mat4Scale);

                glfwSwapBuffers(m_pWindow);
                glfwPollEvents();
            }
        }

        void Terminate()
        {
            glfwDestroyWindow(m_pWindow);
            glfwTerminate();
            exit(EXIT_SUCCESS);
        }

        void SetIsRunning(bool bIsRunning)
        {
            m_bIsRunning = bIsRunning;
        }

    private:
        static void ErrorCallback(int error, const char* description)
        {
            fputs(description, stderr);
        }
        
        static void KeyCallback(GLFWwindow* pWindow, int nKey, int nScancode, int nAction, int nMods)
        {
            /* Unwrap user specific pointer */
            Framework* pFramework = (Framework*)glfwGetWindowUserPointer(pWindow);

            if (nKey == GLFW_KEY_ESCAPE && nAction == GLFW_PRESS) 
            {
                glfwSetWindowShouldClose(pWindow, GL_TRUE);

                if (pFramework)
                {
                    printf("INFO: Set framework to stop, terminating\n");
                    pFramework->SetIsRunning(false);
                }

            }
        }

        void RenderMesh(Pipeline p, Mesh* pMesh, const Vector3f& vec3Position, const Vector3f& vec3Rotation, const Matrix4f& mat4Scale)
        {
            p.SetWorldPosition(vec3Position.x, vec3Position.y, vec3Position.z);
            p.SetRotation(vec3Rotation.x, vec3Rotation.y, vec3Rotation.z);

            Matrix4f mat4WorldTransformation = p.GetWorldTransformation();
            m_pEffect->SetWorldMatrix(mat4WorldTransformation * mat4Scale);

            Matrix4f mat4WorldPerspectiveTransformation = p.GetWorldPerspectiveTransformation(mat4WorldTransformation);
            m_pEffect->SetWVP(mat4WorldPerspectiveTransformation * mat4Scale);

            pMesh->Render();
        }

        LightingTechnique* m_pEffect;
        DirectionalLight m_directionalLight;
        Mesh* m_pPlane;
        Mesh* m_pCube;
        Mesh* m_pSphere;
        Camera* m_pCamera;
        GLFWwindow* m_pWindow;


        bool m_bIsRunning;
};


int main(void)
{
    Framework* pFramework = new Framework();

    if (!pFramework->Init()) {
        return 1;
    }

    pFramework->Run();

    SAFE_DELETE(pFramework);

    return 0;
}
