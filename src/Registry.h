#pragma once

#include <map>
#include <string>
#include <vector>
#include "Mesh.h"

enum RegistryKey {
    CUBE,
    SHIP,
};

struct RegistryValue {
    size_t start {0};
    size_t size {0};
};

class Registry {
    std::map<RegistryKey, RegistryValue> m_entries;
public:
    void Init();
    std::vector<VertexPositionNormalColor> Load();
    void Done();
    RegistryValue& GetEntry(RegistryKey key);
};
