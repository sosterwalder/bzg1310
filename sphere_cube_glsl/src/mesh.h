#ifndef __MESH_H
#define __MESH_H

class Mesh
{
    #define INVALID_MATERIAL 0xFFFFFFF

    public:
        Mesh();
        ~Mesh();

        bool LoadMesh(const std::string& strFilename);
        void Render();

    private:
        bool InitFromScene(const aiScene* pScene, const std::string& strFilename);
        bool InitMesh(unsigned int uIndex, const aiMesh* pAiMesh);
        bool InitMaterials(const aiScene* pScene, const std::string& strFilename);
        void Clear();

        struct MeshEntry {
            MeshEntry();
            ~MeshEntry();

            bool Init(const std::vector<Vertex>& vecVertices, const std::vector<unsigned int>& vecIndices);

            GLuint m_uVertexBuffer;
            GLuint m_uIndexBuffer;
            unsigned int m_nNumIndices;
            unsigned int m_nMaterialIndex;
        };

        std::vector<MeshEntry> m_vecEntries;
        std::vector<Texture*> m_vecTextures;
};

#endif
