#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>
#include <cmath>
#include "Spell.h"

class Character {
protected:
    // Player States
    std::string name;
    int intellect; //Research Success rate, trap efficacy, alchemy success and damage
    float currentHealth;
    float maxHealth;

    //combat
    std::vector<Spell> spells; //The Lady's Spells Inventory

    //Experience to give player
    int exp;

    //Item to Drop ID
    int dropItemId;

    // Weakness ID
    int weaknessId = -1; // Default to -1 (no weakness)

    //Final Boss
    bool isFinalBoss = false;

public:

    Character() = default;

    Character(std::string player_name, int intellect, float currentHealth, float maxHealth) : name(player_name), intellect(intellect),
        currentHealth(currentHealth), maxHealth(maxHealth) {}; //No Exp

    Character(std::string player_name, int intellect,float currentHealth, float maxHealth, int exp) : 
        name(player_name), intellect(intellect), currentHealth(currentHealth), maxHealth(maxHealth), exp(exp) {}; //Exp
    virtual ~Character() = default;

    //Abstract function
    virtual Spell getSpell() = 0;
    virtual bool addSpell(int spellId) = 0;

    // Public Getters
    int getIntellect() const { return intellect; }
    float getCurrentHealth() const { return std::round(currentHealth * 100) / 100; } // Round to 2 decimal places
    float getMaxHealth() const { return maxHealth; }

    // Public Setters
    void SetIntellect(int intel) { intellect = intel; }
    void setCurrentHealth(float health) { currentHealth = health; }
    void setMaxHealth(float health) { maxHealth = health; }
    int getExperience() const { return exp; }
    std::string getName() const { return name; }

    // Combat
    int getDropItemId() const { return dropItemId; }
    std::vector<Spell>& getSpellsInventory() { return spells; }
    void displaySpells() const;
    int getWeaknessId() const { return weaknessId; }
    void setDropItemId(int itemId) { dropItemId = itemId; }
    void setWeaknessId(int weakness) { weaknessId = weakness; }
    

    // Core Gameplay Methods
    void healPlayer(float amount);
    virtual void toString() const;

    // Save/Load Methods
    virtual void save(std::fstream& file) const;
    virtual void load(std::fstream& file);

    //operator overloading - acts as doDamage function
    friend void operator*(Character& instigator, Character& target);

    bool getIsFinalBoss() const { return isFinalBoss; }
    void setIsFinalBoss(bool isBoss) { isFinalBoss = isBoss; }
};

#endif

const Item* GetItemDefinitionFromDatabase(int itemId);