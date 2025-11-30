// Room.h
#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "Monster.h"

class Room; 

struct RoomNode {
    Room* north = nullptr; 
    Room* south = nullptr;
    Room* east = nullptr;
    Room* west = nullptr;

};

class Room {
private:
    std::string description;
    std::vector<Monster> monsters; // Enemies in this room
    int roomId;

    int northId = -1;
    int southId = -1;
    int eastId = -1;
    int westId = -1;

public:
    Room() : description("Init Room"), roomId(-1) {}
    Room(int id, const std::string& desc) : description(desc), roomId(id) {}
    // Destructor: RoomNode pointers are not owned, std::vector<Monster> auto-cleans
    ~Room() = default;

    RoomNode connections;

    // --- Getters ---
    const std::string& getDescription() const { return description; }
    int getId() const { return roomId; }
    std::vector<Monster>& getMonsters() { return monsters; }
    

    void addMonster(const Monster& monster) {
        monsters.push_back(monster);
    }

    void displayExits() const {
        std::cout << "Exits available:";
        if (connections.north) std::cout << " [North]";
        if (connections.south) std::cout << " [South]";
        if (connections.east) std::cout << " [East]";
        if (connections.west) std::cout << " [West]";
        std::cout << std::endl;
    }

    void removeDefeatedMonsters() {
        // Remove monsters whose current health is zero or less
        monsters.erase(
            std::remove_if(monsters.begin(), monsters.end(), 
                [](const Monster& m) { return m.getCurrentHealth() <= 0; }),
            monsters.end()
        );
    }
    
    bool isClear() const {
        return monsters.empty();
    }

    void clearMonsters() {
        monsters.clear();
    }

    // Save/Load Room
    void saveRoom(std::fstream& file) const {
        file.write(reinterpret_cast<const char*>(&roomId), sizeof(roomId));
        size_t descLength = description.size();
        file.write(reinterpret_cast<const char*>(&descLength), sizeof(descLength));
        file.write(description.c_str(), descLength);

        size_t monsterCount = monsters.size();
        file.write(reinterpret_cast<const char*>(&monsterCount), sizeof(monsterCount));
        for (const auto& monster : monsters) {
            monster.save(file);
        }

        //Save North Connection ID
        int northIdToSave = connections.north ? connections.north->getId() : -1;
        file.write(reinterpret_cast<const char*>(&northIdToSave), sizeof(northIdToSave));

        //Save South Connection ID
        int southIdToSave = connections.south ? connections.south->getId() : -1;
        file.write(reinterpret_cast<const char*>(&southIdToSave), sizeof(southIdToSave));

        //Save East Connection ID
        int eastIdToSave = connections.east ? connections.east->getId() : -1;
        file.write(reinterpret_cast<const char*>(&eastIdToSave), sizeof(eastIdToSave));

        //Save West Connection ID
        int westIdToSave = connections.west ? connections.west->getId() : -1;
        file.write(reinterpret_cast<const char*>(&westIdToSave), sizeof(westIdToSave));
    }

    void loadRoom(std::fstream& file) {
        file.read(reinterpret_cast<char*>(&roomId), sizeof(roomId));

        size_t descLength = 0;
        file.read(reinterpret_cast<char*>(&descLength), sizeof(descLength));
        description.resize(descLength);
        file.read(&description[0], descLength);

        size_t monsterCount = 0;
        file.read(reinterpret_cast<char*>(&monsterCount), sizeof(monsterCount));
        monsters.clear();
        for (size_t i = 0; i < monsterCount; ++i) {
            Monster monster;
            monster.load(file);
            monsters.push_back(monster);
        }

        //Load Connection IDs
        file.read(reinterpret_cast<char*>(&northId), sizeof(northId));
        file.read(reinterpret_cast<char*>(&southId), sizeof(southId));
        file.read(reinterpret_cast<char*>(&eastId), sizeof(eastId));
        file.read(reinterpret_cast<char*>(&westId), sizeof(westId));
    }

    void reconnect(const std::map<int, Room*>& roomLookup) {
        auto getRoomPtr = [&](int id) -> Room* {
            if (id != -1 && roomLookup.count(id)) {
                return roomLookup.at(id);
            }
            return nullptr;
        };

        connections.north = getRoomPtr(northId);
        connections.south = getRoomPtr(southId);
        connections.east = getRoomPtr(eastId);
        connections.west = getRoomPtr(westId);

        northId = -1;
        southId = -1;
        eastId = -1;
        westId = -1;
    }
};

#endif