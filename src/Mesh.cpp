//
// Created by jpc on 26/11/2022.
//

#include "Mesh.h"
#include "StanfordParser.h"
#include <stdexcept>
#include <vcruntime_string.h>
#include "OpenSimplexNoise.h"
#include <numbers>
#include <algorithm>
#include <stdexcept>

std::vector<VertexPositionNormalColor> Mesh::GetVertices() {
    std::vector<VertexPositionNormalColor> buffer;
    for (auto& triangle: m_triangles){
        // here we compute the normal to the triangle
        DirectX::XMVECTOR v[3];
        for (int i = 0; i < 3; i++){
            v[i] = DirectX::XMLoadFloat3(&m_vertices[triangle.indices[i]].position);
        }
        auto dir1 = DirectX::XMVectorSubtract(v[1], v[0]);
        auto dir2 = DirectX::XMVectorSubtract(v[0], v[2]);
        auto cross = DirectX::XMVector3Cross(dir1, dir2);
        auto norm = DirectX::XMVector3Normalize(cross);
        for (unsigned int index : triangle.indices){
            DirectX::XMStoreFloat3(&m_vertices[index].normal, norm);
            buffer.push_back(m_vertices[index]);
        }
    }
    return buffer;
}

Mesh Mesh::Ship() {
    std::vector<VertexPositionNormalColor> vertices {
            {DirectX::XMFLOAT3(5, 0, 0), DirectX::XMFLOAT3(0, 0, 0), {0xFF, 0x00, 0x00, 0xFF}},
            {DirectX::XMFLOAT3(0, 0, 2), DirectX::XMFLOAT3(0, 0, 0), {0x00, 0xFF, 0x00, 0xFF}},
            {DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(0, 0, 0), {0x00, 0x00, 0xFF, 0xFF}},
            {DirectX::XMFLOAT3(-5, -5, 0), DirectX::XMFLOAT3(0, 0, 0), {0x80, 0x80, 0x80, 0xFF}},
            {DirectX::XMFLOAT3(-5, 5, 0), DirectX::XMFLOAT3(0, 0, 0), {0x80, 0x80, 0x80, 0xFF}},
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

Mesh Mesh::Cube() {
    std::vector<VertexPositionNormalColor> vertices {
            {DirectX::XMFLOAT3(-0.5f,-0.5f,-0.5f), DirectX::XMFLOAT3(0, 0, 0), {0x00, 0x00, 0x00, 0xFF}},
            {DirectX::XMFLOAT3(-0.5f,-0.5f, 0.5f), DirectX::XMFLOAT3(0, 0, 0), {0x00, 0x00, 0xFF, 0xFF}},
            {DirectX::XMFLOAT3(-0.5f, 0.5f,-0.5f), DirectX::XMFLOAT3(0, 0, 0), {0x00, 0xFF, 0x00, 0xFF}},
            {DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(0, 0, 0), {0x00, 0xFF, 0xFF, 0xFF}},

            {DirectX::XMFLOAT3(0.5f,-0.5f,-0.5f), DirectX::XMFLOAT3(0, 0, 0), {0xFF, 0x00, 0x00, 0xFF}},
            {DirectX::XMFLOAT3(0.5f,-0.5f, 0.5f), DirectX::XMFLOAT3(0, 0, 0), {0xFF, 0x00, 0xFF, 0xFF}},
            {DirectX::XMFLOAT3(0.5f, 0.5f,-0.5f), DirectX::XMFLOAT3(0, 0, 0), {0xFF, 0xFF, 0x00, 0xFF}},
            {DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(0, 0, 0), {0xFF, 0xFF, 0xFF, 0xFF}},
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

class MeshHandler: public StanfordHandler {
public:
    std::vector<VertexPositionNormalColor> m_vertices;
    std::vector<Triangle> m_triangles;
    void Invalid() override {}
    void Ply() override {}
    void Format(std::string_view &form, std::string_view &version) override {}
    void Element(std::string_view &name, int quantity) override {}
    void Property(std::string_view &dataType, std::string_view &name) override {}
    void PropertyList(std::string_view &countType, std::string_view &dataType, std::string_view &name) override {}
    void EndHeader() override {}
    void DataFixed(std::string &name, size_t index, const std::vector<byte> &data, size_t lo, size_t hi) override {
        if (name != "vertex" || hi - lo != 16){
            throw std::runtime_error("Unexpected data chunk");
        }
        VertexPositionNormalColor vertex {};
        memcpy(&vertex.position, &data[lo], sizeof(vertex.position));
        memcpy(&vertex.color, &data[lo + sizeof(DirectX::XMFLOAT3)], sizeof(vertex.color));
        m_vertices.push_back(vertex);
    }
    void DataVariable(std::string &name, size_t index, size_t count, const std::vector<byte> &data, size_t lo, size_t hi) override {
        if (name != "face" || hi - lo != 12 || count != 3){
            throw std::runtime_error("Unexpected data chunk");
        }
        Triangle triangle {};
        auto len = sizeof(unsigned int);
        memcpy(&triangle.indices[0], &data[lo], len);
        memcpy(&triangle.indices[1], &data[lo + 2 * len], len);
        memcpy(&triangle.indices[2], &data[lo + len], len);
        m_triangles.push_back(triangle);
    }
};

Mesh Mesh::FromHeightStrip(const std::vector<DirectX::XMFLOAT2>& heights) {
    std::vector<VertexPositionNormalColor> vertices;
    std::vector<Triangle> triangles;
    std::array<byte, 4> color {0xFF, 0xFF, 0xFF, 0xFF};
    UINT pos {0};
    for (auto i = 0; i < heights.size(); i++){
        auto iPlusOne {(i + 1) % heights.size()};
        std::array<VertexPositionNormalColor, 4> vs;
        float x = static_cast<float>(i);
        vs[0].position = DirectX::XMFLOAT3{x, 0, heights[i].x};
        vs[1].position = DirectX::XMFLOAT3{x, 1, heights[i].y};
        vs[2].position = DirectX::XMFLOAT3{x + 1, 1, heights[iPlusOne].y};
        vs[3].position = DirectX::XMFLOAT3{x + 1, 0, heights[iPlusOne].x};
        for (auto& vertex: vs){
            vertex.color = color;
            vertices.push_back(vertex);
        }
        triangles.push_back(Triangle{pos, pos + 1, pos + 2});
        triangles.push_back(Triangle{pos + 2, pos + 3, pos});
        pos += vs.size();
    }
    return {vertices, triangles};
}

Mesh Mesh::FromStanford(const std::vector<byte>& data) {
    std::vector<VertexPositionNormalColor> vertices;
    std::vector<Triangle> triangles;
    StanfordParser parser {data};
    MeshHandler handler;
    parser.Parse(handler);
    return {handler.m_vertices, handler.m_triangles};
}


