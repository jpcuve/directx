#pragma once

#include <map>
#include <string>
#include <vector>
#include "Mesh.h"

enum RegistryKey {
    CUBE,
    SHIP,
    PLAYGROUND,
};

struct RegistryValue {
    size_t start {0};
    size_t size {0};
};

class Registry {
    std::map<RegistryKey, std::vector<RegistryValue>> m_entries;
public:
    void Init();
    std::vector<VertexPositionNormalColor> Load(size_t playgroundEdge, size_t surfaceExtent);
    void Done();
    void AddEntry(RegistryKey key, RegistryValue value);
    RegistryValue& GetSingleEntry(RegistryKey key);
    std::vector<RegistryValue>& GetEntry(RegistryKey key);
};
