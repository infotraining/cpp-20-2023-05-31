module; // start module unit with global module fragment

export module EShop;

import <iostream>;
import <string>;
import <vector>;

namespace std _GLIBCXX_VISIBILITY(default){} // Fix for gcc 12.2 & import <vector> or any std container

struct Order
{
    unsigned int count;
    std::string name;
    double price;

    Order(unsigned int count, std::string order_name, double price)
        : count{count}
        , name{std::move(order_name)}
        , price{price}
    { }
};

void print(const Order& order); // not exported

export class Customer
{
private:
    std::string name_;
    std::vector<Order> orders_;

public:
    Customer(std::string name)
        : name_{std::move(name)}
    { }

    void buy(std::string order_name, double price)
    {
        Order order{1, std::move(order_name), price};

        ::print(order);
        
        orders_.push_back(std::move(order));
    }

    void buy(unsigned int count, std::string order_name, double price)
    {
        Order order{count, std::move(order_name), price};

        ::print(order);

        orders_.push_back(std::move(order));
    }

    double total_price() const;
    double average_price() const;
    void print() const;
};