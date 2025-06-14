//
// Created by Patrick on 9 Jun 2025.
//

#include "DungeonMap.hpp"

using Engine::Point;

Point DungeonMap::getFloorOffset(int i, int j){
    bool up = isWall(i-1, j);
    bool left = isWall(i, j-1);
    bool right = isWall(i, j+1);
    bool top_left = isWall(i-1, j-1);
    bool top_right = isWall(i-1, j+1);

    if(up && left && right){
        return Point(12, 3);
    }
    if(up && left){
        return Point(11, 0);
    }
    if(up && right){
        return Point(13, 0);
    }
    if(left){
        if(top_left)
            return Point(11, 1);
        else
            return Point(15, 1);

    }
    if(right){
        if(top_right)
            return Point(13, 1);
        else
            return Point(14, 1);
    }
    if(up){
        return Point(12, 0);
    }
    if(top_left){
        return Point(11, 3);
    }
    if(top_right){
        return Point(13, 3);
    }

    return Point(12, 4);
}

// Calculate the offset from the source assets
Point DungeonMap::getWallOffset(int i, int j){
    bool up = isWall(i-1, j);
    bool down = isWall(i+1, j);
    bool left = isWall(i, j-1);
    bool right = isWall(i, j+1);

    if(up && down && left && right){
        return Point(3, 5);
    }
    if(up && down && right){
        bool left_floor = isFloor(i, j - 1);
        bool right_down = isWall(i + 1, j + 1);
        bool right_up = isWall(i - 1, j + 1);
        if (right_down && right_up)
            return Point(2, 5);
        if (left_floor) {
            return Point(1, 3);
        }
        else {
            return Point(4, 5);
        }
    }
    if (up && down && left) {
        bool right_floor = isFloor(i, j + 1);
        bool left_down = isWall(i + 1, j - 1);
        bool left_up = isWall(i - 1, j - 1);
        if (left_down && left_up)
            return Point(4, 5);
        if (right_floor) {
            return Point(1, 3);
        }
        else {
            return Point(2, 5);
        }
    }
    if(down && right && left){
        bool down_right_wall = isWall(i + 1, j + 1);
        bool down_left_wall = isWall(i + 1, j - 1);
        bool down_right = isFloor(i+1, j+1);
        bool down_left = isFloor(i+1, j-1);
        if(down_right_wall && down_left_wall)
            return Point(3, 4);
        if((down_right ^ down_left) == 0)
            return Point(8, 5);
        if(down_right)
            return Point(4, 4);
        if(down_left)
            return Point(2, 4);
    }
    if(left && right){
        if(isFloor(i+1, j))
            return Point(7, 5);
        else
            return Point(7, 7);
    }
    if(down && up){
        bool left_ = isFloor(i, j-1);
        bool right_ = isFloor(i, j+1);
        if((left_ ^ right_) == 0)
            return Point(1, 3);
        if(left_)
            return Point(2, 5);
        return Point(4, 5);
    }
    if(down && right){
        if(isFloor(i+1, j+1))
            return Point(4, 5);
        else
            return Point(2, 4);
    }
    if(down && left){
        if (isFloor(i, j + 1) && isFloor(i + 1, j - 1))
            return Point(1, 2);
        else if(isFloor(i+1, j-1))
            return Point(2, 5);
        else
            return Point(4, 4);
    }
    if(up && right){
        if(isFloor(i+1, j))
            return Point(2, 6);
        else
            return Point(5, 6);
    }
    if(up && left){
        if(isFloor(i+1, j))
            return Point(4, 6);
        else
            return Point(6, 6);
    }
    if(left || right){
        if(isFloor(i+1, j))
            return Point(7, 5);
        else
            return Point(7, 7);
    }
    if(up || down){
        bool left_ = isFloor(i, j - 1);
        bool right_ = isFloor(i, j + 1);
        if ((left_ ^ right_) == 0)
            return Point(1, 3);
        if (left_)
            return Point(2, 5);
        return Point(4, 5);
    }

    return Point(0, 0);
}

Point DungeonMap::getHoleOffset(int i, int j){
    bool up = isNothing(i-1, j);
    bool U_Floor = isFloor(i-1, j);

    if(up){
        return Point(13, 9);
    }
    if(U_Floor){
        bool UL = isFloor(i-1, j-1);
        bool UR = isFloor(i-1, j+1);
        if(UL && UR) return Point(13, 11);
        if(UL) return Point(14, 11);
        if(UR) return Point(12, 11);
        return Point(10, 12);
    }
    return Point(13, 9);
}



void DungeonMap::generateMapOffset() {
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            switch(mapVec[i][j]){
                case WALL:
                    assetOffsets[i][j] = getWallOffset(i, j);
                break;

                case FLOOR:
                case BARRIER:
                    assetOffsets[i][j] = getFloorOffset(i, j);
                break;

                case HOLE:
                    assetOffsets[i][j] = getHoleOffset(i, j);
                break;

                case NOTHING:
                default:
                    assetOffsets[i][j] = Point(0, 0);
                break;
            }
        }
    }
}

DungeonMap::DungeonMap(int row, int col, const std::vector<std::vector<Tile>> &mapVec, std::string map_name)
    : Map(map_name, 16, row, col, mapVec) {
}
