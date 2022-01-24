#include <utheque/utheque.hpp> // library of URDF

#include <iostream>

int main()
{
    std::string model_file = utheque::path("arm.urdf");

    std::cout << "URDF full path from utheque library:" << model_file << std::endl;

    return 0;
}
