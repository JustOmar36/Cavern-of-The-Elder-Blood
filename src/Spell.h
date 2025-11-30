#ifndef SPELL_H
#define SPELL_H
#include "Item.h"

#include <string>
class Spell : public Item{
private:
    float damage; // Damage dealt by the spell

public:
    Spell() = default;

    Spell(std::string name, float damage)
        : Item(0, name, "", true, false), damage(damage) {}

    Spell(int id, std::string name, std::string desc, float damage)
        : Item(id, name, desc, true, false), damage(damage) {}

    ~Spell() = default;

    float getDamage() const { return damage; }
};

#endif