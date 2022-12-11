//
// Created by jpc on 05/12/2022.
//

#include "Registry.h"
#include "Mesh.h"

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
