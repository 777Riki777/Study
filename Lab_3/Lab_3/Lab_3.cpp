#include <iostream>
#include <string>

using namespace std;
class Character {
private:
    string name;
    int health; 
    int attack;
    int defense;

public:
    Character(const string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        if (health < 0) health = 0;
        if (attack < 0) attack = 0;
        if (defense < 0) defense = 0;
    }
    bool operator==(const Character& other) const {
        return (name == other.name && health == other.health);
    }
    friend ostream& operator<<(ostream& os, const Character& character) {
        os << "Character: " << character.name
            << "\nHP: " << character.health
            << "\nAttack: " << character.attack
            << "\nDefense: " << character.defense;
        return os;
    }
};
class Weapon {
private:
    string name;
    int damage;
    int durability;

public:
    Weapon(const string& n, int dmg, int dur = 100)
        : name(n), damage(dmg), durability(dur) {
        if (damage < 0) damage = 1;
        if (durability < 0) durability = 1;
    }
    string getName() const { return name; }
    int getDamage() const { return damage; }
    int getDurability() const { return durability; }
    Weapon operator+(const Weapon& other) const {
        string newName = name + " + " + other.name;
        int newDamage = damage + other.damage;
        int newDurability = (durability + other.durability) / 2;

        return Weapon(newName, newDamage, newDurability);
    }
    bool operator>(const Weapon& other) const {
        return damage > other.damage;
    }
    void displayInfo() const {
        cout << "Weapon: " << name
            << "\n- Damage: " << damage
            << "\n- Durability: " << durability << endl;
    }

    friend ostream& operator<<(ostream& os, const Weapon& wpn) {
        os << wpn.name << " (DMG: " << wpn.damage << ", DUR: " << wpn.durability << ")";
        return os;
    }
};

int main() {
    Character knight("Sir Arthur", 120, 25, 15);
    Character knight2("Sir Arthur", 120, 30, 10); 
    Character archer("Robin", 80, 30, 8);

    cout << "=== Character Tests ===" << endl;

    if (knight == knight2) {
        cout << "Knights are considered equal (same name and health)" << endl;
    }

    if (!(knight == archer)) {
        cout << "Knight and Archer are different characters" << endl;
    }

    cout << "\nKnight stats:\n" << knight << endl;

    cout << "\n=== Weapon Tests ===" << endl;

    Weapon blade("Steel Blade", 15, 90);
    Weapon magicStaff("Magic Staff", 20, 70);
    Weapon hammer("Battle Hammer", 25, 85);
    Weapon dagger("Assassin's Dagger", 12, 60);

    cout << "Available weapons:" << endl;
    cout << "1. " << blade << endl;
    cout << "2. " << magicStaff << endl;
    cout << "3. " << hammer << endl;
    cout << "4. " << dagger << endl;
    cout << "\n== Combining weapons ==" << endl;
    Weapon enchantedBlade = blade + magicStaff;
    cout << "Created: " << enchantedBlade << endl;

    Weapon ultimateWeapon = enchantedBlade + hammer;
    cout << "Created ultimate weapon: " << ultimateWeapon << endl;

    cout << "\n== Comparing weapons ==" << endl;

    if (hammer > magicStaff) {
        cout << hammer.getName() << " is more powerful than "
            << magicStaff.getName() << endl;
    }
    else {
        cout << magicStaff.getName() << " is more powerful than "
            << hammer.getName() << endl;
    }

    if (dagger > blade) {
        cout << dagger.getName() << " is more powerful than "
            << blade.getName() << endl;
    }
    else {
        cout << blade.getName() << " is more powerful than "
            << dagger.getName() << endl;
    }

    if (ultimateWeapon > hammer) {
        cout << "Ultimate weapon is more powerful than any single weapon" << endl;
    }

    return 0;
}