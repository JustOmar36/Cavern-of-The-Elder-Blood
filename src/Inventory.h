#ifndef INVENTORY_H
#define INVENTORY_H

#include <map>
#include <string>
#include <iostream>
#include <cassert>
#include "Item.h"

template <typename T>
class Inventory {
private:
    std::map<int, int> items;
    std::string inventoryName;

public:
    Inventory() = default;

    void setInventoryName(const std::string& name) {
        inventoryName = name;
    }

    ~Inventory() {
        items.clear();
    }

    void addItem(const T& item) {
        assert(item.getItemId() > 0 && "Item ID must be positive.");

        if (items.count(item.getItemId())) {
            // Item exists, increment the count
            items[item.getItemId()]++;
            std::cout << item.getName() << " added to inventory. Total: " << items[item.getItemId()] << std::endl;
        } else {
            // New item, add definition and set count to 1
            items[item.getItemId()] = 1;
            std::cout << item.getName() << " added to inventory (x1)." << std::endl;
        }
    }

    bool removeItem(int itemId) {
        assert(itemId > 0 && "Item ID must be positive.");

        if (!items.count(itemId) || items[itemId] < 1) {
            return false; // Item not found or not enough items
        }
    
        items[itemId] -= 1;
        
        if (items[itemId] <= 0) {
            // Remove the item completely if the count hits zero
            items.erase(itemId);
        }
        
        return true;
    }

    int getItemCount(int itemId) const {
        if (items.count(itemId)) {
            return items.at(itemId);
        }
        return 0;
    }

    const T* getItem(int itemId) const {
        if (items.count(itemId)) {
            return GetItemDefinitionFromDatabase(itemId);
        }
        return nullptr;
    }

    std::map<int, int> getAllItems() const {
        return items;
    }

    void clearInventory() {
        items.clear();
    }

    bool isEmpty() const {
        return items.empty();
    }

    void displayInventory() const {
        std::cout << "--- " << inventoryName << " ---" << std::endl;
        if (items.empty()) {
            std::cout << "Empty." << std::endl;
        } else {
            for (const auto& pair : items) {
                int itemId = pair.first;
                int count = pair.second;
                
                const T* itemDef = getItem(itemId); 
                
                if (itemDef) {
                    // Display the item's name and the count
                    std::cout << itemId << ". " << itemDef->getName() << " x" << count << std::endl;
                } else {
                    // Fallback in case of a corrupted ID
                    std::cout << "- Unknown Item (ID: " << itemId << ") x" << count << std::endl;
                }
            }
        }
        std::cout << "-----------------" << std::endl;
    }

};

#endif