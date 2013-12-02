#include "engine.h"

Mesh::MeshEntry::MeshEntry()
{
    m_uVertexBuffer     = INVALID_OGL_VALUE;
    m_uIndexBuffer      = INVALID_OGL_VALUE;
    m_nNumIndices       = 0;
    m_nMaterialIndex    = 0;

};

Mesh::MeshEntry::~MeshEntry()
{
    if (m_uVertexBuffer != INVALID_OGL_VALUE)
    {
        glDeleteBuffers(1, &m_uVertexBuffer);
    }

    if (m_uIndexBuffer != INVALID_OGL_VALUE)
    {
        glDeleteBuffers(1, &m_uIndexBuffer);
    }
}

bool Mesh::MeshEntry::Init(const std::vector<Vertex>& vecVertices, const std::vector<unsigned int>& vecIndices)
{
    m_nNumIndices = vecIndices.size();

    glGenBuffers(1, &m_uVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_uVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vecVertices.size(), &vecVertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &m_uIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_nNumIndices, &vecIndices[0], GL_STATIC_DRAW);

    return true;
}

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
    Clear();
}

void Mesh::Clear()
{
    for (unsigned int i = 0; i < m_vecTextures.size(); i++)
    {
        // BREAKPOINT
        raise(SIGINT);
        SAFE_DELETE(m_vecTextures[i]);
    }
}

bool Mesh::LoadMesh(const std::string& strFilename)
{
    Clear();

    bool bIsLoaded = false;
    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(strFilename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if (pScene)
    {
        bIsLoaded = InitFromScene(pScene, strFilename);
        printf("INFO: Loaded mesh '%s'\n", strFilename.c_str());
    }
    else {
        printf("Error parsing '%s': '%s'\n", strFilename.c_str(), importer.GetErrorString());
    }

    return bIsLoaded;
}

bool Mesh::InitFromScene(const aiScene* pScene, const std::string& strFilename)
{
    m_vecEntries.resize(pScene->mNumMeshes);
    m_vecTextures.resize(pScene->mNumMaterials);

    for (unsigned int i = 0; i < m_vecEntries.size(); i++)
    {
        const aiMesh* pAiMesh = pScene->mMeshes[i];
        printf("INFO: Found mesh '%s'\n", pAiMesh->mName.C_Str());
        InitMesh(i, pAiMesh);
        printf("INFO: Initialized mesh '%s'\n", pAiMesh->mName.C_Str());
    }

    return InitMaterials(pScene, strFilename);
}

bool Mesh::InitMesh(unsigned int uIndex, const aiMesh* pAiMesh) 
{
    m_vecEntries[uIndex].m_nMaterialIndex = pAiMesh->mMaterialIndex;

    std::vector<Vertex> vecVertices;
    std::vector<unsigned int> vecIndices;

    const aiVector3D vec3Zero(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < pAiMesh->mNumVertices; i++)
    {
        const aiVector3D* pVecPosition              = &(pAiMesh->mVertices[i]);
        const aiVector3D* pVecNormal                = &(pAiMesh->mNormals[i]);
        const aiVector3D* pVecTextureCoordinate     = pAiMesh->HasTextureCoords(0) ? &(pAiMesh->mTextureCoords[0][i]) : & vec3Zero;

        Vertex v(   Vector3f(pVecPosition->x, pVecPosition->y, pVecPosition->z),
                    Vector2f(pVecTextureCoordinate->x, pVecTextureCoordinate->y),
                    Vector3f(pVecNormal->x, pVecNormal->y, pVecNormal->z)
                );

        vecVertices.push_back(v);
    }

    for (unsigned int i = 0; i < pAiMesh->mNumFaces; i++)
    {
        const aiFace& face = pAiMesh->mFaces[i];
        assert(face.mNumIndices == 3);

        vecIndices.push_back(face.mIndices[0]);
        vecIndices.push_back(face.mIndices[1]);
        vecIndices.push_back(face.mIndices[2]);
    }

    m_vecEntries[uIndex].Init(vecVertices, vecIndices);

    return true;
}

bool Mesh::InitMaterials(const aiScene* pScene, const std::string& strFilename)
{
    std::string::size_type slash_index = strFilename.find_last_of("/");
    std::string strDirectory = "";

    if (slash_index == std::string::npos)
    {
        strDirectory = ".";
    }
    else if (slash_index == 0) {
        strDirectory = "/";
    }
    else {
        strDirectory = strFilename.substr(0, slash_index);
    }

    bool bIsInitialized = true;

    for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
    {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_vecTextures[i] = nullptr;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString strPath;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &strPath, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
            {
                std::string strFullPath = strDirectory + "/" + strPath.data;
                m_vecTextures[i] = new Texture(GL_TEXTURE_2D, strFullPath.c_str());
                printf("INFO: Mesh has texture '%s'\n", strFullPath.c_str());

                if (!m_vecTextures[i]->Load())
                {
                    printf("ERROR: Error loading texture '%s'\n", strFullPath.c_str());
                    SAFE_DELETE(m_vecTextures[i]);

                    bIsInitialized = false;
                }
                else
                {
                    printf("INFO: Loaded texture '%s'\n", strFullPath.c_str());
                }
            }
        }

        if (!m_vecTextures[i])
        {
            m_vecTextures[i] = new Texture(GL_TEXTURE_2D, "./data/white.png");

            bIsInitialized = m_vecTextures[i]->Load();
        }

    }

    return bIsInitialized;
}

void Mesh::Render()
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    for (unsigned int i = 0 ; i < m_vecEntries.size() ; i++) 
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vecEntries[i].m_uVertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vecEntries[i].m_uIndexBuffer);

        const unsigned int uMaterialIndex = m_vecEntries[i].m_nMaterialIndex;

        if (uMaterialIndex < m_vecTextures.size() && m_vecTextures[uMaterialIndex])
        {
            m_vecTextures[uMaterialIndex]->Bind(GL_TEXTURE0);
        }

        glDrawElements(GL_TRIANGLES, m_vecEntries[i].m_nNumIndices, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}
