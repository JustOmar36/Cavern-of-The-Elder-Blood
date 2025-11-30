#include "Character.h"
#include "TheLady.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <cmath>

void operator*(Character& instigator, Character& target) {

    // Damage Calculations
    // Base damage scaled by attacker intellect (multiplicative increase)
    Spell spell = instigator.getSpell();
    float damage = spell.getDamage() * (1.0 + instigator.getIntellect() / 20.0);
    
    // Defense scales as a percentage reduction (multiplicative mitigation)
    float defenseMultiplier = (1.0 - (target.getIntellect() / 50.0));
    damage *= defenseMultiplier;

    if(target.getWeaknessId() != spell.getItemId()) {
        damage *= 0.5; // 50% less damage if no weakness
    }else{
        damage *= 0.7; // 30% less damage if weakness
    }
    
    // Round damage to nearest tenth
    damage = std::round(damage * 10.0) / 10.0;

    std::cout << "\n" << instigator.getName() << " uses " << spell.getName() << " on " << target.getName() << "!" << std::endl;

    // Target dodge - only triggers if damage is negligible
    if (damage < 0.5) {
        std::cout << target.getName() << " Dodged" << std::endl;
        return;
    }

    // Apply damage
    target.setCurrentHealth(target.getCurrentHealth() - damage);
    std::cout << instigator.getName() << " dealt " << damage << " damage to " << target.getName() << std::endl;

    //Death and Reward Logic
    if (target.getCurrentHealth() <= 0) {
        std::cout << "\n" << target.getName() << " has been killed!" << std::endl;

        if(target.getIsFinalBoss()) {
            std::cout << "Congratulations! You have defeated the Final Boss and completed the game!" << std::endl;
            exit(0); // End the game
        }

        // If the Instrigator is Lady -> Gain Experience and Item Drop
        if (TheLady* lady = dynamic_cast<TheLady*>(&instigator)) {

            lady->gainExperience(target.getExperience());
            lady->addToBestiary(target.getName(), GetItemDefinitionFromDatabase(target.getWeaknessId())->getName());

            // Lady picks up dropped item if any
            if (target.getDropItemId() > 0) {
                const Item* droppedItem = GetItemDefinitionFromDatabase(target.getDropItemId());
                if (droppedItem) {
                    lady->pickUpItem(*droppedItem);
                } else {
                    std::cout << "Dropped item ID " << target.getDropItemId() << " not found in database." << std::endl;
                }
            }
        }
    }
}

std::map<int, std::shared_ptr<Item>> ITEM_DATABASE = {
    // Ingredients (IDs 100-199)
    {101, std::make_shared<Item>(101, "Herbal Leaf", "A fibrous plant leaf used in alchemy.", "Health Potion Recipe", false, false)},
    {102, std::make_shared<Item>(102, "Luminous Dust", "Shimmering dust from rare mushrooms.", "Intellect Potion Recipe", false, false)},
    // {103, std::make_shared<Item>(103, "Crystal Shard", "A fragment of a magical crystal.", "Greater Health Potion Recipe", false, false)},
    // {104, std::make_shared<Item>(104, "Pixie Dust", "Magical dust with enchanting properties.", "Greater Intellect Potion Recipe", false, false)},
    // {105, std::make_shared<Item>(105, "Phoenix Essence", "A fiery essence with rejuvenating properties.", "Elixir of Vitality Recipe", false, false)},
    // {106, std::make_shared<Item>(106, "Sage Blossom", "A rare flower that enhances wisdom.", "Elixir of Wisdom Recipe", false, false)},
    
    // Potions (IDs 200-299)
    {201, std::make_shared<Item>(201, "Health Potion", "Restores a portion of health when consumed.", "Heals to Max Health", false, true)},
    {202, std::make_shared<Item>(202, "Intellect Potion", "Increases intellect for a short time.", "Increases Intellect by 3 for 4 turns", false, true)},
    // {203, std::make_shared<Item>(203, "Greater Health Potion", "Restores a large portion of health when consumed.", "Greater Health Potion Recipe", false, true)},
    // {204, std::make_shared<Item>(204, "Greater Intellect Potion", "Greatly increases intellect for a short time.", "Greater Intellect Potion Recipe", false, true)},
    // {205, std::make_shared<Item>(205, "Elixir of Vitality", "Temporarily boosts health significantly.", "Elixir of Vitality Recipe", false, true)},
    // {206, std::make_shared<Item>(206, "Elixir of Wisdom", "Temporarily boosts intellect significantly.", "Elixir of Wisdom Recipe", false, true)},
    
    // Attacks (IDs 300-499)
    // Player spells - balanced to feel powerful but intellect-dependent
    {301, std::make_shared<Spell>(301, "Fireball", "A spell that launches a fiery projectile at enemies.", 2.5)},
    {302, std::make_shared<Spell>(302, "Ice Shard", "A spell that hurls a shard of ice to freeze foes.", 3.5)},
    {303, std::make_shared<Spell>(303, "Wind Gust", "A spell that unleashes a gust of wind to knock back enemies.", 2.5)},
    {304, std::make_shared<Spell>(304, "Earthquake", "A spell that causes the ground to shake violently.", 2.5)},
    {305, std::make_shared<Spell>(305, "Water Whip", "A spell that lashes out with a whip of water.", 2.5)},

    // Monster spells - scaled by their intellect, adjusted for difficulty balance
    {401, std::make_shared<Spell>(401, "Claw Swipe", "A quick swipe with sharp claws.", 1.2)}, // Wolf
    {402, std::make_shared<Spell>(402, "Bite", "A bite attack.", 1.0)}, // Bat
    {403, std::make_shared<Spell>(403, "Fire Breath", "A fiery breath attack.", 2.0)}, // Dragon, Fire Elemental
    {404, std::make_shared<Spell>(404, "Poison Spit", "Spits venomous poison at the target.", 1.8)}, // Spider
    {405, std::make_shared<Spell>(405, "Tail Whip", "A powerful tail strike.", 1.6)}, // Giant Snake
    {406, std::make_shared<Spell>(406, "Rock Throw", "Hurls a rock at the target.", 1.3)}, // Goblin, Troll
    {407, std::make_shared<Spell>(407, "Vicious Bite", "A bite attack that drains life.", 1.9)}, // Ghoul
    {408, std::make_shared<Spell>(408, "Shadow Claw", "A dark claw attack that saps strength.", 2.0)}, // Dark Mage
    {409, std::make_shared<Spell>(409, "Nature's Grasp", "A powerful root attack that sends spikes upward.", 2.0)} // dryad
};

const Item* GetItemDefinitionFromDatabase(int itemId) {
    auto it = ITEM_DATABASE.find(itemId);
    if (it != ITEM_DATABASE.end()) {
        return it->second.get();
    }

    return nullptr;
}

void Character::toString() const {
    std::cout << "Character: " << name << std::endl;
    std::cout << "Intellect: " << intellect << std::endl;
    std::cout << "Health: " << currentHealth << "/" << maxHealth << std::endl;
}

void Character::healPlayer(float amount) {
    Character::setCurrentHealth(Character::getCurrentHealth() + amount);
    if (Character::getCurrentHealth() > Character::getMaxHealth()) {
        Character::setCurrentHealth(Character::getMaxHealth()); // Cap health at max
    }
    std::cout << Character::getName() << " healed for " << amount << " points." << std::endl;
};

void Character::displaySpells() const {
    std::cout << "--- Spells ---" << std::endl;
    if (spells.empty()) {
        std::cout << "No spells learned." << std::endl;
    } else {
        int count = 1;
        for (const auto& spell : spells) {
            std::cout << count << ". " << spell.getName() << " - " << spell.getDamage() << " Damage" << std::endl;
            count++;
        }
    }
    std::cout << "--------------" << std::endl;
}

//Save/Load Methods
void Character::save(std::fstream& file) const {
    file.write(reinterpret_cast<const char*>(&intellect), sizeof(intellect));
    file.write(reinterpret_cast<const char*>(&currentHealth), sizeof(currentHealth));
    file.write(reinterpret_cast<const char*>(&maxHealth), sizeof(maxHealth));
    file.write(reinterpret_cast<const char*>(&exp), sizeof(exp));
    file.write(reinterpret_cast<const char*>(&dropItemId), sizeof(dropItemId));
    file.write(reinterpret_cast<const char*>(&weaknessId), sizeof(weaknessId));
    file.write(reinterpret_cast<const char*>(&isFinalBoss), sizeof(isFinalBoss));

    size_t nameLength = name.length();
    file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    file.write(name.c_str(), nameLength);

    //save spells
    size_t spellCount = spells.size();
    file.write(reinterpret_cast<const char*>(&spellCount), sizeof(spellCount));
    for (const auto& spell : spells) {
        int itemId = spell.getItemId();
        file.write(reinterpret_cast<const char*>(&itemId), sizeof(itemId));
    }
}

void Character::load(std::fstream& file) {
    file.read(reinterpret_cast<char*>(&intellect), sizeof(intellect));
    file.read(reinterpret_cast<char*>(&currentHealth), sizeof(currentHealth));
    file.read(reinterpret_cast<char*>(&maxHealth), sizeof(maxHealth));
    file.read(reinterpret_cast<char*>(&exp), sizeof(exp));
    file.read(reinterpret_cast<char*>(&dropItemId), sizeof(dropItemId));
    file.read(reinterpret_cast<char*>(&weaknessId), sizeof(weaknessId));
    file.read(reinterpret_cast<char*>(&isFinalBoss), sizeof(isFinalBoss));

    size_t nameLength = 0;
    file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
    
    if (nameLength > 0) {
        char* nameBuffer = new char[nameLength + 1];
        file.read(nameBuffer, nameLength);
        nameBuffer[nameLength] = '\0'; // Null-terminate the string
        name = nameBuffer;
        delete[] nameBuffer;
    } else {
        name = "";
    }

    //load spells
    size_t spellCount = 0;
    file.read(reinterpret_cast<char*>(&spellCount), sizeof(spellCount));
    spells.clear();
    for (size_t i = 0; i < spellCount; ++i) {
        int itemId = 0;
        file.read(reinterpret_cast<char*>(&itemId), sizeof(itemId));
        const Item* spellDef = GetItemDefinitionFromDatabase(itemId);
        if (spellDef && spellDef->getIsSpell()) {
            spells.push_back(static_cast<const Spell&>(*spellDef));
        }
    }
}