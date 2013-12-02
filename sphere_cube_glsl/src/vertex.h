#ifndef __VERTEX_H
#define __VERTEX_H

class Vertex
{
    public:
        Vertex() {}
        Vertex(const Vector3f& vec3Position, const Vector2f& vec2Texture, const Vector3f& vec3Normal) : m_vec3Position(vec3Position), m_vec2Texture(vec2Texture), m_vec3Normal(vec3Normal) {}

    private:
        Vector3f m_vec3Position;
        Vector2f m_vec2Texture;
        Vector3f m_vec3Normal;
};

#endif
