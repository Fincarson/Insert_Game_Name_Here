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
    FLOOR,
    WALL,
    HOLE,
};


class Map : public Engine::IObject {
private:
    int row;
    int col;
    std::vector<std::vector<Tile>> mapVec;

    std::shared_ptr<ALLEGRO_BITMAP> assets;
    std::vector<std::vector<Engine::Point>> assetOffsets;

protected:
    static int SOURCE_TILE_SIZE;
    static std::string MAP_ASSETS_PATH;

    void getMapOffset();

public:
    Map(): row(0), col(0) {}
    explicit Map(int row, int col, std::vector<std::vector<Tile>> mapVec);

    Tile GetTile(int x, int y) const { return mapVec[y][x]; }

    void Draw() const override;
};



#endif //MAP_HPP
