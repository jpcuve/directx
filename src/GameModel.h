#pragma once

#include <vector>
#include "stdafx.h"

class GameModel {
public:
    GameModel();
    FLOAT GetHeight(FLOAT x, FLOAT y);
private:
    FLOAT m_playgroundEdge {60.0};
    FLOAT m_surfaceEdge {30.0};
    FLOAT m_maxHeight {10.0};
    size_t m_heightResolution {60};
    std::vector<FLOAT> m_heightMap;
    DirectX::XMFLOAT3 m_player {43, 19, 0};
    size_t ClampCoordinate(long c) const;
    long LowHeightCoordinate(FLOAT c) const;
};
