#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <chrono>
#include <random>

using namespace std;

class Enemy {
    string type;
    int hp;
    int dmg;
    int shield;
public:
    Enemy(string t, int h, int d, int s) : type(t), hp(h), dmg(d), shield(s) {}

    void show() const {
        cout << type << " (HP: " << hp << ", DMG: " << dmg << ", DEF: " << shield << ")\n";
    }

    void receive(int val) {
        int loss = val - shield;
        if (loss < 1) loss = 1;
        hp -= loss;
        if (hp < 0) hp = 0;
    }

    bool alive() const { return hp > 0; }
    int power() const { return dmg; }
    string label() const { return type; }
};

class Player {
    string id;
    int hp;
    int atk;
    int res;
public:
    Player(string i, int h, int a, int r) : id(i), hp(h), atk(a), res(r) {}

    void show() const {
        cout << id << " (HP: " << hp << ", ATK: " << atk << ", DEF: " << res << ")\n";
    }

    void receive(int val) {
        int loss = val - res;
        if (loss < 1) loss = 1;
        hp -= loss;
        if (hp < 0) hp = 0;
    }

    bool alive() const { return hp > 0; }
    int power() const { return atk; }
    string label() const { return id; }
};

vector<Enemy> horde;
mutex hordeLock;

Player knight("Valen", 110, 23, 9);
mutex playerLock;

void spawn() {
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<> hpD(25, 60);
    uniform_int_distribution<> atkD(6, 16);
    uniform_int_distribution<> defD(1, 7);

    vector<string> types = { "Shade", "Wraith", "Golem", "Imp", "Ghoul" };

    while (true) {
        this_thread::sleep_for(chrono::seconds(4));

        int i = rng() % types.size();
        string t = types[i];
        int h = hpD(rng);
        int a = atkD(rng);
        int d = defD(rng);

        lock_guard<mutex> lock(hordeLock);
        horde.emplace_back(t, h, a, d);
        cout << "A wild " << t << " has appeared!\n";
    }
}

void clash() {
    while (true) {
        this_thread::sleep_for(chrono::seconds(2));

        lock_guard<mutex> lock1(hordeLock);
        lock_guard<mutex> lock2(playerLock);

        if (horde.empty()) continue;

        Enemy& foe = horde.front();

        cout << "\n>>> Encounter <<<\n";
        knight.show();
        foe.show();

        foe.receive(knight.power());
        cout << knight.label() << " strikes " << foe.label() << "!\n";

        if (foe.alive()) {
            knight.receive(foe.power());
            cout << foe.label() << " retaliates!\n";
        }

        if (!foe.alive()) {
            cout << foe.label() << " vanquished.\n";
            horde.erase(horde.begin());
        }

        if (!knight.alive()) {
            cout << knight.label() << " has perished. The end.\n";
            exit(0);
        }

        cout << ">>> ========= <<<\n\n";
    }
}

int main() {
    cout << "Warrior begins the journey...\n";
    knight.show();

    thread t1(spawn);
    thread t2(clash);

    t1.detach();
    t2.detach();

    while (true) {
        this_thread::sleep_for(chrono::seconds(3));

        lock_guard<mutex> lock(hordeLock);
        cout << "\n--- Monsters Present: " << horde.size() << " ---\n";
        for (auto& foe : horde) {
            foe.show();
        }
    }

    return 0;
}
