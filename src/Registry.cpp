//
// Created by jpc on 05/12/2022.
//

#include "Registry.h"
#include "Mesh.h"

void Registry::Init() {

}

std::vector<VertexPositionNormalColor> Registry::Load() {
    std::map<std::string, Mesh> meshes {
            {"ship", Mesh::Ship()},
            {"cube", Mesh::Cube()},
    };
    std::vector<VertexPositionNormalColor> data;
    size_t pos {0};
    for (auto& e: meshes){
        auto vertices { e.second.GetVertices()};
        data.insert(data.end(), vertices.begin(), vertices.end());
        std::string k {e.first};
        Entry v {pos, vertices.size()};
        m_entries.insert({k, v});
        pos += vertices.size();
    }
    return data;
}
