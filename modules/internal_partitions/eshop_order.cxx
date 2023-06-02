module EShop:Order; // interface partition declaration

import <string>;

struct Order
{
    unsigned int count;
    std::string name;
    double price;

    Order(unsigned int count, std::string order_name, double price)
        : count{count}
        , name{std::move(order_name)}
        , price{price}
    {}
};

void print(const Order& order);