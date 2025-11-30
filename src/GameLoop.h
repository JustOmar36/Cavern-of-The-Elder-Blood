#include <iostream>
#include <string>
#include <sstream>
#include <algorithm> 
#include <limits>
#include "Character.h"
#include "TheLady.h" 
#include "Monster.h" 
#include "Item.h"
#include "Room.h"
#include <fstream>
#include <vector>
#include <map>

using namespace std;

// Global item database definition (Needed for GetItemDefinitionFromDatabase)
extern std::map<int, Item> ITEM_DATABASE;

void printSeparator() {
    cout << "\n==================================\n" << endl;
}

void printSplashScreen() {
    const std::string splashScreen[] = {
        "############################################################",
        "#                                                          #",
        "#                CAVERN OF THE ELDER BLOOD                 #",
        "#                                                          #",
        "#                        (COTEB)                           #",
        "#                                                          #",
        "#                A C++ Project by Omar Masri               #",
        "#                                                          #",
        "############################################################"
    };

    const int numLines = sizeof(splashScreen) / sizeof(splashScreen[0]);
    for (int i = 0; i < numLines; ++i) {
        std::cout << splashScreen[i] << std::endl;
    }
}

void handleItemtoDisplay(TheLady& player, Inventory<Item>& inventory) {
    inventory.displayInventory();
    if (inventory.isEmpty()) {
        cout << "\nYou have no items in this inventory.\n" << endl;
        return;
    }

    cout << "\nEnter the Item ID of the item to view details (0 to cancel): ";
    std::string itemInput;

    while (true) {
        if (!std::getline(cin, itemInput)) {
            cin.clear();
            cout << "\nInput error. Cancelling." << std::endl;
            return;
        }

        std::stringstream ss(itemInput);
        int itemId;
        if (!(ss >> itemId)) {
            cout << "\nInvalid input. Please enter a numeric Item ID (0 to cancel): ";
            continue; // ask again
        }

        std::string leftover;
        if (ss >> leftover) {
            cout << "\nInvalid input. Please enter a single numeric Item ID (0 to cancel): ";
            continue; // ask again
        }

        if (itemId == 0) {
            return; // Cancel viewing
        }

        if (inventory.getItemCount(itemId) > 0) {
            player.getItemInfo(itemId);
            return;
        } else {
            cout << "\nYou do not have that item.\n" << std::endl;
            return;
        }
    }
}

// Function to handle the potions command
bool handlePotionsCommand(TheLady& player) {
    player.getPotionsInventory().displayInventory();
    if (player.getPotionsInventory().isEmpty()) {
        cout << "\nYou have no potions to use.\n" << endl;
        return false;
    }

    cout << "\nEnter the Item ID of the potion to use (0 to cancel): ";
    std::string potionInput;

    // Read a full line and validate it as an integer to avoid stream failure
    while (true) {
        if (!std::getline(cin, potionInput)) {
            // Input stream error; clear and return
            cin.clear();
            cout << "\nInput error. Cancelling potion use." << endl;
            return false;
        }

        std::stringstream ss(potionInput);
        int potionId;
        if (!(ss >> potionId)) {
            cout << "\nInvalid input. Please enter a numeric Item ID (0 to cancel): ";
            continue; // ask again
        }

        std::string leftover;
        if (ss >> leftover) {
            cout << "\nInvalid input. Please enter a single numeric Item ID (0 to cancel): ";
            continue; // ask again
        }

        if (potionId == 0) {
            return false; // Cancel using potion
        }

        if (player.getPotionsInventory().getItemCount(potionId) > 0) {
            player.usePotion(potionId);
            return true;
        } else {
            cout << "\nYou do not have that potion.\n" << endl;
            return false;
        }
    }
    return false;
}

void handleCampCommand(TheLady& player) {
    cout << "\nYou set up camp and rest for a while.\n" << endl;
    string camp_input;
    while (camp_input != "leave") {
        cout << "Enter a command (stats, ingredients, potions, study, brew, bestiary, leave): ";
        getline(cin, camp_input);
        transform(camp_input.begin(), camp_input.end(), camp_input.begin(), ::tolower);
        
        if (camp_input == "stats") {
            player.toString();
        } else if (camp_input == "ingredients") {
            handleItemtoDisplay(player, player.getIngredientsInventory());
        } else if (camp_input == "potions") {
            if (player.getPotionsInventory().isEmpty()) {
                cout << "\nYou have no potions to manage.\n" << endl;
                continue;
            }
            cout << "1. Consume Potion" << endl;
            cout << "2. View Potions Info\n" << endl;
            cout << "Enter choice (1, 2 , or 0 to cancel): ";
            string choice;
            getline(cin, choice);
            if (choice == "0") {
                continue; // Cancel potion management
            } else if (choice == "1") {
                if(handlePotionsCommand(player)){
                    continue;
                }
            } else if (choice == "2") {
                handleItemtoDisplay(player, player.getPotionsInventory());
            } else {
                cout << "\nInvalid choice.\n" << endl;
            }
        } else if (camp_input == "study") {
            player.getIngredientsInventory().displayInventory();
            if (player.getIngredientsInventory().isEmpty()) {
                cout << "\nYou have no ingredients to study.\n" << endl;
                continue;
            }
            int itemId;
            string temp_id_str;
            cout << "\nEnter the Item ID of the ingredient to study (0 to cancel): ";
            getline(cin, temp_id_str);
            stringstream ss_study(temp_id_str);
            if (temp_id_str == "0") {
                continue; // Cancel studying
            }
            if (ss_study >> itemId) {
                player.studyItem(itemId);
            } else {
                cout << "\nInvalid Item ID input.\n" << endl;
            };
        } else if (camp_input == "brew") {
            player.getIngredientsInventory().displayInventory();
            if (player.getIngredientsInventory().isEmpty()) {
                cout << "\nYou have no ingredients to brew with.\n" << endl;
                continue;
            }
            int recipeId;
            string temp_id_str;
            cout << "\nEnter the Item ID of the recipe to brew (0 to cancel): ";
            getline(cin, temp_id_str);
            stringstream ss_brew(temp_id_str);
            if (temp_id_str == "0") {
                continue; // Cancel brewing
            }
            if (ss_brew >> recipeId) {
                player.brewPotion(recipeId);
            } else {
                cout << "\nInvalid Recipe ID input.\n" << endl;
            }
            cout << "\n" << endl;
            player.getPotionsInventory().displayInventory();
        } else if (camp_input == "bestiary") {
            player.displayBestiary();
        } 
        else if (camp_input == "leave") {
            cout << "\nYou pack up your camp and prepare to continue your journey.\n" << endl;
        } else {
            cout << "\nUnknown camp command." << endl;
        }
    }
}

// Function to handle the player's turn in combat
bool handlePlayerTurn(TheLady& player, Room& room) { 
    if (room.isClear()) {
        cout << "\nThe room is clear. You can now move.\n" << endl;
        return false;
    }

    bool validActionTaken = false;
    while (!validActionTaken) {
        cout << "\n--- Your Turn ---\n" << endl;
        cout << "Monsters remaining (" << room.getMonsters().size() << "):" << endl;
        for (size_t i = 0; i < room.getMonsters().size(); ++i) {
            cout << i + 1 << ". " << room.getMonsters()[i].getName() 
                 << " (HP: " << room.getMonsters()[i].getCurrentHealth() << ")" << endl;
        }

        string input;
        cout << "Choose an action (attack [Target Number], flee, potions): ";
        getline(cin, input);

        transform(input.begin(), input.end(), input.begin(), ::tolower);

        stringstream ss(input);
        string command;
        ss >> command;
        
        if (command == "attack") {
            int targetIndex;
            
            if (ss >> targetIndex && targetIndex > 0 && targetIndex <= static_cast<int>(room.getMonsters().size())) {
                Monster& target = room.getMonsters()[targetIndex - 1];
                
                player.setCurrentTarget(&target);
                player * target; 
                
                // Clear defeated enemies after the attack
                room.removeDefeatedMonsters();
                player.updateIntellectBoost(); // Update damage potion effect after player turn
                validActionTaken = true;
                return false;

            } else {
                cout << "\nInvalid target number. Try again.\n" << endl;
            }
        } else if (command == "flee") {
            player.flee();
            return true;  // End combat (fled)

        } else if (command == "potions") {
            if(handlePotionsCommand(player)){
                validActionTaken = true;
                return false;
            }else{
                continue; // Prompt for action again
            }
        
        } else {
        cout << "\nUnknown command. Try 'attack [Target Number]', 'flee', or 'potions'.\n" << endl;
        }
    }
    return false;
}

// Function to handle the monsters' turn in combat
void handleMonsterTurn(TheLady& player, Room& room) {
    if (room.isClear()) return;

    cout << "\n--- Monster Turn ---" << endl;
    for (Monster& monster : room.getMonsters()) {
        if (player.getCurrentHealth() > 0 && monster.getCurrentHealth() > 0) {
            monster * player; // Monster attacks The Lady 
            cout << player.getName() << "'s Health: " << player.getCurrentHealth() << "/" << player.getMaxHealth() << endl;
        }
    }
}

// Function to save the game state
void saveGame(const TheLady& player, const std::vector<Room>& rooms) {
    std::cout << "Saving game..." << std::endl;

    //Save Player Data
    std::fstream playerFile("./build/bin/saves/player.dat", std::ios::out | std::ios::binary);
    if (playerFile.is_open()) {
        player.save(playerFile);
        playerFile.close();
    } else {
        std::cerr << "Error: Could not open player save file." << std::endl;
        return;
    }

    //Save ALL Room Data
    std::fstream mapFile("./build/bin/saves/map.dat", std::ios::out | std::ios::binary);
    if (mapFile.is_open()) {
        size_t roomCount = rooms.size();
        mapFile.write(reinterpret_cast<const char*>(&roomCount), sizeof(roomCount));
        
        for (const auto& room : rooms) {
            room.saveRoom(mapFile);
        }
        mapFile.close();
    } else {
        std::cerr << "Error: Could not open map save file." << std::endl;
    }
}

// Function to load the game state
bool loadGame(TheLady& player, std::vector<Room>& rooms) {
    std::cout << "Loading game..." << std::endl;
    
    std::map<int, Room*> roomLookup;

    std::fstream mapFile("./build/bin/saves/map.dat", std::ios::in | std::ios::binary);
    if (mapFile.is_open()) {
        rooms.clear();
        size_t roomCount = 0;
        mapFile.read(reinterpret_cast<char*>(&roomCount), sizeof(roomCount));

        if (roomCount == 0) {
            std::cerr << "Error: Map file is empty. Cannot load map." << std::endl;
            mapFile.close();
            return false;
        }

        rooms.reserve(roomCount);  // Reserve capacity to prevent reallocation
        for (size_t i = 0; i < roomCount; ++i) {
            Room newRoom; 
            newRoom.loadRoom(mapFile);
            rooms.push_back(newRoom);
            
            roomLookup[newRoom.getId()] = &rooms.back();
        }
        mapFile.close();
        std::cout << "Rooms loaded (Pass 1 complete)." << std::endl;
    } else {
        std::cerr << "Error: Could not open map load file. Cannot load game." << std::endl;
        return false;
    }

    //rebuild room connections
    for (Room& room : rooms) {
        room.reconnect(roomLookup);
    }
    std::cout << "Room connections rebuilt." << std::endl;
    
    std::fstream playerFile("./build/bin/saves/player.dat", std::ios::in | std::ios::binary);
    if (playerFile.is_open()) {
        player.load(playerFile);
        playerFile.close();
        
        int currentId = player.getCurrentRoomId();
        if (currentId != -1 && roomLookup.count(currentId)) {
            player.setCurrentRoom(roomLookup[currentId]);
            std::cout << "Player placed in Room " << currentId << "." << std::endl;
        } else if (roomLookup.count(1)) {
            player.setCurrentRoom(roomLookup[1]); 
            std::cout << "Warning: Player start room invalid. Placed in Room 1." << std::endl;
        } else {
             std::cerr << "Cannot set player room." << std::endl;
            return false;
        }

        int previousId = player.getPreviousRoomId();
        if (previousId != -1 && roomLookup.count(previousId)) {
            player.setPreviousRoom(roomLookup[previousId]);
        }
        std::cout << "Player data loaded." << std::endl;

    } else {
        std::cerr << "Error: Could not open player load file." << std::endl;
        return false;
    }
    return true;
}

// Function to initialize the game map with rooms and monsters
void initializeMap(TheLady& player, std::vector<Room>& rooms) {
    rooms.clear(); 

    /*********** MONSTERS & DROPS ***********/
    
    // Name, Intel, HP, Exp
    // Spell Drops
    Monster spider(string("Giant Spider"), 1, 3, 4, 302); // Drops Ice Shard
    spider.setWeaknessId(301); // Weakness to Fireball
    spider.addSpell(404); // Posion Spit

    Monster wolf(string("Dire Wolf"), 2, 8, 6, 303); // Drops Wind Gust
    wolf.setWeaknessId(301); // Weakness to Fireball
    wolf.addSpell(401); // Claw Swipe

    Monster bat(string("Cave Bat"), 1, 2, 3, 305); // Drops Water Whip
    bat.setWeaknessId(301); // Weakness to Fireball
    bat.addSpell(402); // Bite

    Monster ghoul(string("Cave Ghoul"), 2, 12, 10, 304); // Drops Earth Quake
    ghoul.setWeaknessId(302); // Weakness to Ice Shard
    ghoul.addSpell(407); // Vicious Bite


    // Ingredient Drops
    // Herbal Leaf Drop
    Monster giantSnake(string("Giant Snake"), 2, 5, 5, 101); // Drops Herbal Leaf
    giantSnake.setWeaknessId(305); // Weakness to Water Whip
    giantSnake.addSpell(404); // Poison Spit
    giantSnake.addSpell(405); // Tail Whip

    Monster lesserVampyre(string("Lesser Vampyre"), 5, 15, 14, 101); // Drops Herbal Leaf
    lesserVampyre.setWeaknessId(301); // Weakness to Fireball
    lesserVampyre.addSpell(402); // Bite
    lesserVampyre.addSpell(407); // Vicious Bite
    lesserVampyre.addSpell(408); // Dark Claw

    Monster dryad(string("Forest Dryad"), 8, 20, 18, 101); // Drops Herbal Leaf
    dryad.setWeaknessId(301); // Weakness to Fireball
    dryad.addSpell(304); // Wind Gust
    dryad.addSpell(401); // Claw Swipe
    dryad.addSpell(409); // Nature's Grasp

    Monster mage(string("Dark Mage"), 6, 20, 18, 101); // Drops herbal Leaf
    mage.setWeaknessId(303); // Weakness to Wind Gust
    mage.addSpell(301); // Fireball
    mage.addSpell(302); // Ice Shard
    mage.addSpell(408); // Dark Claw

    Monster elementalMage(string("Elemental Mage"), 4, 25, 22, 101); // Drops Herbal Leaf
    elementalMage.setWeaknessId(303); // Weakness to Wind Gust
    elementalMage.addSpell(301); // Fireball
    elementalMage.addSpell(303); // Lighting Bolt
    elementalMage.addSpell(304); // Wind Gust
    elementalMage.addSpell(305); // Earth Quake

    // Luminous Dust Drop
    Monster goblin(string("Cave Goblin"), 1, 5, 5, 102); // Drops Luminous Dust
    goblin.setWeaknessId(301); // Weakness to Fireball
    goblin.addSpell(406); // Rock Throw

    Monster Pixie(string("Pixie"), 10, 5, 9, 102); // Drops Luminous Dust
    Pixie.setWeaknessId(304); // Weakness to Earth Quake
    Pixie.addSpell(304); // Wind Gust
    Pixie.addSpell(306); // Water Whip
    Pixie.addSpell(409); // Nature's Grasp

    Monster gargyole(string("Stone Gargoyle"), 5, 30, 32, 102); // Drops Luminous Dust
    gargyole.setWeaknessId(302); // Weakness to Ice Shard
    gargyole.addSpell(305); // Tail Whip
    gargyole.addSpell(406); // Rock Throw


    // Potions Drops
    Monster lich(string("Ancient Lich"), 10, 20, 40, 202); // Drops Intellect Potion
    lich.setWeaknessId(301); // Weakness to Fireball
    lich.addSpell(408); // Shadow Claw
    lich.addSpell(302); // Ice Shard

    Monster archMage(string("Arch Mage"), 12, 25, 45, 201); // Drops Health Potion
    archMage.setWeaknessId(302); // Weakness to Ice Shard
    archMage.addSpell(301); // Fireball
    archMage.addSpell(303); // Lightning Bolt
    archMage.addSpell(304); // Wind Gust
    archMage.addSpell(305); // Earth Quake
    archMage.addSpell(306); // Water Whip

    Monster greaterVampyre(string("Greater Vampyre"), 8, 20, 28, 201); // Drops Health Potion
    greaterVampyre.setWeaknessId(301); // Weakness to Fireball
    greaterVampyre.addSpell(402); // Bite
    greaterVampyre.addSpell(407); // Vicious Bite
    greaterVampyre.addSpell(408); // Dark Claw
    greaterVampyre.addSpell(301); // Fireball
    greaterVampyre.addSpell(404); // Posion Spit

    // Final Boss
    Monster VampyreElder(string("Vampyre Elder"), 15, 50, 60); //Final Boss, No Drop
    VampyreElder.setWeaknessId(301); // Weakness to Fireball
    VampyreElder.setIsFinalBoss(true);
    VampyreElder.addSpell(402); // Bite
    VampyreElder.addSpell(407); // Vicious Bite
    VampyreElder.addSpell(408); // Dark Claw
    VampyreElder.addSpell(301); // Fireball
    VampyreElder.addSpell(303); // Lightning Bolt
    VampyreElder.addSpell(304); // Wind Gust
    VampyreElder.addSpell(404); // Posion Spit

    /*********** Levels ***********/
    Room room1(1, "The Cavern Mouth: Cold air flows outward like the last exhalation of something sleeping below. Weak daylight barely reaches inside."); // 1
    Room room2(2, "A Soaked Passage: Puddles gather where water drips from the ceiling — but the liquid is faintly pink, tinted by mineral iron."); // 2
    Room room3(3, "A Narrow Passage: A cold current rushes past your face, sharp and metallic, like the smell of fresh blood."); // 3
    Room room4(4, "Boulder Grave: Toppled boulders lie like giant tombstones. Strange grooves on them resemble claw marks."); // 4
    Room room5(5, "Murmuring Grotto: Whispers bounce through this chamber, though their source is unseen. The air feels thick with unseen eyes."); // 5
    Room room6(6, "Crimson Vein Hall: The walls pulse faintly red, as if the cavern itself has a heartbeat. The ground is littered with shattered bones."); // 6
    Room room7(7, "Fungal Clove: Bioluminescent fungi cast eerie glows, illuminating grotesque shapes that seem to shift when not directly observed."); // 7
    Room room8(8, "Vampyric Manor Front: The remnants of an ancient manor loom, its walls slick with moisture and stained with dark streaks that resemble dried blood."); // 8
    Room room9(9, "Vampyric Manor Entrance Hall: The grand entrance hall is draped in shadows, with tattered banners hanging from the walls and a chilling draft that seems to whisper forgotten secrets."); // 9
    Room room10(10, "Torture Chamber: Rusted instruments of pain are scattered about, and the air is thick with the scent of decay and despair."); // 10
    Room room11(11, "The Pit: A vast chasm splits the manor, its depths shrouded in darkness. Faint, agonized wails echo from below, sending shivers down your spine."); // 11
    Room room12(12, "Ember Blood Hall: The walls here glow with an unnatural red light, as if infused with molten blood. The heat is oppressive, and the air tastes of iron and ash."); // 12
    Room room13(13, "Manor Dining Hall: A long table dominates the room, set for a feast that never was. Broken dishes and tarnished silverware lie amidst dried bloodstains."); // 13
    Room room14(14, "Manor Kitchen: The kitchen is a scene of chaos, with overturned pots and scattered ingredients. The scent of spoiled food mingles with the metallic tang of blood."); // 14
    Room room15(15, "Trophy Room: The walls are adorned with grotesque trophies—skulls, bones, and other macabre artifacts taken from fallen adventurers."); // 15
    Room room16(16, "Ritual Chamber: Dark symbols are etched into the floor and walls, glowing faintly with an eerie light. The air hums with latent magical energy."); // 16
    Room room17(17, "Elder Blood Sanctum: A creature of immense power resides here, its presence warping the very fabric of reality. The ground is littered with the remnants of those who dared to challenge it."); // 17

    // Populate Rooms
    
    // Room 2
    room2.addMonster(bat);
    
    // Room 3 
    room3.addMonster(goblin); 
    room3.addMonster(giantSnake);
    room3.addMonster(giantSnake);
    
    // Room 4
    room4.addMonster(spider); 
    room4.addMonster(goblin);
    room4.addMonster(wolf);
    
    // Room 5
    room5.addMonster(bat);
    room5.addMonster(bat);
    room5.addMonster(spider);
    room5.addMonster(goblin);
    room5.addMonster(wolf); 
    
    // Room 6
    room6.addMonster(giantSnake); 
    room6.addMonster(giantSnake);
    room6.addMonster(goblin);
    room6.addMonster(goblin);
    
    // Room 7
    room7.addMonster(giantSnake);
    room7.addMonster(ghoul); 
    
    // Room 8
    room8.addMonster(lesserVampyre);
    room8.addMonster(mage);
    
    // Room 9
    room9.addMonster(dryad);
    room9.addMonster(elementalMage);
    
    // Room 10
    room10.addMonster(greaterVampyre);
    room10.addMonster(mage);
    
    // Room 11: The Pit - EXTRA HARD OPTIONAL CHALLENGE (Come back later stronger)
    room11.addMonster(archMage); 
    room11.addMonster(lich);
    room11.addMonster(greaterVampyre);
    room11.addMonster(greaterVampyre);
    
    // Room 12
    room12.addMonster(archMage);
    room12.addMonster(gargyole);
    
    // Room 13
    room13.addMonster(lich);
    room13.addMonster(archMage);
    room13.addMonster(mage);
    
    // Room 14
    room14.addMonster(archMage);
    room14.addMonster(archMage);
    room14.addMonster(elementalMage);
    room14.addMonster(greaterVampyre);
    
    // Room 15
    room15.addMonster(greaterVampyre);
    room15.addMonster(greaterVampyre);
    room15.addMonster(lich);
    room15.addMonster(archMage);
    room15.addMonster(elementalMage);
    
    // Room 16
    room16.addMonster(archMage);
    room16.addMonster(lich);
    room16.addMonster(gargyole);
    
    // Room 17: FINAL BOSS
    room17.addMonster(VampyreElder);

    // Add rooms to the dungeon first
    rooms.reserve(17); // Reserve space for 17 rooms
    rooms.push_back(room1);
    rooms.push_back(room2);
    rooms.push_back(room3);
    rooms.push_back(room4);
    rooms.push_back(room5);
    rooms.push_back(room6);
    rooms.push_back(room7);
    rooms.push_back(room8);
    rooms.push_back(room9);
    rooms.push_back(room10);
    rooms.push_back(room11);
    rooms.push_back(room12);
    rooms.push_back(room13);
    rooms.push_back(room14);
    rooms.push_back(room15);
    rooms.push_back(room16);
    rooms.push_back(room17);

    // Define Connections
    rooms[0].connections.south = &rooms[1]; // room1 -> room2 South
    rooms[1].connections.north = &rooms[0]; // room2 -> room1 North

    rooms[1].connections.south = &rooms[2];  // room2 -> room3 South
    rooms[2].connections.north = &rooms[1];  // room3 -> room2 North

    rooms[2].connections.south = &rooms[3];  // room3 -> room4 South
    rooms[3].connections.north = &rooms[2];  // room4 -> room3 North

    rooms[3].connections.east = &rooms[4];  // room4 -> room5 East
    rooms[4].connections.west = &rooms[3];  // room5 -> room4 West

    rooms[1].connections.east = &rooms[5];  // room2 -> room6 East
    rooms[5].connections.west = &rooms[1];  // room6 -> room2 West

    rooms[4].connections.east = &rooms[6];  // room5 -> room7 East
    rooms[6].connections.west = &rooms[4];  // room7 -> room5 West

    rooms[5].connections.east = &rooms[10];  // room6 -> room11 East
    rooms[10].connections.west = &rooms[5];  // room11 -> room6 West

    rooms[6].connections.south = &rooms[7];  // room7 -> room8 South
    rooms[7].connections.north = &rooms[6];  // room8 -> room7 North

    rooms[7].connections.south = &rooms[8];  // room8 -> room9 South
    rooms[8].connections.north = &rooms[7];  // room9 -> room8 North

    rooms[8].connections.east = &rooms[9];  // room9 -> room10 East
    rooms[9].connections.west = &rooms[8];  // room10 -> room9 West

    rooms[9].connections.north = &rooms[10];  // room10 -> room11 North
    rooms[10].connections.south = &rooms[9];  // room11 -> room10 South

    rooms[8].connections.south = &rooms[11];  // room9 -> room12 South
    rooms[11].connections.north = &rooms[8];  // room12 -> room9 North

    rooms[8].connections.west = &rooms[12];  // room9 -> room13 West
    rooms[12].connections.east = &rooms[8];  // room13 -> room9 East

    rooms[12].connections.south = &rooms[13];  // room13 -> room14 South
    rooms[13].connections.north = &rooms[12];  // room14 -> room13 North

    rooms[13].connections.south = &rooms[14];  // room14 -> room15 South
    rooms[14].connections.north = &rooms[13];  // room15 -> room14 North

    rooms[14].connections.east = &rooms[11];  // room15 -> room12 East
    rooms[11].connections.west = &rooms[14];  // room12 -> room15 West

    rooms[11].connections.east = &rooms[15];  // room12 -> room16 East
    rooms[15].connections.west = &rooms[11];  // room16 -> room12 West

    rooms[15].connections.south = &rooms[16];  // room16 -> room17 South
    rooms[16].connections.north = &rooms[15];  // room17 -> room16 North

    rooms[15].connections.north = &rooms[9];  // room16 -> room10 North
    rooms[9].connections.south = &rooms[15];  // room10 -> room16 South


    /* Set Player Starting Room */
    player.setCurrentRoom(&rooms[0]);
    player.setCurrentRoomId(rooms[0].getId());
}

class GameLoop {
private:
    TheLady player;
    std::vector<Room> rooms;

    void loop() {
        printSeparator();
        cout << "Welcome, The Lady, to the Dungeon of the Forgotten." << endl;
        cout << "Your journey begins now..." << endl;
        
        if (player.getCurrentRoom() == nullptr) {
            cerr << "CRITICAL ERROR: Player's current room is NULL after load/init. Exiting." << endl;
            return;
        }

        // Game Loop
        while (player.getCurrentHealth() > 0) {
            Room* currentRoom = player.getCurrentRoom();

            if (!currentRoom->isClear()) {
                cout << "\nThe room is filled with hostile creatures!\n" << endl;
                auto & monsters = currentRoom->getMonsters();
                cout << "Monsters in the room:" << endl;
                for (size_t i = 0; i < monsters.size(); ++i) {
                    cout << i + 1 << ". " << monsters[i].getName() << endl;
                }
                printSeparator();
            }

            while (!currentRoom->isClear()) {
                if (currentRoom->isClear()) {
                    saveGame(player, rooms); // Save game on clearing room
                }

                // Combat
                if (handlePlayerTurn(player, *currentRoom)) {
                    // If player fled, the room changed, so break the combat loop.
                    currentRoom = player.getCurrentRoom();
                    break;
                } 
                if (player.getCurrentHealth() <= 0) break; 

                handleMonsterTurn(player, *currentRoom);
                if (player.getCurrentHealth() <= 0) break;
            }

            // After combat/flee, check if the player died
            if (player.getCurrentHealth() <= 0) break;

            // Explore
            printSeparator();
            cout << "Enter a command (move, camp, potions, stats, bestiary, menu): ";

            string input;
            getline(cin, input);
            transform(input.begin(), input.end(), input.begin(), ::tolower);

            currentRoom = player.getCurrentRoom(); // Re-fetch in case of flee

            if (input == "move"){
                cout << "\nCurrent Location: " << currentRoom->getDescription() << "\n" << endl; 
                currentRoom->displayExits();

                string direction;
                cout << "\nEnter direction to move (north/south/east/west) or 'cancel' to stop moving: ";
                getline(cin, direction);
                transform(direction.begin(), direction.end(), direction.begin(), ::tolower);
                
                if (direction != "cancel") {
                    player.move(direction);
                }
                
            } else if (input == "camp") {
                handleCampCommand(player);
                saveGame(player, rooms); // Save game on leaving camp
            } else if (input == "potions") {
                handlePotionsCommand(player);
            } else if (input == "stats") {
                player.toString();
            } else if (input == "bestiary") {
                player.displayBestiary();
            } else if (input == "menu") {
                saveGame(player, rooms); // Save game on exiting to menu
                break;
            } else {
                cout << "Unknown command. Try 'move', 'camp', 'potions', 'stats', 'bestiary', or 'menu'." << endl;
            }
        }
        end(player);
    }

    void end(TheLady& player) {
        printSeparator();

        if (player.getCurrentHealth() <= 0) {
            cout << "The Lady has fallen..." << endl;
            cout << "Loading Game From Last Save Point..." << endl;
            if(loadGame(player, rooms)) {
                cout << "Game Loaded Successfully. Resuming Game..." << endl;
                loop(); // Restart game loop
            } else {
                cout << "No valid save found. Starting a New Game..." << endl;
                initializeMap(player, rooms);
                saveGame(player, rooms); // Save initial state
                loop();
            }
        } else {
            cout << "Returning to main menu..." << endl;
            start();
        }
    }

public:
    void start() {
        string input;
        
        while (true) {
            printSplashScreen();
            cout << "1. New Game" << endl;
            cout << "2. Load Game" << endl;
            cout << "3. Exit" << endl;
            cout << "Choose an option: ";
            getline(cin, input);

            if (input == "1") {
                initializeMap(player, rooms);
                saveGame(player, rooms); // Save initial state
                loop();
                break;
            } else if (input == "2") {
                if(loadGame(player, rooms)) {
                    loop();
                    break;
                } else {
                    cout << "Failed to load game. Starting a New Game..." << endl;
                    initializeMap(player, rooms);
                    saveGame(player, rooms); // Save initial state
                    loop();
                    break;
                }
            } else if (input == "3") {
                cout << "Exiting game." << endl;
                break;
            }else {
                cout << "Invalid option. Please try again." << endl;
            }
        }
    }
};