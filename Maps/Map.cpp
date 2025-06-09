//
// Created by Patrick on 9 Jun 2025.
//

#include "Map.hpp"

#include <allegro5/allegro_primitives.h>
#include <allegro5/bitmap_draw.h>

#include "utility.hpp"
#include "Engine/Resources.hpp"

#define DRAW_HITBOX

Map::Map(std::string mapAssetsPath, int sourceTileSize, int row, int col, std::vector<std::vector<Tile>> mapVec)
    : assets(Engine::Resources::GetInstance().GetBitmap(mapAssetsPath)), SOURCE_TILE_SIZE(sourceTileSize),
      row(row), col(col), mapVec(mapVec),
      assetOffsets(row, std::vector<Engine::Point>(col, Engine::Point(0, 0))){

}

void Map::Draw(const Engine::Point & camera) const {
    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++){
            int dy = i * TILE_SIZE - camera.y; // destiny y axis
            int dx = j * TILE_SIZE - camera.x; // destiny x axis

            Engine::Point curOffset = assetOffsets[i][j];

            // Draw walls and floors and stuff.
            al_draw_scaled_bitmap(
                assets.get(), // image
                SOURCE_TILE_SIZE * curOffset.x, SOURCE_TILE_SIZE * curOffset.y, 16, 16, // source x, source y, width, height
                dx, dy, TILE_SIZE, TILE_SIZE, // dest x, dest y, dest width, dest height
                0 // flag : set 0
                );

        #ifdef DRAW_HITBOX
            al_draw_rectangle(
                dx, dy, dx + TILE_SIZE, dy + TILE_SIZE,
                al_map_rgb(0, 255, 0), 1
            );
        #endif
        }
    }
}

Tile Map::GetTile(int x, int y) const { return mapVec[y][x]; }

bool Map::isWall(int i, int j) const {
    if(i < 0 || j < 0 || i >= row || j >= col) return false;
    if(mapVec[i][j] == WALL) return true;
    return false;
}

bool Map::isFloor(int i, int j) const {
    if(i < 0 || j < 0 || i >= row || j >= col) return false;
    if(mapVec[i][j] == FLOOR) return true;
    return false;
}

bool Map::isNothing(int i, int j) const {
    if(i < 0 || j < 0 || i >= row || j >= col) return true;
    if(mapVec[i][j] == NOTHING || mapVec[i][j] == HOLE) return true;
    return false;
}

