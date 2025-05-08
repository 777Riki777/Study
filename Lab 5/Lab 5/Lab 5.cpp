#include <iostream>
#include <deque>
#include <string>
#include <list>

class GameObject {
protected:
    std::string name;
    int health;

public:
    GameObject(std::string n, int h) : name(n), health(h) {}
    virtual void displayInfo() const = 0;
    virtual ~GameObject() {}
};

class Wizard : public GameObject {
private:
    int mana;

public:
    Wizard(std::string n, int h, int m) : GameObject(n, h), mana(m) {}

    void displayInfo() const override {
        std::cout << "Wizard: " << name << " [Health: " << health << ", Mana: " << mana << "]" << std::endl;
    }
};

class Monster : public GameObject {
private:
    std::string element;

public:
    Monster(std::string n, int h, std::string e) : GameObject(n, h), element(e) {}

    void displayInfo() const override {
        std::cout << "Monster: " << name << " [Health: " << health << ", Element: " << element << "]" << std::endl;
    }
};

template <typename EntityType>
class GameSystem {
private:
    std::list<EntityType> gameObjects;

public:
    void registerObject(const EntityType& obj) {
        gameObjects.push_back(obj);
    }

    void showAllObjects() const {
        for (const auto& obj : gameObjects) {
            obj->displayInfo();
        }
    }
};

template <typename T>
class Queue {
private:
    std::deque<T> data;

public:
    void push(const T& item) {
        data.push_back(item);
    }

    T pop() {
        if (data.empty()) {
            throw std::runtime_error("Empty queue!");
        }

        T front = data.front();
        data.pop_front();
        return front;
    }

    bool isEmpty() {
        return data.empty();
    }

    void display() {
        if (data.empty()) {
            std::cout << "Empty queue" << std::endl;
            return;
        }

        std::cout << "Queue contents: ";
        for (const auto& item : data) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    GameSystem<GameObject*> world;

    std::cout << "--- Game world initialization ---" << std::endl;
    world.registerObject(new Wizard("Merlin", 85, 150));
    world.registerObject(new Monster("Dragon", 200, "Fire"));
    world.registerObject(new Wizard("Gandalf", 95, 180));
    world.showAllObjects();

    std::cout << "\n--- Testing number queue ---" << std::endl;
    Queue<int> numQueue;

    numQueue.push(5);
    numQueue.push(10);
    numQueue.push(15);
    numQueue.display();

    try {
        int val = numQueue.pop();
        std::cout << "Popped value: " << val << std::endl;
        numQueue.display();
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << "\n--- Testing string queue ---" << std::endl;
    Queue<std::string> textQueue;

    textQueue.push("Hello");
    textQueue.push("World");
    textQueue.push("RPG");
    textQueue.display();

    try {
        std::string text = textQueue.pop();
        std::cout << "Popped text: " << text << std::endl;
        textQueue.display();
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}