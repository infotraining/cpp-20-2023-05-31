import Shapes;
import <iostream>;

int main()
{
    Shapes::Point pt{100, 200};

    std::cout << pt << "\n";

    pt.translate(20, 40);

    std::cout << pt << "\n";
}