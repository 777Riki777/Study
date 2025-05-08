#include <iostream>
#include <string>

using namespace std;

class Person {
private:
    string name;
    int age;
    string email;
    string address;

public:
    string getName() const {
        return name;
    }

    int getAge() const {
        return age;
    }

    string getEmail() const {
        return email;
    }

    string getAddress() const {
        return address;
    }

    void setName(const string& value) {
        if (!value.empty()) {
            name = value;
        }
        else {
            cerr << "Error: name cannot be empty" << endl;
        }
    }

    void setAge(int value) {
        if (value >= 0 && value <= 120) {
            age = value;
        }
        else {
            cerr << "Error: age must be between 0 and 120" << endl;
        }
    }

    void setEmail(const string& value) {
        if (value.find('@') != string::npos) {
            email = value;
        }
        else {
            cerr << "Error: invalid email format" << endl;
        }
    }

    void setAddress(const string& value) {
        if (!value.empty()) {
            address = value;
        }
        else {
            cerr << "Error: address cannot be empty" << endl;
        }
    }

    void show() const {
        cout << "Name: " << name
            << ", Age: " << age
            << ", Email: " << email
            << ", Address: " << address << endl;
    }
};

int main() {
    Person p;

    p.setName("Alice Johnson");
    p.setAge(34);
    p.setEmail("alice.j@example.com");
    p.setAddress("45 Central Avenue, Boston");

    cout << "Name: " << p.getName() << endl;
    cout << "Age: " << p.getAge() << endl;
    cout << "Email: " << p.getEmail() << endl;
    cout << "Address: " << p.getAddress() << endl;

    p.setName("");
    p.setAge(999);
    p.setEmail("wrong-email");
    p.setAddress("");

    p.show();

    return 0;
}
