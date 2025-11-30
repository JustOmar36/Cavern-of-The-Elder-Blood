#ifndef THELADY_H
#define THELADY_H

#include <string>
#include "Character.h"
#include "Inventory.h"
#include "Item.h"
#include "Spell.h"
#include "Room.h"
#include <vector>

class Monster;
class Room;

class TheLady : public Character {

protected:
    int expToNextLevel;
    int level;

    Inventory<Item> ingredients; //The Lady's Ingredients Inventory
    Inventory<Item> potions;     //The Lady's Potions Inventory

    std::vector<Item> knownRecipes; // List of known recipes
    std::vector<Item> studiedIngredients; // List of brewed potions

    Room* currentRoom; // The Lady's current location in the dungeon
    Room* previousRoom; // The Lady's previous location in the dungeon
    int CurrentRoomId = -1; 
    int PreviousRoomId = -1;

    Monster* currentTarget; // The Lady's current combat target

    // Damage Potion Effect tracking
    int intellectBoostAmount; // Amount to boost damage by
    int intellectBoostTurnsRemaining; // Number of turns the boost lasts
    int intellectBeforeBoost; // Original damage to restore

    std::vector<std::pair<std::string, std::string>> bestiary; // Monster name and weakness

public:

    TheLady();
    virtual ~TheLady();

    //abstract function
    Spell getSpell() override;
    bool addSpell(int spellId) override;

    // Damage boost management
    void updateIntellectBoost(); // Called each turn to decrement duration and revert if needed

    int getLevel() const { return level; }
    int getExperienceToNextLevel() const { return expToNextLevel; }
    void SetLevel(int lvl) { level = lvl; }
    void SetExperience(int experience) { exp = experience; }
    void SetExperienceToNextLevel(int expNext) { expToNextLevel = expNext; }
    void gainExperience(int experience);

    // Bestiary management
    const std::vector<std::pair<std::string, std::string>>& getBestiary() const { return bestiary; }
    void addToBestiary(const std::string& monsterName, const std::string& weaknessName);
    void displayBestiary() const;

    //Combat
    Monster* getCurrentTarget() const { return currentTarget; }
    void setCurrentTarget(Monster* target) { currentTarget = target; }

    //Movement
    void move(const std::string& direction);
    void setCurrentRoom(Room* room) { currentRoom = room; }
    void setPreviousRoom(Room* room) { previousRoom = room; }
    void setCurrentRoomId(int roomId) { CurrentRoomId = roomId; }
    void setPreviousRoomId(int roomId) { PreviousRoomId = roomId; }
    Room* getCurrentRoom() const { return currentRoom; }
    Room* getPreviousRoom() const { return previousRoom; }
    int getCurrentRoomId() const { return CurrentRoomId; }
    int getPreviousRoomId() const { return PreviousRoomId; }
    void flee();

    //Skills
    void brewPotion(int recipeItemId);
    void usePotion(int potionItemId);

    // Inventory
    void pickUpItem(const Item& item);
    void studyItem(int itemId); 
    void getItemInfo(int itemId) const;
    const std::vector<Item>& getKnownRecipes() const { return knownRecipes; }
    const std::vector<Item>& getStudiedIngredients() const { return studiedIngredients; }
    void addKnownRecipe(const Item& recipe) { knownRecipes.push_back(recipe); }
    void addIngredient(const Item& ingredient) { studiedIngredients.push_back(ingredient); }
    Inventory<Item>& getIngredientsInventory() { return ingredients; }
    Inventory<Item>& getPotionsInventory() { return potions; }

    //Override
    void toString() const override;

    // Override Save/Load The Lady
    void save(std::fstream& file) const override;
    void load(std::fstream& file) override;
};

#endif