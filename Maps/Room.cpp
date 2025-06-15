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
#include "Enemy/Agis.hpp"
#include "Enemy/Coins.hpp"
#include "Enemy/Knight.hpp"
#include "Enemy/Zombie.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Sprites/Chest.hpp"
#include "Sprites/FakePanda.hpp"
#include "Sprites/ShopDisplay.hpp"

void Room::loadRoom(std::string filename) {
    filename = "Resource/maps/" + filename;
    mapName = "map_packets.png";
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
    std::unordered_map<int, std::vector<Engine::Point>> passagewayTiles;

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

                case 'A':
                    mapName = "map_packets3.png";
                case 'Z':
                case 'K':
                case 'C':
                case 'F':
                    mapVec[i][j] = FLOOR;
                    entities.push_back({Engine::Point(j * TILE_SIZE, i * TILE_SIZE), line[j]});
                break;

                case 'H':  // Colliding entity
                case 'S':
                    mapVec[i][j] = BARRIER;
                    entities.push_back({Engine::Point(j * TILE_SIZE, i * TILE_SIZE), line[j]});
                break;

                default:
                    if (isdigit(line[j])) {
                        mapVec[i][j] = FLOOR;
                        int passagewayId = line[j] - '0';

                        posToPassageway[Engine::Point(j, i)] = passagewayId;
                        passagewayTiles[passagewayId].push_back(Engine::Point(j, i));
                        passageways[passagewayId].pos = passagewayTiles[passagewayId][passagewayTiles[passagewayId].size() / 2];
                        // If there are multiple passageway tiles, take the median.

                    } else {
                        mapVec[i][j] = FLOOR;
                    }
            }
        }
    }

    AddNewObject(map = new DungeonMap(row, col, mapVec, mapName));
    map->generateMapOffset();

    /*
     * 2. Read additional arguments (e.g. passageways to other rooms)
     */
    auto gameScene = dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
    std::vector<std::map<std::string, int>> chestContents;
    std::vector<std::pair<std::string, int>> shopItems;

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

        } else if (command == "ShopItem") {
            std::string shopItemId;
            int price;
            lineStream >> shopItemId >> price;
            shopItems.push_back({shopItemId, price});

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
    auto itCurShopItem = shopItems.begin();

    static const std::map<std::string, int> DEFAULT_CHEST = {{"coin", 5}};
    std::map<std::string, int> curChest = DEFAULT_CHEST;

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

            case 'A': {
                float size = TILE_SIZE * 5;
                EnemyGroup->AddNewObject(new Agis(pos.x - size/2, pos.y - size/2, size, size, 10, 500, map, player));
                break;
            }

            case 'H':
                curChest = (itCurChest != chestContents.end()) ? *(itCurChest++) : DEFAULT_CHEST;
                InteractableGroup->AddNewObject(new Chest(pos.x, pos.y, TILE_SIZE, TILE_SIZE, player, curChest));
            break;

            case 'F':
                EnemyGroup->AddNewObject(new FakePanda(pos.x, pos.y, TILE_SIZE, TILE_SIZE));
            break;

            case 'S':
                if (itCurShopItem == shopItems.end()) throw std::out_of_range("not enough shop display arguments");
                InteractableGroup->AddNewObject(new ShopDisplay(pos.x, pos.y, TILE_SIZE, TILE_SIZE,
                    player, itCurShopItem->first, itCurShopItem->second));

                ++itCurShopItem;
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
