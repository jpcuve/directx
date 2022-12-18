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
private:
    std::map<RegistryKey, std::vector<RegistryValue>> m_entries;
    size_t m_playgroundEdge;
    size_t m_surfaceExtent;
public:
    Registry(size_t playgroundEdge, size_t surfaceExtent): m_playgroundEdge(playgroundEdge), m_surfaceExtent(surfaceExtent){};
    size_t GetPlaygroundEdge(){ return m_playgroundEdge; }
    size_t GetSurfaceExtent(){ return m_surfaceExtent; }
    void Init();
    std::vector<VertexPositionNormalColor> Load();
    void Done();
    void AddEntry(RegistryKey key, RegistryValue value);
    RegistryValue& GetSingleEntry(RegistryKey key);
    std::vector<RegistryValue>& GetEntry(RegistryKey key);
};
