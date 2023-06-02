export module EShop:Customer;

import :Order; // import partition module

import <string>;
import <vector>;
import <iostream>;

namespace std _GLIBCXX_VISIBILITY(default){} // Fix for gcc 12.2 & import <vector> or any std container

export class Customer
{
private:
    std::string name_;
    std::vector<Order> orders_;

public:
    Customer(std::string name)
        : name_{std::move(name)}
    {}

    void buy(std::string order_name, double price)
    {
        std::cout << name_ << " is buying " << order_name << " for a " << price << "$\n";
        orders_.push_back(Order{1, std::move(order_name), price});
    }

    void buy(unsigned int count, std::string order_name, double price)
    {
        orders_.push_back(Order{count, std::move(order_name), price});
    }

    double total_price() const;
    double average_price() const;
    void print() const;
};