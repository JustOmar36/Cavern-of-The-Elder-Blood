#include "TheLady.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include <string>

// Helper to check if a recipe is known
bool isRecipeKnown(const TheLady& lady, int itemId) {
    for (const auto& recipe : lady.getKnownRecipes()) {
        if (recipe.getItemId() == itemId) {
            return true; // Recipe is known
        }
    }
    return false;
}

// Helper to add a recipe to Lady's known list
void addRecipe(TheLady& lady, const Item& item) {
    if (item.getItemId() != 0 && !isRecipeKnown(lady, item.getItemId())) {
        lady.addKnownRecipe(item);
    }
}

bool isIngredientStudied(const TheLady& lady, int itemId) {
    for (const auto& ingredient : lady.getStudiedIngredients()) {
        if (ingredient.getItemId() == itemId) {
            return true; // Ingredient is studied
        }
    }
    return false;
}

void addStudiedIngredient(TheLady& lady, const Item& item) {
    if (item.getItemId() != 0 && !isIngredientStudied(lady, item.getItemId())) {
        lady.addIngredient(item);
    }
}

TheLady::TheLady() : 
    
    Character(std::string("The Lady"), 1, 12, 12, 0)
{
    level = 1;
    expToNextLevel = 5;
    intellectBoostAmount = 0;
    intellectBoostTurnsRemaining = 0;
    intellectBeforeBoost = 0;

    ingredients.setInventoryName("Ingredients");
    potions.setInventoryName("Potions");

    addSpell(301); // Basic Spell
}

TheLady::~TheLady() {
    knownRecipes.clear();
    studiedIngredients.clear(); 
    bestiary.clear();
    spells.clear();
    
    currentRoom = nullptr;
    previousRoom = nullptr;
    currentTarget = nullptr;
    
    std::cout << "The Lady expects your return." << std::endl;
}

void TheLady::gainExperience(int experience) {
    exp += experience;
    std::cout << "Gained " << experience << " experience points." << std::endl;

    if (exp >= expToNextLevel) {
        exp -= expToNextLevel;
        level++;
        intellect += 1; // Increase intellect by 1 each level
        maxHealth += 3; // Increase max health by 3 each level to improve survivability
        currentHealth = maxHealth; // Restore health to new max on level up so player can continue without immediate death
        expToNextLevel *= 1.2; // Increase exp needed for next level
        std::cout << "Leveled up to Level " << level << "!" << std::endl;
    }
}

void TheLady::toString() const {
    std::cout << "Character: " << name << std::endl;
    std::cout << "Level: " << level << std::endl;
    std::cout << "Experience: " << exp << std::endl;
    std::cout << "Experience to Next Level: " << expToNextLevel << std::endl;
    std::cout << "Intellect: " << intellect << std::endl;
    std::cout << "Health: " << currentHealth << "/" << maxHealth << std::endl;

    std::cout << "----------------------------------" << std::endl;
    std::cout << "--- Known Spells (" << spells.size() << ") ---" << std::endl;
    for(const auto& spell : spells) {
        std::cout << "- " << spell.getName() << std::endl;
    }

    std::cout << "----------------------------------" << std::endl;
    std::cout << "--- Known Recipes (" << knownRecipes.size() << ") ---" << std::endl;
    for(const auto& recipe : knownRecipes) {
        std::cout << "- " << recipe.getName() << std::endl;
    }
    std::cout << "----------------------------------" << std::endl;
}

// Inventory
void TheLady::getItemInfo(int itemId) const {
    assert(itemId > 0 && "Item ID must be positive.");

    const Item* item = GetItemDefinitionFromDatabase(itemId);

    if (item) {
        std::cout << "Item Info:" << std::endl;
        std::cout << "Name: " << item->getName() << std::endl;
        std::cout << "Description: " << item->getDescription() << std::endl;
        
        if (isIngredientStudied(*this, itemId)) {
            std::cout << "Effect: " << item->getStudyEffectKey() << std::endl;
        } else if (isRecipeKnown(*this, itemId)) {
            std::cout << "Effect: " << item->getStudyEffectKey() << std::endl;
        }else {
            std::cout << "Effect: [Unknown - Study the ingredient to reveal]" << std::endl;
        }
        
    } else {
        std::cout << "Item with ID " << itemId << " not found in ingredients inventory." << std::endl;
    }
}

void TheLady::pickUpItem(const Item& item) {
    if (item.getIsConsumable()) {
        potions.addItem(item);
        std::cout << "Picked up: " << item.getName() << " (Potion)" << std::endl;
        return;
    }

    if (item.getIsSpell()) {
        if(addSpell(item.getItemId())) {
            std::cout << "Picked up: " << item.getName() << " (Spell)" << std::endl;
        }
        return;
    }

    ingredients.addItem(item);
    std::cout << "Picked up: " << item.getName() << std::endl;

}

void TheLady::studyItem(int itemId) {
    const Item* itemToStudy = GetItemDefinitionFromDatabase(itemId);

    if (!itemToStudy || ingredients.getItemCount(itemId) == 0) {
        std::cout << "You do not have the item (" << itemId << ") in your ingredients to study." << std::endl;
        return;
    }
    
    if (isIngredientStudied(*this, itemId)) {
        std::cout << "You have already studied the " << itemToStudy->getName() << "." << std::endl;
        return;
    }

    if (itemToStudy->getStudyEffectKey().empty()) {
        std::cout << itemToStudy->getName() << " offers no knowledge to be gained." << std::endl;
        return;
        
    } else {
        // Reveal the effect on the item instance in the inventory
        std::cout << "The Lady studies the " << itemToStudy->getName() << "." << std::endl;
        std::cout << "Knowledge Gained: " << itemToStudy->getStudyEffectKey() << " unlocked!\n" << std::endl;
        std::cout << itemToStudy->getName() << " was consumed in the process." << std::endl;

        // Mark the item as revealed
        addStudiedIngredient(*this, *itemToStudy);

        // Add to known recipes
        itemToStudy = GetItemDefinitionFromDatabase(itemId + 100); // Recipes are offset by 100 from ingredient IDs
        if (!itemToStudy) {
            std::cout << "Error: Could not find recipe item for ID " << (itemId + 100) << "." << std::endl;
            return;
        }

        addRecipe(*this, *itemToStudy);
    
        ingredients.removeItem(itemId);
    }

}

void TheLady::brewPotion(int recipeItemId) {
    recipeItemId += 100; // Potion IDs are offset by 100 from recipe IDs
    const Item* potionDef = GetItemDefinitionFromDatabase(recipeItemId);
    if (!potionDef) {
        std::cout << "Error: Invalid Potion ID (" << recipeItemId << ")." << std::endl;
        return;
    } 

    // --- Recipe Checks ---
    if (!isRecipeKnown(*this, potionDef->getItemId())) {
        std::cout << "You haven't studied this item yet!" << std::endl;
        return;
    }
    
    if (recipeItemId == 201) { // Health Potion
        const int HERBAL_NODE_ID = 101;
        
        if (ingredients.getItemCount(HERBAL_NODE_ID) >= 1) {
            
            //Consume ingredients
            ingredients.removeItem(HERBAL_NODE_ID); 
            
            //Create and add potion to the potions inventory
            Item healthPotion = *potionDef; // Get a copy from the database
            potions.addItem(healthPotion);
            
            std::cout << "Successfully brewed a Health Potion!" << std::endl;
            
        } else {
            std::cout << "Missing ingredient: Need " << 1 << " Herbal Node(s)." << std::endl;
        }
    } else if (recipeItemId == 202) { // Damage Potion
        const int LUMINOUS_DUST_ID = 102;
        
        if (ingredients.getItemCount(LUMINOUS_DUST_ID) >= 1) {
            
            //Consume ingredients
            ingredients.removeItem(LUMINOUS_DUST_ID); 
            
            //Create and add potion to the potions inventory
            Item damagePotion = *potionDef; // Get a copy from the database
            potions.addItem(damagePotion);
            
            std::cout << "Successfully brewed a Damage Potion!" << std::endl;
            
        } else {
            std::cout << "Missing ingredient: Need " << 1 << " Luminous Dust(s)." << std::endl;
        }
    }

    //More recipes to come...
}

void TheLady::usePotion(int potionItemId) {
    const Item* potionDef = GetItemDefinitionFromDatabase(potionItemId);
    if (!potionDef) {
        std::cout << "Error: Invalid Potion ID (" << potionItemId << ")." << std::endl;
        return;
    } 

    if (potions.getItemCount(potionItemId) == 0) {
        std::cout << "You do not have any " << potionDef->getName() << " to use." << std::endl;
        return;
    }

    if (potionItemId == 201) { // Health Potion
        healPlayer(static_cast<float>(getMaxHealth())); // Heal to full
        potions.removeItem(potionItemId); 
        std::cout << "Used a Health Potion. Restored " << getMaxHealth() << " health." << std::endl;
        std::cout << "Current Health: " << getCurrentHealth() << "/" << getMaxHealth() << std::endl;
    }

    else if (potionItemId == 202) { // Damage Potion
        intellectBoostAmount = 3;
        intellectBeforeBoost = intellect; // Save current damage
        intellect += intellectBoostAmount; // Apply boost
        intellectBoostTurnsRemaining = 4; // Boost lasts for 4 turns
        potions.removeItem(potionItemId); 
        std::cout << "Used a Intellect Potion. Base intellect increased by " << intellectBoostAmount << " for 4 turns." << std::endl;
        std::cout << "Current Base Intellect: " << getIntellect() << std::endl;
    }

    //More potions to come...
}

void TheLady::updateIntellectBoost() {
    if (intellectBoostTurnsRemaining > 0) {
        intellectBoostTurnsRemaining--;
        std::cout << "Intellect boost turns remaining: " << intellectBoostTurnsRemaining << std::endl;
        
        if (intellectBoostTurnsRemaining == 0) {
            // Revert damage to base
            intellect = intellectBeforeBoost;
            std::cout << "Intellect potion effect ended. Base intellect reverted to " << intellect << "." << std::endl;
        }
    }
}

//combat
Spell TheLady::getSpell() {
    displaySpells();
    if (spells.empty()) {
        std::cout << "\nNo spells available!\n" << std::endl;
        return Spell(); // Return default spell
    }

    int choice;
    std::cout << "\nChoose a spell by number: ";
   std::string choiceStr;
   std::getline(std::cin, choiceStr);
   try {
       choice = std::stoi(choiceStr);
   } catch (...) {
       choice = 0; // Invalid input
   }
    if (choice < 1 || choice > static_cast<int>(spells.size())) {
        std::cout << "Invalid choice. Try again." << std::endl;
        return getSpell(); // Recursive call until valid choice
    }

    // For simplicity, return the first spell in the inventory
    return spells[choice - 1];
}

bool TheLady::addSpell(int spellId) {
    const Item* spellDef = GetItemDefinitionFromDatabase(spellId);
    for (const auto& spell : spells) {
        if (spell.getItemId() == spellId) {
            return false;
        }
    }
    if (spellDef && spellDef->getIsSpell()) {
        spells.push_back(static_cast<const Spell&>(*spellDef));
        return true;
    } else {
        std::cout << "Error: Invalid Spell ID (" << spellId << ")." << std::endl;
        return false;
    }
}

// Bestiary
void TheLady::addToBestiary(const std::string& monsterName, const std::string& weaknessName) {
    for (const auto& entry : bestiary) {
        if (entry.first == monsterName) {
            return; // Already recorded
        }
    }
    std::cout << monsterName << "'s weakness (" << weaknessName << ") has been recorded in the bestiary.\n" << std::endl;
    bestiary.emplace_back(monsterName, weaknessName);
}

void TheLady::displayBestiary() const {
    std::cout << "\n--- Bestiary ---\n" << std::endl;
    if (bestiary.empty()) {
        std::cout << "The bestiary is empty." << std::endl;
        return;
    }
    for (const auto& entry : bestiary) {
        std::cout << "Monster: " << entry.first << " | Weakness: " << entry.second << std::endl;
    }
    std::cout << "----------------\n" << std::endl;
}

//Movement
void TheLady::flee() {
    if (previousRoom) {
        setCurrentRoom(previousRoom);
        previousRoom = nullptr;
        TheLady::currentTarget = nullptr;
        TheLady::setCurrentHealth(TheLady::getCurrentHealth() - 0.5); // Heal to full on flee
        std::cout << "The Lady loses 0.5 health while fleeing." << std::endl;
        std::cout << "The Lady flees back to the previous room." << std::endl;
        std::cout << "You are now in: " << currentRoom->getDescription() << std::endl;
    } else {
        std::cout << "No previous room to flee to!" << std::endl;
    }
}

void TheLady::move(const std::string& direction) {
    if (!currentRoom) {
        std::cout << "The Lady is nowhere! Cannot move." << std::endl;
        return;
    }

    Room* nextRoom = nullptr;

    if (direction == "north") {
        nextRoom = currentRoom->connections.north;
    } else if (direction == "south") {
        nextRoom = currentRoom->connections.south;
    } else if (direction == "east") {
        nextRoom = currentRoom->connections.east;
    } else if (direction == "west") {
        nextRoom = currentRoom->connections.west;
    } else {
        std::cout << "Invalid direction. Choose north, south, east, or west." << std::endl;
        return;
    }

    if (nextRoom) {
        setPreviousRoom(currentRoom);
        setPreviousRoomId(getPreviousRoom() ? getPreviousRoom()->getId() : -1);

        setCurrentRoom(nextRoom);
        setCurrentRoomId(getCurrentRoom() ? getCurrentRoom()->getId() : -1);

        std::cout << "The Lady moves " << direction << "." << std::endl;
        std::cout << "You are now in: " << currentRoom->getDescription() << std::endl;
    } else {
        std::cout << "There is no path to the " << direction << "." << std::endl;
    }
}

//Save/Load The Lady
void TheLady::save(std::fstream& file) const {
    Character::save(file); 

    file.write(reinterpret_cast<const char*>(&level), sizeof(level));
    file.write(reinterpret_cast<const char*>(&expToNextLevel), sizeof(expToNextLevel));

    //save inventories and known recipes as needed
    size_t knownRecipeCount = knownRecipes.size();
    file.write(reinterpret_cast<const char*>(&knownRecipeCount), sizeof(knownRecipeCount));
    for (const auto& recipe : knownRecipes) {
        int itemId = recipe.getItemId();
        file.write(reinterpret_cast<const char*>(&itemId), sizeof(itemId));
    }

    size_t studiedIngredientsCount = studiedIngredients.size();
    file.write(reinterpret_cast<const char*>(&studiedIngredientsCount), sizeof(studiedIngredientsCount));
    for (const auto& ingredient : studiedIngredients) {
        int itemId = ingredient.getItemId();
        file.write(reinterpret_cast<const char*>(&itemId), sizeof(itemId));
    }

    size_t ingredientsCount = ingredients.getAllItems().size();
    file.write(reinterpret_cast<const char*>(&ingredientsCount), sizeof(ingredientsCount));
    for (const auto& item : ingredients.getAllItems()) {
        file.write(reinterpret_cast<const char*>(&item.first), sizeof(item.first)); // itemId
        file.write(reinterpret_cast<const char*>(&item.second), sizeof(item.second)); // count
    }

    size_t potionsCount = potions.getAllItems().size();
    file.write(reinterpret_cast<const char*>(&potionsCount), sizeof(potionsCount));
    for (const auto& item : potions.getAllItems()) {
        file.write(reinterpret_cast<const char*>(&item.first), sizeof(item.first)); // itemId
        file.write(reinterpret_cast<const char*>(&item.second), sizeof(item.second)); // count
    }

    //save current and previous rooms using stored IDs (avoid dereferencing possibly-invalid pointers)
    int currentRoomId = CurrentRoomId;
    file.write(reinterpret_cast<const char*>(&currentRoomId), sizeof(currentRoomId));

    int previousRoomId = PreviousRoomId;
    file.write(reinterpret_cast<const char*>(&previousRoomId), sizeof(previousRoomId));

    if (currentRoomId == -1) {
        std::cout << "Warning: The Lady's current room ID is -1 during save." << std::endl;
    }

    // besitary
    size_t bestiaryCount = bestiary.size();
    file.write(reinterpret_cast<const char*>(&bestiaryCount), sizeof(bestiaryCount));
    for (const auto& entry : bestiary) {
        size_t nameLength = entry.first.length();
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        file.write(entry.first.c_str(), nameLength);
        size_t weaknessLength = entry.second.length();
        file.write(reinterpret_cast<const char*>(&weaknessLength), sizeof(weaknessLength));
        file.write(entry.second.c_str(), weaknessLength);
    }
}

void TheLady::load(std::fstream& file) {
    Character::load(file);

    file.read(reinterpret_cast<char*>(&level), sizeof(level));
    file.read(reinterpret_cast<char*>(&expToNextLevel), sizeof(expToNextLevel));

    size_t knownRecipeCount = 0;
    file.read(reinterpret_cast<char*>(&knownRecipeCount), sizeof(knownRecipeCount));
    knownRecipes.clear();
    for (size_t i = 0; i < knownRecipeCount; ++i) {
        int itemId = 0;
        file.read(reinterpret_cast<char*>(&itemId), sizeof(itemId));
        const Item* recipe = GetItemDefinitionFromDatabase(itemId);
        if (recipe) {
            knownRecipes.push_back(*recipe);
        }
    }

    size_t studiedIngredientsCount = 0;
    file.read(reinterpret_cast<char*>(&studiedIngredientsCount), sizeof(studiedIngredientsCount));
    studiedIngredients.clear();
    for (size_t i = 0; i < studiedIngredientsCount; ++i) {
        int itemId = 0;
        file.read(reinterpret_cast<char*>(&itemId), sizeof(itemId));
        const Item* ingredient = GetItemDefinitionFromDatabase(itemId);
        if (ingredient) {
            studiedIngredients.push_back(*ingredient);
        }
    }

    size_t ingredientsCount = 0;
    file.read(reinterpret_cast<char*>(&ingredientsCount), sizeof(ingredientsCount));
    ingredients.clearInventory();
    for (size_t i = 0; i < ingredientsCount; ++i) {
        int itemId = 0;
        int count = 0;
        file.read(reinterpret_cast<char*>(&itemId), sizeof(itemId));
        file.read(reinterpret_cast<char*>(&count), sizeof(count));
        const Item* itemDef = GetItemDefinitionFromDatabase(itemId);
        if (itemDef) {
            for (int j = 0; j < count; ++j) {
                ingredients.addItem(*itemDef);
            }
        }
    }

    size_t potionsCount = 0;
    file.read(reinterpret_cast<char*>(&potionsCount), sizeof(potionsCount));
    potions.clearInventory();
    for (size_t i = 0; i < potionsCount; ++i) {
        int itemId = 0;
        int count = 0;
        file.read(reinterpret_cast<char*>(&itemId), sizeof(itemId));
        file.read(reinterpret_cast<char*>(&count), sizeof(count));
        const Item* itemDef = GetItemDefinitionFromDatabase(itemId);
        if (itemDef) {
            for (int j = 0; j < count; ++j) {
                potions.addItem(*itemDef);
            }
        }
    }

    //load current and previous rooms
    file.read(reinterpret_cast<char*>(&CurrentRoomId), sizeof(CurrentRoomId));
    file.read(reinterpret_cast<char*>(&PreviousRoomId), sizeof(PreviousRoomId));

    // bestiary
    size_t bestiaryCount = 0;
    file.read(reinterpret_cast<char*>(&bestiaryCount), sizeof(bestiaryCount));
    bestiary.clear();
    for (size_t i = 0; i < bestiaryCount; ++i) {
        size_t nameLength = 0;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        std::string monsterName(nameLength, ' ');
        file.read(&monsterName[0], nameLength);
        size_t weaknessLength = 0;
        file.read(reinterpret_cast<char*>(&weaknessLength), sizeof(weaknessLength));
        std::string weaknessName(weaknessLength, ' ');
        file.read(&weaknessName[0], weaknessLength);
        bestiary.emplace_back(monsterName, weaknessName);
    }

}