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
    // the playground, build height map
    OpenSimplexNoise::Noise noise;
    size_t playgroundResolution = 20;
    float maxHeight = 3;
    std::vector<float> heightMap(playgroundResolution * playgroundResolution, 0.0f);
    for (auto i = 0; i < playgroundResolution; i++){
        auto x{static_cast<double>(i) / static_cast<double>(playgroundResolution)} ;
        auto p1 {cos(x * 2.0 * std::numbers::pi)};
        auto p2 {sin(x * 2.0 * std::numbers::pi)};
        for (auto j = 0; j < playgroundResolution; j++){
            auto y {static_cast<double>(j) / static_cast<double>(playgroundResolution)};
            auto p3 {cos(y * 2.0 * std::numbers::pi)};
            auto p4 {sin(y * 2.0 * std::numbers::pi)};
            heightMap[j * playgroundResolution + i] = static_cast<float>(noise.eval(p1, p2, p3, p4)) * maxHeight;
        }
    }
    // take strips
    for (auto j = 0; j < playgroundResolution - 1; j++){
        std::vector<DirectX::XMFLOAT2> heightStrip;
        for (auto i = 0; i < playgroundResolution; i++){
            DirectX::XMFLOAT2 pair {heightMap[j * playgroundResolution + i], heightMap[(j + 1) * playgroundResolution + i]};
            heightStrip.push_back(pair);
        }
        auto mesh = Mesh::FromHeightStrip(heightStrip, 1.0f);
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
