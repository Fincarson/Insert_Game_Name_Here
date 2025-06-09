//
// Created by Patrick on 9 Jun 2025.
//

#ifndef DUNGEONMAP_HPP
#define DUNGEONMAP_HPP
#include "Map.hpp"


class DungeonMap : public Map {
protected:
    static int SOURCE_TILE_SIZE;
    static std::string MAP_ASSETS_PATH;

    Engine::Point getFloorOffset(int i, int j);
    Engine::Point getWallOffset(int i, int j);
    Engine::Point getHoleOffset(int i, int j);

public:
    DungeonMap(int row, int col, const std::vector<std::vector<Tile>> &mapVec);
    void generateMapOffset() override;
};



#endif //DUNGEONMAP_HPP
