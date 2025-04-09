#include <iostream>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;
    int baseDefense;
    const int maxHealth = 100;
    bool defending = false;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d), baseDefense(d) {
    }

    int getHealth() const {
        return health;
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.takeDamage(damage);
            std::cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << " attacks " << enemy.name << ", but it has no effect!" << std::endl;
        }
    }

    void heal(int amount) {
        if (amount > 0) {
            health += amount;
            if (health > maxHealth) {
                health = maxHealth;
            }
            std::cout << name << " heals for " << amount << " HP." << std::endl;
        }
    }

    void takeDamage(int amount) {
        if (amount > 0) {
            health -= amount;
            if (health < 0) {
                health = 0;
            }
            std::cout << name << " takes " << amount << " damage!" << std::endl;
        }
    }

    void defend() {
        if (!defending) {
            defense += 10;
            defending = true;
            std::cout << name << " is defending! Defense temporarily increased to " << defense << "." << std::endl;
        }
    }

    void endTurn() {
        if (defending) {
            defense = baseDefense;
            defending = false;
            std::cout << name << "'s defense returns to normal: " << defense << "." << std::endl;
        }
    }

    bool isAlive() const {
        return health > 0;
    }

    std::string getName() const {
        return name;
    }
};

int main() {
    Character hero("Hero", 100, 20, 10);
    Character monster("Goblin", 50, 15, 5);

    int turn = 1;
    while (hero.isAlive() && monster.isAlive()) {
        std::cout << "\n--- Turn " << turn << " ---\n";
        hero.displayInfo();
        monster.displayInfo();
        int choice;
        std::cout << "\nChoose action for " << hero.getName() << ":\n";
        std::cout << "1 - Attack\n";
        std::cout << "2 - Defend\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;

        if (choice == 1) {
            hero.attackEnemy(monster);
        }
        else if (choice == 2) {
            hero.defend();
        }
        else {
            std::cout << "Invalid choice. Skipping turn.\n";
        }

        if (monster.isAlive()) {
            monster.attackEnemy(hero);
        }

        hero.endTurn();

        turn++;
    }

    std::cout << "\n=== Battle Over ===\n";
    if (hero.isAlive()) {
        std::cout << "Hero wins!\n";
    }
    else {
        std::cout << "Goblin wins!\n";
    }

    return 0;
}
