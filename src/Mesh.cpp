//
// Created by jpc on 26/11/2022.
//

#include "Mesh.h"
#include "StanfordParser.h"
#include <stdexcept>
#include <vcruntime_string.h>

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

class MeshHandler: public StanfordHandler {
public:
    std::vector<VertexPositionColor> m_vertices;
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
        VertexPositionColor vertex {};
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

Mesh Mesh::FromStanford(const std::vector<byte>& data) {
    std::vector<VertexPositionColor> vertices;
    std::vector<Triangle> triangles;
    StanfordParser parser {data};
    MeshHandler handler;
    parser.Parse(handler);
    return {handler.m_vertices, handler.m_triangles};
}
