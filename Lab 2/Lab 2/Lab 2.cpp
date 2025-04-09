#include <iostream>
#include <string>

// Класс персонажа
class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;
    bool defending = false;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Character " << name << " created!\n";
    }

    ~Character() {
        std::cout << "Character " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "[Character] " << name << " | HP: " << health
            << " | ATK: " << attack << " | DEF: " << defense;
        if (defending) std::cout << " (Defending)";
        std::cout << "\n";
    }

    std::string getName() const { return name; }
    int getHealth() const { return health; }

    void takeDamage(int dmg) {
        int actualDefense = defending ? defense * 2 : defense;
        int damage = dmg - actualDefense;
        if (damage > 0) {
            health -= damage;
            std::cout << name << " takes " << damage << " damage.\n";
        }
        else {
            std::cout << name << " blocks the attack!\n";
        }
    }

    void attackEnemy(class Monster& enemy);

    void defend() {
        defending = true;
        std::cout << name << " is defending and increases defense!\n";
    }

    void endTurn() {
        defending = false;
    }

    bool isAlive() const {
        return health > 0;
    }
};

// Класс монстра
class Monster {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Monster " << name << " created!\n";
    }

    ~Monster() {
        std::cout << "Monster " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "[Monster] " << name << " | HP: " << health
            << " | ATK: " << attack << " | DEF: " << defense << "\n";
    }

    std::string getName() const { return name; }
    int getHealth() const { return health; }

    void takeDamage(int dmg) {
        int damage = dmg - defense;
        if (damage > 0) {
            health -= damage;
            std::cout << name << " takes " << damage << " damage.\n";
        }
        else {
            std::cout << name << " blocks the attack!\n";
        }
    }

    void attackCharacter(Character& player) {
        std::cout << name << " attacks " << player.getName() << "!\n";
        player.takeDamage(attack);
    }

    bool isAlive() const {
        return health > 0;
    }
};

// Реализация атаки игрока после объявления класса Monster
void Character::attackEnemy(Monster& enemy) {
    std::cout << name << " attacks " << enemy.getName() << "!\n";
    enemy.takeDamage(attack);
}

// Основная функция
int main() {
    Character hero("Knight", 100, 25, 10);
    Monster goblin("Goblin", 60, 20, 5);

    while (hero.isAlive() && goblin.isAlive()) {
        std::cout << "\n--- STATUS ---\n";
        hero.displayInfo();
        goblin.displayInfo();

        std::cout << "\nChoose action:\n1. Attack\n2. Defend\n> ";
        int choice;
        std::cin >> choice;

        if (choice == 1) {
            hero.attackEnemy(goblin);
        }
        else if (choice == 2) {
            hero.defend();
        }
        else {
            std::cout << "Invalid choice. Turn skipped.\n";
        }

        if (goblin.isAlive()) {
            goblin.attackCharacter(hero);
        }

        hero.endTurn();
    }

    std::cout << "\n--- RESULT ---\n";
    if (hero.isAlive())
        std::cout << "You defeated the " << goblin.getName() << "!\n";
    else
        std::cout << "You were defeated by the " << goblin.getName() << "...\n";

    return 0;
}
