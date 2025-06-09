//
// Created by Patrick on 9 Jun 2025.
//

#ifndef MAP_HPP
#define MAP_HPP
#include <memory>
#include <string>
#include <vector>
#include <allegro5/bitmap.h>

#include "Engine/IObject.hpp"


enum Tile {
    NOTHING,
    FLOOR,
    WALL,
    HOLE,
};


class Map : public Engine::IObject {
    friend class Room;

protected:
    int row;
    int col;
    std::vector<std::vector<Tile>> mapVec;

    std::shared_ptr<ALLEGRO_BITMAP> assets;
    std::vector<std::vector<Engine::Point>> assetOffsets;
    const int SOURCE_TILE_SIZE;

public:
    Map(): row(0), col(0), SOURCE_TILE_SIZE(0) {}

    Map(std::string assetsPath, int sourceTileSize,
        int row, int col, std::vector<std::vector<Tile>> mapVec);

    void Draw(const Engine::Point & camera) const override;

    virtual void generateMapOffset() = 0;  // Abstract method to load the spritesheet offset of each tile.

    Tile GetTile(int x, int y) const;
    bool isWall(int i, int j) const;
    bool isFloor(int i, int j) const;
    bool isNothing(int i, int j) const;
    [[nodiscard]] int getRow() const { return row; }
    [[nodiscard]] int getCol() const { return col; }
};



#endif //MAP_HPP
