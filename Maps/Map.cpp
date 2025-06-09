//
// Created by Patrick on 9 Jun 2025.
//

#include "Map.hpp"

#include <allegro5/allegro_primitives.h>
#include <allegro5/bitmap_draw.h>

#include "utility.hpp"
#include "Engine/Resources.hpp"

int Map::SOURCE_TILE_SIZE = 32;
std::string Map::MAP_ASSETS_PATH = "map_packets.png";

#define DRAW_HITBOX

Map::Map(int row, int col, std::vector<std::vector<Tile>> mapVec)
    : row(row), col(col), mapVec(mapVec), assets(Engine::Resources::GetInstance().GetBitmap(MAP_ASSETS_PATH)),
      assetOffsets(row, std::vector<Engine::Point>(col)){

    getMapOffset();
}

void Map::Draw() const {
    Engine::Point cam(0, 0);  // TODO make camera asap

    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++){
            int dy = i * TILE_SIZE - cam.y; // destiny y axis
            int dx = j * TILE_SIZE - cam.x; // destiny x axis

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

void Map::getMapOffset() {
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            // TODO replace with the i2p 1 stuff
            assetOffsets[i][j] = Engine::Point(j, i);
        }
    }
}
