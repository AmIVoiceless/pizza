#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ===== Базовый класс MenuItem =====
class MenuItem {
protected:
    string name;
    double basePrice;
public:
    MenuItem(const string& n, double p) : name(n), basePrice(p) {}
    virtual ~MenuItem() = default;
    virtual double calculatePrice() const = 0;
    virtual void display() const = 0;
    string getName() const { return name; }
};

// ===== Структура Topping =====
struct Topping {
    string name;
    double price;
    Topping(const string& n, double p) : name(n), price(p) {}
};

// ===== Класс Pizza =====
class Pizza : public MenuItem {
public:
    enum class Size { SMALL, MEDIUM, LARGE };
    enum class BaseType { THIN, TRADITIONAL, THICK };

private:
    Size size;
    BaseType baseType;
    vector<Topping> toppings;

public:
    Pizza(const string& n, double p)
        : MenuItem(n, p), size(Size::MEDIUM), baseType(BaseType::TRADITIONAL) {
    }

    void setSize(Size s) { size = s; }
    void addTopping(const Topping& t) { toppings.push_back(t); }

    double calculatePrice() const override {
        double price = basePrice;
        for (const auto& t : toppings) price += t.price;
        if (size == Size::SMALL) price *= 0.9;
        else if (size == Size::LARGE) price *= 1.2;
        return price;
    }

    void display() const override {
        cout << "Pizza: " << name << " | Size: ";
        if (size == Size::SMALL) cout << "Small";
        else if (size == Size::MEDIUM) cout << "Medium";
        else cout << "Large";
        cout << " | Toppings: ";
        for (const auto& t : toppings) cout << t.name << " ";
        cout << "| Price: " << calculatePrice() << "\n";
    }
};

// ===== Класс Drink =====
class Drink : public MenuItem {
private:
    double volume;
    bool isCarbonated;
public:
    Drink(const string& n, double p, double vol, bool carbon)
        : MenuItem(n, p), volume(vol), isCarbonated(carbon) {
    }

    double calculatePrice() const override {
        return basePrice + (volume * 0.01);
    }

    void display() const override {
        cout << "Drink: " << name << " | " << volume << "ml | "
            << (isCarbonated ? "Carbonated" : "Still")
            << " | Price: " << calculatePrice() << "\n";
    }
};

// ===== Класс SideDish =====
class SideDish : public MenuItem {
private:
    string portionSize;
public:
    SideDish(const string& n, double p, const string& size)
        : MenuItem(n, p), portionSize(size) {
    }

    double calculatePrice() const override {
        return basePrice;
    }

    void display() const override {
        cout << "Side Dish: " << name << " | Portion: " << portionSize
            << " | Price: " << calculatePrice() << "\n";
    }
};

// ===== Класс Menu =====
class Menu {
private:
    vector<Pizza> pizzas;
    vector<Drink> drinks;
    vector<SideDish> sides;
    vector<Topping> toppings;

public:
    Menu() {
        pizzas.emplace_back("Pepperoni", 400);
        drinks.emplace_back("Coke", 100, 500, true);
        drinks.emplace_back("Water", 50, 500, false);
        sides.emplace_back("Fries", 150, "Medium");

        toppings.emplace_back("Cheese", 50);
        toppings.emplace_back("Mushrooms", 40);
        toppings.emplace_back("Bacon", 60);
    }

    void displayMenu() const {
        cout << "\n--- MENU ---\n";
        for (size_t i = 0; i < pizzas.size(); ++i)
            cout << "P" << i << ": " << pizzas[i].getName() << "\n";
        for (size_t i = 0; i < drinks.size(); ++i)
            cout << "D" << i << ": " << drinks[i].getName() << "\n";
        for (size_t i = 0; i < sides.size(); ++i)
            cout << "S" << i << ": " << sides[i].getName() << "\n";
    }

    void displayToppings() const {
        cout << "\n--- TOPPINGS ---\n";
        for (size_t i = 0; i < toppings.size(); ++i)
            cout << i << ": " << toppings[i].name << " (" << toppings[i].price << ")\n";
    }

    Pizza getPizzaCopy(int index) const { return pizzas.at(index); }
    Drink getDrinkCopy(int index) const { return drinks.at(index); }
    SideDish getSideDishCopy(int index) const { return sides.at(index); }
    Topping getToppingCopy(int index) const { return toppings.at(index); }
    size_t pizzaCount() const { return pizzas.size(); }
    size_t drinkCount() const { return drinks.size(); }
    size_t sideCount() const { return sides.size(); }
    size_t toppingCount() const { return toppings.size(); }
};

// ===== Главная программа =====
int main() {
    Menu menu;
    vector<MenuItem*> order;

    cout << "=== Welcome to Pizzeria Simulator ===\n";
    menu.displayMenu();

    while (true) {
        cout << "\nEnter item code (e.g., P0, D1, S0), or 'done': ";
        string input;
        cin >> input;
        if (input == "done") break;

        char type = input[0];
        int index = stoi(input.substr(1));

        if (type == 'P' && index < menu.pizzaCount()) {
            Pizza p = menu.getPizzaCopy(index);

            cout << "Choose size (1=Small, 2=Medium, 3=Large): ";
            int s; cin >> s;
            p.setSize(static_cast<Pizza::Size>(s - 1));

            cout << "Add toppings? (y/n): ";
            char c; cin >> c;
            if (c == 'y') {
                menu.displayToppings();
                cout << "Enter topping indexes separated by space (end with -1): ";
                int t;
                while (cin >> t && t != -1) {
                    if (t >= 0 && t < menu.toppingCount())
                        p.addTopping(menu.getToppingCopy(t));
                }
            }

            order.push_back(new Pizza(p));
        }
        else if (type == 'D' && index < menu.drinkCount()) {
            order.push_back(new Drink(menu.getDrinkCopy(index)));
        }
        else if (type == 'S' && index < menu.sideCount()) {
            order.push_back(new SideDish(menu.getSideDishCopy(index)));
        }
        else {
            cout << "Invalid input.\n";
        }
    }

    cout << "\n=== Your Order ===\n";
    double total = 0;
    for (const auto& item : order) {
        item->display();
        total += item->calculatePrice();
    }
    cout << "Total: " << total << " T\n";

    for (auto ptr : order) delete ptr;
    return 0;
}
