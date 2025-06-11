//
// Created by Patrick on 9 Jun 2025.
//

#ifndef ROOM_HPP
#define ROOM_HPP

#include <string>

#include "Map.hpp"
#include "Engine/Group.hpp"


class Room : public Engine::Group {
private:
    Map* map = nullptr;

    void loadRoom(std::string filename);

public:
    Room(std::string filename);
    void Update(float deltaTime) override;
    void Draw(const Engine::Point &camera) const override;

    Engine::Point Spawn;

    Group * EnemyGroup;
    Group * BulletGroup;

    int GetRows() { return map->row; }
    int GetCols() { return map->col; };
    Map* getMap() const { return map; }
};



#endif //ROOM_HPP
