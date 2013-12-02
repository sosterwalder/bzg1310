#ifndef __TEXTURE_H
#define __TEXTURE_H

class Texture
{
    public:
        Texture(GLenum eTextureTarget, const std::string& strFilename) : m_eTextureTarget(eTextureTarget), m_strFilename(strFilename), m_pImage(nullptr){}

        bool Load();
        void Bind(GLenum eTextureUnit);

    private:
        GLenum m_eTextureTarget;
        std::string m_strFilename;
        GLuint m_uTextureObject;
        Magick::Image* m_pImage;
        Magick::Blob m_blob;
};

#endif
