#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

class Entity {
public:
    virtual void showStats() const = 0;
    virtual int getHealth() const = 0;
    virtual ~Entity() = default;
};

class Player : public Entity {
    string name;
    int health;
    int level;
public:
    Player(string n, int h, int l) : name(n), health(h), level(l) {}
    void showStats() const override {
        cout << "Player: " << name << " (Health: " << health << ", Level: " << level << ")\n";
    }
    int getHealth() const override { return health; }
};

class Enemy : public Entity {
    string name;
    int health;
    string category;
public:
    Enemy(string n, int h, string c) : name(n), health(h), category(c) {}
    void showStats() const override {
        cout << "Enemy: " << name << " (Health: " << health << ", Category: " << category << ")\n";
    }
    int getHealth() const override { return health; }
};

template <typename T>
class GameManager {
    vector<T> entityList;
public:
    void addEntity(const T& entity) {
        if (entity->getHealth() <= 0) {
            throw invalid_argument("Cannot add entity with zero or negative health");
        }
        entityList.push_back(entity);
    }
    void showAllEntities() const {
        for (const auto& entity : entityList) {
            entity->showStats();
        }
    }
};

template <typename T>
class Queue {
    vector<T> data;
public:
    void push(const T& item) {
        data.push_back(item);
    }

    T pop() {
        if (data.empty()) {
            throw out_of_range("Unable to retrieve element - queue is empty");
        }
        T frontItem = data.front();
        data.erase(data.begin());
        return frontItem;
    }

    bool isEmpty() const {
        return data.empty();
    }

    void printQueue() const {
        if (data.empty()) {
            cout << "Queue has no elements\n";
            return;
        }

        cout << "Current queue: ";
        for (const auto& item : data) {
            cout << item << " ";
        }
        cout << "\n";
    }
};

int main() {
    try {
        cout << "===== Game Entities =====\n";
        GameManager<Entity*> gameWorld;
        gameWorld.addEntity(new Player("Warrior", 150, 3));
        gameWorld.addEntity(new Enemy("Dragon", 300, "Boss"));
        gameWorld.showAllEntities();

        cout << "\n===== Invalid Entity Test =====\n";
        gameWorld.addEntity(new Player("Weak Character", -20, 1));
    }
    catch (const invalid_argument& e) {
        cerr << "Game Error: " << e.what() << endl;
    }

    try {
        cout << "\n===== Queue Operations =====\n";
        Queue<int> numberQueue;

        numberQueue.push(5);
        numberQueue.push(10);
        numberQueue.push(15);
        numberQueue.printQueue();

        cout << "First pop: " << numberQueue.pop() << endl;
        cout << "Second pop: " << numberQueue.pop() << endl;
        cout << "Third pop: " << numberQueue.pop() << endl;

        cout << "Attempting to pop from empty queue..." << endl;
        numberQueue.pop();
    }
    catch (const out_of_range& e) {
        cerr << "Queue Error: " << e.what() << endl;
    }

    return 0;
}