#include <iostream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

class Item {
protected:
    string name;
    int value;
    float weight;

public:
    Item(const string& itemName, int itemValue, float itemWeight)
        : name(itemName), value(itemValue), weight(itemWeight) {
    }

    virtual ~Item() {}

    virtual void display() const {
        cout << name << " (Value: " << value << " gold, Weight: " << weight << " kg)";
    }

    string getName() const { return name; }
    int getValue() const { return value; }
    float getWeight() const { return weight; }
};

class Weapon : public Item {
private:
    int damage;

public:
    Weapon(const string& weaponName, int weaponValue, float weaponWeight, int weaponDamage)
        : Item(weaponName, weaponValue, weaponWeight), damage(weaponDamage) {
    }

    void display() const override {
        cout << name << " (Weapon, DMG: " << damage
            << ", Value: " << value << " gold, Weight: " << weight << " kg)";
    }

    int getDamage() const { return damage; }
};

class Potion : public Item {
private:
    string effect;

public:
    Potion(const string& potionName, int potionValue, float potionWeight, const string& potionEffect)
        : Item(potionName, potionValue, potionWeight), effect(potionEffect) {
    }

    void display() const override {
        cout << name << " (Potion, Effect: " << effect
            << ", Value: " << value << " gold, Weight: " << weight << " kg)";
    }

    string getEffect() const { return effect; }
};

class Inventory {
private:
    vector<unique_ptr<Item>> items;
    float maxWeight;

public:
    Inventory(float max = 100.0) : maxWeight(max) {}

    bool addItem(unique_ptr<Item> item) {
        float currentWeight = getTotalWeight();
        if (currentWeight + item->getWeight() > maxWeight) {
            cout << "Cannot add " << item->getName() << ": inventory weight limit exceeded!" << endl;
            return false;
        }

        items.push_back(move(item));
        return true;
    }

    void addItem(const string& itemName, int value = 1, float weight = 0.5) {
        auto newItem = make_unique<Item>(itemName, value, weight);
        if (addItem(move(newItem))) {
            cout << "Added basic item: " << itemName << endl;
        }
    }

    void displayInventory() const {
        cout << "\n===== INVENTORY =====" << endl;

        if (items.empty()) {
            cout << "  (empty)" << endl;
        }
        else {
            float totalWeight = 0.0f;
            int totalValue = 0;

            for (size_t i = 0; i < items.size(); ++i) {
                cout << "  " << (i + 1) << ". ";
                items[i]->display();
                cout << endl;

                totalWeight += items[i]->getWeight();
                totalValue += items[i]->getValue();
            }

            cout << "\n  Total items: " << items.size()
                << "\n  Total weight: " << totalWeight << "/" << maxWeight << " kg"
                << "\n  Total value: " << totalValue << " gold" << endl;
        }

        cout << "===================" << endl;
    }

    bool removeItem(size_t index) {
        if (index >= 1 && index <= items.size()) {
            string itemName = items[index - 1]->getName();
            items.erase(items.begin() + (index - 1));
            cout << "Removed: " << itemName << endl;
            return true;
        }
        cout << "Invalid item index!" << endl;
        return false;
    }

    float getTotalWeight() const {
        float total = 0.0f;
        for (const auto& item : items) {
            total += item->getWeight();
        }
        return total;
    }

    size_t getItemCount() const {
        return items.size();
    }
};

int main() {
    Inventory playerInventory(50.0);

    cout << "Creating player inventory..." << endl;

    playerInventory.addItem("Gold Coin", 1, 0.1);
    playerInventory.addItem("Map of Dungeon", 5, 0.2);
    playerInventory.addItem("Torch", 2, 0.5);

    playerInventory.addItem(make_unique<Weapon>("Iron Sword", 25, 3.5, 8));
    playerInventory.addItem(make_unique<Weapon>("Bow", 40, 1.5, 6));

    playerInventory.addItem(make_unique<Potion>("Health Potion", 15, 0.3, "Restore 50 HP"));
    playerInventory.addItem(make_unique<Potion>("Mana Potion", 20, 0.3, "Restore 30 MP"));

    playerInventory.displayInventory();

    playerInventory.removeItem(3);

    playerInventory.addItem(make_unique<Weapon>("Dragon Slayer", 1000, 45.0, 50));

    playerInventory.displayInventory();

    cout << "Program completed successfully!" << endl;
    return 0;
}