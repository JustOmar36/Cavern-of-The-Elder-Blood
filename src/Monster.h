#ifndef MONSTER_H
#define MONSTER_H

#include <string>
#include "Character.h" 

class Monster : public Character {
public:
    Monster() : Character("", 0, 0, 0, 0) {};

    Monster(std::string name, int intel, float hp, int exp) : Character(name, intel, hp, hp, exp) {
        setDropItemId(-1); // no item
    }
    
    Monster(std::string name, int intel, float hp, int exp, int dropId) 
    : Character(name, intel, hp, hp, exp) {
        setDropItemId(dropId);
    }

    virtual ~Monster() = default;

    Spell getSpell() override {
        
        if (spells.empty()) {
            return Spell(); // No spells available
        }

        // Return a random spell from the spells vector
        int randomIndex = rand() % spells.size();
        return spells[randomIndex];
    }

    bool addSpell(int spellId) override {
        const Item* spellDef = GetItemDefinitionFromDatabase(spellId);
        if (spellDef && spellDef->getIsSpell()) {
            spells.push_back(static_cast<const Spell&>(*spellDef));
            return true;
        }
        return false;
    }
};

#endif