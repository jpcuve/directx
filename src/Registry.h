#pragma once

#include <map>
#include <string>
#include <vector>
#include "Mesh.h"

struct Entry {
    size_t start {0};
    size_t size {0};
};

class Registry {
    std::map<std::string, Entry> m_entries;
public:
    void Init();
    std::vector<VertexPositionNormalColor> Load();
    void Done();
};
