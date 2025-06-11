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

private:
    // For enemy pathfinding
    std::vector<std::vector<int>> distMap;  // 2D vector that stores the Manhattan distance of each tile to the player.
    Engine::Point lastPlayerTilePos;

protected:
    int row;
    int col;
    std::vector<std::vector<Tile>> mapVec;

    std::shared_ptr<ALLEGRO_BITMAP> assets;
    std::vector<std::vector<Engine::Point>> assetOffsets;
    const int SOURCE_TILE_SIZE;

public:
    Map(std::string assetsPath, int sourceTileSize, int row, int col, std::vector<std::vector<Tile>> mapVec);

    void Draw(const Engine::Point & camera) const override;
    void UpdateDistMap(Engine::Point playerPos, bool forceUpdate = false);

    virtual void generateMapOffset();  // Abstract method to load the spritesheet offset of each tile.

    Tile GetTile(int x, int y) const;
    float GetDist(Engine::Point pos) const;

    bool isWall(int i, int j) const;
    bool isFloor(int i, int j) const;
    bool isNothing(int i, int j) const;
    bool isWalkable(int i, int j) const;

    [[nodiscard]] int getRow() const { return row; }
    [[nodiscard]] int getCol() const { return col; }
};



#endif //MAP_HPP
