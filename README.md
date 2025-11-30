# Final Project – Cavern of the Elder Blood

This project is my final submission for the Game Design, Development, and Innovation master’s program at Duke University.

*Cavern of the Elder Blood* is a quick text-based adventure set in the Witcher universe, where you follow **The Lady**, a sorceress obsessed with studying the beasts she slays. Rumors of Elder Blood have reached her, and her pursuit of knowledge—and power—drives her into the depths of a dangerous cavern filled with monsters, magic, and mystery.

```The objective of this assignment is to put object-oriented programming into practice while applying data structures and algorithms commonly used in game development.```

Below are the instructions to run the project.

### How to Run:

    ```bash
    cd ./src
    make
    ./src/build/bin/LOTEB
---
#### Developer: Omar Masri
#### ENJOY - omar.masri@duke.edu - For feedback!
---

# Backstory
NOTICE TO GUILDS: THE LADY (RESEARCH CLASS)

Hear this: there's a new name fouling the good air of the Continent, whispered by nervous sellswords and Novigrad merchants—The Lady. She is no contract hunter. Her interest lies not in coin, but in the monstrous flesh itself. She subdues and vivisects, claiming to chase the ancient truths of the Conjunction.

Her obsession is the Elder Blood. The moment whispers of the Witcher's disastrous encounter with an Ancient Vampyre in Toussaint reached her, she turned her sights to the Crimson Labyrinth, believing it to hold the mother lode. It's a place even the Wolf School avoids.

Consider this a final warning: if you see a woman armed with strange carrying a scholar's bag, turn back. If her body is found, retrieve her journal. The loss of a sorceress is cheap, but her notes on failure will be priceless.

# Central Goal

Explore a dungeon and defeat enemies to become stronger. Defeating enemies will provide resources and experience to strengthen the main character to prepare for the encounter with the Elder Blood.

# Central Gameplay Features:

## 1. Characters

- ```Core Stats:``` Used for game balance

    ```
    - Intellect (INT): Works as damage boost and damage negatation in combat.
        
    - Each Character has an Intellect stat.
## Overview Gameplay

- Turn-based, menu-driven combat (select attack, spell, use potion, or flee)
- Exploration consists of moving between rooms, encountering monsters, and setting camp to manage resources
- Progression by experience and item pickups (spells, ingredients, potions)

## Player (The Lady)

The player character (called "The Lady") is the central protagonist. Key systems:

- Stats: Health, Max Health, Intellect (INT), Experience, Level
- Inventory: Ingredients and Potions
    - To Interact with Ingredients and Potions you must enter their ID number and not their name (This follows the structure of `101` or `202` as seen the pop ups during gameplay).
- Known Spells and Studied Recipes: Spells may be learned from monster drops; recipes are learned by studying ingredients

### Combat

- Turn-based: player and monsters take turns. The player chooses actions by menu.
- Actions: Attack (select target), Use Potion, Flee (returns to previous room and costs a small HP penalty).
    - Attacking requires the target number with the input (i.e `attack 1` or `attack 2` and so on)
- Combat ends when all enemies in the room are defeated or the player successfully flees.

### Camp

- While at camp the player can: view stats, inspect ingredients, consume potions, study ingredients (to learn recipes), brew potions (if ingredients and recipes are available), and view the bestiary.

### Movement

- Move between rooms, encounter monsters, or return to camp.

### Leveling

- The player will gain experience on enemy defeat.
- Leveling up will increase Intellect by 1, Max Health by 3, and will resort current health.
- Visit stats page to see current level and experience.

## Inventory and Items

- Ingredients: `Herbal Leaf`, `Luminous Dust`
- Potions: `Health Potion` (restores health), `Intellect Potion` (temporarily increases INT)
- Spells: `Fireball`, `Ice Shard`, `Wind Gust`, `Earthquake`, `Water Whip` — each spell has a base damage and may interact with monster weaknesses

## Monsters

Monsters have drops, weaknesses (which amplify damage from specific spells), and spells they can use. Below is a cleaned, consistent list.

- Giant Spider — Drops: Ice Shard — Weakness: Fireball — Spells: Poison Spit
- Dire Wolf — Drops: Wind Gust — Weakness: Fireball — Spells: Claw Swipe
- Cave Bat — Drops: Water Whip — Weakness: Fireball — Spells: Bite
- Cave Ghoul — Drops: Earthquake — Weakness: Ice Shard — Spells: Vicious Bite
- Giant Snake — Drops: Herbal Leaf — Weakness: Water Whip — Spells: Poison Spit, Tail Whip
- Lesser Vampyre — Drops: Herbal Leaf — Weakness: Fireball — Spells: Bite, Vicious Bite, Shadow Claw
- Forest Dryad — Drops: Herbal Leaf — Weakness: Fireball — Spells: Wind Gust, Claw Swipe, Nature's Grasp
- Dark Mage — Drops: Herbal Leaf — Weakness: Wind Gust — Spells: Fireball, Ice Shard, Shadow Claw
- Elemental Mage — Drops: Herbal Leaf — Weakness: Wind Gust — Spells: Fireball, Lightning Bolt, Wind Gust, Earthquake
- Cave Goblin — Drops: Luminous Dust — Weakness: Fireball — Spells: Rock Throw
- Pixie — Drops: Luminous Dust — Weakness: Earthquake — Spells: Wind Gust, Water Whip, Nature's Grasp
- Stone Gargoyle — Drops: Luminous Dust — Weakness: Ice Shard — Spells: Tail Whip, Rock Throw
- Ancient Lich — Drops: Intellect Potion — Weakness: Fireball — Spells: Shadow Claw, Ice Shard
- Arch Mage — Drops: Health Potion — Weakness: Ice Shard — Spells: Fireball, Lightning Bolt, Wind Gust, Earthquake, Water Whip
- Greater Vampyre — Drops: Health Potion — Weakness: Fireball — Spells: Bite, Vicious Bite, Shadow Claw, Fireball, Poison Spit
- Vampyre Elder (Final Boss) — Drops: None — Weakness: Fireball — Spells: Bite, Vicious Bite, Shadow Claw, Fireball, Lightning Bolt, Wind Gust, Poison Spit

## Rooms and Level Design

- Rooms are nodes that may contain monsters. The player can select to move North, South, East, or West as long as another Room node is connected. The player may return to rooms already visited.

## Saving/Loading

- All player, enemy, and room information will be automatically saved at the beginning of the game, after combat, and leaving the camp.

- Upon death, the game will load from the last successful save. 

## Gameplay Instructions (Prototype)

1. Start/load the game and explore rooms.
2. When you enter a room with enemies, use the combat menu to fight: attack [Target Number], flee, or use potion.
3. Loot drops may include ingredients (learn recipes), spells, and potions.
4. Visit camp to study ingredients (learn recipes) and brew potions.
5. Repeat: explore, fight, learn, and upgrade until you're ready to challenge the Vampyre Elder.

## C++ Features

### Save/Load
The game automatically saves after combat, when leaving camp, and before exiting through the terminal. You’ll find that Character.cpp, TheLady.cpp, and Room.h define the functionality for saving and loading their binary data, while GameLoop.h provides the helper functions that manage saving and loading the overall game state.

### MakeFile
Makefile will handle everything for you and will generate the main executable at:
    
    ./src/build/bin/LOTEB

### Object Oriented Programming
 - `TheLady` and `Monster` inherit from `Character`, and `Spells` inherits from `Item`
    - Constructor overloading
    - Inheritance
    - Encapsulation
    - Polymorphism
 - The `*` operator is overloaded in `Character.cpp` as a helper function for attacks between characters.
 - `Inventory.h` handles ID validation before retrieving any items from the `ITEM_DATABASE` map.

 ### Addtional Features
 - Linked List: `Room` Follows a Graph/Linked List structure. Rooms have connections (North, South, East, and West) for the player to navigate through
 - Abstraction: `Character` Includes Abstracted functions and their definitions are found in `Monster` and `TheLady`.
 - Templates: `Invetory` utilizes templates to create an inventory system used by The Lady.
 - Recursion: `TheLady.cpp` implements a recursive function called `getSpell`, which calls itself when the player inputs an invalid spell choice
 - Load & Save: 
    - `Character` implements save/load functionality. 
    - `The Lady` Overrides that functionality. 
     - `Room` implements save/load functionality 
    - `GameLoop` utilizes the functionality found in `Character`, `TheLady`, and `Room` to save all game information in a directory.
        ```
        ./src/build/bin/saves
        ```
 - Splash Screen


