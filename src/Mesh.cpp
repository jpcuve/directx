//
// Created by jpc on 26/11/2022.
//

#include "Mesh.h"

std::vector<VertexPositionColor> Mesh::GetVertices() {
    std::vector<VertexPositionColor> buffer;
    for (auto& triangle: m_triangles){
        for (auto& index: triangle.indices){
            auto& vertex = m_vertices[index];
            buffer.push_back(vertex);  // should copy hopefully
        }
    }
    return buffer;
}

Mesh Mesh::ship() {
    std::vector<VertexPositionColor> vertices {
            {DirectX::XMFLOAT3(5, 0, 0), {0xFF, 0x00, 0x00, 0xFF}},
            {DirectX::XMFLOAT3(0, 0, 2), {0x00, 0xFF, 0x00, 0xFF}},
            {DirectX::XMFLOAT3(0, 0, 0), {0x00, 0x00, 0xFF, 0xFF}},
            {DirectX::XMFLOAT3(-5, -5, 0), {0x80, 0x80, 0x80, 0xFF}},
            {DirectX::XMFLOAT3(-5, 5, 0), {0x80, 0x80, 0x80, 0xFF}},
    };
    std::vector<Triangle> triangles {
            {0, 3, 1},
            {0, 1, 4},
            {0, 2, 3},
            {0, 4, 2},
            {1, 3, 2},
            {1, 2, 4},
    };
    return {vertices, triangles};
}

Mesh Mesh::cube() {
    std::vector<VertexPositionColor> vertices {
            {DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f), {0x00, 0x00, 0x00, 0xFF}},
            {DirectX::XMFLOAT3(-0.5f,-0.5f, 0.5f), {0x00, 0x00, 0xFF, 0xFF}},
            {DirectX::XMFLOAT3(-0.5f, 0.5f,-0.5f), {0x00, 0xFF, 0x00, 0xFF}},
            {DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), {0x00, 0xFF, 0xFF, 0xFF}},

            {DirectX::XMFLOAT3(0.5f,-0.5f,-0.5f), {0xFF, 0x00, 0x00, 0xFF}},
            {DirectX::XMFLOAT3(0.5f,-0.5f, 0.5f), {0xFF, 0x00, 0xFF, 0xFF}},
            {DirectX::XMFLOAT3(0.5f, 0.5f,-0.5f), {0xFF, 0xFF, 0x00, 0xFF}},
            {DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), {0xFF, 0xFF, 0xFF, 0xFF}},
    };
    std::vector<Triangle> triangles {
            {0,2,1}, // -x
            {1,2,3},
            {4,5,6}, // +x
            {5,7,6},
            {0,1,5}, // -y
            {0,5,4},
            {2,6,7}, // +y
            {2,7,3},
            {0,4,6}, // -z
            {0,6,2},
            {1,3,7}, // +z
            {1,7,5},
    };
    return {vertices, triangles};
}
