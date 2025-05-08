#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <locale>

using namespace std;

class User {
protected:
    string name;
    int id;
    int accessLevel;

public:
    User(const string& name, int id, int accessLevel) : name(name), id(id) {
        setAccessLevel(accessLevel);
        if (name.empty()) {
            throw runtime_error("Имя не может быть пустым");
        }
    }

    string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    void setName(const string& newName) {
        if (newName.empty()) throw runtime_error("Имя не может быть пустым");
        name = newName;
    }

    void setAccessLevel(int newLevel) {
        if (newLevel < 0) throw runtime_error("Уровень доступа должен быть неотрицательным");
        accessLevel = newLevel;
    }

    virtual void displayInfo() const {
        cout << "Имя: " << name << " (ID: " << id << "), Уровень доступа: " << accessLevel;
    }

    virtual ~User() = default;
};

class Student : public User {
    string group;

public:
    Student(const string& name, int id, int accessLevel, const string& group)
        : User(name, id, accessLevel), group(group) {
    }

    string getGroup() const { return group; }
    void setGroup(const string& newGroup) { group = newGroup; }

    void displayInfo() const override {
        User::displayInfo();
        cout << ", Группа: " << group << endl;
    }
};

class Teacher : public User {
    string department;

public:
    Teacher(const string& name, int id, int accessLevel, const string& department)
        : User(name, id, accessLevel), department(department) {
    }

    string getDepartment() const { return department; }
    void setDepartment(const string& newDepartment) { department = newDepartment; }

    void displayInfo() const override {
        User::displayInfo();
        cout << ", Кафедра: " << department << endl;
    }
};

class Administrator : public User {
    string position;

public:
    Administrator(const string& name, int id, int accessLevel, const string& position)
        : User(name, id, accessLevel), position(position) {
    }

    string getPosition() const { return position; }
    void setPosition(const string& newPosition) { position = newPosition; }

    void displayInfo() const override {
        User::displayInfo();
        cout << ", Должность: " << position << endl;
    }
};

class Resource {
    string title;
    int minAccessLevel;

public:
    Resource(const string& title, int minAccessLevel) : title(title) {
        setMinAccessLevel(minAccessLevel);
        if (title.empty()) throw runtime_error("Ресурс должен иметь название");
    }

    string getTitle() const { return title; }
    int getMinAccessLevel() const { return minAccessLevel; }

    void setTitle(const string& newTitle) {
        if (newTitle.empty()) throw runtime_error("Ресурс должен иметь название");
        title = newTitle;
    }

    void setMinAccessLevel(int level) {
        if (level < 0) throw runtime_error("Уровень доступа не может быть отрицательным");
        minAccessLevel = level;
    }

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= minAccessLevel;
    }

    void print() const {
        cout << "Ресурс: " << title << " (Минимальный уровень доступа: " << minAccessLevel << ")" << endl;
    }
};

template <typename UserType, typename ResourceType>
class AccessControlSystem {
    vector<unique_ptr<UserType>> users;
    vector<unique_ptr<ResourceType>> resources;

public:
    void addUser(unique_ptr<UserType> user) {
        users.push_back(move(user));
    }

    void addResource(unique_ptr<ResourceType> resource) {
        resources.push_back(move(resource));
    }

    bool hasAccess(const UserType& user, const ResourceType& resource) const {
        return resource.checkAccess(user);
    }

    UserType* findUser(const string& name) const {
        for (const auto& user : users) {
            if (user->getName() == name) return user.get();
        }
        return nullptr;
    }

    ResourceType* findResource(const string& title) const {
        for (const auto& res : resources) {
            if (res->getTitle() == title) return res.get();
        }
        return nullptr;
    }

    void sortUsersByAccessLevel() {
        sort(users.begin(), users.end(), [](const auto& a, const auto& b) {
            return a->getAccessLevel() < b->getAccessLevel();
            });
    }

    void listUsers() const {
        cout << "\n=== Зарегистрированные пользователи ===" << endl;
        for (const auto& user : users) {
            user->displayInfo();
        }
    }

    void listResources() const {
        cout << "\n=== Доступные ресурсы ===" << endl;
        for (const auto& res : resources) {
            res->print();
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");

    AccessControlSystem<User, Resource> system;

    try {
        system.addUser(make_unique<Student>("Иванов", 1001, 1, "Группа A1"));
        system.addUser(make_unique<Teacher>("Петров", 2001, 5, "Информатика"));
        system.addUser(make_unique<Administrator>("Сидоров", 3001, 10, "Главный администратор"));

        system.addResource(make_unique<Resource>("Аудитория 101", 1));
        system.addResource(make_unique<Resource>("Серверная", 5));
        system.addResource(make_unique<Resource>("Библиотека", 1));
        system.addResource(make_unique<Resource>("Кабинет декана", 8));

        system.listUsers();
        system.listResources();

        User* user = system.findUser("Иванов");
        Resource* resource = system.findResource("Серверная");

        if (user && resource) {
            if (system.hasAccess(*user, *resource)) {
                cout << "\nДоступ разрешен!" << endl;
            }
            else {
                cout << "\nДоступ запрещен!" << endl;
            }
        }

        system.sortUsersByAccessLevel();
        cout << "\nПользователи, отсортированные по уровню доступа:" << endl;
        system.listUsers();

    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }

    return 0;
}
