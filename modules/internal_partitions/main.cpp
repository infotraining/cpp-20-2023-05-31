import EShop;

import <iostream>;

int main()
{
	std::cout << "Modules & Implementation units\n";
	
	Customer c1{"Jan Kowalski"};

	c1.buy("wine", 159.90);
	c1.buy(2, "wine glass", 9.20);

	std::cout << "\n------------\n";
		
	c1.print();

	std::cout << "Average: " << c1.average_price() << '\n';
}