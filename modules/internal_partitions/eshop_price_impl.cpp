module EShop; // implementation unit of module EShop

double Customer::total_price() const
{
    double total = 0.0;

    for (const auto& order : orders_)
        total += order.count * order.price;

    return total;
}

double Customer::average_price() const
{
    if (orders_.empty())
        return 0.0;

    return total_price() / orders_.size();
}