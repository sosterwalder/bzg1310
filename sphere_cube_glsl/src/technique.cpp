#include "engine.h"

Technique::Technique()
{
    m_shaderProg = 0;
}


Technique::~Technique()
{
    // Delete the intermediate shader objects that have been added to the program
    // The list will only contain something if shaders were compiled but the object itself
    // was destroyed prior to linking.
    for (ShaderObjList::iterator it = m_shaderObjList.begin() ; it != m_shaderObjList.end() ; it++)
    {
        glDeleteShader(*it);
    }

    if (m_shaderProg != 0)
    {
        glDeleteProgram(m_shaderProg);
        m_shaderProg = 0;
    }
}


bool Technique::Init()
{
    m_shaderProg = glCreateProgram();

    if (m_shaderProg == 0) {
        fprintf(stderr, "ERROR: Error creating shader program\n");
        return false;
    }

    return true;
}

// Use this method to add shaders to the program. When finished - call finalize()
bool Technique::AddShader(GLenum ShaderType, std::string strShaderPath)
{
    GLuint ShaderObj = glCreateShader(ShaderType);
    std::string strShaderType = "Unknown";

    if (ShaderType == GL_VERTEX_SHADER) {
        strShaderType = "Vertex";
    }
    else if (ShaderType == GL_FRAGMENT_SHADER) {
        strShaderType = "Fragment";
    }


    if (ShaderObj == 0) {
        fprintf(stderr, "ERROR: Error creating shader type %s\n", strShaderType.c_str());
        return false;
    }

    // Save the shader object - will be deleted in the destructor
    m_shaderObjList.push_back(ShaderObj);

    // Load shader from file
    const GLchar* pShaderText = Util::File2String(strShaderPath.c_str());

    glShaderSource(ShaderObj, 1, &pShaderText, NULL);

    glCompileShader(ShaderObj);

    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

    //glGetShaderiv(m_HandleVertex, GL_INFO_LOG_LENGTH, &bufflen);

    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "ERROR: Error compiling shader type %s: '%s'\n", strShaderType.c_str(), InfoLog);
        return false;
    }

    glAttachShader(m_shaderProg, ShaderObj);

    return true;
}


// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Technique::Finalize()
{
    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };

    glLinkProgram(m_shaderProg);

    glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        return false;
	}

    glValidateProgram(m_shaderProg);
    glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        return false;
    }

    // Delete the intermediate shader objects that have been added to the program
    for (ShaderObjList::iterator it = m_shaderObjList.begin() ; it != m_shaderObjList.end() ; it++)
    {
        glDeleteShader(*it);
    }

    m_shaderObjList.clear();

    return true;
}


void Technique::Enable()
{
    glUseProgram(m_shaderProg);
}


GLint Technique::GetUniformLocation(const char* pUniformName)
{
    GLint Location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (Location == 0xFFFFFFFF)
    {
        fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return Location;
}
