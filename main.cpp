#include <iostream>
#include "Slice.h"

int main() {
    const char* data = "Hello, World!";
    size_t size = strlen(data);

    Slice slice1(data, size); // Create a Slice from a character array
    std::cout << "Slice 1: " << slice1.ToString() << std::endl;

    Slice slice2("Hello"); // Create a Slice from a C-style string
    std::cout << "Slice 2: " << slice2.ToString() << std::endl;

    std::string str = "World!";
    Slice slice3(str); // Create a Slice from a std::string
    std::cout << "Slice 3: " << slice3.ToString() << std::endl;

    if (slice1.starts_with(slice2)) {
        std::cout << "Slice 1 starts with Slice 2" << std::endl;
    } else {
        std::cout << "Slice 1 does not start with Slice 2" << std::endl;
    }

    if (slice3 == slice2) {
        std::cout << "Slice 3 is equal to Slice 2" << std::endl;
    } else {
        std::cout << "Slice 3 is not equal to Slice 2" << std::endl;
    }

    return 0;
}
