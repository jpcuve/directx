//
// Created by jpc on 05/12/2022.
//

#include <numbers>
#include "Registry.h"
#include "Mesh.h"
#include "OpenSimplexNoise.h"

void Registry::Init() {

}

std::vector<VertexPositionNormalColor> Registry::Load() {
    std::map<RegistryKey, Mesh> meshes {
            {RegistryKey::SHIP, Mesh::Ship()},
            {RegistryKey::CUBE, Mesh::Cube()},
    };
    std::vector<VertexPositionNormalColor> data;
    size_t pos {0};
    for (auto& e: meshes){
        auto vertices { e.second.GetVertices()};
        data.insert(data.end(), vertices.begin(), vertices.end());
        RegistryValue v {pos, vertices.size()};
        AddEntry(e.first, v);
        pos += vertices.size();
    }
    // the playground, build height map on height * height square
    auto width {m_playgroundEdge + m_surfaceExtent + 1};
    auto height {m_playgroundEdge};
    OpenSimplexNoise::Noise noise;
    std::vector<float> heightMap(height * height, 0.0f);
    for (auto j = 0; j < height; j++){
        auto y{static_cast<double>(j) / static_cast<double>(height)} ;
        auto p1 {cos(y * 2.0 * std::numbers::pi)};
        auto p2 {sin(y * 2.0 * std::numbers::pi)};
        for (auto i = 0; i < height; i++){
            auto x {static_cast<double>(i) / static_cast<double>(height)};
            auto p3 {cos(x * 2.0 * std::numbers::pi)};
            auto p4 {sin(x * 2.0 * std::numbers::pi)};
            heightMap[j * height + i] = static_cast<float>(noise.eval(p1, p2, p3, p4));
        }
    }
    // take strips, with width
    for (auto j = 0; j < height; j++){
        auto jPlusOne {(j + 1) % height};
        std::vector<DirectX::XMFLOAT2> heightStrip;
        for (auto i = 0; i < width; i++){
            auto ri {i % height};
            DirectX::XMFLOAT2 pair {heightMap[j * height + ri], heightMap[jPlusOne * height + ri]};
            heightStrip.push_back(pair);
        }
        auto mesh = Mesh::FromHeightStrip(heightStrip);
        auto vertices { mesh.GetVertices()};
        data.insert(data.end(), vertices.begin(), vertices.end());
        RegistryValue v {pos, vertices.size()};
        AddEntry(RegistryKey::PLAYGROUND, v);
        pos += vertices.size();
    }
    return data;
}

RegistryValue& Registry::GetSingleEntry(RegistryKey key){
    return GetEntry(key)[0];
}

std::vector<RegistryValue> &Registry::GetEntry(RegistryKey key) {
    return m_entries[key];
}

void Registry::AddEntry(RegistryKey key, RegistryValue value) {
    if (!m_entries.contains(key)){
        std::vector<RegistryValue> v;
        m_entries.insert({key, v});
    }
    auto& v = m_entries[key];
    v.push_back(value);
}
