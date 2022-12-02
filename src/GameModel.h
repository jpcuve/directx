#pragma once

#include <vector>
#include "stdafx.h"

class GameModel {
public:
    GameModel();
    FLOAT GetHeight(DirectX::XMFLOAT2 &position);
    std::vector<DirectX::XMFLOAT3> LocalMap(DirectX::XMFLOAT2 &position);
private:
    FLOAT m_playgroundEdge {60.0};
    size_t m_playgroundResolution {10};
    size_t m_surfaceResolution {5};
//    FLOAT m_surfaceEdge {30.0};
    FLOAT m_maxHeight {10.0};
    std::vector<FLOAT> m_heightMap;
    [[nodiscard]] size_t AdjustCoordinate(long c) const;
    [[nodiscard]] long LowHeightCoordinate(FLOAT c) const;
};
