#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <thread>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <limits>

using namespace std;

class Logger {
public:
    static void log(const string& message) {
        ofstream file("log.txt", ios::app);
        if (file) file << message << endl;
    }
};

class Item {
    string name, type;
    int value, upgradeLevel;

public:
    Item(const string& n, const string& t, int v, int u = 0)
        : name(n.length() > 50 ? n.substr(0, 50) : n), type(t), value(v), upgradeLevel(u) {
    }

    string getName() const { return name; }
    string getType() const { return type; }
    int getValue() const { return value; }
    int getUpgradeLevel() const { return upgradeLevel; }

    void upgrade() {
        upgradeLevel++;
        value += 1 + upgradeLevel;
        name += "+";
    }
};

class Inventory {
    vector<Item> items;

public:
    void addItem(const Item& item) {
        if (items.size() < 50)
            items.push_back(item);
        else
            throw runtime_error("Inventory full!");
    }

    void display() const {
        if (items.empty()) {
            cout << "Inventory is empty.\n";
        }
        else {
            for (size_t i = 0; i < items.size(); ++i) {
                cout << i + 1 << ". " << items[i].getName() << " [" << items[i].getType()
                    << ", Value: " << items[i].getValue() << ", Upgrade: "
                    << items[i].getUpgradeLevel() << "]\n";
            }
        }
    }

    vector<Item>& getItems() { return items; }

    void removeItem(int index) {
        if (index >= 0 && index < (int)items.size())
            items.erase(items.begin() + index);
    }

    vector<Item*> findWeapons() {
        vector<Item*> result;
        for (auto& item : items)
            if (item.getType() == "weapon")
                result.push_back(&item);
        return result;
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        for (const auto& item : items) {
            file << item.getName() << "," << item.getType() << ","
                << item.getValue() << "," << item.getUpgradeLevel() << "\n";
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        items.clear();
        string name, type;
        int value, upgrade;
        while (getline(file, name, ',') && getline(file, type, ',') &&
            (file >> value).ignore() && (file >> upgrade).ignore()) {
            items.emplace_back(name, type, value, upgrade);
            file.ignore(); 
        }
    }
};

class Monster {
protected:
    string name;
    int health, attack, defense;
    bool boss;
    vector<Item> loot;

public:
    Monster(const string& n, int hp, int atk, int def, bool isBoss)
        : name(n), health(hp), attack(atk), defense(def), boss(isBoss) {
    }

    virtual ~Monster() {}

    string getName() const { return name; }
    bool isAlive() const { return health > 0; }
    int getDefense() const { return defense; }
    void takeDamage(int dmg) { health -= dmg; }

    vector<Item> getLoot() const { return loot; }

    virtual void attackTarget(class Character& target);
    virtual void displayInfo() const {
        cout << name << " [HP: " << health << ", ATK: " << attack << ", DEF: " << defense << "]\n";
    }
};

class Goblin : public Monster {
public:
    Goblin(bool boss = false)
        : Monster("Goblin", boss ? 60 : 30, boss ? 16 : 8, boss ? 4 : 2, boss) {
        if (boss || rand() % 100 > 50)
            loot.emplace_back(boss ? "Goblin King's Dagger" : "Goblin Dagger", "weapon", boss ? 10 : 5);
    }
};

class Dragon : public Monster {
public:
    Dragon(bool boss = false)
        : Monster("Dragon", boss ? 200 : 100, boss ? 40 : 20, boss ? 20 : 10, boss) {
        loot.emplace_back(boss ? "Dragon King Scale" : "Dragon Scale", "weapon", boss ? 20 : 10);
        loot.emplace_back(boss ? "Dragon Essence" : "Dragon Blood", "potion", boss ? 100 : 50);
    }
};

class Skeleton : public Monster {
public:
    Skeleton(bool boss = false)
        : Monster("Skeleton", boss ? 80 : 40, boss ? 20 : 10, boss ? 10 : 5, boss) {
        if (boss || rand() % 100 > 70)
            loot.emplace_back(boss ? "Skeleton Lord's Blade" : "Bone Sword", "weapon", boss ? 15 : 7);
    }
};

class Character {
    string name;
    int health, maxHealth, baseAttack, baseDefense;
    int attack, defense, level, experience, monstersDefeated;
    bool isDefending;
    Item* currentWeapon;
    Inventory inventory;

public:
    Character(const string& n, int hp, int atk, int def)
        : name(n), health(hp), maxHealth(hp), baseAttack(atk), baseDefense(def),
        level(1), experience(0), monstersDefeated(0), isDefending(false), currentWeapon(nullptr) {
        attack = baseAttack;
        defense = baseDefense;
    }

    void displayInfo() const {
        cout << "\n=== Character Info ===\n";
        cout << name << " [HP: " << health << "/" << maxHealth
            << ", ATK: " << attack << " (Base: " << baseAttack
            << " + Weapon: " << (currentWeapon ? currentWeapon->getValue() : 0) << ")"
            << ", DEF: " << defense << ", LVL: " << level << ", EXP: " << experience << "/100]\n";
        if (currentWeapon)
            cout << "Equipped weapon: " << currentWeapon->getName() << "\n";
        cout << "Monsters defeated: " << monstersDefeated << "\n";
        cout << "=====================\n";
    }

    void addToInventory(const Item& item) {
        try {
            inventory.addItem(item);
        }
        catch (...) {
            cerr << "Failed to add item to inventory\n";
        }
    }

    void showInventory() {
        cout << "\n=== Inventory ===\n";
        inventory.display();
        cout << "================\n";
    }

    void combineItems() {
        auto& items = inventory.getItems();
        for (size_t i = 0; i + 1 < items.size(); ++i) {
            if (items[i].getType() == "weapon" && items[i + 1].getType() == "weapon") {
                items[i].upgrade();
                inventory.removeItem(i + 1);
                cout << "Combined two weapons into " << items[i].getName() << "!\n";
                return;
            }
        }
        cout << "No weapons to combine!\n";
    }

    void equipWeapon(Item* weapon) {
        if (weapon && weapon->getType() == "weapon") {
            currentWeapon = weapon;
            attack = baseAttack + currentWeapon->getValue();
            cout << "Equipped " << weapon->getName() << "!\n";
        }
    }

    void equipFromInventory() {
        auto weapons = inventory.findWeapons();
        if (weapons.empty()) {
            cout << "No weapons in inventory!\n";
            return;
        }

        cout << "\nChoose weapon to equip:\n";
        for (size_t i = 0; i < weapons.size(); ++i)
            cout << i + 1 << ". " << weapons[i]->getName() << "\n";

        int choice;
        cin >> choice;
        if (choice > 0 && choice <= (int)weapons.size()) {
            equipWeapon(weapons[choice - 1]);
        }
        else {
            cout << "Invalid choice!\n";
        }
    }

    void attackEnemy(Monster& monster) {
        int dmg = max(1, attack - monster.getDefense());
        monster.takeDamage(dmg);
        experience += 10;
        if (experience >= 100) {
            level++;
            experience = 0;
            maxHealth += 10;
            baseAttack += 2;
            baseDefense += 1;
            health = maxHealth;
            cout << "Level up! Now level " << level << "!\n";
        }
    }

    void defend() {
        isDefending = true;
        defense = baseDefense * 2;
    }

    void takeDamage(int dmg) {
        health -= isDefending ? dmg / 2 : dmg;
        if (health < 0) health = 0;
        isDefending = false;
        defense = baseDefense;
    }

    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
        cout << "You heal " << amount << " HP.\n";
    }

    Item* findPotion() {
        auto& items = inventory.getItems();
        for (auto& item : items)
            if (item.getType() == "potion")
                return &item;
        return nullptr;
    }

    void usePotion(Item* potion) {
        if (!potion || potion->getType() != "potion") return;
        heal(potion->getValue());
        auto& items = inventory.getItems();
        auto it = find_if(items.begin(), items.end(),
            [potion](const Item& i) { return &i == potion; });
        if (it != items.end()) items.erase(it);
    }

    void saveGame(const string& filename) const {
        ofstream file(filename);
        if (file) {
            file << name << "\n" << health << "\n" << maxHealth << "\n"
                << baseAttack << "\n" << baseDefense << "\n" << level << "\n"
                << experience << "\n" << monstersDefeated << "\n";
            inventory.saveToFile(filename + "_inv");
        }
    }

    void loadGame(const string& filename) {
        ifstream file(filename);
        if (file) {
            file >> name >> health >> maxHealth >> baseAttack >> baseDefense
                >> level >> experience >> monstersDefeated;
            attack = baseAttack + (currentWeapon ? currentWeapon->getValue() : 0);
            defense = baseDefense;
            inventory.loadFromFile(filename + "_inv");
        }
    }

    int getHealth() const { return health; }
    int getDefense() const { return defense; }
    bool getIsDefending() const { return isDefending; }
    string getName() const { return name; }
    int getMonstersDefeated() const { return monstersDefeated; }

    void monsterDefeated() { monstersDefeated++; }
};

void Monster::attackTarget(Character& target) {
    int dmg = max(1, attack - target.getDefense());
    target.takeDamage(dmg);
    cout << name << " attacks " << target.getName() << " for " << dmg << " damage!\n";
}

class Game {
private:
    Character player;
    vector<unique_ptr<Monster>> monsters;
    random_device rd;
    mt19937 gen;
    vector<string> battleLog;

    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void printSeparator() {
        cout << "\n========================================\n";
    }

    void printBattleLog() {
        if (!battleLog.empty()) {
            cout << "\n=== Battle Log ===\n";
            for (const auto& msg : battleLog)
                cout << msg << "\n";
            cout << "==================\n";
        }
    }

    void generateMonsters() {
        monsters.clear();
        bool boss = player.getMonstersDefeated() > 0 && player.getMonstersDefeated() % 10 == 0;

        monsters.push_back(make_unique<Goblin>(false));
        monsters.push_back(make_unique<Skeleton>(false));

        uniform_int_distribution<> monsterType(1, 3);
        switch (monsterType(gen)) {
        case 1: monsters.push_back(make_unique<Goblin>(boss)); break;
        case 2: monsters.push_back(make_unique<Dragon>(boss)); break;
        case 3: monsters.push_back(make_unique<Skeleton>(boss)); break;
        }

        shuffle(monsters.begin(), monsters.end(), gen);
    }

    void exploreArea() {
        clearScreen();
        cout << "You explore the area...\n";
        this_thread::sleep_for(chrono::seconds(1));

        uniform_int_distribution<> eventDist(1, 100);
        int event = eventDist(gen);

        if (event <= 30) {
            Item foundItem = (rand() % 2 == 0)
                ? Item("Health Potion", "potion", 15)
                : Item("Old Sword", "weapon", 3);
            player.addToInventory(foundItem);
            cout << "You found a " << foundItem.getName() << "!\n";
        }
        else if (event <= 60) {
            cout << "Nothing interesting here.\n";
        }
        else {
            cout << "A monster ambushes you!\n";
            this_thread::sleep_for(chrono::seconds(1));
            fight();
        }

        printSeparator();
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }

    void fight() {
        if (monsters.empty()) {
            cout << "No more monsters!\n";
            return;
        }

        auto& monster = monsters.back();
        battleLog.clear();

        while (player.getHealth() > 0 && monster->isAlive()) {
            clearScreen();
            player.displayInfo();
            cout << "\nEnemy:\n";
            monster->displayInfo();
            printBattleLog();
            printSeparator();

            cout << "1. Attack\n2. Defend\n3. Use Potion\n4. Equip Weapon\n5. Flee\n";
            printSeparator();

            int choice;
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            switch (choice) {
            case 1:
                player.attackEnemy(*monster);
                battleLog.push_back(player.getName() + " attacks " + monster->getName());
                break;
            case 2:
                player.defend();
                battleLog.push_back(player.getName() + " defends.");
                break;
            case 3: {
                Item* potion = player.findPotion();
                if (potion) {
                    player.usePotion(potion);
                    battleLog.push_back(player.getName() + " uses potion!");
                }
                else {
                    cout << "No potions!\n";
                    this_thread::sleep_for(chrono::seconds(1));
                }
                break;
            }
            case 4:
                player.equipFromInventory();
                continue;
            case 5:
                cout << "You fled!\n";
                this_thread::sleep_for(chrono::seconds(1));
                return;
            default:
                cout << "Invalid!\n";
                this_thread::sleep_for(chrono::seconds(1));
                continue;
            }

            if (monster->isAlive()) {
                monster->attackTarget(player);
                battleLog.push_back(monster->getName() + " hits back!");
            }
            this_thread::sleep_for(chrono::seconds(1));
        }

        if (!monster->isAlive()) {
            cout << "You defeated " << monster->getName() << "!\n";
            player.monsterDefeated();
            try {
                auto loot = monster->getLoot();
                for (auto& item : loot) {
                    player.addToInventory(item);
                    cout << "Found " << item.getName() << "!\n";
                }
            }
            catch (...) {
                cerr << "Loot error.\n";
            }
            monsters.pop_back();
            this_thread::sleep_for(chrono::seconds(2));
        }
    }

public:
    Game(const string& playerName)
        : player(playerName, 100, 10, 5), gen(rd()) {
        generateMonsters();
    }

    void start() {
        try {
            while (true) {
                clearScreen();
                player.displayInfo();
                printSeparator();
                cout << "1. Fight\n2. Heal\n3. Inventory\n4. Explore\n5. Combine Weapons\n6. Equip Weapon\n7. Save\n8. Load\n9. Exit\n";
                printSeparator();

                int choice;
                if (!(cin >> choice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                switch (choice) {
                case 1: fight(); break;
                case 2: player.heal(20); break;
                case 3: player.showInventory(); cin.ignore(); cin.get(); break;
                case 4: exploreArea(); break;
                case 5: player.combineItems(); break;
                case 6: player.equipFromInventory(); break;
                case 7: player.saveGame("save.txt"); break;
                case 8: player.loadGame("save.txt"); break;
                case 9: return;
                default: cout << "Invalid!\n"; this_thread::sleep_for(chrono::seconds(1));
                }

                if (monsters.empty()) {
                    generateMonsters();
                }
            }
        }
        catch (const exception& e) {
            cerr << "Game error: " << e.what() << "\n";
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    try {
        cout << "Enter your name: ";
        string name;
        getline(cin, name);

        Game game(name);
        game.start();
    }
    catch (...) {
        cerr << "Fatal error. Game exiting...\n";
        return 1;
    }
    return 0;
}
