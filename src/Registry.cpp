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
        m_entries.insert({e.first, v});
        pos += vertices.size();
    }
    return data;
}

RegistryValue &Registry::GetEntry(RegistryKey key) {
    return m_entries[key];
}
