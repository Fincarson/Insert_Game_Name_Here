//
// Created by Patrick on 9 Jun 2025.
//

#include "Room.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

#include "DungeonMap.hpp"
#include "utility.hpp"
#include "Enemy/Coins.hpp"
#include "Enemy/Knight.hpp"
#include "Enemy/Zombie.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Sprites/Chest.hpp"

void Room::loadRoom(std::string filename) {
    filename = "Resource/maps/" + filename;
    std::ifstream file(filename);
    std::string line;

    /*
     * 1. Read the map
     */
    int row, col;
    file >> row >> col;
    std::getline(file, line);  // Get rid of the linebreak

    std::vector<std::vector<Tile>> mapVec(row, std::vector<Tile>(col));
    std::vector<std::pair<Engine::Point, char>> entities;

    for (int i = 0; i < row; i++) {
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
                case 'K':
                case 'C':
                    mapVec[i][j] = FLOOR;
                    entities.push_back({Engine::Point(j * TILE_SIZE, i * TILE_SIZE), line[j]});
                break;

                case 'H':
                    mapVec[i][j] = FLOOR;
                    entities.push_back({Engine::Point(j * TILE_SIZE, i * TILE_SIZE), line[j]});
                break;

                default:
                    if (isdigit(line[j])) {
                        mapVec[i][j] = FLOOR;
                        posToPassageway[Engine::Point(j, i)] = line[j] - '0';
                        passageways[line[j] - '0'].pos = Engine::Point(j, i);

                    } else {
                        mapVec[i][j] = FLOOR;
                    }
            }
        }
    }

    AddNewObject(map = new DungeonMap(row, col, mapVec));
    map->generateMapOffset();

    /*
     * 2. Read additional arguments (e.g. passageways to other rooms)
     */
    auto gameScene = dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
    std::vector<std::map<std::string, int>> chestContents;

    while (std::getline(file, line)) {
        std::stringstream lineStream(line);
        std::string command;
        lineStream >> command;

        if (command == "" || command.substr(0, 2) == "//") {
            continue;
        } else if (command == "ConnectRooms") {
            int passageId, otherPassageId;
            std::string otherRoomFile;
            lineStream >> passageId >> otherRoomFile >> otherPassageId;

            passageways[passageId].otherRoomFile = otherRoomFile;
            passageways[passageId].otherId = otherPassageId;

        } else if (command == "Say") {
            float delay, duration;
            std::string text;

            lineStream >> delay >> duration;
            lineStream.get();
            std::getline(lineStream, text);

            gameScene->AddSubtitle(delay, duration, text);

        } else if (command == "Chest") {
            std::map<std::string, int> curChestContents;
            std::string item;
            int count;

            while (lineStream >> item >> count) {
                curChestContents[item] = count;
            }

            chestContents.push_back(curChestContents);

        } else {
            Engine::LOG(Engine::ERROR) << "unknown command on the map file argument" << filename << ": " << command;
        }
    }

    /*
     * 3. Summon non-map stuff
     */
    EnemyGroup = new Group;
    InteractableGroup = new Group;
    Player* player = dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene())->GetPlayer();

    auto itCurChest = chestContents.begin();
    auto& curChest = *itCurChest;
    static const std::map<std::string, int> DEFAULT_CHEST = {{"coin", 5}};

    for (auto& [pos, entity] : entities) {
        switch (entity) {
            case 'Z':
                EnemyGroup->AddNewObject(new Zombie(pos.x, pos.y, TILE_SIZE, TILE_SIZE, 10, 100, map, player));
            break;

            case 'K':
                EnemyGroup->AddNewObject(new Knight(pos.x, pos.y, TILE_SIZE, TILE_SIZE, 10, 250, map, player));
            break;

            case 'C':
                EnemyGroup->AddNewObject(new Coins(pos.x, pos.y, TILE_SIZE, TILE_SIZE, map, player));
            break;

            case 'H':
                curChest = (itCurChest != chestContents.end()) ? *(itCurChest++) : DEFAULT_CHEST;
                InteractableGroup->AddNewObject(new Chest(pos.x, pos.y, TILE_SIZE, TILE_SIZE, player, curChest));
            break;

            default:
            break;
        }
    }
}

Room::Room(std::string filename) {
    loadRoom(filename);
    AddNewObject(EnemyGroup);
    AddNewObject(InteractableGroup);
    AddNewObject(BulletGroup = new Group());
}

void Room::Update(float deltaTime) {
    Group::Update(deltaTime);
}

void Room::Draw(const Engine::Point &camera) const {
    Group::Draw(camera);
}

Engine::Point Room::GetPassagewayPos(int passagewayId) {
    return passageways.at(passagewayId).pos;
}

int Room::GetPassagewayId(Engine::Point pos) {
    for (auto& cornerOffset : CORNERS) {
        auto corner = pos + cornerOffset;
        corner.x = std::floor(corner.x / TILE_SIZE);
        corner.y = std::floor(corner.y / TILE_SIZE);

        auto it = posToPassageway.find(corner);
        if (it != posToPassageway.end() && passageways.count(it->second)) {
            return it->second;
        }
    }

    return -1;
}

Room::Passageway & Room::GetPassageway(int passagewayId) {
    return passageways.at(passagewayId);
}
