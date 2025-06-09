//
// Created by Patrick on 9 Jun 2025.
//

#include "Room.hpp"

#include <fstream>

void Room::loadRoom(std::string filename) {
    filename = "Resource/maps/" + filename;
    std::ifstream file(filename);
    std::string buf;

    int row, col;
    file >> row >> col;
    std::getline(file, buf);

    std::vector<std::vector<Tile>> mapVec(row, std::vector<Tile>(col));

    for (int i = 0; i < row; i++) {
        std::string line;
        std::getline(file, line);

        for (int j = 0; j < col; j++) {
            switch (line[j]) {
                case '#': // Case Wall
                    mapVec[i][j] = WALL;
                break;

                case '.': // Case Floor
                    mapVec[i][j] = FLOOR;
                break;

                case '_': // Nothing
                    mapVec[i][j] = HOLE;
                break;

                case 'P': // Spawn Point
                    mapVec[i][j] = FLOOR;
                spawn = Engine::Point(j, i);
                break;

                default:
                    // throw "blablabla";
                    mapVec[i][j] = FLOOR;
            }
        }
    }

    AddNewObject(map = new Map(row, col, mapVec));
}

Room::Room(std::string filename) {
    loadRoom(filename);
}
