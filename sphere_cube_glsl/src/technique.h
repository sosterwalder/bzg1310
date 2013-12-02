#ifndef TECHNIQUE_H
#define	TECHNIQUE_H

class Technique
{
public:

    Technique();
    ~Technique();

    virtual bool Init();
    void Enable();

protected:

    bool AddShader(GLenum ShaderType, std::string strShaderPath);
    bool Finalize();

    GLint GetUniformLocation(const char* pUniformName);

private:

    GLuint m_shaderProg;

    typedef std::list<GLuint> ShaderObjList;
    ShaderObjList m_shaderObjList;
};

#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF

#endif
