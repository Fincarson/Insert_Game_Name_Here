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

    void generateMapOffset() override;

public:
    DungeonMap(int row, int col, const std::vector<std::vector<Tile>> &mapVec);
};



#endif //DUNGEONMAP_HPP
