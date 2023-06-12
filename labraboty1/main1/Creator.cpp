#include <iostream>
#include <fstream>

struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Error: Argument exception (must be equal 3)" << std::endl;
        return 1;
    }

    std::ofstream output(argv[1], std::ios::binary);
    if (!output) {
        std::cerr << "Error: Could not open file for writing" << std::endl;
        return 1;
    }

    int num_records = std::atoi(argv[2]);
    employee record;

    for (int i = 0; i < num_records; i++) {
        std::cout << "Enter the details of employee " << i + 1 << ":" << std::endl;
        std::cout << "ID: ";
        std::cin >> record.num;
        std::cout << "Name: ";
        std::cin >> record.name;
        std::cout << "Hours: ";
        std::cin >> record.hours;
        output.write(reinterpret_cast<char*>(&record), sizeof(record));
    }
    
    output.close();
    return 0;
}
