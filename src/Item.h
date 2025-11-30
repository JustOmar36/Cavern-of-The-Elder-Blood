#ifndef ITEM_H
#define ITEM_H

#include <string>

// Items for pick up, study, and use
class Item { 
protected:
  int itemId;
  std::string name;
  std::string description;
  std::string studyEffectKey;
  bool isSpell;
  bool isConsumable;

public:

  Item() = default;

  // Constructor no study effect
  Item(int id, std::string n, std::string desc, bool spell, bool consumable)
      : itemId(id), name(n), description(desc), studyEffectKey(""), isSpell(spell), isConsumable(consumable) {}

  // Constructor with study effect
  Item(int id, std::string n, std::string desc, std::string key, bool spell, bool consumable)
      : itemId(id), name(n), description(desc), studyEffectKey(key), isSpell(spell), isConsumable(consumable) {}

  ~Item() = default;

  int getItemId() const { return itemId; }
  std::string getName() const { return name; }
  std::string getDescription() const { return description; }
  std::string getStudyEffectKey() const { return studyEffectKey; }
  bool getIsSpell() const { return isSpell; }
  bool getIsConsumable() const { return isConsumable; }

};

#endif