//
// Created by Patrick on 9 Jun 2025.
//

#include "DungeonMap.hpp"


namespace MapOffsetGetter {

/*
 * The thingy from I2P I project.
 */

using Engine::Point;

static Point get_floor_offset_assets(Map * map, int i, int j){
    bool up = map->isWall(i-1, j);
    bool left = map->isWall(i, j-1);
    bool right = map->isWall(i, j+1);
    bool top_left = map->isWall(i-1, j-1);
    bool top_right = map->isWall(i-1, j+1);

    if(up && left && right){
        return (Point){  12,  3 };
    }
    if(up && left){
        return (Point){  11, 0 };
    }
    if(up && right){
        return (Point){  13, 0 };
    }
    if(left){
        if(top_left)
            return (Point){  11,  1 };
        else
            return (Point){  15,  1 };

    }
    if(right){
        if(top_right)
            return (Point){  13,  1 };
        else
            return (Point){  14,  1 };
    }
    if(up){
        return (Point){  12, 0 };
    }
    if(top_left){
        return (Point){  11,  3 };
    }
    if(top_right){
        return (Point){  13,  3 };
    }

    return (Point){12 , 4 };
}

// Calculate the offset from the source assets
static Point get_wall_offset_assets(Map * map, int i, int j){
    bool up = map->isWall(i-1, j);
    bool down = map->isWall(i+1, j);
    bool left = map->isWall(i, j-1);
    bool right = map->isWall(i, j+1);

    if(up && down && left && right){
        return (Point){ 3 , 5  };
    }
    if(up && down && right){
        bool left_floor = map->isFloor(i, j - 1);
        bool right_down = map->isWall(i + 1, j + 1);
        bool right_up = map->isWall(i - 1, j + 1);
        if (right_down && right_up)
            return (Point) { 2 , 5  };
        if (left_floor) {
            return (Point) { 1 , 3  };
        }
        else {
            return (Point) { 4 , 5  };
        }
    }
    if (up && down && left) {
        bool right_floor = map->isFloor(i, j + 1);
        bool left_down = map->isWall(i + 1, j - 1);
        bool left_up = map->isWall(i - 1, j - 1);
        if (left_down && left_up)
            return (Point) { 4 , 5  };
        if (right_floor) {
            return (Point) { 1 , 3  };
        }
        else {
            return (Point) { 2 , 5  };
        }
    }
    if(down && right && left){
        bool down_right_wall = map->isWall(i + 1, j + 1);
        bool down_left_wall = map->isWall(i + 1, j - 1);
        bool down_right = map->isFloor(i+1, j+1);
        bool down_left = map->isFloor(i+1, j-1);
        if(down_right_wall && down_left_wall)
            return (Point) { 3 , 4  };
        if((down_right ^ down_left) == 0)
            return (Point){ 8 , 5  };
        if(down_right)
            return (Point){ 4 , 4  };
        if(down_left)
            return (Point){ 2 , 4  };
    }
    if(left && right){
        if(map->isFloor(i+1, j))
            return (Point){ 7 , 5 };
        else
            return (Point){ 7 , 7 };
    }
    if(down && up){
        bool left_ = map->isFloor(i, j-1);
        bool right_ = map->isFloor(i, j+1);
        if((left_ ^ right_) == 0)
            return (Point){ 1 , 3 };
        if(left_)
            return (Point){ 2 , 5 };
        return (Point){ 4 , 5 };
    }
    if(down && right){
        if(map->isFloor(i+1, j+1))
            return (Point){ 4 , 5  };
        else
            return (Point){ 2 , 4  };
    }
    if(down && left){
        if (map->isFloor(i, j + 1) && map->isFloor(i + 1, j - 1))
            return (Point) { 1 , 2  };
        else if(map->isFloor(i+1, j-1))
            return (Point){ 2 , 5  };
        else
            return (Point){ 4 , 4 };
    }
    if(up && right){
        if(map->isFloor(i+1, j))
            return (Point){ 2 , 6 };
        else
            return (Point){ 5 , 6 };
    }
    if(up && left){
        if(map->isFloor(i+1, j))
            return (Point){ 4 , 6 };
        else
            return (Point){ 6 , 6 };
    }
    if(left || right){
        if(map->isFloor(i+1, j))
            return (Point){ 7 , 5 };
        else
            return (Point){ 7 , 7 };
    }
    if(up || down){
        bool left_ = map->isFloor(i, j - 1);
        bool right_ = map->isFloor(i, j + 1);
        if ((left_ ^ right_) == 0)
            return (Point) { 1 , 3  };
        if (left_)
            return (Point) { 2 , 5  };
        return (Point) { 4 , 5  };
    }

    return (Point){ 0, 0 };
}

static Point get_hole_offset_assets(Map * map, int i, int j){
    bool up = map->isNothing(i-1, j);
    bool U_Floor = map->isFloor(i-1, j);

    if(up){
        return (Point){ 0, 0 };
    }
    if(U_Floor){
        bool UL = map->isFloor(i-1, j-1);
        bool UR = map->isFloor(i-1, j+1);
        if(UL && UR) return (Point){  13,  11 };
        if(UL) return (Point){  14,  11 };
        if(UR) return (Point){  12,  11 };
        return (Point){  10,  12 };
    }
    return (Point){ 0, 0 };
}

static Point get_nothing_offset_assets(Map * map, int i, int j){
    bool U_Floor = map->isFloor(i-1, j);
    if(U_Floor){
        bool UL = map->isFloor(i-1, j-1);
        bool UR = map->isFloor(i-1, j+1);
        if(UL && UR) return (Point){  13,  11 };
        if(UL) return (Point){  14,  11 };
        if(UR) return (Point){  12,  11 };
        return (Point){  10,  12 };
    }
    return (Point){ 0, 0 };
}

}



void DungeonMap::generateMapOffset() {
    using namespace MapOffsetGetter;

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            switch(mapVec[i][j]){
                case WALL:
                    assetOffsets[i][j] = get_wall_offset_assets(this, i, j);
                break;

                case FLOOR:
                    assetOffsets[i][j] = get_floor_offset_assets(this, i, j);
                break;

                case HOLE:
                    assetOffsets[i][j] = get_hole_offset_assets(this, i, j);
                break;

                case NOTHING:
                default:
                    assetOffsets[i][j] = (Point){ 0, 0 };
                break;
            }
        }
    }
}

DungeonMap::DungeonMap(int row, int col, const std::vector<std::vector<Tile>> &mapVec)
    : Map("map_packets.png", 16, row, col, mapVec) {
}
