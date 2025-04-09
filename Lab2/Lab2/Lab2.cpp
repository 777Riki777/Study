#include <iostream>
#include <string>

class Entity {
protected:
    std::string name;
    int health;

public:
    Entity(const std::string& n, int h) : name(n), health(h) {}

    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << std::endl;
    }

    bool isAlive() const {
        return health > 0;
    }

    virtual void takeDamage(int amount) {
        health -= amount;
        if (health < 0) health = 0;
    }

    const std::string& getName() const {
        return name;
    }

    virtual ~Entity() {}
};

class Player : public Entity {
private:
    int experience;
    bool defending;

public:
    Player(const std::string& n, int h, int exp)
        : Entity(n, h), experience(exp), defending(false) {
    }

    void displayInfo() const override {
        Entity::displayInfo();
        std::cout << "Experience: " << experience << std::endl;
    }

    void attack(Entity& target) {
        int damage = 20;
        std::cout << name << " attacks " << target.getName()
            << " for " << damage << " damage!" << std::endl;
        target.takeDamage(damage);
    }

    void defend() {
        defending = true;
        std::cout << name << " is defending and will take less damage next turn!" << std::endl;
    }

    bool isDefending() const {
        return defending;
    }

    void resetDefend() {
        defending = false;
    }

    void takeDamage(int amount) override {
        if (defending) {
            amount /= 2;
            std::cout << name << " reduces the damage to " << amount << "!" << std::endl;
        }
        Entity::takeDamage(amount);
    }

    void gainExperience(int xp) {
        experience += xp;
        std::cout << name << " gained " << xp << " XP! Total XP: " << experience << std::endl;
    }
};

class Enemy : public Entity {
private:
    std::string type;

public:
    Enemy(const std::string& n, int h, const std::string& t)
        : Entity(n, h), type(t) {
    }

    void displayInfo() const override {
        Entity::displayInfo();
        std::cout << "Type: " << type << std::endl;
    }

    virtual void attack(Player& player) {
        int damage = 10;
        std::cout << name << " attacks " << player.getName()
            << " for " << damage << " damage!" << std::endl;
        player.takeDamage(damage);
    }
};

class Boss : public Enemy {
private:
    std::string specialAbility;

public:
    Boss(const std::string& n, int h, const std::string& t, const std::string& ability)
        : Enemy(n, h, t), specialAbility(ability) {
    }

    void displayInfo() const override {
        Enemy::displayInfo();
        std::cout << "Special Ability: " << specialAbility << std::endl;
    }

    void attack(Player& player) override {
        int damage = 25;
        std::cout << name << " uses " << specialAbility << " and deals "
            << damage << " damage!" << std::endl;
        player.takeDamage(damage);
    }
};

void battle(Player& player, Enemy& enemy) {
    std::cout << "\n--- Battle Start ---\n";
    while (player.isAlive() && enemy.isAlive()) {
        player.displayInfo();
        enemy.displayInfo();

        std::cout << "\nChoose action: (1) Attack  (2) Defend\n> ";
        int choice;
        std::cin >> choice;

        player.resetDefend();

        if (choice == 1) {
            player.attack(enemy);
        }
        else if (choice == 2) {
            player.defend();
        }
        else {
            std::cout << "Invalid choice! You miss a turn.\n";
        }

        if (enemy.isAlive()) {
            enemy.attack(player);
        }
    }

    if (player.isAlive()) {
        std::cout << "\n" << enemy.getName() << " defeated!\n";
        player.gainExperience(50);
    }
    else {
        std::cout << "\n" << player.getName() << " was defeated!\n";
    }
}

int main() {
    Player hero("Hero", 100, 0);
    Enemy goblin("Goblin", 50, "Goblin");
    Boss dragon("Dragon", 150, "Dragon", "Fire Breath");

    std::cout << "Welcome to the RPG battle!\n";

    battle(hero, goblin);

    if (hero.isAlive()) {
        std::cout << "\nA Boss appears!\n";
        dragon.displayInfo();
        battle(hero, dragon);
    }

    if (hero.isAlive()) {
        std::cout << "\nVictory! You have defeated all enemies!\n";
    }
    else {
        std::cout << "\nGame Over.\n";
    }

    return 0;
}
