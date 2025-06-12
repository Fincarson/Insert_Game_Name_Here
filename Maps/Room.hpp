//
// Created by Patrick on 9 Jun 2025.
//

#ifndef ROOM_HPP
#define ROOM_HPP

#include <map>
#include <string>
#include <unordered_map>

#include "Map.hpp"
#include "Engine/Group.hpp"


class Room : public Engine::Group {
private:
    struct Passageway {
        Engine::Point pos;
        std::string otherRoomFile;
        int otherId;
    };

    Map* map = nullptr;

    std::unordered_map<Engine::Point, int> posToPassageway;
    std::unordered_map<int, Passageway> passageways;

    void loadRoom(std::string filename);

public:
    Room(std::string filename);
    void Update(float deltaTime) override;
    void Draw(const Engine::Point &camera) const override;

    Engine::Point GetPassagewayPos(int passagewayId);
    int GetPassagewayId(Engine::Point pos);

    Passageway & GetPassageway(int passagewayId);

    Engine::Point Spawn;

    Group * EnemyGroup;
    Group * BulletGroup;

    int GetRows() { return map->row; }
    int GetCols() { return map->col; };
    Map* getMap() const { return map; }
};



#endif //ROOM_HPP
