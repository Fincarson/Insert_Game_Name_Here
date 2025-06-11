//
// Created by Patrick on 9 Jun 2025.
//

#include "Map.hpp"

#include <cmath>
#include <queue>
#include <allegro5/allegro_primitives.h>
#include <allegro5/bitmap_draw.h>

#include "utility.hpp"
#include "Engine/Resources.hpp"
#include "UI/Label.hpp"

#define DRAW_HITBOX

Map::Map(std::string mapAssetsPath, int sourceTileSize, int row, int col, std::vector<std::vector<Tile>> mapVec)
    : assets(Engine::Resources::GetInstance().GetBitmap(mapAssetsPath)), SOURCE_TILE_SIZE(sourceTileSize),
      row(row), col(col), mapVec(mapVec),
      assetOffsets(row, std::vector(col, Engine::Point(0, 0))),
      distMap(row, std::vector(col, -1)) {

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
            /*if (mapVec[i][j] == WALL)*/ al_draw_rectangle(
                dx, dy, dx + TILE_SIZE, dy + TILE_SIZE,
                al_map_rgb(0, 255, 0), 1
            );

            // Pathfinding debug thing.
            Engine::Label label(std::to_string(distMap[i][j]), "Minecraft.ttf", 32,
                (j + 0.5) * TILE_SIZE, (i + 0.5) * TILE_SIZE, 255, 255, 255, 255, 0.5, 0.5);
            label.Draw(camera);

        #endif
        }
    }
}

void Map::UpdateDistMap(Engine::Point playerPos, bool forceUpdate) {
    /*
     * Use BFS to update each tile's distance to the player.
     */
    Engine::Point playerTilePos = (playerPos + Engine::Point(TILE_SIZE, TILE_SIZE) / 2) / TILE_SIZE;
    playerTilePos.x = std::floor(playerTilePos.x);
    playerTilePos.y = std::floor(playerTilePos.y);

    // Only update when the player tile pos changes or forceUpdate is true.
    if (!forceUpdate && playerTilePos == lastPlayerTilePos) return;
    lastPlayerTilePos = playerTilePos;

    // Reset all elements of the distance map to -1.
    for (auto& curRow : distMap) {
        curRow.assign(col, -1);
    }

    std::queue<Engine::Point> posQueue;
    posQueue.push(playerTilePos);
    distMap[playerTilePos.y][playerTilePos.x] = 0;

    static const std::vector DIRECTIONS = {
        Engine::Point(0, 1), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(-1, 0)
    };
    static constexpr int MAX_DEPTH = 20;

    while (!posQueue.empty()) {
        Engine::Point curPos = posQueue.front();
        posQueue.pop();

        int curDepth = distMap[curPos.y][curPos.x];
        if (curDepth > MAX_DEPTH) break;

        for (auto& dir : DIRECTIONS) {
            Engine::Point nextPos = curPos + dir;

            if (isWalkable(nextPos.y, nextPos.x) && distMap[nextPos.y][nextPos.x] == -1) {
                distMap[nextPos.y][nextPos.x] = curDepth + 1;
                posQueue.push(nextPos);
            }
        }
    }
}

void Map::generateMapOffset() {
}

Tile Map::GetTile(int x, int y) const { return mapVec[y][x]; }

int Map::GetDist(Engine::Point tilePos) const {
    // Engine::Point tilePos = pos / TILE_SIZE;
    if (tilePos.x < 0 || tilePos.x >= row || tilePos.y < 0 || tilePos.y >= col) return -1;
    return distMap[tilePos.y][tilePos.x];
}

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

bool Map::isWalkable(int i, int j) const {
    if(i < 0 || j < 0 || i >= row || j >= col) return false;
    return isFloor(i, j);
}

