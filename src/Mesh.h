#pragma once

#include <stdafx.h>
#include <utility>
#include <vector>
#include <string>

struct VertexPositionColor {
    DirectX::XMFLOAT3 position {0.0, 0.0, 0.0};
    unsigned char color[4] {0, 0, 0, 1};
};

struct Triangle {
    unsigned int indices[3] {0, 0, 0};
};

class Mesh {
public:
    Mesh(std::vector<VertexPositionColor> vertices, std::vector<Triangle> triangles):m_vertices{std::move(vertices)}, m_triangles{std::move(triangles)}{};
    std::vector<VertexPositionColor> GetVertices();
    static Mesh ship();
    static Mesh cube();
    static Mesh FromStanford(const std::vector<byte>& data);
private:
    std::vector<VertexPositionColor> m_vertices;
    std::vector<Triangle> m_triangles;

};
