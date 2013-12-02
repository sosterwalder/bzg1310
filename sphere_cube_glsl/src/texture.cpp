#include "engine.h"

bool Texture::Load()
{
    try
    {
        printf("INFO: Trying to load texture '%s'\n", m_strFilename.c_str());
        m_pImage = new Magick::Image(m_strFilename);
        m_pImage->write(&m_blob, "RGBA");
    }
    catch (Magick::Error& error)
    {
        printf("ERRORS: Error loading texture '%s'\n", m_strFilename.c_str());
        printf("ERROR: Error details: '%s'\n", error.what());

        return false;
    }

    printf("INFO: Loaded texture '%s'\n", m_strFilename.c_str());
    glGenTextures(1, &m_uTextureObject);
    glBindTexture(m_eTextureTarget, m_uTextureObject);
    glTexImage2D(m_eTextureTarget, 0, GL_RGB, m_pImage->columns(), m_pImage->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data());
    glTexParameterf(m_eTextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_eTextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    printf("INFO: Bound texture '%s'\n", m_strFilename.c_str());

    return true;
}

void Texture::Bind(GLenum eTextureUnit)
{
    glActiveTexture(eTextureUnit);
    glBindTexture(m_eTextureTarget, m_uTextureObject);
}
