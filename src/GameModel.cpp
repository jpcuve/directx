//
// Created by jpc on 29/11/2022.
//

#include <numbers>
#include "GameModel.h"
#include "OpenSimplexNoise.h"

GameModel::GameModel() {
    OpenSimplexNoise::Noise noise;
    m_heightMap.resize(m_heightResolution * m_heightResolution, 0.0f);
    for (auto i = 0; i < m_heightResolution; i++){
        auto x{static_cast<double>(i) / static_cast<double>(m_heightResolution)} ;
        auto p1 {cos(x * 2.0 * std::numbers::pi)};
        auto p2 {sin(x * 2.0 * std::numbers::pi)};
        for (auto j = 0; j < m_heightResolution; j++){
            auto y {static_cast<double>(j) / static_cast<double>(m_heightResolution)};
            auto p3 {cos(y * 2.0 * std::numbers::pi)};
            auto p4 {sin(y * 2.0 * std::numbers::pi)};
            m_heightMap[j * m_heightResolution + i] = static_cast<float>(noise.eval(p1, p2, p3, p4)) * m_maxHeight;
        }
    }
}

size_t GameModel::ClampCoordinate(long c){
    auto res = static_cast<long>(m_heightResolution);
    return static_cast<size_t>(((c % res) + res) % res);
}

long GameModel::LowHeightCoordinate(FLOAT c){
    auto sc = c * static_cast<FLOAT>(m_heightResolution) / m_playgroundEdge;
    return static_cast<long>(floorf(sc));
}

FLOAT GameModel::GetHeight(FLOAT x, FLOAT y) {
    auto lx {LowHeightCoordinate(x)};
    auto ly {LowHeightCoordinate(y)};
    auto h1 {m_heightMap[ClampCoordinate(ly) * m_heightResolution + ClampCoordinate(lx)]};
    auto h2 {m_heightMap[ClampCoordinate(ly) * m_heightResolution + ClampCoordinate(lx + 1)]};
    auto h3 {m_heightMap[ClampCoordinate(ly + 1) * m_heightResolution + ClampCoordinate(lx + 1)]};
    auto h4 {m_heightMap[ClampCoordinate(ly + 1) * m_heightResolution + ClampCoordinate(lx)]};
    return (h1 + h2 + h3 + h4) / 4;
}
