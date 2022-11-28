#pragma once

#include <stdafx.h>
#include <utility>
#include <vector>
#include <string>

struct VertexPositionNormalColor {
    DirectX::XMFLOAT3 position {0.0, 0.0, 0.0};
    DirectX::XMFLOAT3 normal {0.0, 0.0, 0.0};
    unsigned char color[4] {0, 0, 0, 1};
};

struct Triangle {
    unsigned int indices[3] {0, 0, 0};
};

class Mesh {
public:
    Mesh(std::vector<VertexPositionNormalColor> vertices, std::vector<Triangle> triangles): m_vertices{std::move(vertices)}, m_triangles{std::move(triangles)}{};
    std::vector<VertexPositionNormalColor> GetVertices();
    static Mesh ship();
    static Mesh cube();
    static Mesh noise(size_t extent, float surfaceMultiplier, float heightMultiplier);
    static Mesh FromStanford(const std::vector<byte>& data);
private:
    std::vector<VertexPositionNormalColor> m_vertices;
    std::vector<Triangle> m_triangles;

};
