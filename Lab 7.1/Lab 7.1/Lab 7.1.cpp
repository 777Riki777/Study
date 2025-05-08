#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <stdexcept>

using namespace std;

class Entity {
public:
    virtual void showInfo() const = 0;
    virtual int getHealth() const = 0;
    virtual int getExp() const = 0;
    virtual string getCharName() const = 0;
    virtual string getCategory() const = 0;
    virtual ~Entity() = default;
    virtual void writeToFile(ofstream& file) const = 0;
};

class Character : public Entity {
    string charName;
    int health;
    int experience;
public:
    Character(string n = "", int h = 0, int e = 0) : charName(n), health(h), experience(e) {}

    void showInfo() const override {
        cout << "Character: " << charName << " (Health: " << health << ", Experience: " << experience << ")\n";
    }

    int getHealth() const override { return health; }
    int getExp() const override { return experience; }
    string getCharName() const override { return charName; }
    string getCategory() const override { return "Character"; }

    void writeToFile(ofstream& file) const override {
        file << "Character " << charName << " " << health << " " << experience << "\n";
    }
};

class Monster : public Entity {
    string charName;
    int health;
    string category;
public:
    Monster(string n = "", int h = 0, string c = "") : charName(n), health(h), category(c) {}

    void showInfo() const override {
        cout << "Monster: " << charName << " (Health: " << health << ", Category: " << category << ")\n";
    }

    int getHealth() const override { return health; }
    int getExp() const override { return 0; }
    string getCharName() const override { return charName; }
    string getCategory() const override { return category; }

    void writeToFile(ofstream& file) const override {
        file << "Monster " << charName << " " << health << " " << category << "\n";
    }
};

template <typename T>
class WorldManager {
    vector<T> gameEntities;
public:
    void registerEntity(const T& entity) {
        if (entity->getHealth() <= 0) {
            throw invalid_argument("Entity must have positive health");
        }
        gameEntities.push_back(entity);
    }

    void showAllEntities() const {
        for (const auto& entity : gameEntities) {
            entity->showInfo();
        }
    }

    void saveEntities(const string& filename) const {
        ofstream file(filename);
        if (!file) {
            throw runtime_error("Could not open file for saving game data");
        }
        for (const auto& entity : gameEntities) {
            entity->writeToFile(file);
        }
    }

    void loadEntities(const string& filename) {
        ifstream file(filename);
        if (!file) {
            throw runtime_error("Could not open file for loading game data");
        }
        gameEntities.clear();
        string type;
        while (file >> type) {
            if (type == "Character") {
                string name;
                int health, exp;
                file >> name >> health >> exp;
                registerEntity(make_shared<Character>(name, health, exp));
            }
            else if (type == "Monster") {
                string name, monsterType;
                int health;
                file >> name >> health >> monsterType;
                registerEntity(make_shared<Monster>(name, health, monsterType));
            }
        }
    }
};

int main() {
    try {
        WorldManager<shared_ptr<Entity>> gameWorld;

        gameWorld.registerEntity(make_shared<Character>("Paladin", 120, 7));
        gameWorld.registerEntity(make_shared<Monster>("Troll", 80, "Cave"));
        gameWorld.registerEntity(make_shared<Character>("Archer", 90, 5));
        gameWorld.registerEntity(make_shared<Monster>("Spider", 40, "Forest"));

        cout << "Game world entities:\n";
        gameWorld.showAllEntities();

        string saveFilename = "rpg_world.sav";
        gameWorld.saveEntities(saveFilename);
        cout << "\nGame state saved to " << saveFilename << "\n";

        WorldManager<shared_ptr<Entity>> newWorld;
        newWorld.loadEntities(saveFilename);
        cout << "\nLoaded game world entities:\n";
        newWorld.showAllEntities();

        cout << "\nTrying to add invalid entity...\n";
        try {
            newWorld.registerEntity(make_shared<Character>("Invalid", -10, 1));
        }
        catch (const invalid_argument& e) {
            cout << "Caught exception: " << e.what() << endl;
        }
        cout << "\nTrying to load from non-existent file...\n";
        try {
            newWorld.loadEntities("non_existent_file.sav");
        }
        catch (const runtime_error& e) {
            cout << "Caught exception: " << e.what() << endl;
        }
    }
    catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }

    return 0;
}