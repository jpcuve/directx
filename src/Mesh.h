#pragma once

#include <stdafx.h>
#include <utility>
#include <vector>
#include <string>
#include <array>

struct VertexPositionNormalColor {
    DirectX::XMFLOAT3 position {0.0, 0.0, 0.0};
    DirectX::XMFLOAT3 normal {0.0, 0.0, 0.0};
    std::array<byte, 4> color {0, 0, 0, 1};
};

struct Triangle {
    std::array<unsigned int, 3> indices {0, 0, 0};
};

class Mesh {
public:
    Mesh(std::vector<VertexPositionNormalColor> vertices, std::vector<Triangle> triangles): m_vertices{std::move(vertices)}, m_triangles{std::move(triangles)}{};
    std::vector<VertexPositionNormalColor> GetVertices();
    static Mesh Ship();
    static Mesh Cube();
//    static Mesh Noise(size_t extent, float surfaceMultiplier, float heightMultiplier);
    static Mesh FromStanford(const std::vector<byte>& data);
    static Mesh FromHeightStrip(const std::vector<DirectX::XMFLOAT2> &heights);
private:
    std::vector<VertexPositionNormalColor> m_vertices;
    std::vector<Triangle> m_triangles;
};
