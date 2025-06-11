//
// Created by Patrick on 9 Jun 2025.
//

#include "Room.hpp"

#include <fstream>
#include <iostream>

#include "DungeonMap.hpp"
#include "utility.hpp"
#include "Enemy/Zombie.hpp"
#include "Engine/GameEngine.hpp"

void Room::loadRoom(std::string filename) {
    filename = "Resource/maps/" + filename;
    std::ifstream file(filename);
    std::string buf;

    int row, col;
    file >> row >> col;
    std::getline(file, buf);

    std::vector<std::vector<Tile>> mapVec(row, std::vector<Tile>(col));
    std::vector<std::pair<Engine::Point, char>> enemies;

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
                Spawn = Engine::Point(j, i);
                break;

                case 'Z':
                    mapVec[i][j] = FLOOR;
                    enemies.push_back({Engine::Point(j * TILE_SIZE, i * TILE_SIZE), line[j]});
                break;

                default:
                    mapVec[i][j] = FLOOR;
            }
        }
    }

    AddNewObject(map = new DungeonMap(row, col, mapVec));
    map->generateMapOffset();

    // Summon enemies
    EnemyGroup = new Group;
    Player* player = dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene())->GetPlayer();

    for (auto& [pos, enemy] : enemies) {
        switch (enemy) {
            case 'Z':
                EnemyGroup->AddNewObject(new Zombie(pos.x, pos.y, TILE_SIZE, TILE_SIZE, 10, map, player));
            break;

            default:
            break;
        }
    }
}

Room::Room(std::string filename) {
    loadRoom(filename);
    AddNewObject(EnemyGroup);
    AddNewObject(BulletGroup = new Group());
}

void Room::Update(float deltaTime) {
    Group::Update(deltaTime);
}

void Room::Draw(const Engine::Point &camera) const {
    Group::Draw(camera);
}
