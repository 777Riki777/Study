#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

class Entity {
protected:
    std::string name;
    int health;
    int attackPower;
    int defense;

public:
    Entity(const std::string& n, int h, int a, int d)
        : name(n), health(h), attackPower(a), defense(d) {
    }

    virtual ~Entity() {}

    virtual void attack(Entity& target) {
        int damage = attackPower - target.getDefense();
        if (damage > 0) {
            target.takeDamage(damage);
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    virtual void displayInfo() const {
        std::cout << name << " | HP: " << health << ", Attack: " << attackPower << ", Defense: " << defense << '\n';
    }

    virtual void takeDamage(int amount) {
        health -= amount;
        if (health < 0) health = 0;
    }

    virtual void heal(int amount) {
        health += amount;
        if (health > 100) health = 100;
        std::cout << name << " heals for " << amount << " HP. New HP: " << health << '\n';
    }

    bool isAlive() const {
        return health > 0;
    }

    int getDefense() const {
        return defense;
    }

    const std::string& getName() const {
        return name;
    }
};

class Character : public Entity {
public:
    Character(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }

    void attack(Entity& target) override {
        int damage = attackPower - target.getDefense();
        if (rand() % 100 < 20) {
            damage *= 2;
            std::cout << "Critical hit! ";
        }

        if (damage > 0) {
            target.takeDamage(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "[Player] ";
        Entity::displayInfo();
    }
};

class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }

    void attack(Entity& target) override {
        int damage = attackPower - target.getDefense();
        if (rand() % 100 < 30) {
            damage += 5;
            std::cout << "Poisonous attack! ";
        }

        if (damage > 0) {
            target.takeDamage(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "[Monster] ";
        Entity::displayInfo();
    }
};

class Boss : public Monster {
public:
    Boss(const std::string& n, int h, int a, int d)
        : Monster(n, h, a, d) {
    }

    void attack(Entity& target) override {
        int damage = attackPower - target.getDefense();
        if (rand() % 100 < 40) {
            damage += 10;
            std::cout << "Fire blast! ";
        }

        if (damage > 0) {
            target.takeDamage(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "[BOSS] ";
        Entity::displayInfo();
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    Character hero("Knight", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);
    Boss dragon("Dragon", 120, 30, 10);

    Entity* enemies[] = { &goblin, &dragon };

    for (int i = 0; i < 2; ++i) {
        Entity* enemy = enemies[i];
        std::cout << "\n--- A wild " << enemy->getName() << " appears! ---\n";

        while (hero.isAlive() && enemy->isAlive()) {
            hero.displayInfo();
            enemy->displayInfo();

            std::cout << "\nChoose action:\n1. Attack\n2. Heal\n> ";
            int choice;
            std::cin >> choice;

            if (choice == 1) {
                hero.attack(*enemy);
            }
            else if (choice == 2) {
                hero.heal(15);
            }

            if (enemy->isAlive()) {
                enemy->attack(hero);
            }

            if (!hero.isAlive()) {
                std::cout << hero.getName() << " has been defeated...\n";
                return 0;
            }
        }

        std::cout << enemy->getName() << " has been defeated!\n";
    }

    std::cout << "\nVictory! All enemies are defeated.\n";
    return 0;
}
