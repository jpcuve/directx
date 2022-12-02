//
// Created by jpc on 29/11/2022.
//

#include <numbers>
#include "GameModel.h"
#include "OpenSimplexNoise.h"
#include "debug.h"

GameModel::GameModel() {
    OpenSimplexNoise::Noise noise;
    m_heightMap.resize(m_playgroundResolution * m_playgroundResolution, 0.0f);
    for (auto i = 0; i < m_playgroundResolution; i++){
        auto x{static_cast<double>(i) / static_cast<double>(m_playgroundResolution)} ;
        auto p1 {cos(x * 2.0 * std::numbers::pi)};
        auto p2 {sin(x * 2.0 * std::numbers::pi)};
        for (auto j = 0; j < m_playgroundResolution; j++){
            auto y {static_cast<double>(j) / static_cast<double>(m_playgroundResolution)};
            auto p3 {cos(y * 2.0 * std::numbers::pi)};
            auto p4 {sin(y * 2.0 * std::numbers::pi)};
            m_heightMap[j * m_playgroundResolution + i] = static_cast<float>(noise.eval(p1, p2, p3, p4)) * m_maxHeight;
        }
    }
}

size_t GameModel::AdjustCoordinate(long c) const{
    auto res = static_cast<long>(m_playgroundResolution);
    return static_cast<size_t>(((c % res) + res) % res);
}

long GameModel::LowHeightCoordinate(FLOAT c) const{
    auto sc = c * static_cast<FLOAT>(m_playgroundResolution) / m_playgroundEdge;
    return static_cast<long>(floorf(sc));
}

FLOAT GameModel::GetHeight(DirectX::XMFLOAT2 &position) {
    long l[2] {LowHeightCoordinate(position.x), LowHeightCoordinate(position.y)};
    auto h1 {m_heightMap[AdjustCoordinate(l[1]) * m_playgroundResolution + AdjustCoordinate(l[0])]};
    auto h2 {m_heightMap[AdjustCoordinate(l[1]) * m_playgroundResolution + AdjustCoordinate(l[0] + 1)]};
    auto h3 {m_heightMap[AdjustCoordinate(l[1] + 1) * m_playgroundResolution + AdjustCoordinate(l[0] + 1)]};
    auto h4 {m_heightMap[AdjustCoordinate(l[1] + 1) * m_playgroundResolution + AdjustCoordinate(l[0])]};
    return (h1 + h2 + h3 + h4) / 4;
}

std::vector<DirectX::XMFLOAT3> GameModel::LocalMap(DirectX::XMFLOAT2 &position) {
    std::vector<DirectX::XMFLOAT3> map;
    float step = m_playgroundEdge / static_cast<FLOAT>(m_playgroundResolution);
    auto lb {DirectX::XMFLOAT2 {position.x - m_surfaceResolution * step / 2.0f, position.y - m_surfaceResolution * step / 2.0f}};
    long a[2] {LowHeightCoordinate(lb.x), LowHeightCoordinate(lb.y)};
//    size_t resolution = 1 + m_playgroundResolution * static_cast<size_t>(m_surfaceEdge) / static_cast<size_t>(m_playgroundEdge);
    auto offset {DirectX::XMFLOAT2{lb.x - static_cast<FLOAT>(a[0]) * step, lb.y - static_cast<FLOAT>(a[1]) * step}};
    dbg << "Offset: (" << offset.x << ", " << offset.y << ")" << std::endl << std::flush;
    for (auto i = 0; i < m_surfaceResolution; i++){
        auto xc = AdjustCoordinate(a[0] + i);
        for (auto j = 0; j < m_surfaceResolution; j++){
            auto yc = AdjustCoordinate(a[1] + j);
            dbg << "(" << xc << ", " << yc << ")" << std::endl << std::flush;
            auto x = static_cast<FLOAT>(xc) * step + offset.x - position.x;
            auto y = static_cast<FLOAT>(yc) * step + offset.y - position.y;
            auto height{m_heightMap[xc + yc * m_playgroundResolution]};
            auto point {DirectX::XMFLOAT3{x, y, height}};
            dbg << "[" << point.x << ", " << point.y << ", " << point.z << "]" << std::endl << std::flush;
        }
    }
    return map;
}
